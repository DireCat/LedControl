

/*  Simple LED control Library for Arduino
 *  Author: Denis Tolochenko
 *  https://github.com/DireCat/LedControl 
*/

#ifndef LEDLIBRARY_H
#define LEDLIBRARY_H

#include <Arduino.h>

class Led
{
	public:
		Led (uint8_t pin);						// define led
		void loop ();							// insert in the main loop (need for blinking and timer)
		void on ();								// turn led ON
		void on (uint16_t msec);				// turn led ON for given milliseconds
		void off ();							// turn led OFF
		void toggle();							// toggle led
		void set (boolean level);				// set the led state directly (HIGH/LOW)
		void blink (uint16_t msec = 500);		// blink led with a given interval is milliseconds
		void blink (uint16_t msec, uint8_t count);  // blink led with a given interval only ... times 

	private:
		enum {
			STATIC,
			ON_COUNT,
			BLINK_LOOP,
			BLINK_COUNT
		} state;

		uint8_t pin;
		uint8_t counter;
		uint16_t interval;
		uint32_t t;
};


Led::Led (uint8_t pin) : pin (pin)
{
	pinMode (pin, OUTPUT);
	off();
}

void Led::set (boolean level)
{
	digitalWrite (pin, level);
	state = STATIC;
}

void Led::blink (uint16_t msec)
{
	off();
	state = BLINK_LOOP;
	interval = msec;
}

void Led::blink (uint16_t msec, uint8_t count)
{
	off();
	state = BLINK_COUNT;
	interval = msec;
	counter = count * 2;  // both HIGH and LOW are counted
}

inline void Led::on()
{
	set (true);
}

void Led::on(uint16_t msec)
{
	on();
	state = ON_COUNT;
	t = millis() + msec;
}

inline void Led::off()
{
	set (false);
}

void Led::toggle ()
{
	digitalWrite(pin, !digitalRead(pin));
}

void Led::loop ()
{
	if (state == STATIC)
		return;

	uint32_t ms = millis();
	if (state == ON_COUNT && millis() > t)
		off();
	else if (millis() > t)
	{
		t = millis() + interval;
		if (state == BLINK_COUNT && --counter == 0)
			off();
		else
			toggle();
	}
}

#endif