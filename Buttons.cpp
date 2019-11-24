#include "Buttons.h"


Buttons::Buttons(uint8_t BTN, bool active)
	: pinBTN(BTN),  pinsActive(active), doubleClickEnabled(true), button(Open)
{
	uint8_t configType = (pinsActive == LOW) ? INPUT_PULLUP : INPUT;
  pinMode(pinBTN, configType);
}

void Buttons::service(bool reset)
{
	now = millis();
	
	if (reset) {
		keyDownTicks = 0;
		doubleClickTicks = 0;
		lastButtonCheck = 0;
		button=Open;
	}

  if ( (now - lastButtonCheck) >= ENC_BUTTONINTERVAL)  { 
    lastButtonCheck = now;
    
    if (digitalRead(pinBTN) == pinsActive) { // key is down
      keyDownTicks++;
      if (keyDownTicks > (ENC_HOLDTIME / ENC_BUTTONINTERVAL)) {
        button = Held;
      }
    }

    if (digitalRead(pinBTN) == !pinsActive) { // key is now up
      if (keyDownTicks /* > ENC_BUTTONINTERVAL*/ ) {        

        if (button == Held) {          
          button = Released;  
          doubleClickTicks = 0;
        } else {          
          if (doubleClickTicks > ENC_SINGLECLICKONLY) {   // prevent trigger in single click mode
            if (doubleClickTicks < (ENC_DOUBLECLICKTIME / ENC_BUTTONINTERVAL)) {
              button = DoubleClicked;
              doubleClickTicks = 0;
            }
          } else {
            doubleClickTicks = (doubleClickEnabled) ? (ENC_DOUBLECLICKTIME / ENC_BUTTONINTERVAL) : ENC_SINGLECLICKONLY;
          }
        }

        button = Released;  
		status=false;
      }

      keyDownTicks = 0;
    }

    if (keyDownTicks==1) {
      button=Pressed;
	  status=true;
    }
  
    if (doubleClickTicks > 0) {
      doubleClickTicks--;
      if (--doubleClickTicks == 0) {
        button = Clicked;
      }
    }
  }

}

void Buttons::setButton(Buttons::Button state)
{
	button = state;
}

Buttons::Button Buttons::getButton(void)
{
	Buttons::Button ret = button;
  if (button != Buttons::Held) {
   	button = Buttons::Open; // reset
  }
  return ret;
}

/**
* @brief return status
* @return pressed or no
*/
bool Buttons::getStatus()
{
	return status;
}
