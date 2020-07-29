#ifndef Motors_H
#define Motors_H

#include <Arduino.h>
// Library for using servo on Timer 2 (https://github.com/nabontra/ServoTimer2)
#include <ServoTimer2.h>
// Library for Arduino internal eeprom memory
#include <EEPROM.h>

class Motors
{
	private:
		static const uint8_t _MotorRPin = 9;	// pin for Right servomotor
		static const uint8_t _MotorLPin = 10;	// pin for Left servomotor
		static const uint16_t _SpeedMax = 200;	// normal speed for forward moving (center point+speed microseconds)
		static const uint16_t _SppedSlow = 50;	// speed used for maneuvers

		// stuff used by servomotors
		ServoTimer2 _MotorL;            // left servomotor object
		ServoTimer2 _MotorR;            // right servomotor object
		uint16_t _servoL_center = 1500; // default value for left servomotor center point
		uint16_t _servoR_center = 1500; // default value for right servomotor center point
		uint8_t _servoL_eeprom = 0;     // eeprom memory location for storing point zero of left servomotor
		uint8_t _servoR_eeprom = 2;     // eeprom memory location for storing point zero of right servomotor
		//#define SPEED  200 // normal speed for forward moving (center point+speed microseconds)
		//#define SPEED_SLOW 50 // speed used for maneuvers

	public:
		Motors();
		~Motors();

		enum Motor { left, right };

		/**
		 * 
		 * 
		 */
		void Avanti(uint16_t vel);

		/**
		 * 
		 * 
		 */
		void Indietro(long ms);

		// turns right at slow speed for ms milliseconds
		void Destra(long ms);

		// turns left at slow speed for ms milliseconds
		void Sinistra(long ms);

		// Servomotors stopped
		void Fermo(long ms);

		/**
		 * Store the pulse width in microseconds for a specific Motor
		 * 
		 */
		void Write(Motor motor, int pulseWidth);

		void UpdateZeroSpeed(Motor motor, uint16_t value);
};

#endif