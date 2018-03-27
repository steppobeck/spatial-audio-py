#include "SpatialAudioSender.hpp"

#include <zmq.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

namespace spatialaudiopy{


SpatialAudioSender::SpatialAudioSender(const std::string& server_socket, const unsigned num_slots)
: 	m_running(false),
	m_userdata(num_slots),
	m_mutex(num_slots),
	m_senderthread(0),
	m_serversocket(server_socket)
{
	for(unsigned i = 0; i != num_slots; ++i){
		m_mutex[i] = new boost::mutex;
	}
}

SpatialAudioSender::~SpatialAudioSender(){
	m_running = false;
	m_senderthread->join();
	for(unsigned i = 0; i != m_mutex.size(); ++i){
		delete m_mutex[i];
	}
	delete m_senderthread;
}

void
SpatialAudioSender::start(){
	m_running = true;
	m_senderthread = new boost::thread(boost::bind(&SpatialAudioSender::loop, this));
}


void
SpatialAudioSender::stop(){
	m_running = false;
}

void
SpatialAudioSender::setUserData(const unsigned user_id, const unsigned group_id,
					 const float pos_x, const float pos_y, const float pos_z,
					 const float front_x, const float front_y, const float front_z,
				     const float up_x, const float up_y, const float up_z){
	if(user_id >= m_userdata.size()){
		std::cerr << "ERROR in:  skipping user_id: " << user_id << std::endl;
		return;
	}

	boost::mutex::scoped_lock lock( *(m_mutex[user_id]) );

	m_userdata[user_id].user_id = user_id;
	m_userdata[user_id].group_id = group_id;

	m_userdata[user_id].position[0] = pos_x;
	m_userdata[user_id].position[1] = pos_y;
	m_userdata[user_id].position[2] = pos_z;

	m_userdata[user_id].front[0] = front_x;
	m_userdata[user_id].front[1] = front_y;
	m_userdata[user_id].front[2] = front_z;

	m_userdata[user_id].up[0] = up_x;
	m_userdata[user_id].up[1] = up_y;
	m_userdata[user_id].up[2] = up_z;

}


void
SpatialAudioSender::loop(){

	// 1. prepare socket
  	zmq::context_t ctx(1); // means single threaded
  	zmq::socket_t  server_socket(ctx, ZMQ_PUB); // means a publisher
  	uint32_t hwm = 1;
  	server_socket.setsockopt(ZMQ_SNDHWM,&hwm, sizeof(hwm));
  	std::string endpoint("tcp://" + m_serversocket);
  	server_socket.bind(endpoint.c_str());

	while(m_running){

		// 2. create and fill message
		const unsigned num_users = m_userdata.size();
		zmq::message_t msg( sizeof(unsigned) + sizeof(user_data) * num_users);

		memcpy( (unsigned char* ) msg.data(), (const unsigned char*) &num_users, sizeof(unsigned));
		for(unsigned i = 0; i != num_users; ++i){
			boost::mutex::scoped_lock lock( *(m_mutex[i]) );
			memcpy( ((unsigned char* ) msg.data() + (sizeof(unsigned) + i * sizeof(user_data))), (const unsigned char*) &(m_userdata[i]), sizeof(user_data));
		}
		// 3. send userdate
		server_socket.send(msg);

		// sleep a tiny littly bit
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

}
