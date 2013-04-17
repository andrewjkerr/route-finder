/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - route.cpp
 * Section: XXXX
 *
 */

#include "route.h"
#include "city.h"
#include <stdio.h>
#include <string.h>

using namespace std;

route::route(city* origin, city* destination, string typeOfTransport, double avgTime, double avgCost, string notes){
	this->origin = origin;
	this->destination = destination;
	this->typeOfTransport = typeOfTransport;
	this->actualTime = avgTime;
	this->actualCost = avgCost;
	if(strcmp("plane", typeOfTransport.c_str()) == 0){
		this->flightMultiplier = 99999.99;
	}
	else{
		this->flightMultiplier = 1.0;
	}
	this->avgTime = (this->flightMultiplier * avgTime);
	this->avgCost = (this->flightMultiplier * avgCost);
	this->notes = notes;
}
