/*
 * Noxim - the NoC Simulator
 *
 * (C) 2005-2018 by the University of Catania
 * For the complete list of authors refer to file ../doc/AUTHORS.txt
 * For the license applied to these sources refer to file ../doc/LICENSE.txt
 *
 * This file contains the declaration of the global params needed by Noxim
 * to forward configuration to every sub-block
 */
#ifndef __UTILS_H__
#define __UTILS_H__

#include <systemc.h>
#include <tlm>

#include "DataStructs.h"
#include <iomanip>
#include <sstream>

#define SUBNET_DIM_X 2
#define SUBNET_DIM_Y 2

#ifdef DEBUG

#define LOG (std::cout << std::setw(7) << left << sc_time_stamp().to_double() / GlobalParams::clock_period_ps << " " << name() << "::" << __func__<< "() --> ")
#define RLOG (std::cout << std::setw(7) << left << sc_time_stamp().to_double() / GlobalParams::clock_period_ps << " " << "::" << __func__<< "() --> ")

#else
template <class cT, class traits = std::char_traits<cT> >
class basic_nullbuf: public std::basic_streambuf<cT, traits> {
    typename traits::int_type overflow(typename traits::int_type c)
    {
        return traits::not_eof(c); // indicate success
    }
};

template <class cT, class traits = std::char_traits<cT> >
class basic_onullstream: public std::basic_ostream<cT, traits> {
public:
    basic_onullstream():
    std::basic_ios<cT, traits>(&m_sbuf),
    std::basic_ostream<cT, traits>(&m_sbuf)
    {
        // note: the original code is missing the required this->
        this->init(&m_sbuf);
    }

private:
    basic_nullbuf<cT, traits> m_sbuf;
};

typedef basic_onullstream<char> onullstream;
typedef basic_onullstream<wchar_t> wonullstream;

static onullstream LOG;

#endif

// Output overloading

inline ostream & operator <<(ostream & os, const Flit & flit)
{

    if (GlobalParams::verbose_mode == VERBOSE_HIGH) {

	os << "### FLIT ###" << endl;
	os << "Source Tile[" << flit.src_id << "]" << endl;
	os << "Destination Tile[" << flit.dst_id << "]" << endl;
	switch (flit.flit_type) {
	case FLIT_TYPE_HEAD:
	    os << "Flit Type is HEAD" << endl;
	    break;
	case FLIT_TYPE_BODY:
	    os << "Flit Type is BODY" << endl;
	    break;
	case FLIT_TYPE_TAIL:
	    os << "Flit Type is TAIL" << endl;
	    break;
	}
	os << "Sequence no. " << flit.sequence_no << endl;
	os << "Payload printing not implemented (yet)." << endl;
	os << "Unix timestamp at packet generation " << flit.
	    timestamp << endl;
	os << "Total number of hops from source to destination is " <<
	    flit.hop_no << endl;
    } else {
	os << "(";
	switch (flit.flit_type) {
	case FLIT_TYPE_HEAD:
	    os << "H";
	    break;
	case FLIT_TYPE_BODY:
	    os << "B";
	    break;
	case FLIT_TYPE_TAIL:
	    os << "T";
	    break;
	}

	os <<  flit.sequence_no << ", " << flit.src_id << "->" << flit.dst_id << " VC " << flit.vc_id << ")";
    }

    return os;
}

inline ostream & operator <<(ostream & os,
			     const ChannelStatus & status)
{
    char msg;
    if (status.available)
	msg = 'A';
    else
	msg = 'N';
    os << msg << "(" << status.free_slots << ")";
    return os;
}

inline ostream & operator <<(ostream & os, const NoP_data & NoP_data)
{
    os << "      NoP data from [" << NoP_data.sender_id << "] [ ";

    for (int j = 0; j < DIRECTIONS; j++)
	os << NoP_data.channel_status_neighbor[j] << " ";

    os << "]" << endl;
    return os;
}
inline ostream & operator <<(ostream & os, const TBufferFullStatus & bfs)
{
    os << "[" ;
    for (int j = 0; j < GlobalParams::n_virtual_channels; j++)
	os << bfs.mask[j] << " ";

    os << "]" << endl;
    return os;
}

inline ostream & operator <<(ostream & os, const Coord & coord)
{
    os << "(" << coord.x << "," << coord.y << ")";

    return os;
}

// Trace overloading

inline void sc_trace(sc_trace_file * &tf, const Flit & flit, string & name)
{
    sc_trace(tf, flit.src_id, name + ".src_id");
    sc_trace(tf, flit.dst_id, name + ".dst_id");
    sc_trace(tf, flit.sequence_no, name + ".sequence_no");
    sc_trace(tf, flit.timestamp, name + ".timestamp");
    sc_trace(tf, flit.hop_no, name + ".hop_no");
}

inline void sc_trace(sc_trace_file * &tf, const NoP_data & NoP_data, string & name)
{
    sc_trace(tf, NoP_data.sender_id, name + ".sender_id");
}
inline void sc_trace(sc_trace_file * &tf, const TBufferFullStatus & bfs, string & name)
{
    for (int j = 0; j < GlobalParams::n_virtual_channels; j++)
	sc_trace(tf, bfs.mask[j], name + "VC "+to_string(j));
}

inline void sc_trace(sc_trace_file * &tf, const ChannelStatus & bs, string & name)
{
    sc_trace(tf, bs.free_slots, name + ".free_slots");
    sc_trace(tf, bs.available, name + ".available");
}

// Misc common functions

inline int xy_distance(const Coord & node1, const Coord & node2)
{
    return abs(node1.x - node2.x) + abs(node1.y - node2.y);
}

inline Coord id2Coord(int id)
{
    Coord coord;

    coord.x = id % GlobalParams::mesh_dim_x;
    coord.y = id / GlobalParams::mesh_dim_x;

    assert(coord.x < GlobalParams::mesh_dim_x);
    assert(coord.y < GlobalParams::mesh_dim_y);

    return coord;
}

inline int coord2Id(const Coord & coord)
{
    int id = (coord.y * GlobalParams::mesh_dim_x) + coord.x;

    assert(id < GlobalParams::mesh_dim_x * GlobalParams::mesh_dim_y);

    return id;
}

inline int node_subnet_id(const Coord & node1)
{
    int subnet_x_coord = int(node1.x /SUBNET_DIM_X);
    int subnet_y_coord = int(node1.y /SUBNET_DIM_Y);
    return (GlobalParams::mesh_dim_x/SUBNET_DIM_X) * subnet_y_coord + subnet_x_coord;
}


inline int hub_id_connected_to_node(int id)
{
    for (auto& t : GlobalParams::hub_for_tile)
    {
        if (t.first == id)
        {
            return t.second;
        }
        
    }
    return -1;
}


inline int nearest_connected_to_hub_node_id(int id)
{
    int nearst_distance= GlobalParams::mesh_dim_x * GlobalParams::mesh_dim_x;
    int result = 0;
    for (auto& t : GlobalParams::hub_for_tile)
    {
        int tmp = xy_distance(id2Coord(id),id2Coord(t.first));
        if (tmp < nearst_distance)
        {
            nearst_distance = tmp;
            result = t.first;
        }
        
    }
    return result;
}



inline int nearest_hub_availibale_node_id_in_subnet(int node_id, int subnet_id)
{
    int nearst_distance= GlobalParams::mesh_dim_x * GlobalParams::mesh_dim_x;
    int result = -1;
    //hub_for_tile - map<node_id,hub_id>
    for (auto& t : GlobalParams::hub_for_tile)
    {
        // get the node subnet id for a node connected to a hub and compare it with given subnet_id or subnet_id of given node_id
        if (node_subnet_id(id2Coord(t.first)) == subnet_id)
        {
            // find the distance between the node connected to hub and given node
            int tmp = xy_distance(id2Coord(node_id),id2Coord(t.first));
           //if (tmp < nearst_distance && GlobalParams::network_hub_can_work.find(t.second)->second && GlobalParams::hub_tx_channel_buffer_free_slots.find(t.second)->second>1)
            if (tmp < nearst_distance)
            {
                nearst_distance = tmp;
                result = t.first;
            }
        
        }
    }
    return result;
}


inline int find_nearest_wireless_connected_node_intra_subnet(int node_id)
{
    return nearest_hub_availibale_node_id_in_subnet(node_id, node_subnet_id(id2Coord(node_id)));
}
/*
inline int find_nearest_wireless_connected_node_in_spare_subnet(int node_id)
{
    int number_of_subnets_in_x_dim = GlobalParams::mesh_dim_x /SUBNET_DIM_X;
    int number_of_subnets_in_y_dim = GlobalParams::mesh_dim_y /SUBNET_DIM_Y;
    bool left_e = true;
    bool right_e = true;
    bool down_e = true;
    bool top_e = true;
    int current_subnet = node_subnet_id(id2Coord(node_id));
    if (current_subnet%number_of_subnets_in_x_dim == 0)
        left_e = false;
    if (current_subnet%number_of_subnets_in_x_dim == number_of_subnets_in_x_dim-1)
        right_e = false;
    if ( current_subnet >= (number_of_subnets_in_x_dim * number_of_subnets_in_y_dim) - number_of_subnets_in_x_dim)
        down_e = false;
    if (current_subnet < number_of_subnets_in_x_dim )
        top_e = false;
    
    
    float top_weight = (float)((int)(node_id/GlobalParams::mesh_dim_x) %SUBNET_DIM_Y)/(SUBNET_DIM_Y-1) ;
    float down_weight = 1-top_weight;
    float right_weight = (float)((int)(node_id%GlobalParams::mesh_dim_x) %SUBNET_DIM_X)/(SUBNET_DIM_X-1);
    float left_weight = 1-right_weight;
    
    if (top_e==false)
        top_weight = -1;
    if (down_e==false)
        down_weight = -1;
    if (right_e==false)
        right_weight = -1;
    if (left_e==false)
        left_weight = -1;
    
    float max_weight = top_weight;
    int result = nearest_hub_availibale_node_id_in_subnet(node_id,current_subnet-number_of_subnets_in_x_dim);
    
    if (down_weight > max_weight)
    {
        max_weight = down_weight;
        int ne = nearest_hub_availibale_node_id_in_subnet(node_id,current_subnet+number_of_subnets_in_x_dim);
        if( ne > -1 ) result = ne;
    }
    
    if (right_weight > max_weight)
    {
        max_weight = right_weight;
        int ne = nearest_hub_availibale_node_id_in_subnet(node_id,current_subnet+1);
        if( ne > -1 ) result = ne;
    }
    
    if (left_weight > max_weight)
    {
        max_weight = left_weight;
        int ne = nearest_hub_availibale_node_id_in_subnet(node_id,current_subnet-1);
        if( ne > -1 ) result = ne;
    }
    if (max_weight == -1)
        result = -1;
    return result;
}


inline int number_of_hubs()
{
    int i=0;
    for (auto& t : GlobalParams::network_hub_can_work)
    {
        i++;
    }
    return i;
}
*/

inline int number_of_connections_to_hubs()
{
    int i=0;
    for (auto& t : GlobalParams::hub_for_tile)
    {
        i++;
    }
    return i;
}

inline int hub_to_hub_distance(int hub_free_slots)
{
    int tmp=number_of_connections_to_hubs();
    double wireless_cycle_per_flit = 1/ ( ((GlobalParams::channel_configuration[0].dataRate)/GlobalParams::flit_size) * (GlobalParams::clock_period_ps/1000) );
    
    int result = 0;
    for(int i=1;i<=tmp;i++)
        result +=i;
    return wireless_cycle_per_flit * (result/tmp);
}

inline bool sameRadioHub(int id1, int id2)
{
    map<int, int>::iterator it1 = GlobalParams::hub_for_tile.find(id1); 
    map<int, int>::iterator it2 = GlobalParams::hub_for_tile.find(id2); 

    assert( (it1 != GlobalParams::hub_for_tile.end()) && "Specified Tile is not connected to any Hub");
    assert( (it2 != GlobalParams::hub_for_tile.end()) && "Specified Tile is not connected to any Hub");

    return (it1->second == it2->second);
}

inline bool hasRadioHub(int id)
{
    map<int, int>::iterator it = GlobalParams::hub_for_tile.find(id);

    return (it != GlobalParams::hub_for_tile.end());
}


inline int tile2Hub(int id)
{
    map<int, int>::iterator it = GlobalParams::hub_for_tile.find(id); 
    assert( (it != GlobalParams::hub_for_tile.end()) && "Specified Tile is not connected to any Hub");
    return it->second;
}

inline int selectChannel(int src_hub, int dst_hub)
{  
    
    vector<int> & first = GlobalParams::hub_configuration[src_hub].txChannels;
    vector<int> & second = GlobalParams::hub_configuration[dst_hub].rxChannels;

    vector<int> intersection;

    for (unsigned int i=0;i<first.size();i++)
    {
    for (unsigned int j=0;j<second.size();j++)
    {
        if (first[i] ==second[j])
        intersection.push_back(first[i]);
    }
    }

    if (intersection.size()==0) return NOT_VALID;

    return intersection[rand()%intersection.size()];

}

inline void printMap(string label, const map<string,double> & m,std::ostream & out)
{
    out << label << " = [" << endl;
    for (map<string,double>::const_iterator i = m.begin();i!=m.end();i++)
	out << "\t" << std::scientific << i->second << "\t % " << i->first << endl;

    out << "];" << endl;
}

template<typename T> std::string i_to_string(const T& t){
         std::stringstream s;
	 s << t;
         return s.str();
}

#endif
