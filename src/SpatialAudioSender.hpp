#ifndef SPATIAL_AUDIO_SERVER_HPP
#define SPATIAL_AUDIO_SERVER_HPP

#include <string>
#include <vector>

namespace boost{
  class thread;
  class mutex;
}


namespace spatialaudiopy{

struct user_data{
	unsigned user_id;
	unsigned group_id;
	float position[3];
	float front[3];
	float up[3];
};


class SpatialAudioSender{

public:
	SpatialAudioSender(const std::string& server_socket, const unsigned num_slots);
	~SpatialAudioSender();
	void start();
	void stop();

	void setUserData(const unsigned user_id, const unsigned group_id,
					 const float pos_x, const float pos_y, const float pos_z,
					 const float front_x, const float front_y, const float front_z,
				     const float up_x, const float up_y, const float up_z);

private:
	void loop();
	bool m_running;
	std::vector<user_data> m_userdata;
	std::vector<boost::mutex*> m_mutex;
	boost::thread* m_senderthread;
	std::string m_serversocket;
};

}

#endif // #ifndef SPATIAL_AUDIO_SERVER_HPP
