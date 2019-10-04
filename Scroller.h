#ifndef __SCROLLER__H_
#define __SCROLLER__H_

#include <Arduino.h>
#include "Ui.h"

class Scroller {
public:
	void setup(String);
	int loop();
private:
	String _text;
	int _pos;
	long int _lastMillis;
};

#endif