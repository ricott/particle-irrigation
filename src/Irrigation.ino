#include <RelayShield.h>

STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));

// -----------------------------------
// Controlling Water Irrigation System
// -----------------------------------
//Create an instance of the RelayShield library, so we have something to talk to
const int relay_grassNorth = 1;
const int relay_plants = 2;
const int relay_grassWest = 3;
const int relay_grassSouth = 4;
const String event_prefix = "garden/irrigation/";

RelayShield myRelays;

void setup() {
   Particle.publish(event_prefix + "setup", "starting", PRIVATE);
   Time.zone(+2);  // Set to correct time zone
   //Setup relays
   myRelays.begin();
   myRelays.allOff();

   Particle.function("waterGrassNo", waterGrassN);
   Particle.function("waterGrassS", waterGrassS);
   Particle.function("waterGrassW", waterGrassW);
   Particle.function("waterPlants", waterPlants);
}

void loop() {
}

int waterPlants(String command) {
  return relayCommand("plants", relay_plants, command);
}

int waterGrassW(String command) {
  return relayCommand("grass/west", relay_grassWest, command);
}

int waterGrassN(String command) {
  return relayCommand("grass/north", relay_grassNorth, command);
}

int waterGrassS(String command) {
  return relayCommand("grass/south", relay_grassSouth, command);
}

int relayCommand(String relayName, int relay, String command) {
  if (command == "?") {
    return isRelayOn(relay);
  } else if (command == "0" && isRelayOn(relay)) {
    turnOffRelay(relayName, relay);
    return 0;
  } else if (command == "1" && !isRelayOn(relay)) {
    turnOnRelay(relayName, relay);
    return 1;
  } else {
    return -1;
  }
}

bool isRelayOn(int relay){
  if (myRelays.isOn(relay)) {
    return true;
  } else {
    return false;
  }
}
void turnOnRelay(String relayName, int relay){
  if (relayName != "") {
    Particle.publish(event_prefix + relayName, "on", PRIVATE);
  }
  myRelays.on(relay);
}
void turnOffRelay(String relayName, int relay){
  if (relayName != "") {
    Particle.publish(event_prefix + relayName, "off", PRIVATE);
  }
  myRelays.off(relay);
}
