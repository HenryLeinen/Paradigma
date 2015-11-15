#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "ParadigmaData.h"
#include <curl/curl.h>

using namespace Paradigma;

#define		DAEMON_NAME	"HeizungD"
#define		MAX_TIMEOUT	30		//	this counter depends on the sleep value in the main processing loop

/******************************************************
 *
 * Static Data
 *
 *  usb             - Handle for the usb interfaces
 *
 *  dta		    - Monitor Object which parsed
 *                    Data received by the Paradigma
 *
 *  mysql	    - Handle for the SQL database
 */ 
static int usb;
static ParadigmaMonitorData	dta;

const unsigned int ChannelID = 48961;
const char WriteAPIKey[] = "5BXPOG4JUBN8EOEI";



/******************************************************
 * Function   - requestData()
 *
 * Function to request new data stream from Paradigma
 */
void requestData()
{
	serialPutchar(usb, 0x0a);
	serialPutchar(usb, 0x01);
	serialPutchar(usb, 0x14);
	serialPutchar(usb, 0xE1);
}


/*******************************************************
 * Function    - stopData()
 *
 * Function to stop the data stream from Paradigma
 */
void stopData()
{
	serialPutchar(usb, 0x0a);
	serialPutchar(usb, 0x01);
	serialPutchar(usb, 0x18);
	serialPutchar(usb, 0xde);
}

/********************************************************
 * Function      - updateDeviceData()
 *
 * Function writes a new value into a devices output
 */
void updateDeviceDataf(const char* devName, float value)
{
	FILE *f = fopen(devName, "w+");
	if (f != NULL) {
		fprintf(f, "%f", value);
		fclose(f);
	}
}

void updateDeviceDatal(const char* devName, unsigned long value)
{
	FILE *f = fopen(devName, "w+");
	if (f != NULL) {
		fprintf(f, "%ld", value);
		fclose(f);
	}
} 


/********************************************************
 * Callback     - OnDataChanged1()
 *
 * Function will be called whenever the data object 1 of
 * Paradigma is received. Contains mainly the temperatures
 * This function will send the new data to the cloud.
 * This function will also write the new data into our 
 * SQL database.
 */
void OnDataChanged1(void)
{
	char postField[512];
	CURL *curl;
	CURLcode res;
	float aussen_temp;
	float wasser_temp;
	float kesselvorlauf_temp;
	float kesselruecklauf_temp;
	float raumtempHK1_temp, raumtempHK2_temp;
	float vorlauftempHK1_temp, vorlauftempHK2_temp;
	float ruecklauftempHK1_temp, ruecklauftempHK2_temp;
	float pufferoben_temp, pufferunten_temp;

	unsigned long kesselstarts;
	unsigned long  betriebsstunden;
	unsigned char fehler_fuehler;
	unsigned short fehler_kessel;


	/* get the new data from parser */
	aussen_temp = dta.getAussentemp();
	wasser_temp = dta.getWarmwassertemp();
	kesselvorlauf_temp = dta.getKesselvorlauf();
	kesselruecklauf_temp = dta.getKesselruecklauf();
	raumtempHK1_temp = dta.getRaumtemperaturHK1();
	raumtempHK2_temp = dta.getRaumtemperaturHK2();
	vorlauftempHK1_temp = dta.getVorlauftemperaturHK1();
	vorlauftempHK2_temp = dta.getVorlauftemperaturHK2();
	ruecklauftempHK1_temp = dta.getRuecklauftemperaturHK1();
	ruecklauftempHK2_temp = dta.getRuecklauftemperaturHK2();
	pufferoben_temp = dta.getPuffertemperaturOben();
	pufferunten_temp = dta.getPuffertemperaturUnten();

	kesselstarts = dta.getAnzahlKesselstarts();
	betriebsstunden = dta.getBetriebsstundenKessel();
	fehler_fuehler = dta.getStoercodeFuehler();
	fehler_kessel = dta.getStoercodeKessel();


	/* Update device values */
	updateDeviceDataf("/dev/paradigma/AussenTemp", aussen_temp);
	updateDeviceDataf("/dev/paradigma/WasserTemp", wasser_temp);
	updateDeviceDataf("/dev/paradigma/Kesselvorlauf", kesselvorlauf_temp);
	updateDeviceDataf("/dev/paradigma/Kesselruecklauf", kesselruecklauf_temp);
	updateDeviceDataf("/dev/paradigma/RaumTempHK1", raumtempHK1_temp);
	updateDeviceDataf("/dev/paradigma/RaumTempHK2", raumtempHK2_temp);
	updateDeviceDataf("/dev/paradigma/VorlaufTempHK1", vorlauftempHK1_temp);
	updateDeviceDataf("/dev/paradigma/VorlaufTempHK2", vorlauftempHK2_temp);
	updateDeviceDataf("/dev/paradigma/RuecklaufTempHK1", ruecklauftempHK1_temp);
	updateDeviceDataf("/dev/paradigma/RuecklaufTempHK2", ruecklauftempHK2_temp);
	updateDeviceDataf("/dev/paradigma/PufferTemperaturOben", pufferoben_temp);
	updateDeviceDataf("/dev/paradigma/PufferTemperaturUnten", pufferunten_temp);

	/* initialize the curl library */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl)
	{
		/* First set the url that is about to receive our POST. */
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.thingspeak.com/update");
		/* Now specify the POST data */
		sprintf(postField, "api_key=%s&field1=%.2f&field4=%.2f&field5=%.2f&field6=%.2f&field2=%6ld&field3=%6ld&field7=%3d&field8=%3d", 
			WriteAPIKey, 
			aussen_temp, 
			wasser_temp,
			kesselvorlauf_temp,
			kesselruecklauf_temp,
			betriebsstunden,
			kesselstarts,
			fehler_fuehler,
			fehler_kessel);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postField);

		/* Perform the request */
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			syslog(LOG_ERR, "curl_easy_perform() failed: %s", curl_easy_strerror(res));
		}
		else
		{
			syslog(LOG_INFO, "Successfully sent data to the cloud !");
		}
		curl_easy_cleanup(curl);
	} else
	{
		syslog(LOG_ERR, "Failed to post request to cloud.");
	}
	curl_global_cleanup();
}

/*************************************************************
 * Callback    - OnDataChanged2()
 *
 * Function will be called whenever the data object 2 of
 * Paradigma is received. Contains mainly counters.
 * This function will send the new data to the cloud.
 */
void OnDataChanged2(void)
{
	unsigned long kesselstarts;
	unsigned long  betriebsstunden;
	unsigned char fehler_fuehler;
	unsigned short fehler_kessel;

	kesselstarts = dta.getAnzahlKesselstarts();
	betriebsstunden = dta.getBetriebsstundenKessel();
	fehler_fuehler = dta.getStoercodeFuehler();
	fehler_kessel = dta.getStoercodeKessel();

	updateDeviceDatal("/dev/paradigma/Betriebsstunden", betriebsstunden);
	updateDeviceDatal("/dev/paradigma/KesselStarts", kesselstarts);
	updateDeviceDatal("/dev/paradigma/FehlerKessel", fehler_kessel);
	updateDeviceDatal("/dev/paradigma/FehlerFuehler", fehler_fuehler);

#if 0
	char postField[512];
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.thingspeak.com/update");
		sprintf(postField, "api_key=%s&field2=%6ld&field3=%6ld&field7=%3d&field8=%3d",
			WriteAPIKey,
			(int)dta.getBetriebsstundenKessel(),
			(int)dta.getAnzahlKesselstarts(),
			(int)dta.getStoercodeFuehler(),
			(int)dta.getStoercodeKessel());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postField);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			syslog(LOG_ERR, "curl_easy_perform() failed: %s", curl_easy_strerror(res));
		}
		else
		{
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
 	}
	else
	{
		syslog(LOG_ERR, "Failed to post request for data2 update to the cloud !");
	}
	curl_global_cleanup();
#endif
}


bool daemon_init()
{
	syslog(LOG_INFO, "Initializing damon...");

	/* Initialize the wiringPi Library */
	wiringPiSetup();

	/* Attach callbackfunctions to data parser */
	dta.attach1(OnDataChanged1);
	dta.attach2(OnDataChanged2);

	syslog(LOG_INFO, "Trying opening USB device ...");
	usb = serialOpen("/dev/ttyUSB0", 9600);
	if (usb == -1)
	{
		syslog(LOG_ERR, "Failed to open USB device !");
		return false;
	}
	syslog(LOG_INFO, "Requesting data from Paradigma");
	requestData();

	syslog(LOG_INFO, "Initialization done ok");

	return true;
}

void daemon_process()
{
	static int cnt = 0;
	static int cnt_total = 0;

	int nAvail = serialDataAvail(usb);

	if (nAvail <= 0)
	{
		/* no data received this time */
		cnt++;
		cnt_total++;
		if (cnt > MAX_TIMEOUT)
		{
			/* too often no response, so request data again */
			syslog(LOG_WARNING, "Paradigma data timeout. No response received since %ld slices. Requesting again.", cnt_total);
			requestData();
			cnt = 0;
		}
	}
	else
	{
		cnt = 0;
		cnt_total = 0;
		while(nAvail--)
		{
			int s = serialGetchar(usb);
			dta << s;
		}
	}
}


int main(int argc, char *argl[])
{
	/* Initialize the log file recording */
#ifdef DEBUG
	setlogmask(LOG_UPTO(LOG_INFO));
#else
	setlogmask(LOG_UPTO(LOG_WARNING));
#endif
	openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_DAEMON);

	syslog(LOG_INFO, "Entering daemon...");

	pid_t pid, sid;

	/* fork the current process */
	pid = fork();

	/* Check if something went wrong */
	if (pid < 0)
	{
		syslog(LOG_PERROR, "Failed to fork process !");
		printf("Failed to fork process !\n");
		exit (EXIT_FAILURE);
	}

	/* Check if we received a good pid */
	if (pid > 0)
	{
		printf("Daemon is running with pid %ld!\n", pid);

		/* Write our pidfile */
		if (argc > 1)
		{
			FILE *pidFile = fopen(argl[1], "w");
			if (pidFile != NULL)
			{
				fprintf(pidFile, "%d", pid);
				fclose(pidFile);
			} 
			else
			{
				syslog(LOG_WARNING, "Unable to write  PIDFILE !");
			}
		}
		exit (EXIT_SUCCESS);
	}

	umask(0);

	/* create a new signature for our child */
	sid = setsid();
	if (sid < 0 )
		exit (EXIT_FAILURE);

	/* Change the working directory */
	if (chdir("/")  < 0)
		exit (EXIT_FAILURE);

	/* close the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* run the preprocessing of the daemon */
	if (!daemon_init())
		exit (EXIT_FAILURE);

	/* We will now start the processing of the maintask of the daemon */
	while (true)
	{
		daemon_process();
		sleep(1);
	}

	stopData();
	serialClose(usb);

	/* And finally close the log */
	closelog();
	return 0;
}
