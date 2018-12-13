#ifndef __NOXIMROUTING_PyROUTING_H__
#define __NOXIMROUTING_PyROUTING_H__

#include "RoutingAlgorithm.h"
#include "RoutingAlgorithms.h"
#include "../Router.h"

using namespace std;

class Routing_PyROUTING : RoutingAlgorithm {
	public:
		vector<int> route(Router * router, const RouteData & routeData);

		static Routing_PyROUTING * getInstance();

	private:
		Routing_PyROUTING(){};
		~Routing_PyROUTING(){};

		static Routing_PyROUTING * routing_PyROUTING;
		static RoutingAlgorithmsRegister routingAlgorithmsRegister;
};

#endif
