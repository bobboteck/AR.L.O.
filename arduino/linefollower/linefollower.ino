// Libraries for OLED display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Motors.hpp"

// Arduino used pins
#define MotorRPin 9       // signal for right servomotor
#define MotorLPin 10      // signal for left servomotor
//#define P1 6              // pushbutton P1
//#define P2 7              // pushbutton P2
//#define trigPin 8         // HC-SR04 - trigger
//#define echoPin 2         // HC-SR04 - echo

#define SPEED_MAX  200 // normal speed for forward moving (center point+speed microseconds)
#define SPEED_SLOW 50 // speed used for maneuvers

// stuff used by oled display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Motors ArloMotors(MotorRPin, MotorLPin);

static const int sensorPin = A0;	// select the input pin for the potentiometer
static const int ledPin = 13;		// select the pin for the LED
static const int lineCenter = 500;	// Sensor value for line positioned at center
static const int deltaCenter = 100;
//int linePosition = 0;	// variable to store the value coming from the sensor

void setup()
{
	// declare the ledPin as an OUTPUT:
	pinMode(ledPin, OUTPUT);

	ArloMotors.EnableMotors();
	ArloMotors.Stop();

	Serial.begin(9600);

	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
	{ // Address 0x3C for 128x32
		Serial.println(F("SSD1306 allocation failed"));
		while(1); // Don't proceed, loop forever
	}
	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(SSD1306_WHITE,SSD1306_BLACK);
}

void loop()
{
    display.setTextSize(2);
    display.setCursor(0,0);
	display.print("ARLOLF ");
	
	// read the value from the sensor:
	int linePosition = analogRead(sensorPin);
	Serial.print("Line: "); Serial.print(linePosition); 
	display.println(linePosition);

	if(linePosition > 870)
	{
		display.print("----------");
		//Motors.Stop();
	}
	else if(linePosition < 140)
	{
		display.print("??????????");
		ArloMotors.Forward(SPEED_MAX/6);
	}
	else
	{
		int centerOffSet = linePosition - lineCenter;

		if(abs(centerOffSet) <= deltaCenter)
		{
			display.print("    ||    ");
			ArloMotors.Forward(SPEED_MAX/6);
		}
		else
		{
			int motorLeftSpeed = SPEED_MAX;
			int motorRightSpeed = SPEED_MAX;

			if(centerOffSet < 0)
			{
				display.print("  > ..    ");
				motorLeftSpeed = abs((abs(centerOffSet) - 100) - 200);
			}
			else
			{
				display.print("    .. <  ");
				motorRightSpeed = abs((centerOffSet - 100) - 200);
			}

			Serial.print(" Left: "); Serial.print(motorLeftSpeed); Serial.print(" - Right: "); Serial.print(motorRightSpeed);
			ArloMotors.Forward(motorLeftSpeed / 6, motorRightSpeed / 6);
		}
	}

	display.display();
	Serial.println();
}
