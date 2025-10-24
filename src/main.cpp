#include <Arduino.h>

#include "app_config.h"

TL_STATE tl_state = TL_STATE_RED;

void setup() {

}

void loop() {

}

void app_periph_init() {
  // Traffic light gpio configurations
  pinMode(TL_RED_GPIO_NUM, OUTPUT);
  pinMode(TL_YELLOW_GPIO_NUM, OUTPUT);
  pinMode(TL_GREEN_GPIO_NUM, OUTPUT);

  // Pedestrian button gpio configurations
  pinMode(BUTTON_GPIO_NUM, INPUT);
}