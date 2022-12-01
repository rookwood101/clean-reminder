#include <Esp.h>
#include <stdint.h>
#define CLEAN_REMINDER_DEBUG 1

// Deep-sleep for specified amount of hours, one hour at a time.
// If powered on (not a deep-sleep reset), nothing will happen.
// Call this twice: in the beginning of setup (end_of_setup == false)
// and at the end of setup (end_of_setup == true).


// memory offsets, not actual values
#define ADDR_MAGIC_NUMBER 8
#define SLEEP_CYCLE 16

// With larger values, deep-sleep is unrealiable: it might never wake up and consume a lot of power.
// Therefore sleep one hour at a time.
// In reality, the ESP sleeps a bit less than the 60 minutes it is told to.
const static uint32_t MAX_SLEEP = 3600e6; // 1 hour in microseconds
const static uint32_t LONG_SLEEP = 20e6; // 10 seconds in microseconds
const static uint32_t SHORT_SLEEP = 5e6; // 1 seconds in microseconds
const static uint64_t LED_AFTER_CYCLE = 3; // show LED after cycle 3
const static uint64_t MAGIC_NUMBER = 584128;


void rtcWrite64(uint32_t offset, uint64_t new_value) {
    ESP.rtcUserMemoryWrite(offset, (uint32_t*)&new_value, sizeof(new_value));
}

uint64_t rtcRead64(uint32_t offset) {
    uint64_t value;
    ESP.rtcUserMemoryRead(offset, (uint32_t*)&value, sizeof(value));
    return value;
}

void onWake(uint64_t cycles) {
    if (rtcRead64(ADDR_MAGIC_NUMBER) != MAGIC_NUMBER) {
      resetSleep();
    }  
    bool waking_from_sleep = ESP.getResetReason() == "Deep-Sleep Wake";
    
    if (waking_from_sleep) {
        Serial.print("Waking up from deep-sleep via reset pin. Current number of sleep cycles: ");
        Serial.println(cycles);        
    } else {
        // not sure how to wake up any other way
        Serial.println("Waking up by some other means. Resetting");
        resetSleep();
    }

}

bool isItTimeYet(uint64_t cycles) {
    // TODO this is not working
    Serial.println("Is it time yet? Checking if cycles (" + String(cycles) + ") >= led cycle (" + String(LED_AFTER_CYCLE) + ")");
    return cycles >= LED_AFTER_CYCLE;
}

void resetSleep() {
    Serial.println("Resetting sleep. Current sleep duration: 0");
    rtcWrite64(SLEEP_CYCLE, 0);
    rtcWrite64(ADDR_MAGIC_NUMBER, MAGIC_NUMBER);
    sleep(100, 0);
}

void sleep(uint64_t duration, uint64_t current_cycles) {
    rtcWrite64(SLEEP_CYCLE, current_cycles + 1);
    Serial.println("Going to deep-sleep for " + String(duration) + " micro seconds.");
    ESP.deepSleep(duration, WAKE_RF_DISABLED);
}
void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Begin");
    uint64_t cycles = rtcRead64(SLEEP_CYCLE);
    onWake(cycles);
    if (isItTimeYet(cycles)) {
        Serial.println("complete!");
        // no need to trigger LED manually as it flickers a little on reset
        // pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
        // digitalWrite(LED_BUILTIN, cycles % 2 == 0 ? LOW : HIGH);
        sleep(SHORT_SLEEP, cycles);
        // never gets here
    }
    sleep(LONG_SLEEP, cycles);
    // never gets here
}

void loop() {
  // nothing
}
