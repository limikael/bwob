#ifndef __LIGHTSTATE_H__
#define __LIGHTSTATE_H__

#include "State.h"

class PrState: public State {
	public:
		int loop(char key);
};

class PsState: public State {
	public:
		int loop(char key);
};

class LightState: public State {
	public:
		int loop(char key);	

	private:
		int _currentChannel=0;
};

class AllLightState: public State {
	public:
		int loop(char key);	

	private:
		bool areAllLightsSame();
};

#endif