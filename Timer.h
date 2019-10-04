#ifndef __TIMER_H__
#define __TIMER_H__

#include <Arduino.h>
#include "Ui.h"

class Timer {
public:
	void setup(String prompt, unsigned long duration);
	void setup(String prompt, int h, int m, int s);
	int loop();
	float getFrac();
	float getReverseFrac();
private:
	unsigned long _start,_duration;
	unsigned long remaining();
	String _prompt;
};

#endif
