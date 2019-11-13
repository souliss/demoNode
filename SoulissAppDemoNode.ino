/**************************************************************************
	Souliss - DEMO NODE

	demo node shown in soulissApp

	Refer to Souliss Wiki to know about souliss framework APIs and functions used below
	tested w/ souliss v7.2-beta.1 and arduino 1.8.10
	on Olimex ESP8266EVB boards, using Visual Studio community edition 2019

	shine@angelic.it
	11/2019

***************************************************************************/



//DHT is actually optional and enables real sensor readings
#include <GetConfig.h>
#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHTPIN 11
DHT dht(DHTPIN, DHTTYPE);

// Configure the framework
#include <SoulissFramework.h>

#include "bconf/Olimex_ESP8266EVB.h"    // Load the code directly on the ESP8266
#include "conf/Gateway.h"               // The main node is the Gateway, we have just one node
#include "conf/DynamicAddressing.h"


// Define the WiFi name and password
#define WIFICONF_INSKETCH
#define WiFi_SSID               "yourSSID"
#define WiFi_Password           "yourown"    

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>

/*** All configuration includes should be above this line ***/
#include "Souliss.h"

// This identify theonly real device (relay)
#define RELAY_PIN           5

// SLOTs (fake, test purposes)
#define MYLEDLOGIC				0 
#define USED_SLOT1_N1			1
#define USED_SLOT3_N1			3
#define USED_SLOT5_N1			5
#define USED_SLOT7_N1			7
#define HUM						9// Leave 2 slots for sensor
#define TEMP0					11// Leave 2 slots for sensor
#define USED_SLOT13_N1          13

void setup()
{
	Serial.begin(115200);
	Initialize();

	// Connect to the WiFi network and get an address from DHCP
	GetIPAddress();

	Serial.println(myvNet_dhcp);
	SetAsGateway(myvNet_dhcp);// Set this node as gateway for SoulissApp  

	// This node will serve all the others in the network providing an address
	SetAddressingServer();

	Set_T14(MYLEDLOGIC);
	Set_T12(USED_SLOT1_N1);
	Set_T32(USED_SLOT3_N1);
	Set_T16(USED_SLOT5_N1);
	Set_T15(USED_SLOT7_N1);
	Set_T19(USED_SLOT13_N1);
	Set_Humidity(HUM);
	Set_Temperature(TEMP0);
	pinMode(RELAY_PIN, OUTPUT); // Use pin 5 as output 
}

void loop()
{
	// Here we start to play
	EXECUTEFAST() {

		UPDATEFAST();

		FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
			Logic_T14(MYLEDLOGIC);
			//only one 'real' device
			DigOut(RELAY_PIN, Souliss_T1n_Coil, MYLEDLOGIC);
		}

		FAST_110ms() {//less frequent update
			Logic_T12(USED_SLOT1_N1);
			Logic_T32(USED_SLOT3_N1);
			Logic_T16(USED_SLOT5_N1);
			Logic_T15(USED_SLOT7_N1);
			Logic_T19(USED_SLOT13_N1);
			Logi
		}

		// Here we handle communications
		FAST_GatewayComms();
	}
	EXECUTESLOW()
	{
		UPDATESLOW();

		SLOW_10s() {
			Logic_Humidity(HUM);
			Logic_Temperature(TEMP0);

		}

		SLOW_50s() {
			float h = 65;//dht.readHumidity();
			// Read temperature as Celsius (the default)
			float t = 22;// dht.readTemperature();
			ImportAnalog(HUM, &h);
			ImportAnalog(TEMP0, &t);
		}
	}
}
