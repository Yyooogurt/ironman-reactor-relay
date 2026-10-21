/*
  Iron Man Reactor — relay version
  Keyestudio KS0077/KS0078/KS0079 + 5V relay module + HT-5W

  Wiring:
    Relay S/IN -> D9
    Relay +/VCC -> 5V
    Relay -/GND -> GND

    Button:
    D2 -> button -> GND
    (uses INPUT_PULLUP, no external resistor needed)

  Relay contacts:
    power supply + -> COM
    NO -> HT-5W +
    HT-5W - -> power supply -

  IMPORTANT:
  The lamp must be powered from a source with the correct voltage for HT-5W.
  Do not power the HT-5W from Arduino D9.
*/

const byte RELAY_PIN  = 9;
const byte BUTTON_PIN = 2;

// Keyestudio KS0011-type relay is normally active HIGH.
// If your relay works in reverse, change true to false.
const bool RELAY_ACTIVE_HIGH = true;
const unsigned long DOUBLE_PRESS_WINDOW_MS = 400;

bool reactorOn = false;
bool waitingForSecondPress = false;
unsigned long firstPressTime = 0;

void relaySet(bool on) {
  byte level;

  if (RELAY_ACTIVE_HIGH) {
    level = on ? HIGH : LOW;
  } else {
    level = on ? LOW : HIGH;
  }

  digitalWrite(RELAY_PIN, level);
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
}

void flashRelay(unsigned int onMs, unsigned int offMs) {
  relaySet(true);
  delay(onMs);
  relaySet(false);
  delay(offMs);
}

bool buttonPressed() {
  if (digitalRead(BUTTON_PIN) != LOW) return false;

  delay(30);  // debounce
  if (digitalRead(BUTTON_PIN) != LOW) return false;

  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(5);
  }

  delay(30);
  return true;
}

void reactorStart() {
  reactorOn = false;
  relaySet(false);

  // Short flashes get closer together until the reactor is fully lit.
  // The final 80 ms on / 40 ms off pulses are about as fast as a mechanical
  // relay should be switched for this effect.
  const unsigned int offTimes[] = {
    1200, 1000, 850, 700, 580, 470, 380, 300,
    240, 190, 150, 115, 90, 70, 55, 40
  };
  for (byte i = 0; i < sizeof(offTimes) / sizeof(offTimes[0]); i++) {
    flashRelay(80, offTimes[i]);
  }

  // Brief rapid burst before the reactor stays on.
  for (byte i = 0; i < 7; i++) {
    flashRelay(80, 40);
  }

  relaySet(false);
  // delay(350);

  // Reactor online: stay lit until the button is pressed again.
  relaySet(true);
  reactorOn = true;
}

void reactorStop() {
  relaySet(false);
  reactorOn = false;
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  relaySet(false);
}

void loop() {
  if (!reactorOn && waitingForSecondPress &&
      millis() - firstPressTime > DOUBLE_PRESS_WINDOW_MS) {
    waitingForSecondPress = false;
    reactorStart();
    return;
  }

  if (buttonPressed()) {
    if (reactorOn) {
      reactorStop();
      waitingForSecondPress = false;
    } else if (waitingForSecondPress &&
               millis() - firstPressTime <= DOUBLE_PRESS_WINDOW_MS) {
      waitingForSecondPress = false;
      relaySet(true);
      reactorOn = true;
    } else {
      waitingForSecondPress = true;
      firstPressTime = millis();
    }
  }
}
