#include <Arduino.h>

#include "app_config.h"

TL_STATE tl_state = TL_STATE_RED;

void app_periph_init();

void setup() {
  Serial.begin(BAUD_RATE);

  app_periph_init();
}

void loop() {
  uint16_t tl_delay_epoch;
  uint16_t buzzer_delay_epoch;
  uint16_t buzzer_current_delay;
  bool buzzer_on = false;
  switch (tl_state)
  {
  case TL_STATE_RED: {
    // Both the tl delay and the buzzer can share the same epoch
    tl_delay_epoch = millis();
    buzzer_delay_epoch = tl_delay_epoch;
    digitalWrite(TL_RED_GPIO_NUM, HIGH);
    while ( ( millis() - tl_delay_epoch ) < 10000) {
         buzzer_current_delay = millis();
         if ( ( buzzer_current_delay - buzzer_delay_epoch ) < 250 ) {
            if (buzzer_on) {
              Serial.println("Buzzer on");
              tone(BUZZER_GPIO_NUM, BUZZER_FREQ);
              buzzer_on = true;
            } else {
              Serial.println("Buzzer off");
              tone(BUZZER_GPIO_NUM, 0);
              buzzer_on = false;
            }
         }
    }
    tone(BUZZER_GPIO_NUM, 0);
    buzzer_on = false;
    digitalWrite(TL_RED_GPIO_NUM, LOW);
    tl_state = TL_STATE_RED_YELLOW;
    break;
  }
  case TL_STATE_RED_YELLOW: {
    digitalWrite(TL_YELLOW_GPIO_NUM, HIGH);
    delay(2000);
    digitalWrite(TL_YELLOW_GPIO_NUM, LOW);
    tl_state = TL_STATE_GREEN;
    break;
  }
  case TL_STATE_YELLOW: {
    digitalWrite(TL_YELLOW_GPIO_NUM, HIGH);
    delay(2000);
    digitalWrite(TL_YELLOW_GPIO_NUM, LOW);
    tl_state = TL_STATE_RED;
    break;
  }
  case TL_STATE_GREEN: {
    break;
  }
  default: {
    Serial.println("Unhandled State. Default to red");
    tl_state = TL_STATE_RED;
  }
  }

}

void app_periph_init() {
  // Traffic light gpio configurations
  pinMode(TL_RED_GPIO_NUM, OUTPUT);
  pinMode(TL_YELLOW_GPIO_NUM, OUTPUT);
  pinMode(TL_GREEN_GPIO_NUM, OUTPUT);

  // Pedestrian button gpio configurations
  pinMode(BUTTON_GPIO_NUM, INPUT);
}