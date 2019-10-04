#ifndef __STATSTATE_H__
#define __STATSTATE_H__

#include "State.h"

class StatScrollState: public State {
public:
	int loop(char key);
	void setup(String message);
private:
	Scroller _scroller;
};

class StatState: public State {
private:
	State *_subState=0;
	StatScrollState _scrollState;
public:
	int loop(char key);
};

#endif
