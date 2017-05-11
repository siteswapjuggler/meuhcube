#include <EEPROM.h>          // accès à l'EEPROM interne de l'arduino

#define EXTCLOCK    0xF0          // set external clock
#define FRAMERATE   0xF1          // define interFrame value
#define LAYERRATE   0xF2          // define interLayer value

void loadParams() {
  refresh    = EEPROM.read(EXTCLOCK);
  interFrame = EEPROM.read(FRAMERATE);
  interLayer = (EEPROM.read(LAYERRATE) << 8) | EEPROM.read(LAYERRATE + 1);
}

void saveParams() {
  EEPROM.write(EXTCLOCK   , refresh);
  EEPROM.write(FRAMERATE  , interFrame);
  EEPROM.write(LAYERRATE  , interLayer >> 8);
  EEPROM.write(LAYERRATE + 1, interLayer & 0xFF);
}

void defaultParams() {
  refresh = true;
  interFrame = 20;
  interLayer = 1428;
}
