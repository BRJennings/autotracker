/********************************************************************
* File: README.txt
* Author: Brendan Jennings
* Purpose: This file explains how to use this database application
* 
/*******************************************************************/

Purpose of this Database Application:
	This database application  is to track the inventory of a 
	medium sized car rental agency with multiple offices.

Functionality:
	This application allows the administrator to enter vehicle, 
	and office information into the database itself. It was decided
	that the user of the application would not have the ability to 
	enter new car information. The user has the ability to search for 
	car information on individual models, and to enter and search for
	reservations. Users can also look up inventory available at 
	different offices.

	The user also has the ability to enter new customers into the 
	database. 

Design:
	This application uses Oracle Database Express Edition 11g. It 
	has been implemented in C++, using the OCCI (Oracle C++ Call 
	Interface) library. It requires Oracle X11g, C++, a C++ 
	compiler, and the OCCI library installed.

How to run this program:
	Change directory into the directory the program is stored in 
	and type 'make'. This will create the object file and executable.

	In Oracle, the database tables must be created and populated 
	with the sample data before running the program. After logging
	into Oracle, build the tables by executing the tables.sql
	program; use the command '@tables'. The tables can be populated 
	with sample data after the tables have been built; use the 
	command '@data' to enter sample data into the tables.

	The application can now be run by using the command 
	'./autotracker'.

Testing:
	Testing was carried out on all 6 menu functions and the program 
	as a whole. I created test cases to determine if the system 
	could handle: invalid or garbage input, invalid date formats, 
	and non-existent user information. The application is now able 
	to handle most of these cases.
