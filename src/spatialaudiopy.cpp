/*
 * This inclusion should be put at the beginning.  It will include <Python.h>.
 */

#include <SpatialAudioSender.hpp>

#include <boost/python.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>


/*
 * This is a macro Boost.Python provides to signify a Python extension module.
 */
BOOST_PYTHON_MODULE(spatialaudiopy) {
    // An established convention for using boost.python.
    using namespace boost::python;
    using namespace spatialaudiopy;


    // expose the class RemoteRecorder
    class_<SpatialAudioSender>("SpatialAudioSender",
        init<std::string const &, unsigned int const >())
        .def("start", &SpatialAudioSender::start)
        .def("stop", &SpatialAudioSender::stop)
        .def("setUserData", &SpatialAudioSender::setUserData)
        .def("setUserMatrixA", &SpatialAudioSender::setUserMatrixA)
        .def("setUserMatrixB", &SpatialAudioSender::setUserMatrixB)
        .def("setUserMatrixC", &SpatialAudioSender::setUserMatrixC)
        .def("setUserMatrixD", &SpatialAudioSender::setUserMatrixD)
        //.def("is_paused", &RemoteRecorder::is_paused)
        //.add_property("filename", &RemoteRecorder::get_filename, &RemoteRecorder::set_filename)
    ;  
}


