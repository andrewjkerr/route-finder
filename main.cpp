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
#include <stdlib.h>
#include <string.h>
#include <map>
#include "city.h"
#include "route.h"

using namespace std;

// Define some global maps - one for city name as a key and one for country name as a key
// Why use both? Because the input can be either a city or a country!
map<string, city*> cityMap;
map<string, city*> countryMap;

// Comparison classes for priority queues!
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

// Uses Dijkstra's Algorithm to find the best route depending on whether the user wants
// the fastest or cheapest route.
stack<city*> dijkstra(string origin, string destination, string compSwitch){
	/******************************
	 * 							  *
	 * START DIJKSTRA'S ALGORITHM *
	 * 							  *
	 ******************************/
	city* originCity;
	city* destinationCity;

	// Let's grab the origin/destination pointers ;)
	// Checks if a city name is passed in; if not then checks if it's the country name.
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

	// Sets the path value of the root node to 0
	originCity->setPathValue(0);

	// Creates a min heap comparing the path value
	priority_queue<city*, vector<city*>, ComparePath> pathQueue;
	pathQueue.push(originCity);

	// Loop that runs until there's no where else to go!
	while(!pathQueue.empty()){
		city* temp = pathQueue.top();
		pathQueue.pop();

		if(temp->isVisited()){
			continue;
		}
		if(temp->pathvalue == 99999999.99){
			break;
		}

		// Get the destinations for the current node
		vector<route*> tempDest = temp->getDestinations();

		for(int i = 0; i < tempDest.size(); i++){
			city* tempDestCity = tempDest[i]->getDestination();

			// Sets the prior city pointer just in case it doesn't get set in the if statement!
			if(tempDestCity->priorCity == NULL){
				tempDestCity->setPriorCity(temp);
			}

			// Calculates the weight of the edge (route) based off of what the user wants
			double tempDestWeight;
			if(strcmp("fastest", compSwitch.c_str()) == 0){
				tempDestWeight = tempDest[i]->getAvgTime();
			}
			else if(strcmp("cheapest", compSwitch.c_str()) == 0){
				tempDestWeight = tempDest[i]->getAvgCost();
			}
			else{
				cout << "Please select either 'fastest' or 'cheapest'" << endl;
				exit(0);
			}

			// Checks whether: 1) the node was already visited and
			// 2) whether the pathvalue of origin + weight is greater than the pathvalue of the destination
			if((!tempDestCity->isVisited()) && ((temp->pathvalue + tempDestWeight) < tempDestCity->pathvalue)){

				// Changes the pathvalue of the destination
				tempDestCity->setPathValue(temp->pathvalue + tempDestWeight);

				// Push the destination onto the priority queue
				pathQueue.push(tempDestCity);

				// And set the previous city to the origin!
				tempDestCity->setPriorCity(temp);
			}
		}

		// Set the visit bool to true
		temp->visitCity();
	}

	// Goes through the queue and puts the cities "right side up" in a stack
	stack<city*> routeFound;
	city* currentCity = destinationCity;
	while(currentCity != NULL && currentCity != originCity){
		routeFound.push(currentCity);
		currentCity = currentCity->priorCity;
	}
	routeFound.push(originCity);

	// And return the stack to calculate the route!
	return routeFound;
}


// Calculates the correct route that was taken by Dijkstra's Algorithm based on what the user wants.
vector<route*> calculateRoute(stack<city*> routeFound, string compSwitch){
	vector<route*> correctRoute;
	if(strcmp("fastest", compSwitch.c_str()) == 0){

		// Can't use !routeFound.empty() because of seg faults!
		while(routeFound.size() > 1){
			// Sets the origin
			city* previousMarker = routeFound.top();
			routeFound.pop();

			// Sets the destination
			city* currentMarker = routeFound.top();

			// Creates the min heap based on what the user wants
			priority_queue<route*, vector<route*>, CompareTime> pathQueue;
			for(int i = 0; i < previousMarker->getDestinations().size(); i++){
				if(previousMarker->getDestinations()[i]->getDestination() == currentMarker){
					pathQueue.push(previousMarker->getDestinations()[i]);
				}
			}
			if(!pathQueue.empty()){
				// Since the priority queue sorts by what the user wants, it will be the
				// cheapest or fastest route on top!
				correctRoute.push_back(pathQueue.top());
			}
		}
	}

	if(strcmp("cheapest", compSwitch.c_str()) == 0){
		while(routeFound.size() > 1){
			city* previousMarker = routeFound.top();
			routeFound.pop();
			city* currentMarker = routeFound.top();
			priority_queue<route*, vector<route*>, CompareCost> pathQueue;
			for(int i = 0; i < previousMarker->getDestinations().size(); i++){
				if(previousMarker->getDestinations()[i]->getDestination() == currentMarker){
					pathQueue.push(previousMarker->getDestinations()[i]);
				}
			}
			if(!pathQueue.empty()){
				correctRoute.push_back(pathQueue.top());
			}
		}
	}

	return correctRoute;
}

void genHTML(string origin, string compSwitch, string destination, string outputFileName, vector<route*> correctRoute){
	// Generate the HTML for the Google Map!
	ofstream htmlOutput;
	htmlOutput.open(outputFileName.c_str());
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
}

int main(int argc, char* argv[]){
	// The following is disabled for Eclipse!
	// FYI -- Need to add command line args later~
	if(argc != 7){
		cout << "You haven't entered enough command line arguments!" << endl;
		return 0;
	}

	string citiesCSV = argv[1];
	string routesCSV = argv[2];

	//string citiesCSV = "cities_big.csv";
	//string routesCSV = "routes_big.csv";

	ifstream cities;
	cities.open(citiesCSV.c_str());
	ifstream routes;
	routes.open(routesCSV.c_str());
	if(!cities.good()){
		cout << "Your cities csv file is corrupt!" << endl;
		return 0;
	}

	if(!routes.good()){
		cout << "Your routes csv file is corrupt!" << endl;
		return 0;
	}

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

	// Set the command line arguments as variables
	string origin = argv[4];
	string destination = argv[5];
	string compSwitch = argv[6];
	string outputFileName = argv[3];

	stack<city*> routeFound = dijkstra(origin, destination, compSwitch);
	vector<route*> correctRoute = calculateRoute(routeFound, compSwitch);
	genHTML(origin, compSwitch, destination, outputFileName, correctRoute);

	return 0;
}
