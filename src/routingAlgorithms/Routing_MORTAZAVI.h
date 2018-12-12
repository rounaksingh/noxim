#ifndef __NOXIMROUTING_MORTAZAVI_H__
#define __NOXIMROUTING_MORTAZAVI_H__

#include "RoutingAlgorithm.h"
#include "RoutingAlgorithms.h"
#include "../Router.h"

using namespace std;

class Routing_MORTAZAVI : RoutingAlgorithm {
	public:
		vector<int> route(Router * router, const RouteData & routeData);

		static Routing_MORTAZAVI * getInstance();

	private:
		Routing_MORTAZAVI(){};
		~Routing_MORTAZAVI(){};

		static Routing_MORTAZAVI * routing_MORTAZAVI;
		static RoutingAlgorithmsRegister routingAlgorithmsRegister;
};

#endif
