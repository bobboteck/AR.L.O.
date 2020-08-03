/*
 * Name          : Motors.cpp
 * @author       : Roberto D'Amico (Bobboteck)
 * Last modified : 03.08.2020
 * Revision      : 1.1.0
 *
 * Modification History:
 * Date         Version     Modified By		Description
 * 2020-08-03	1.1.0		Roberto D'Amico	Add new method Stop
 * 2020-07-30   1.0.0       Roberto D'Amico Created Library
 * 
 * ... LICENSE ...
 *
 *  This file is part of the .... Project (https://github.com/bobboteck/....).
 *	Copyright (c) 2020 Roberto D'Amico (Bobboteck).
 *
 */
#include "Motors.hpp"

Motors::Motors(uint8_t pinMotorRight, uint8_t pinMotorLeft)
{
	_MotorRPin = pinMotorRight;
	_MotorLPin = pinMotorLeft;

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

void Motors::EnableMotors()
{
	_MotorL.attach(_MotorLPin);
	_MotorR.attach(_MotorRPin);
}

void Motors::Forward(uint16_t speed)
{
	if(speed > _SpeedMax) speed = _SpeedMax;

	_MotorL.write(_servoL_center + speed);
	_MotorR.write(_servoR_center - speed);
}

void Motors::Backwards(long ms)
{
	long timenow = millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center - _SppedSlow);
		_MotorR.write(_servoR_center + _SppedSlow);
	}
}

void Motors::Right(long ms)
{
	long timenow = millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center + _SppedSlow);
		_MotorR.write(_servoR_center + _SppedSlow);
	}
}

void Motors::Left(long ms)
{
	long timenow = millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center - _SppedSlow);
		_MotorR.write(_servoR_center - _SppedSlow);
	}
}

void Motors::Stop(long ms)
{
	long timenow=millis();

	while((millis() - timenow) < ms)
	{
		_MotorL.write(_servoL_center);
		_MotorR.write(_servoR_center);
	}
}

void Motors::Stop()
{
	_MotorL.write(_servoL_center);
	_MotorR.write(_servoR_center);
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

void Motors::SetZeroSpeed(Motor motor, uint16_t value)
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