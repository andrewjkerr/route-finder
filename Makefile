###########################################
# Author: andrewjkerr
# COP3530 Project #4
# Due Date: Friday April 19, 2013
###########################################

Project4 : main.o city.o route.o
	g++ -o Project4 main.o city.o route.o
       
main.o : main.cpp city.h route.h
	g++ -c main.cpp
 
city.o : city.cpp city.h route.h
	g++ -c city.cpp
 
route.o : route.cpp city.h route.h
	g++ -c route.cpp
