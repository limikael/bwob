#ifndef __EFFSTATE_H__
#define __EFFSTATE_H__

#include "State.h"

class EnvelopeSettingState: public State {
	public:
		int loop(char key);	
		void setup();
		int getValue();

	private:
		Prompt _prompt;
		int _value;
};

class LevelState: public State {
	public:
		LevelState(char *prompt, char key, int addr);
		int loop(char key);	
		void setupLevelState(int envelopeValue);
		static float linMap(float sFrom, float sTo, float dFrom, float dTo, float v);

	protected:
		virtual float getLevelByFrac(float frac)=0;
		float _envelopeFrac;

	private:
		String _promptText;
		char _keyChar;
		int _addr;
		Timer _timer;
		Prompt _prompt;
		int _originalLevels[MAXCHANNELS];
};

class FadeState: public LevelState {
	public:
		FadeState(): LevelState("F.",'2',100) {};

	protected:
		virtual float getLevelByFrac(float frac);
};

class TimerState: public LevelState {
	public:
		TimerState(): LevelState("t.",'1',104) {};

	protected:
		virtual float getLevelByFrac(float frac);
};

class NightState: public LevelState {
	public:
		NightState(): LevelState("n.",'4',108) {};

	protected:
		virtual float getLevelByFrac(float frac);
};

class SleepState: public LevelState {
	public:
		SleepState(): LevelState("S.",'5',112) {};

	protected:
		virtual float getLevelByFrac(float frac);
};

class EffState: public State {
	public:
		void setup(void);
		int loop(char key);

	private:
		State *_subState=0;
		FadeState _fadeState;
		TimerState _timerState;
		NightState _nightState;
		SleepState _sleepState;
		EnvelopeSettingState _envelopeSettingState;
};

#endif
