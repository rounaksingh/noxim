

// Author : S.H.Mortazavi  (mortazavi91@gmail.com) 
// Combined with Noxim by Rounak Singh Narde <rounaksingh17@gmail.com>

#include <iostream>
#include "Routing_PyROUTING.h"
#include <pybind11/embed.h>
#include <pybind11/stl.h>


namespace py = pybind11;
using namespace py::literals;

RoutingAlgorithmsRegister Routing_PyROUTING::routingAlgorithmsRegister("PyROUTING", getInstance());

Routing_PyROUTING * Routing_PyROUTING::routing_PyROUTING = 0;

Routing_PyROUTING * Routing_PyROUTING::getInstance() {
    if ( routing_PyROUTING == 0 )
        routing_PyROUTING = new Routing_PyROUTING();
    
    return routing_PyROUTING;
}

vector<int> Routing_PyROUTING::route(Router * router, const RouteData & routeData)
{
    // import PyROUTING module from python
    // run the route function with arguments
    py::scoped_interpreter guard{};
    // The module should be at the path of compiled noxim
    // In general the Routing_PyROUTING.py should be located in the bin folder
    py::module rppr = py::module::import("Routing_PyROUTING");
    vector <int> directions;
    directions = rppr.attr("route")(&routeData).cast<vector <int>>();
    return directions;

}
