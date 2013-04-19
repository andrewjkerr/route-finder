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
#include <stack>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include "city.h"
#include "route.h"

using namespace std;

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

void clear(priority_queue<route*, vector<route*>, CompareCost> &pq)
{
	priority_queue<route*, vector<route*>, CompareCost> empty;
	swap(pq, empty);
}

int main(int argc, char* argv[]){
	// The following is disabled for Eclipse!
	// FYI -- Need to add command line args later~
	/*if(argc != 7){
		cout << "You haven't entered enough command line arguments!" << endl;
		return 0;
	}*/
	ifstream cities;
	cities.open("cities_old.csv");
	ifstream routes;
	routes.open("routes_old.csv");
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

	// PARSING DOSE CITIES BRO
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

	// MOAR PARSING BUT WITH ROUTES BRO
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

	cout << "-- COMPILE CONFIRMED --" << endl;

	/******************************
	 * 							  *
	 * START DIJKSTRA'S ALGORITHM *
	 * 							  *
	 ******************************/
	// DEBUG VALUES
	string origin = "Italy";
	string destination = "Kazakhstan";
	string compSwitch = "cheapest";
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

	originCity->setPathValue(0);
	priority_queue<city*, vector<city*>, ComparePath> pathQueue;
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
		for(int i = 0; i < tempDest.size(); i++){
			city* tempDestCity = tempDest[i]->getDestination();
			if(tempDestCity->priorCity == NULL){
				tempDestCity->setPriorCity(temp);
			}
			double tempDestWeight;
			if(strcmp("fastest", compSwitch.c_str()) == 0){
				tempDestWeight = tempDest[i]->getAvgTime();
			}
			else if(strcmp("cheapest", compSwitch.c_str()) == 0){
				tempDestWeight = tempDest[i]->getAvgCost();
			}
			else{
				cout << "Please select either 'fastest' or 'cheapest'" << endl;
				return 0;
			}
			if((!tempDestCity->isVisited()) && ((temp->pathvalue + tempDestWeight) < tempDestCity->pathvalue)){
				tempDestCity->setPathValue(temp->pathvalue + tempDestWeight);
				pathQueue.push(tempDestCity);
				tempDestCity->setPriorCity(temp);
			}
		}
		temp->visitCity();
	}

	stack<city*> routeFound;
	city* currentCity = destinationCity;
	while(currentCity != NULL && currentCity != originCity){
		routeFound.push(currentCity);
		currentCity = currentCity->priorCity;
	}

	routeFound.push(originCity);
	while(!routeFound.empty()){
		city* previousMarker = routeFound.top();
		routeFound.pop();
		city* currentMarker = routeFound.top();
		priority_queue<route*, vector<route*>, CompareCost> pathQueue;
		clear(pathQueue);
		for(int i = 0; i < previousMarker->getDestinations().size(); i++){
			if(previousMarker->getDestinations()[i]->getDestination() == currentMarker){
				pathQueue.push(previousMarker->getDestinations()[i]);
			}
		}
		if(!pathQueue.empty()){
			cout << previousMarker->getCity() << ", " << previousMarker->getCountry() << " -> " << pathQueue.top()->getDestination()->getCity() << ", " << pathQueue.top()->getDestination()->getCountry();
			cout << "(" << pathQueue.top()->getTypeOfTransport() << " - " << pathQueue.top()->getActualTime() << " hours - $" << pathQueue.top()->getActualCost() << ")" << endl;
		}
	}

	return 0;
}
