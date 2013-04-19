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

void clearCost(priority_queue<route*, vector<route*>, CompareCost> &pq)
{
	priority_queue<route*, vector<route*>, CompareCost> empty;
	swap(pq, empty);
}

void clearTime(priority_queue<route*, vector<route*>, CompareTime> &pq)
{
	priority_queue<route*, vector<route*>, CompareTime> empty;
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
	string outputFileName = "sampleOutputItalyKazakhstanCheapest.html";
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

	vector<route*> correctRoute;
	while(!routeFound.empty()){
		city* previousMarker = routeFound.top();
		routeFound.pop();
		city* currentMarker = routeFound.top();
		if(strcmp("fastest", compSwitch.c_str()) == 0){
			priority_queue<route*, vector<route*>, CompareTime> pathQueue;
		}
		else if(strcmp("cheapest", compSwitch.c_str()) == 0){
			priority_queue<route*, vector<route*>, CompareCost> pathQueue;
		}
		else{
			cout << "Please select either 'fastest' or 'cheapest'" << endl;
			return 0;
		}
		// clear(pathQueue);
		for(int i = 0; i < previousMarker->getDestinations().size(); i++){
			if(previousMarker->getDestinations()[i]->getDestination() == currentMarker){
				pathQueue.push(previousMarker->getDestinations()[i]);
			}
		}
		if(!pathQueue.empty()){
			correctRoute.push_back(pathQueue.top());
		}
	}

	ofstream htmlOutput;
	htmlOutput.open(outputFileName);
	if(htmlOutput.is_open()){
		htmlOutput << "<html>\n";
		htmlOutput << "<head>\n";
		compSwitch[0] = toupper(compSwitch[0]);
		htmlOutput << "<title>" << compSwitch << " path from " << origin << " to " << destination << "</title></head>\n";
		htmlOutput << "<script type='text/javascript' src='http://maps.google.com/maps/api/js?sensor=false'></script>\n";
		htmlOutput << "<script>function initialize() { \n";
		htmlOutput << "var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP}; \n";
		htmlOutput << "var map=new google.maps.Map(document.getElementById('map'), myOptions); \n";
		int markerNum = 0;
		for(int i = 0; i < correctRoute.size(); i++){
			htmlOutput << "var marker" << markerNum << " = new google.maps.Marker({ position: new google.maps.LatLng("<< correctRoute[i]->getOrigin()->getLatitude() << ", " << correctRoute[i]->getOrigin()->getLongitude() << "), map: map, title:'" << correctRoute[i]->getOrigin()->getCity() << ", " << correctRoute[i]->getOrigin()->getCountry() << "'});"  << "\n";
			markerNum++;
			htmlOutput << "var marker" << markerNum << " = new google.maps.Marker({ position: new google.maps.LatLng("<< correctRoute[i]->getDestination()->getLatitude() << ", " << correctRoute[i]->getDestination()->getLongitude() << "), map: map, title:'" << correctRoute[i]->getDestination()->getCity() << ", " << correctRoute[i]->getDestination()->getCountry() << "'});"  << "\n";
			markerNum++;
			htmlOutput << "var contentString" << i << " = \"" << correctRoute[i]->getOrigin()->getCity() << ", " << correctRoute[i]->getOrigin()->getCountry() << " --> " << correctRoute[i]->getDestination()->getCity() << ", " << correctRoute[i]->getDestination()->getCountry() << " (" << correctRoute[i]->getTypeOfTransport() << " - " << correctRoute[i]->getActualTime() << " hours - $" << correctRoute[i]->getActualCost() << ")\";" << "\n";
			htmlOutput << "var path" << i << " = new google.maps.Polyline({ path: [new google.maps.LatLng(" << correctRoute[i]->getOrigin()->getLatitude() << ", " << correctRoute[i]->getOrigin()->getLongitude() << "), new google.maps.LatLng("<< correctRoute[i]->getDestination()->getLatitude() << ", " << correctRoute[i]->getDestination()->getLongitude() << ")], strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2});" << "\n";
			htmlOutput << "path" << i << ".setMap(map);\n";
			htmlOutput << "google.maps.event.addListener(path" << i << ", 'click', function(event) { alert(contentString" << i << "); });\n";
		}
		htmlOutput << "} google.maps.event.addDomListener(window, 'load', initialize);\n";
		htmlOutput << "</script>";
		htmlOutput << "</head>\n";
		htmlOutput << "<body><div id='map' style='width:100%;height:100%;'></div></body>\n";
		htmlOutput << "</html>\n";
	}
	htmlOutput.close();

	cout << "WOOT WROTE TO FILE!" << endl;

	return 0;
}
