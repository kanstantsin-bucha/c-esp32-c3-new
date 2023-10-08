#include <Arduino.h>
#include <LogController.h>

int16_t count = 0;

void setup() {
  // put your setup code here, to run once:
    lg.begin(Serial, 115200);
    lg.addTimeMark = true;
    lg.disabled = false;
    count = 1;
}

void loop() {
  count += 1;
  for(int64_t i; i < 1000000; i++) {

  }
  Llf("Count: %d\r\n", count);
}
