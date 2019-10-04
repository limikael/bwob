#include "StatState.h"

static String lz(unsigned long n) {
	char s[80];
	sprintf(s,"%02d",n);
	return String(s);
}

static String fmtrt() {
	unsigned long m=millis();
	unsigned long sec=(m/(1000L))%60;
	unsigned long min=(m/(1000L*60L))%60;
	unsigned long hr=(m/(1000L*60L*60L))%24;
	unsigned long days=m/(1000L*60L*60L*24L);

	return
		String(days)+String("d ")+
		lz(hr)+String(".")+lz(min)+String(".")+lz(sec);
}

static String fmtf(float n, int decimals) {
	char s[80];
	dtostrf(n,0,decimals,s);

	return String(s);
}

int StatState::loop(char key) {
	if (_subState) {
		if (_subState->loop(key)) {
			_subState=NULL;
			key='\0';
		}

		else
			return 0;
	}

	Ui::instance()->print("StAt.");

	String ver=
		String("bwob v4.2");

	String curPwr=
		String("CwrPwr. ")+
		fmtf(Ui::instance()->getChannelPower(),1);

	String totPwrHr=
		String("totPwrHr. ")+
		fmtf(Ui::instance()->getPowerMinutes()/60.0,1);

	String upt=
		String("UPt. ")+fmtrt();

	float upMinutes=floor((float)millis()/(1000.0*60.0));
	float powerMinutes=Ui::instance()->getPowerMinutes();
	String avgPwr=
		String("AvGPwr. ")+fmtf(powerMinutes/upMinutes,1);

	switch (Ui::keyNum(key)) {
		case 1:
			_subState=&_scrollState;
			_scrollState.setup(ver);
			break;

		case 2:
			_subState=&_scrollState;
			_scrollState.setup(curPwr);
			break;

		case 3:
			_subState=&_scrollState;
			_scrollState.setup(totPwrHr);
			break;

		case 4:
			_subState=&_scrollState;
			_scrollState.setup(upt);
			break;

		case 5:
			_subState=&_scrollState;
			_scrollState.setup(avgPwr);
			break;

		/*case 0:
			_subState=&_scrollState;
			String s=ver
				+String("    ")+curPwr
				+String("    ")+totPwrHr
				+String("    ")+upt
				+String("    ")+avgPwr;

			_scrollState.setup(s);
			break;*/
	}

	return Ui::keyDir(key);
}

void StatScrollState::setup(String message) {
	_scroller.setup(message);
}

int StatScrollState::loop(char key) {
	int loopRes=_scroller.loop();

	return ((key!='\0') || loopRes);
}
