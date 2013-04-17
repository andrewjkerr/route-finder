/*
 * Author: andrewjkerr | @andrewuf
 * COP3530 Project #4 - main.cpp
 * Section: XXXX
 *
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
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

class ComparePath {
public:
	bool operator()(city* city1, city* city2) {
		if(city1->pathvalue < city2->pathvalue){
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

	/*
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
	*/

	/******************************
	 * 							  *
	 * START DIJKSTRA'S ALGORITHM *
	 * 							  *
	 ******************************/
	// DEBUG VALUES
	string origin = "Italy";
	string destination = "Kazakhstan";
	city* originCity;
	city* destinationCity;

	// Let's grab the origin/destination pointers ;)
	try {
		originCity = cityMap.at(origin);
	}
	catch (const out_of_range& oor) {
		try{
			originCity = countryMap.at(origin);
		}
		catch(const std::out_of_range& oor){
			cerr << "City/country does not exist! (Actual error: " << oor.what() << ")" << endl;
		}
	}

	try {
		destinationCity = cityMap.at(destination);
	}
	catch (const out_of_range& oor) {
		try{
			destinationCity = countryMap.at(destination);
		}
		catch(const std::out_of_range& oor){
			cerr << "City/country does not exist! (Actual error: " << oor.what() << ")" << endl;
		}
	}

	// DEBUG COUTS -- TESTING FINDING CITY POINTERS FROM HASHMAP
	// cout << "Origin name: " << originCity->getCountry() << endl;
	// cout << "Destination name: " << destinationCity->getCountry() << endl;

	/*vector<route*> routesOnTheWay;
	city* currentCity = originCity;
	int i = 0;
	while(currentCity->getCity().compare(destinationCity->getCity()) != 0 && i <= 10){
		currentCity->visitCity();
		vector<route*> currentCityDests = currentCity->getDestinations();
		priority_queue<route*, vector<route*>, CompareCost> test1;
		for(int i = 0; i < currentCityDests.size(); i++){
			test1.push(currentCityDests[i]);
		}
		if(test1.top()->getDestination()->isVisited()){
			test1.pop();
		}
		route* bestRoute = test1.top();
		routesOnTheWay.push_back(bestRoute);
		cout << "Destination City: " << bestRoute->getDestination()->getCountry() << endl;
		currentCity = bestRoute->getDestination();
		i++;
	}*/

	originCity->setPathValue(0);
	priority_queue<city*, vector<city*>, ComparePath> pathQueue;
	int curDistance = 0;
	pathQueue.push(originCity);
	while(!pathQueue.empty()){
		city* temp = pathQueue.top();
		pathQueue.pop();
		if(temp->isVisited()){
			continue;
		}
		if(temp->pathvalue == 99999999.99){
			break;
		}
		vector<route*> tempDest = temp->getDestinations();
		cout << temp->pathvalue << endl;
		for(int i = 0; i < tempDest.size(); i++){
			city* tempDestCity = tempDest[i]->getDestination();
			if(tempDestCity->priorCity == NULL){
				tempDestCity->setPriorCity(temp);
			}
			cout << tempDestCity->priorCity->getCity() << " goes to " << tempDestCity->getCity() << endl;
			double tempDestWeight = tempDest[i]->getAvgTime();
			if((!tempDestCity->isVisited()) && ((temp->pathvalue + tempDestWeight) < tempDestCity->pathvalue)){
				cout << tempDestCity->getCity() << ": "<< temp->pathvalue + tempDestWeight << endl;
				tempDestCity->setPathValue(temp->pathvalue + tempDestWeight);
				pathQueue.push(tempDestCity);
				tempDestCity->setPriorCity(temp);
			}
		}
		temp->visitCity();
	}

	cout << "-------------------" << endl;

	city* currentCity = destinationCity;
	int i = 0;
	while(currentCity != NULL && currentCity != originCity && i <= 10){
		cout << currentCity->getCity() << endl;
		currentCity = currentCity->priorCity;
		i++;
	}

	cout << originCity->getCity() << endl;

	return 0;
}
