/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - city.cpp
 * Section: XXXX
 *
 */

#include<vector>
#include "route.h"
#include "city.h"

using namespace std;

class route;

city::city(string country, string cityName, string latitude, string longitude){
	this->country = country;
	this->cityName = cityName;
	this->latitude = latitude;
	this->longitude = longitude;
}

void city::pb(route* datRoute){
	destinationRoutes.push_back(datRoute);
}
