#ifndef __PRESETMANAGER_H__
#define __PRESETMANAGER_H__

#include "State.h"
#include <Arduino.h>

class PresetManager {
	public:
		static PresetManager *instance();
		void loadPresets();
		void savePreset(int presetNum);
		void activatePreset(int presetNum);
		bool isPresetActive(int presetNum);
		int getActivePreset();

	private:
		byte _presetData[10][MAXCHANNELS];
};

#endif