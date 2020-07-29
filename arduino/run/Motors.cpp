#include "Motors.hpp"

Motors::Motors()
{
	_MotorL.attach(_MotorLPin);
	_MotorR.attach(_MotorRPin);

	// center values used for servomotors, loaded from eeprom
	// if in the eeprom are saved values out of the range 500-2500, 1500 will be used
	// (this happens when eeprom memory was never used since contains 0xFFFF value)
	uint16_t LV = EEPROM.read(_servoL_eeprom) << 8;
	LV |= EEPROM.read(_servoL_eeprom + 1);
	uint16_t RV = EEPROM.read(_servoR_eeprom) << 8;
	RV |= EEPROM.read(_servoR_eeprom + 1);
	if (LV < 2501 && LV > 499) _servoL_center = LV;
	if (RV < 2501 && RV > 499) _servoR_center = RV;
}

Motors::~Motors()
{}

void Motors::Avanti(uint16_t vel)
{
	if(vel > _SpeedMax) vel = _SpeedMax;

	_MotorL.write(_servoL_center + vel);
	_MotorR.write(_servoR_center - vel);
}

void Motors::Indietro(long ms)
{
	long timenow = millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center - _SppedSlow);
		_MotorR.write(_servoR_center + _SppedSlow);
	}
}

void Motors::Destra(long ms)
{
	long timenow = millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center + _SppedSlow);
		_MotorR.write(_servoR_center + _SppedSlow);
	}
}


void Motors::Sinistra(long ms)
{
	long timenow = millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center - _SppedSlow);
		_MotorR.write(_servoR_center - _SppedSlow);
	}
}


void Motors::Fermo(long ms)
{
	long timenow=millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center);
		_MotorR.write(_servoR_center);
	}
}

void Motors::Write(Motors::Motor motor, int pulseWidth)
{
	if(motor == Motor::left)
	{
		_MotorL.write(pulseWidth);
	}
	else
	{
		_MotorR.write(pulseWidth);
	}
}

void Motors::UpdateZeroSpeed(Motor motor, uint16_t value)
{
	if(motor == Motor::left)
	{
		// I must split the 16bit value in two bytes
		// and save values in 2 different EEPROM locations
		EEPROM.update(_servoL_eeprom, (uint8_t)(value >> 8));
		delay(4); // an eeprom.write takes 3.3mS
		EEPROM.update(_servoL_eeprom + 1, (uint8_t)(value & 0x00FF));
		delay(4);
	}
	else
	{
		// I must split the 16bit value in two bytes
		// and save values in 2 different EEPROM locations
		EEPROM.update(_servoR_eeprom, (uint8_t)(value >> 8));
		delay(4); // an eeprom.write takes 3.3mS
		EEPROM.update(_servoR_eeprom + 1, (uint8_t)(value & 0x00FF));
		delay(4);
	}
}