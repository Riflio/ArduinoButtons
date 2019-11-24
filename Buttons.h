#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <Arduino.h>
#include <stdint.h>

#define ENC_BUTTONINTERVAL    5  // check button every x milliseconds, also debouce time
#define ENC_DOUBLECLICKTIME  600  // second click within 600ms
#define ENC_HOLDTIME        2200  // report held button after 1.2s
#define ENC_SINGLECLICKONLY 1

class Buttons 
{
public: 

  typedef enum Button_e {
    	Open = 0,
    	Closed,    
    	Pressed,
    	Held,
    	Released,    
    	Clicked,
    	DoubleClicked    
  	} Button;

	Buttons(uint8_t BTN, bool active = LOW);
	Button getButton(void);
	bool getStatus();
	void setButton(Buttons::Button state);
	void service(bool reset=false);

private:
	const uint8_t pinBTN;
	const bool pinsActive;
	bool doubleClickEnabled;
	volatile Button button;
	bool status;
	long now = 0;	
	uint16_t keyDownTicks = 0;
	uint8_t doubleClickTicks = 0;
	unsigned long lastButtonCheck = 0;
};

#endif