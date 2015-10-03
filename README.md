# Paradigma
Program to read the paradigma heating data and sending it to a sensor cloud service using raspberry pi.

This requires of course a service interfaces which can translate the paradigma bus messages onto USB.

I use a paradigma pelleti mini.

Required libraries:
	CURL4 			- required to communicate HTTP-POSTs with cloud
	sysklogd 		- required to use syslog service
	libmysqlclient-dev	- for writing into the mysql database

# INSTALLATION
This program is expected to run as a service in the background. Therefore it will start the communication
with the Paradigma device every 15 mins and query for data. Once all data sets are being received, the
communication is stopped and the program goes idle.

In future releases I want to implement the possibility to modify the temperature and to initiat a one-time-start
of water heating. This is currently not yet implemented.

To install the program as a service and to run the program as a service I have included a small script which will
perform the following activities :
	1. Stop any running instance of the heizung service/daemon
	2. Create the required folder for the executable
	3. Copy the binary heizungd - which is the service or daemon executable - to the right system folder
	4. Start the service/daemon.

# MAKE
Just type make on the commandline and all the files will be compiled and linked
