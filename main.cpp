/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - main.cpp
 * Section: XXXX
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include "city.h"
#include "route.h"

using namespace std;

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
		cout << "\n" << endl;
	}
	return 0;
}
