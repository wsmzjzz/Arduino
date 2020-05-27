#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

const int pulseWire = 0;
const int led = 13;
int threshold = 650;

PulseSensorPlayground pulseSensor;

void setup() {
    Serial.begin(9600);

    pulseSensor.analogInput(pulseWire);
    pulseSensor.blinkOnPulse(led);
    pulseSensor.setThreshold(threshold);

    if (pulseSensor.begin()) {
        Serial.println("Created!");
    }
}
void loop() {
    int BPM = pulseSensor.getBeatsPerMinute();

    if (pulseSensor.sawStartOfBeat()) {
        Serial.println(" * Happend! ");
        Serial.println("BPM: ");
        Serial.println(BPM);
    }
    delay(20);
}