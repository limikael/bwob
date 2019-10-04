#ifndef __PROMPT_H__
#define __PROMPT_H__

#include <Arduino.h>
#include "Ui.h"

class Prompt {
public:
	void setup(String prompt, int entryLength=3);
	int loop(char key);
	bool isCanceled();
	bool isDone();
	unsigned long getVal();
	unsigned long getMinSecDuration();
	unsigned long getHrMinDuration();
	void setEnabled(bool enabled);
	bool isEnabled();

private:
	String _prompt;
	String _entry;
	bool _canceled;
	bool _enabled;
	int _entryLength;
};

#endif