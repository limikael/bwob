#include "Scroller.h"

void Scroller::setup(String text) {
	_text=String("    ")+text+String("    ");
	_pos=0;
	_lastMillis=millis();
}

int Scroller::loop() {
	int res=0;

	if (millis()>_lastMillis+500) {
		_lastMillis=millis();

		_pos++;
		if (_text.charAt(_pos)=='.')
			_pos++;

		if (_pos>_text.length()-4) {
			res=1;
			_pos=_text.length()-4;
		}
	}

	Ui::instance()->print(_text.substring(_pos));

	return res;
}