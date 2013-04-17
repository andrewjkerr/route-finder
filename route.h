/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - route.h
 * Section: XXXX
 *
 */

#ifndef ROUTE_H
#define ROUTE_H

#include<string>
#include "city.h"

using namespace std;

class city;

class route{
public:
	route(city* origin, city* destination, string typeOfTransport, double avgTime, double avgCost, string notes);
	city* getOrigin(){return origin;}
	city* getDestination() {return destination;}
	string getTypeOfTrasport() {return typeOfTransport;}
	double getActualCost() {return actualCost;}
	double getActualTime() {return actualTime;}
	double getAvgTime() {return avgTime;}
	double getAvgCost() {return avgCost;}
	string getNotes() {return notes;}

private:
	city* origin;
	city* destination;
	string typeOfTransport, notes;
	double avgTime, avgCost, flightMultiplier, actualCost, actualTime;
};

#endif
