#include <Arduino.h>

#include "app_config.h"

TL_STATE tl_state = TL_STATE_RED;

void app_periph_init();
bool app_button_pushed();

void setup() {
  Serial.begin(BAUD_RATE);

  app_periph_init();
}

void loop() {
  // Starting reference point for the tl countdown
  uint16_t tl_delay_epoch;
  // Starting reference point for the buzzer countdown
  uint16_t buzzer_delay_epoch;

  // Countdown timer before toggling the buzzer
  uint16_t buzzer_countdown_ms;

  // Comparison value for the buzzer countdown
  // Overwrties the reference point (epoch) when count down goes to 0
  uint16_t current_time;

  // Is the buzzer on?
  bool buzzer_on = false;

  switch (tl_state) {
  case TL_STATE_RED: {
    current_time = millis();
    
    // Both the tl delay and the buzzer can share the same epoch on start
    tl_delay_epoch = current_time;
    buzzer_delay_epoch = current_time;

    buzzer_countdown_ms = 250;

    digitalWrite(TL_RED_GPIO_NUM, HIGH);

    // Until the 10 seconds have passed...
    while ( ( ( current_time = millis() ) - tl_delay_epoch) < 10000) {
      // iIn the event where the buzzer time reference needs to be reset
      // Reset and toggle the buzzer
      if ((current_time - buzzer_delay_epoch) < buzzer_countdown_ms) {
        buzzer_delay_epoch = current_time;
        if (!buzzer_on) {
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
    
    // Turn off everything and prepare to go to the next state
    tone(BUZZER_GPIO_NUM, 0);
    buzzer_on = false;
    digitalWrite(TL_RED_GPIO_NUM, LOW);
    tl_state = TL_STATE_RED_YELLOW;
    break;
  }
  case TL_STATE_RED_YELLOW: {
    // Simple State, just idle for 2 secs
    digitalWrite(TL_YELLOW_GPIO_NUM, HIGH);
    delay(2000);
    digitalWrite(TL_YELLOW_GPIO_NUM, LOW);
    tl_state = TL_STATE_GREEN;
    break;
  }
  case TL_STATE_YELLOW: {
    // Simple State, just idle for 2 secs
    digitalWrite(TL_YELLOW_GPIO_NUM, HIGH);
    delay(2000);
    digitalWrite(TL_YELLOW_GPIO_NUM, LOW);
    tl_state = TL_STATE_RED;
    break;
  }
  case TL_STATE_GREEN: {
    digitalWrite(TL_GREEN_GPIO_NUM, HIGH);

    // Spin until button is high
    while (!app_button_pushed()) {

    }

    Serial.println("button pushed");

    // Begin the countdown timer for 5 seconds
    buzzer_delay_epoch = millis();
    uint16_t buzzer_green_state_countdown = 500;
    while (digitalRead(BUTTON_GPIO_NUM) == 0) { 
      current_time = millis();
      if ( ( current_time - buzzer_delay_epoch) < buzzer_green_state_countdown) {
        buzzer_delay_epoch = current_time;
        if (!buzzer_on) {
          Serial.println("Buzzer on");
          tone(BUZZER_GPIO_NUM, BUZZER_FREQ);
          buzzer_on = true;
          buzzer_green_state_countdown = 1500;
        } else {
          Serial.println("Buzzer off");
          tone(BUZZER_GPIO_NUM, 0);
          buzzer_on = false;
          buzzer_green_state_countdown = 5000;
        }
      }
    }

    tl_delay_epoch = millis();
    while ( ( millis() - tl_delay_epoch ) < 5000 ) {
      // same logic as the spin loop
    }

    digitalWrite(TL_GREEN_GPIO_NUM, LOW);
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

bool app_button_pushed() {
  return digitalRead(BUTTON_GPIO_NUM) == 1;
}