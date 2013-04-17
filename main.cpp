/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - main.cpp
 * Section: XXXX
 *
 */

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include "city.h"
#include "route.h"

using namespace std;

class CompareCost {
public:
    bool operator()(route* route1, route* route2) {
    	if(route1->getAvgCost() < route2->getAvgCost()){
    		return false;
    	}

    	else{
    		return true;
    	}
    }
};

class CompareTime {
public:
	bool operator()(route* route1, route* route2) {
		if(route1->getAvgTime() < route2->getAvgTime()){
			return false;
		}

		else{
			return true;
		}
	}
};

int main(){
	ifstream cities;
	cities.open("cities.csv");
	ifstream routes;
	routes.open("routes.csv");
	if(!cities.good()){
		cout << "cities.csv is corrupt!" << endl;
		return 0;
	}

	if(!routes.good()){
		cout << "routes.csv is corrupt!" << endl;
		return 0;
	}

	map<string, city*> cityMap;
	map<string, city*> countryMap;

	while(!cities.eof()){
		string country;
		string cityName;
		string latitude;
		string longitude;
		getline(cities, country, ',');
		getline(cities, cityName, ',');
		getline(cities, latitude, ',');
		getline(cities, longitude);
		if(strcmp(" ", country.c_str()) != 0){
			city* datCity = new city(country, cityName, latitude, longitude);
			cityMap[cityName] = datCity;
			countryMap[country] = datCity;
		}
	}

	while(!routes.eof()){
		string origin;
		string destination;
		string typeOfTransport;
		string avgTime;
		string avgCost;
		string notes;
		getline(routes, origin, ',');
		getline(routes, destination, ',');
		getline(routes, typeOfTransport, ',');
		getline(routes, avgTime, ',');
		getline(routes, avgCost, ',');
		getline(routes, notes);
		if(strcmp(" ", origin.c_str()) != 0){
			city* originCity = cityMap.find(origin)->second;
			city* destCity = cityMap.find(destination)->second;
			route* datRoute = new route(originCity, destCity, typeOfTransport, ::atof(avgTime.c_str()), ::atof(avgCost.c_str()), notes);
			originCity->pb(datRoute);
		}
	}

	// DEBUG COUT TO CONFIRM COMPILING
	cout << "-- COMPILE CONFIRMED --" << endl;

	// DEBUG -- TESTING MAPS
	city* debugCity = cityMap["Abu Dhabi"];
	vector<route* > testVector = debugCity->getDestinations();
	for(int i = 0; i < testVector.size(); i++){
		cout << "---------" << endl;
		cout << testVector[i]->getOrigin()->getCity() << endl;
		cout << testVector[i]->getDestination()->getCity() << endl;
		cout << testVector[i]->getTypeOfTrasport() << endl;
		cout << testVector[i]->getAvgCost() << endl;
		cout << testVector[i]->getActualCost() << endl;
		cout << "--" << endl;
		cout << testVector[i]->getAvgTime() << endl;
		cout << testVector[i]->getActualTime() << endl;
		cout << "\n" << endl;
	}

	// DEBUG -- TESTING PRIORITY QUEUE (COST)
	priority_queue<route*, vector<route* >, CompareCost> costQueue;
	// city* debugCity = cityMap["Abu Dhabi"];
	// vector<route* > testVector = debugCity->getDestinations();
	for(int i = 0; i < testVector.size(); i++){
		costQueue.push(testVector[i]);
	}

	while(!costQueue.empty()){
		route* tempRoute = costQueue.top();
		cout << tempRoute->getDestination()->getCity() << endl;
		costQueue.pop();
	}

	cout << "-------------------" << endl;

	// DEBUG -- TESTING PRIORITY QUEUE (TIME)
	priority_queue<route*, vector<route* >, CompareTime> timeQueue;
	// city* debugCity = cityMap["Abu Dhabi"];
	// vector<route* > testVector = debugCity->getDestinations();
	for(int i = 0; i < testVector.size(); i++){
		timeQueue.push(testVector[i]);
	}

	while(!timeQueue.empty()){
		route* tempRoute = timeQueue.top();
		cout << tempRoute->getDestination()->getCity() << endl;
		timeQueue.pop();
	}

	return 0;
}
