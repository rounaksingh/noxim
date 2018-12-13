//#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
//#include <pybind11/stl.h>

#include "DataStructs.h"
#include "Utils.h"

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(noxim, m) {
    // DataStructs 
    py::class_<Coord>(m, "Coord")
    	.def_readwrite("x", &Coord::x)
    	.def_readwrite("y", &Coord::y)
    	.def("__eq__", [](const Coord &a, const Coord &b) {
    		if(a.x==b.x && a.y==b.y)
    			return true;
    		else
    			return false;
		})
		.def(py::init<>());

	py::class_<RouteData>(m, "RouteData")
    	.def_readwrite("current_id", &RouteData::current_id)
    	.def_readwrite("src_id", &RouteData::src_id)
    	.def_readwrite("dst_id", &RouteData::dst_id)
    	.def_readwrite("dir_in", &RouteData::dir_in)
    	.def_readwrite("vc_id", &RouteData::vc_id)
    	.def(py::init<>());
    //m.def("route_data_init", [](RouteData r) { r.current_id=0;r.src_id=0;r.dst_id=0;r.dir_in=0;r.vc_id=0; });

    py::class_<Packet>(m, "Packet")
    	.def_readwrite("src_id", &Packet::src_id)
    	.def_readwrite("dst_id", &Packet::dst_id)
    	.def_readwrite("vc_id", &Packet::vc_id)
    	.def_readwrite("timestamp", &Packet::timestamp)
    	.def_readwrite("size", &Packet::size)
    	.def_readwrite("flit_left", &Packet::flit_left)
    	.def_readwrite("use_low_voltage_path", &Packet::use_low_voltage_path)
    	.def(py::init<>())
    	.def(py::init<const int, const int, const int, const double, const int>())
        .def("make", &Packet::make);

    // Utils.h functions
    m.def("id2Coord", &id2Coord);
    m.def("coord2Id", &coord2Id);
    m.def("sameRadioHub", &sameRadioHub);
    m.def("hasRadioHub", &hasRadioHub);
    m.def("tile2Hub", &tile2Hub);

    // GlobalParams
    py::class_<GlobalParams>(m, "GlobalParams")
        .def_readwrite_static("verbose_mode", &GlobalParams::verbose_mode)
        .def_readwrite_static("trace_mode", &GlobalParams::trace_mode)
        .def_readwrite_static("trace_filename", &GlobalParams::trace_filename)
        .def_readwrite_static("mesh_dim_x", &GlobalParams::mesh_dim_x)
        .def_readwrite_static("mesh_dim_y", &GlobalParams::mesh_dim_y)
        .def_readwrite_static("r2r_link_length", &GlobalParams::r2r_link_length)
        .def_readwrite_static("r2h_link_length", &GlobalParams::r2h_link_length)
        .def_readwrite_static("buffer_depth", &GlobalParams::buffer_depth)
        .def_readwrite_static("flit_size", &GlobalParams::flit_size)
        .def_readwrite_static("min_packet_size", &GlobalParams::min_packet_size)
        .def_readwrite_static("max_packet_size", &GlobalParams::max_packet_size)
        .def_readwrite_static("routing_algorithm", &GlobalParams::routing_algorithm);
/*        .def_readwrite_static("routing_table_filename", &GlobalParams::routing_table_filename)
        .def_readwrite_static("selection_strategy", &GlobalParams::selection_strategy)
        .def_readwrite_static("packet_injection_rate", &GlobalParams::packet_injection_rate)
        .def_readwrite_static("probability_of_retransmission", &GlobalParams::probability_of_retransmission)
        .def_readwrite_static("locality", &GlobalParams::locality)
        .def_readwrite_static("traffic_distribution", &GlobalParams::traffic_distribution)
        .def_readwrite_static("traffic_table_filename", &GlobalParams::traffic_table_filename)
        .def_readwrite_static("config_filename", &GlobalParams::config_filename)
        .def_readwrite_static("power_config_filename", &GlobalParams::power_config_filename)
        .def_readwrite_static("clock_period_ps", &GlobalParams::clock_period_ps)
        .def_readwrite_static("simulation_time", &GlobalParams::simulation_time)
        .def_readwrite_static("n_virtual_channels", &GlobalParams::n_virtual_channels)
        .def_readwrite_static("reset_time", &GlobalParams::reset_time)
        .def_readwrite_static("stats_warm_up_time", &GlobalParams::stats_warm_up_time)
        .def_readwrite_static("rnd_generator_seed", &GlobalParams::rnd_generator_seed)
        .def_readwrite_static("detailed", &GlobalParams::detailed)
        .def_readwrite_static("hotspots", &GlobalParams::hotspots)
        .def_readwrite_static("dyad_threshold", &GlobalParams::dyad_threshold)
        .def_readwrite_static("max_volume_to_be_drained", &GlobalParams::max_volume_to_be_drained)
        .def_readwrite_static("show_buffer_stats", &GlobalParams::show_buffer_stats)
        .def_readwrite_static("use_winoc", &GlobalParams::use_winoc)
        .def_readwrite_static("use_powermanager", &GlobalParams::use_powermanager)
        .def_readwrite_static("default_channel_configuration", &GlobalParams::default_channel_configuration)
        .def_readwrite_static("channel_configuration", &GlobalParams::channel_configuration)
        .def_readwrite_static("default_hub_configuration", &GlobalParams::default_hub_configuration)
        .def_readwrite_static("hub_configuration", &GlobalParams::hub_configuration)
        .def_readwrite_static("hub_for_tile", &GlobalParams::hub_for_tile)
        .def_readwrite_static("power_configuration", &GlobalParams::power_configuration)
        .def_readwrite_static("ascii_monitor", &GlobalParams::ascii_monitor)
        .def_readwrite_static("channel_selection", &GlobalParams::channel_selection);*/

}

