/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - city.h
 * Section: XXXX
 *
 */

#ifndef CITY_H
#define CITY_H

#include<string>
#include<vector>
#include "route.h"

using namespace std;

class route;

class city{
public:
	city(string country, string cityName, string latitude, string longitude);
	string getCountry(){return country;}
	string getCity() {return cityName;}
	string getLatitude() {return latitude;}
	string getLongitude() {return longitude;}
	bool isVisited() {return visited;}
	void visitCity(){this->visited = true;}
	vector<route* > getDestinations() {return destinationRoutes;}
	void pb(route* datRoute);
	void setPathValue(double pathValue){this->pathvalue = pathValue;}
	void setPriorCity(city* priorCity) {this->priorCity = priorCity;}
	double pathvalue;
	city* priorCity;

private:
	string country, cityName, latitude, longitude;
	vector<route*> destinationRoutes;
	bool visited;
};

#endif
