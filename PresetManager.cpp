#include "PresetManager.h"
#include <Arduino.h>
#include <EEPROM.h>

PresetManager *PresetManager::instance() {
	static PresetManager pm;
	return &pm;
}

void PresetManager::loadPresets() {
	for (int pi=0; pi<10; pi++) {
		Ui::instance()->print("Ld "+String(pi));
		for (int ci=0; ci<Ui::instance()->getNumChannels(); ci++) {
			byte val=EEPROM.read(pi*MAXCHANNELS+ci);
			if (val<0)
				val=0;

			if (val>9)
				val=9;

			_presetData[pi][ci]=val;
		}
	}
}

void PresetManager::savePreset(int presetNum) {
	for (int ci=0; ci<Ui::instance()->getNumChannels(); ci++) {
		_presetData[presetNum][ci]=Ui::instance()->getChannelLevel(ci);
		byte val=_presetData[presetNum][ci];
		EEPROM.write(presetNum*MAXCHANNELS+ci,val);
	}
}

void PresetManager::activatePreset(int presetNum) {
	for (int ci=0; ci<Ui::instance()->getNumChannels(); ci++)
		Ui::instance()->setChannelLevel(ci,_presetData[presetNum][ci]);
}

bool PresetManager::isPresetActive(int presetNum) {
	for (int ci=0; ci<Ui::instance()->getNumChannels(); ci++)
		if (_presetData[presetNum][ci]!=Ui::instance()->getChannelLevel(ci))
			return false;

	return true;
}

int PresetManager::getActivePreset() {
	for (int i=0; i<10; i++)
		if (isPresetActive(i))
			return i;

	return -1;
}