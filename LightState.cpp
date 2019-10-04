#include "LightState.h"
#include "PresetManager.h"

int PrState::loop(char key) {
	if (Ui::keyNum(key)>=0)
		PresetManager::instance()->activatePreset(Ui::keyNum(key));

	int active=PresetManager::instance()->getActivePreset();

	if (active>=0)
		Ui::instance()->print("Pr. "+String(active));

	else
		Ui::instance()->print("Pr. -");

	return Ui::keyDir(key);
}

int PsState::loop(char key) {
	char longKey=Ui::instance()->readLong();

	if (Ui::keyNum(longKey)>=0)
		PresetManager::instance()->savePreset(Ui::keyNum(longKey));

	int active=PresetManager::instance()->getActivePreset();

	if (active>=0)
		Ui::instance()->print("PS. "+String(active));

	else
		Ui::instance()->print("PS. -");

	return Ui::keyDir(key);
}

int LightState::loop(char key) {
	if (Ui::keyNum(key)>=0)
		Ui::instance()->setChannelLevel(_currentChannel,Ui::keyNum(key));

	String s=
		String("L")+String(_currentChannel+1)
		+String(". ")+String(Ui::instance()->getChannelLevel(_currentChannel));

	Ui::instance()->print(s);
	_currentChannel+=Ui::keyDir(key);

	if (_currentChannel<0) {
		_currentChannel=0;
		return -1;
	}

	else if (_currentChannel>=Ui::instance()->getNumChannels()) {
		_currentChannel=Ui::instance()->getNumChannels()-1;
		return 1;
	}

	return 0;
}

bool AllLightState::areAllLightsSame() {
	int v=Ui::instance()->getChannelLevel(0);

	for (int i=0; i<Ui::instance()->getNumChannels(); i++)
		if (v!=Ui::instance()->getChannelLevel(i))
			return false;

	return true;
}

int AllLightState::loop(char key) {
	if (Ui::keyNum(key)>=0) {
		for (int i=0; i<Ui::instance()->getNumChannels(); i++)
			Ui::instance()->setChannelLevel(i,Ui::keyNum(key));
	}

	if (areAllLightsSame())
		Ui::instance()->print("AL. "+String(Ui::instance()->getChannelLevel(0)));

	else
		Ui::instance()->print("AL. -");

	return Ui::keyDir(key);
}
