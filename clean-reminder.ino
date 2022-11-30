#include <Esp.h>
#include<stdint.h>

// Deep-sleep for specified amount of hours, one hour at a time.
// If powered on (not a deep-sleep reset), nothing will happen.
// Call this twice: in the beginning of setup (end_of_setup == false)
// and at the end of setup (end_of_setup == true).


// memory offsets, not actual values
#define TOTAL_SLEEP 0
#define CURRENT_SLEEP 8
#define SLEEP_CYCLE 16

// With larger values, deep-sleep is unrealiable: it might never wake up and consume a lot of power.
// Therefore sleep one hour at a time.
// In reality, the ESP sleeps a bit less than the 60 minutes it is told to.
const static uint32_t MAX_SLEEP_CYCLE = 3600e6; // 1 hour in microseconds
const static uint64_t TOTAL_SLEEP_DURATION = 4*3600e6; // 4 hours in microseconds
// const static uint32_t MAX_SLEEP_CYCLE = 10e6; // 10 seconds in microseconds
// const static uint64_t TOTAL_SLEEP_DURATION = 40e6; // 40 seconds in microseconds


void rtcWrite64(uint32_t offset, uint64_t new_value) {
    ESP.rtcUserMemoryWrite(offset, (uint32_t*)&new_value, sizeof(new_value));
}

uint64_t rtcRead64(uint32_t offset) {
    uint64_t value;
    ESP.rtcUserMemoryRead(offset, (uint32_t*)&value, sizeof(value));
    return value;
}

void onWake() {
    bool waking_from_sleep = ESP.getResetReason() == "Deep-Sleep Wake";
    
    if (waking_from_sleep) {
        uint64_t stored_sleep = rtcRead64(CURRENT_SLEEP);
        uint64_t sleep_cycle = rtcRead64(SLEEP_CYCLE);
        rtcWrite64(CURRENT_SLEEP, stored_sleep + sleep_cycle);
        Serial.print("Waking up from deep-sleep via reset pin. Current sleep duration: ");
        Serial.println(stored_sleep + sleep_cycle);
    } else {
        Serial.println("Waking up by some other means");
        resetSleep();
    }

}

bool isItTimeYet(uint64_t total_sleep_s) {
    // TODO this is not working
    uint64_t current_sleep = rtcRead64(CURRENT_SLEEP);
    Serial.println("Is it time yet? Checking if current sleep (" + String(current_sleep) + ") >= total sleep (" + String(total_sleep_s) + ")");
    return current_sleep >= total_sleep_s;
}

void resetSleep() {
    Serial.println("Resetting sleep. Current sleep duration: 0");
    rtcWrite64(CURRENT_SLEEP, 0);
}

void sleep(uint64_t total_sleep_s) {
    uint64_t current_sleep = rtcRead64(CURRENT_SLEEP);
    if (current_sleep < total_sleep_s) {
        uint32_t next_sleep_cycle = min(total_sleep_s - current_sleep, (uint64_t)MAX_SLEEP_CYCLE);
        rtcWrite64(SLEEP_CYCLE, next_sleep_cycle);
        Serial.println("Going to deep-sleep for " + String(next_sleep_cycle) + " seconds.");
        ESP.deepSleep(next_sleep_cycle, WAKE_RF_DISABLED);
    }
}
void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Begin");
    onWake();
    if (isItTimeYet(TOTAL_SLEEP_DURATION)) {
        Serial.println("complete!");
        resetSleep();
        pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
        return; // go to loop
    }
    sleep(TOTAL_SLEEP_DURATION);
    // never gets here
}

void loop() {
    // LED turns on on LOW
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
}
