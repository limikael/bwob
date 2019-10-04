#include "EffState.h"
#include <EEPROM.h>

void EffState::setup() {
	_envelopeSettingState.setup();
}

int EffState::loop(char key) {
	if (_subState) {
		int l=_subState->loop(key);

		if (l==555) {
			_subState=&_timerState;
			_timerState.setupLevelState(_envelopeSettingState.getValue());
			return 0;
		}

		else if (l) {
			_subState=NULL;
			key='\0';
		}

		else
			return 0;
	}

	Ui::instance()->print("EFF.");

	switch (Ui::keyNum(key)) {
		case 1:
			_subState=&_timerState;
			_timerState.setupLevelState(_envelopeSettingState.getValue());
			break;

		case 2:
			_subState=&_fadeState;
			_fadeState.setupLevelState(_envelopeSettingState.getValue());
			break;

		case 4:
			_subState=&_nightState;
			_nightState.setupLevelState(_envelopeSettingState.getValue());
			break;

		case 5:
			_subState=&_sleepState;
			_sleepState.setupLevelState(_envelopeSettingState.getValue());
			break;

		case 9:
			_subState=&_envelopeSettingState;
			_subState->setup();
			break;
	}

	return Ui::keyDir(key);
}

void EnvelopeSettingState::setup() {
	_prompt.setup("En.",2);
	_prompt.setEnabled(false);

	EEPROM.get(116,_value);

	if (_value<0)
		_value=0;

	if (_value>50)
		_value=50;
}

int EnvelopeSettingState::loop(char key) {
	if (_prompt.isCanceled())
		return 1;

	if (_prompt.isEnabled()) {
		_prompt.loop(key);

		if (_prompt.isDone()) {
			_value=_prompt.getVal();
			EEPROM.put(116,_value);
			setup();
		}

		else
			return 0;
	}

	else if (Ui::keyNum(key)>=0) {
		_prompt.setEnabled(true);
		_prompt.loop(key);
		return;
	}

	char s[8];
	sprintf(s,"%02d",_value);
	Ui::instance()->print(String("En.")+String(s));

	return Ui::keyDir(key);
}

int EnvelopeSettingState::getValue() {
	return _value;
}

float LevelState::linMap(float sFrom, float sTo, float dFrom, float dTo, float v) {
	float frac=(v-sFrom)/(sTo-sFrom);
	return dFrom+(dTo-dFrom)*frac;
}

float TimerState::getLevelByFrac(float frac) {
	if (frac==1.0)
		return 0;

	else
		return 1;
}

float FadeState::getLevelByFrac(float frac) {
	return 1.0-frac;
}

float NightState::getLevelByFrac(float frac) {
	if (frac<_envelopeFrac)
		return LevelState::linMap(0,_envelopeFrac,1,0,frac);

	if (frac>1.0-_envelopeFrac)
		return LevelState::linMap(1.0-_envelopeFrac,1,0,1,frac);

	return 0;
}

float SleepState::getLevelByFrac(float frac) {
	if (frac>1.0-_envelopeFrac)
		return LevelState::linMap(1.0-_envelopeFrac,1,0,1,frac);

	return 0;
}

LevelState::LevelState(char *promptText, char keyChar, int addr) {
	_promptText=String(promptText);
	_keyChar=keyChar;
	_addr=addr;
}

void LevelState::setupLevelState(int envelopeValue) {
	_envelopeFrac=(float)envelopeValue/100.0;
	_prompt.setup(_promptText);

	for (int i=0; i<Ui::instance()->getNumChannels(); i++)
		_originalLevels[i]=Ui::instance()->getChannelLevel(i);

	unsigned long duration;
	EEPROM.get(_addr,duration);
	if (duration<1500)
		duration=1500;
	if (duration>10L*60L*60L*1000L)
		duration=10L*60L*60L*1000L;

	_timer.setup(_promptText,duration);
}

int LevelState::loop(char key) {
	if (_prompt.isCanceled())
		return 1;

	if (Ui::instance()->readLong()==_keyChar)
		_prompt.setEnabled(true);

	if (_prompt.isEnabled()) {
		for (int i=0; i<Ui::instance()->getNumChannels(); i++)
			Ui::instance()->setChannelLevel(i,_originalLevels[i]);

		_prompt.loop(key);

		if (_prompt.isDone()) {
			_prompt.setEnabled(false);
			unsigned long duration=_prompt.getHrMinDuration();
			EEPROM.put(_addr,duration);
			_timer.setup(_promptText,duration);
		}

		return 0;
	}

	int done=_timer.loop();
	float fracLevel=getLevelByFrac(_timer.getFrac());
	if (done)
		fracLevel=getLevelByFrac(1.0);

	for (int i=0; i<Ui::instance()->getNumChannels(); i++)
		Ui::instance()->setChannelLevel(i,fracLevel*(float)_originalLevels[i]);

	if (done && getLevelByFrac(1)>.5)
		return 555;

	if (key || done)
		return 1;

	return 0;
}
