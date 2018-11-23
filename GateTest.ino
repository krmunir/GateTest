// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       GateTest.ino
    Created:	Fri 23 Nov 2018 12:43:17 PM
    Author:     COHDA\kmunir
*/

/*******************************************************

This code runs on a separate NodeMCU (or similar) and provides :
- serial output simulating signal from SlidingGateSensor whilst opening and closing
- led outputs indicating opening / closing

*******************************************************/

/********** #Includes **********/
#include <SoftwareSerial\SoftwareSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/********** Pin Assignments **********/
//Outputs
constexpr auto CLOSING_LED_PIN = 99;
constexpr auto OPENING_LED_PIN = 99;
constexpr auto SERIAL_RX = 99;
constexpr auto SERIAL_TX = 99;

//Inputs


/********** Constants **********/
const unsigned short gateClosedReading = 7000;
const unsigned short gateOpenReading = 1500;


/********** Forward Declarations **********/
char* concat(const char *s1, const char *s2);
SoftwareSerial slidingGateDistanceSensorSerial;


/********** Main Program Code  **********/
// The setup() function runs once each time the micro-controller starts
void setup()
{
	SoftwareSerial slidingGateDistanceSensorSerial(SERIAL_RX, SERIAL_TX, true); //sig from sensor is inverted
	pinMode(CLOSING_LED_PIN, OUTPUT);
	pinMode(OPENING_LED_PIN, OUTPUT);

}

// Add the main program code into the continuous loop() function
void loop()
{
	char *posStringRaw;
	char *posStringFormatted;
	int gatePosition = gateClosedReading;

	//************ open gate **************
	do
	{
		//print position to serial port
		itoa(gatePosition, posStringRaw, 10);
		posStringFormatted = concat("R", posStringRaw);
		slidingGateDistanceSensorSerial.write(posStringFormatted);
		free(posStringFormatted);
		--gatePosition; //gate opening

		//turn on isOpening LED
		digitalWrite(OPENING_LED_PIN, HIGH);
		digitalWrite(CLOSING_LED_PIN, LOW);
		yield();
	} while (gatePosition > gateOpenReading);

	//************ close gate **************
	do
	{
		//print position to serial port
		itoa(gatePosition, posStringRaw, 10);
		posStringFormatted = concat("R", posStringRaw);
		slidingGateDistanceSensorSerial.write(posStringFormatted);
		free(posStringFormatted);
		++gatePosition; //gate closing

		//turn on isClosing LED
		digitalWrite(OPENING_LED_PIN, LOW);
		digitalWrite(CLOSING_LED_PIN, HIGH);
		yield();
	} while (gatePosition < gateClosedReading);
}


char* concat(const char *s1, const char *s2)
{
	char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
	// in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}