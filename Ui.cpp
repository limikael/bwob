#include "Ui.h"

Ui::Ui() {
	_lastMeasure=millis();
}

void Ui::setup() {
	_oldChar='\0';
	pinMode(_latchPin,OUTPUT);
	pinMode(_clockPin,OUTPUT);
	pinMode(_dataPin,OUTPUT);
	pinMode(_readKeyboardPin,INPUT);
	for (int i=0; i<4; i++) {
		pinMode(_displayGroundPins[i],OUTPUT);
		digitalWrite(_displayGroundPins[i],HIGH);
	}

	for (int i=0; i<_numChannels; i++)
		_channelLevels[i]=0;

	_pressTime=0;
	_activityTime=millis();
}

void Ui::writeShiftReg(int val) {
	digitalWrite(_latchPin,LOW);
	shiftOut(_dataPin,_clockPin,MSBFIRST,val);
	digitalWrite(_latchPin,HIGH);
}

void Ui::displayOff() {
	for (int i=0; i<4; i++)
		digitalWrite(_displayGroundPins[i],HIGH);
}

void Ui::displayOn(int num, char chr, int dp) {
	int codes[]={
		0x80, 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F, 0x00,0x40,
		0x6D,0x77,0x73,0x38,0x50,0x78,
		0x7c,0x1c,0x5c,0x5e,0x08,0x79,0x71,0x54,
		0x39,0x76,0x3e,0x3d,0x1c,0x1c,0x54
	};
	char *chars=
		".0123456789 -"
		"SAPLrt"
		"bwod_EFn"
		"CHUGvuN";

	int index=0;

	displayOff();

	int i=0;
	while (chars[i]) {
		if (chr==chars[i])
			index=i;

		i++;
	}

	int code=codes[index];
	if (dp)
		code|=0x80;

	writeShiftReg(code);
	digitalWrite(_displayGroundPins[num],LOW);
}

void Ui::print(String s) {
	if (!isEnabled()) {
		delay(4);
		return false;
	}

	int index=0;

	for (int i=0; i<4; i++) {
		if (index<s.length()-1 && s.charAt(index+1)=='.') {
			displayOn(i,s.charAt(index),1);
			index+=2;
		}

		else if (index<s.length()) {
			displayOn(i,s.charAt(index),0);
			index++;
		}

		else
			displayOn(i,' ',0);

		delay(1);
	}

	displayOff();
}

bool Ui::isEnabled() {
	if (millis()>_activityTime+15000)
		return false;

	return true;
}

void Ui::printf(char *fmt, int v) {
	char target[8];

	sprintf(target,fmt,v);
	print(target);
}

char Ui::getCurrentKeyChar() {
	int keyCodes[12]={130,66,34, 136,72,40, 132,68,36, 144,80,48};
	char *chars="123456789*0#";
	unsigned int val=0;

	for (int i=0; i<8; i++) {
		writeShiftReg(1<<i);
		if (digitalRead(_readKeyboardPin))
			val|=(1<<i);
	}

	char keyChar='\0';
	for (int i=0; i<12; i++)
		if (val==keyCodes[i])
			return chars[i];

	return '\0';
}

char Ui::read() {
	char keyChar=getCurrentKeyChar();

	if (keyChar!=_oldChar) {
		_oldChar=keyChar;

		if (keyChar) {
			if (!isEnabled()) {
				delay(100);
				_activityTime=millis();
				return '\0';
			}

			_pressTime=millis();
			_activityTime=millis();
		}

		return keyChar;
	}

	return '\0';
}

char Ui::readLong() {
	char keyChar=getCurrentKeyChar();

	if (keyChar!=_oldLongPressChar) {
		_activityTime=millis();
		_pressTime=millis();
		_oldLongPressChar=keyChar;
	}

	if (_pressTime && millis()>_pressTime+1000L && keyChar==_oldLongPressChar) {
		_pressTime=0;
		return keyChar;
	}

	return '\0';
}

int Ui::keyDir(char c) {
	if (c=='*')
		return -1;

	else if (c=='#')
		return 1;

	else
		return 0;
}

int Ui::keyNum(char c) {
	int n=c-'0';

	if (n<0 || n>9)
		n=-1;

	return n;
}

Ui *Ui::instance() {
	static Ui ui;
	return &ui;
}

void Ui::setChannelLevel(int channelNum, float level) {
	_channelLevels[channelNum]=level;

	if (channelNum<getNumChannels())
		analogWrite(_channelPins[channelNum],255.0*level/9.0);
}

int Ui::getChannelLevel(int channelNum) {
	return _channelLevels[channelNum];
}

int Ui::getNumChannels() {
	return _numChannels;
}

float Ui::getChannelPower() {
	float total=0;

	for (int i=0; i<getNumChannels(); i++)
		total+=(float)_channelLevels[i]/9.0;

	return total;
}

float Ui::getPowerMinutes() {
	return _powerMinutes;
}

void Ui::measurePower() {
	unsigned long _measureTime=millis();
	if (_measureTime/60000L!=_lastMeasure/60000L) {
		_powerMinutes+=getChannelPower()*(float)(_measureTime-_lastMeasure)/60000.0;
		_lastMeasure=_measureTime;
	}
}