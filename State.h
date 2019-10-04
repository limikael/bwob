#ifndef __STATE_H__
#define __STATE_H__

#include "Ui.h"
#include "Scroller.h"
#include "Prompt.h"
#include "Timer.h"

class State {
public:
	virtual void setup() {};
	virtual int loop(char key)=0;
};

#endif