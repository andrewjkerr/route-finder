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
	this->actualCost = avgCost;
	if(strcmp("plane", typeOfTransport.c_str()) == 0){
		this->flightMultiplier = 9999999.99;
	}
	else{
		this->flightMultiplier = 1.0;
	}
	this->avgCost = (this->flightMultiplier * avgCost);
	this->avgTime = avgTime;
	this->notes = notes;
}