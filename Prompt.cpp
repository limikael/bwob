#include "Prompt.h"

void Prompt::setup(String prompt, int entryLength) {
	_prompt=prompt;
	_entry=String("");
	_canceled=false;
	_enabled=false;
	_entryLength=entryLength;
}

void Prompt::setEnabled(bool enabled) {
	_enabled=enabled;
}

bool Prompt::isEnabled() {
	return _enabled;
}

bool Prompt::isCanceled() {
	return _canceled;
}

bool Prompt::isDone() {
	return (_entry.length()>=_entryLength);
}

unsigned long Prompt::getVal() {
	return atol(_entry.c_str());
}

unsigned long Prompt::getMinSecDuration() {
	unsigned long v=getVal();

	return (v/100L)*60L*1000L+(v%100L)*1000L;
}

unsigned long Prompt::getHrMinDuration() {
	unsigned long v=getVal();

	return (v/100L)*60L*60L*1000L+(v%100L)*60L*1000L;
}

int Prompt::loop(char key) {
	if (_canceled || key=='*' || key=='#') {
		_canceled=true;
		return 1;
	}

	if (key && _entry.length()<_entryLength)
		_entry.concat(String(key));

	String cursor="";
	if (millis()%1000>500)
		cursor="_";

	Ui::instance()->print(_prompt+_entry+cursor);

	return (_entry.length()>=_entryLength);
}