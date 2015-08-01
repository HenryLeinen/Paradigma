# Paradigma
Program to read the paradigma heating data and sending it to a sensor cloud service using raspberry pi.

This requires of course a service interfaces which can translate the paradigma bus messages onto USB.

I use a paradigma pelleti mini.

Required libraries:
	CURL4 - required to communicate HTTP-POSTs with cloud
	sysklogd - required to use syslog service
