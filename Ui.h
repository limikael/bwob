#ifndef __UI_H__
#define __UI_H__

#include <Arduino.h>

#define MAXCHANNELS 10

class Ui {
	public:
		void setup();
		void print(String s);
		void printf(char *s, int v);
		char read();
		char readLong();
		int keyNum();
		int keyDir();
		char keyChar();
		static int keyDir(char c);
		static int keyNum(char c);
		static Ui *instance();
		void setChannelLevel(int channelNum, float level);
		int getChannelLevel(int channelNum);
		int getNumChannels();
		float getChannelPower();
		void measurePower();
		float getPowerMinutes();

	private:
		Ui();
		void writeShiftReg(int val);
		void displayOff();
		void displayOn(int mum, char chr, int dp);
		char getCurrentKeyChar();
		int _clockPin=2;
		int _latchPin=4;
		int _dataPin=7;
		int _readKeyboardPin=A0;
		int _displayGroundPins[4]={A4,A5,A2,A3};
		int _channelPins[MAXCHANNELS]={9,10,11,3,5};
		int _channelLevels[MAXCHANNELS];
		int _numChannels=5;
		char _oldChar='\0';
		char _oldLongPressChar='\0';
		unsigned long _pressTime;
		unsigned long _lastMeasure;
		unsigned long _activityTime;
		float _powerMinutes=0;
		bool isEnabled();
};

#endif
