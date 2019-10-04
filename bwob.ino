#include "Ui.h"
#include "State.h"
#include "LightState.h"
#include "StatState.h"
#include "EffState.h"
#include "PresetManager.h"

const int numStates=6;
State *states[numStates];
int stateNum;

void setup() {
  Serial.begin(9600);

  PresetManager::instance()->loadPresets();

  Ui::instance()->setup();
  states[0]=new PrState();
  states[1]=new PsState();
  states[2]=new StatState();
  states[3]=new EffState();
  states[4]=new LightState();
  states[5]=new AllLightState();

  stateNum=0;
  states[stateNum]->setup();

  for (int i=0; i<Ui::instance()->getNumChannels(); i++)
    Ui::instance()->setChannelLevel(i,9);
}

void loop() {
  Ui::instance()->measurePower();

  int oldStateNum=stateNum;
  stateNum+=states[stateNum]->loop(Ui::instance()->read());
  if (stateNum<0)
    stateNum=0;
  if (stateNum>=numStates)
    stateNum=numStates-1;

  if (oldStateNum!=stateNum)
    states[stateNum]->setup();
}
