

// Author : S.H.Mortazavi  (mortazavi91@gmail.com) 
// Combined with Noxim by Rounak Singh Narde <rounaksingh17@gmail.com>

#include "Routing_MORTAZAVI.h"
#include "../Utils.h"

RoutingAlgorithmsRegister Routing_MORTAZAVI::routingAlgorithmsRegister("MORTAZAVI", getInstance());

Routing_MORTAZAVI * Routing_MORTAZAVI::routing_MORTAZAVI = 0;

Routing_MORTAZAVI * Routing_MORTAZAVI::getInstance() {
    if ( routing_MORTAZAVI == 0 )
        routing_MORTAZAVI = new Routing_MORTAZAVI();
    
    return routing_MORTAZAVI;
}

vector<int> Routing_MORTAZAVI::route(Router * router, const RouteData & routeData)
{
    Coord current = id2Coord(routeData.current_id);
    Coord destination = id2Coord(routeData.dst_id);
    vector <int> directions;
    
    //if (GlobalParams::verbose_mode > VERBOSE_OFF) 
        RLOG << "src_node: " << id2Coord(routeData.src_id) << " dst_node: " << destination << " current_node: " << current << endl;
    
    double delta = 6;
    int current_subnet_id = node_subnet_id(current);
    int destination_subnet_id = node_subnet_id(destination);
    
    if (current_subnet_id!=destination_subnet_id)
    {
        int nearest_wireless_connected_node_in_current_subnet = find_nearest_wireless_connected_node_intra_subnet(routeData.current_id);
        int nearest_wireless_connected_node_in_dst_subnet = find_nearest_wireless_connected_node_intra_subnet(routeData.dst_id);
        
        if ( nearest_wireless_connected_node_in_current_subnet != -1 && nearest_wireless_connected_node_in_dst_subnet != -1 )
        {
            // wired connection distance from source node to the node connected to hub
            int current_to_wireless_hub_distance = xy_distance(current,id2Coord(nearest_wireless_connected_node_in_current_subnet));
            
            // wired connection distance the node connected to hub in dest subnet to dest node
            int wireless_hub_to_destination_distance = xy_distance(id2Coord(nearest_wireless_connected_node_in_dst_subnet),destination);
            
            //wired distance
            int h_w = current_to_wireless_hub_distance+wireless_hub_to_destination_distance;
            // distance if flit sent from source to dest using wired
            int h_b = xy_distance( current, destination);
            if ((h_w + delta < h_b) && GlobalParams::use_winoc)
            {
                //cout<< h_w << " " << h_b << endl;
                if (hasRadioHub(routeData.current_id)){
                    vector<int> dirv;
                    dirv.push_back(DIRECTION_HUB);
                    return dirv;
                }
                Coord near_wireless_node = id2Coord(nearest_wireless_connected_node_in_current_subnet);
                
                if (near_wireless_node.x > current.x)
                    directions.push_back(DIRECTION_EAST);
                else if (near_wireless_node.x < current.x)
                    directions.push_back(DIRECTION_WEST);
                else if (near_wireless_node.y > current.y)
                    directions.push_back(DIRECTION_SOUTH);
                else
                    directions.push_back(DIRECTION_NORTH);
                return directions;
            }
        }
    }
    
    
    if (destination.x > current.x)
        directions.push_back(DIRECTION_EAST);
    else if (destination.x < current.x)
        directions.push_back(DIRECTION_WEST);
    else if (destination.y > current.y)
        directions.push_back(DIRECTION_SOUTH);
    else
        directions.push_back(DIRECTION_NORTH);
    
    return directions;
}
