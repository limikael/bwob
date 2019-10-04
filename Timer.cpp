#include "Timer.h"

void Timer::setup(String prompt, int h, int m, int s) {
	setup(prompt,
		(unsigned long)s*1000
		+(unsigned long)m*60*1000
		+(unsigned long)h*60*60*1000
	);
}

void Timer::setup(String prompt, unsigned long duration) {
	_prompt=prompt;
	_start=millis();
	_duration=duration;
}

float Timer::getFrac() {
	unsigned long m=millis();

	return ((float)m-(float)_start)/(float)_duration;
}

float Timer::getReverseFrac() {
	return 1.0-getFrac();
}

unsigned long Timer::remaining() {
	unsigned long m=millis();

	if (m>_start+_duration)
		return 0;

	return _start+_duration-m;
}

int Timer::loop() {
	unsigned long rem=remaining();

	if (!rem)
		return 1;

	unsigned long remainingSecs=(rem/1000)%60;
	unsigned long remainingMins=(rem/1000/60)%60;
	unsigned long remainingHrs=(rem/1000/60/60)%60;

	char disp1[10];
	char disp2[10];

	if (remainingMins<10) {
		if (remainingMins<1)
			sprintf(disp1," ");

		else
			sprintf(disp1,"%1d",remainingMins);

		sprintf(disp2,"%02d",remainingSecs);
	}

	else {
		sprintf(disp1,"%1d",remainingHrs);
		sprintf(disp2,"%02d",remainingMins);
	}

	String dot=String();
	if (rem%1000>500)
		dot=String(".");

	Ui::instance()->print(_prompt+String(disp1)+dot+String(disp2));

	return 0;
}
