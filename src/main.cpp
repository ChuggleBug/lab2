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
    digitalWrite(TL_RED_GPIO_NUM, HIGH);
    
    // Both the tl delay and the buzzer can share the same epoch on start
    current_time = millis();
    tl_delay_epoch = current_time;
    buzzer_delay_epoch = current_time;

    // Countdown for buzzer toggle
    // This state does not modify this value during the countdown
    buzzer_countdown_ms = TL_DELAY_RED_BUZZER_MS;

    // Until the 10 seconds have passed...
    while ( ( ( current_time = millis() ) - tl_delay_epoch) < TL_DELAY_RED_MS) {
      // In the event where the buzzer time reference needs to be reset
      // Reset and toggle the buzzer
      if ((current_time - buzzer_delay_epoch) >= buzzer_countdown_ms) {
        buzzer_delay_epoch = current_time;
        if (!buzzer_on) {
          // Serial.println("Red: Buzzer on");
          tone(BUZZER_GPIO_NUM, BUZZER_FREQ);
          buzzer_on = true;
        } else {
          // Serial.println("Red: Buzzer off");
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
    delay(TL_DELAY_YELLOW_MS);
    digitalWrite(TL_YELLOW_GPIO_NUM, LOW);
    tl_state = TL_STATE_GREEN;
    break;
  }
  case TL_STATE_YELLOW: {
    // Simple State, just idle for 2 secs
    digitalWrite(TL_YELLOW_GPIO_NUM, HIGH);
    delay(TL_DELAY_YELLOW_MS);
    digitalWrite(TL_YELLOW_GPIO_NUM, LOW);
    tl_state = TL_STATE_RED;
    break;
  }
  case TL_STATE_GREEN: {
    digitalWrite(TL_GREEN_GPIO_NUM, HIGH);

    // The buzzer is off upon entering the green light (other 
    // states manually set this)
    // Upon entering the buzzer is off and the countdown begins
    buzzer_countdown_ms = TL_DELAY_RED_BUZZER_OFF_MS;

    // Spin until button is high
    current_time = millis();
    buzzer_delay_epoch = current_time;
    while (!app_button_pushed()) {
      current_time = millis();
      if ( (current_time - buzzer_delay_epoch) >= buzzer_countdown_ms ) {
        buzzer_delay_epoch = current_time;
        // Toggle buzzer (as well as the countdown)
        if (!buzzer_on) {
            // Set the countdown to 500 (to remain on)
            // Serial.println("Green: Buzzer on");
            buzzer_countdown_ms = TL_DELAY_RED_BUZZER_ON_MS;
            buzzer_on = true;
            tone(BUZZER_GPIO_NUM, BUZZER_FREQ);
        } else {
            // Set the countdown to 1500 (to remain off)
            // Serial.println("Green: Buzzer off");
            buzzer_countdown_ms = TL_DELAY_RED_BUZZER_OFF_MS;
            buzzer_on = false;
            tone(BUZZER_GPIO_NUM, 0);
        }
      }
    }

    // Begin the countdown timer for 5 seconds
    // Technically this could be combined in the previous while loop
    // but this implementation is easier
    current_time = millis();
    tl_delay_epoch = current_time;
    while ( ( ( current_time = millis() ) - tl_delay_epoch) < TL_DELAY_GREEN_MS) {
      if ( (current_time - buzzer_delay_epoch) >= buzzer_countdown_ms ) {
        buzzer_delay_epoch = current_time;
        // Toggle buzzer (as well as the countdown)
        if (!buzzer_on) {
            // Set the countdown to 500 (to remain on)
            // Serial.println("Green: Buzzer on");
            buzzer_countdown_ms = TL_DELAY_RED_BUZZER_ON_MS;
            buzzer_on = true;
            tone(BUZZER_GPIO_NUM, BUZZER_FREQ);
        } else {
            // Set the countdown to 1500 (to remain off)
            // Serial.println("Green: Buzzer off");
            buzzer_countdown_ms = TL_DELAY_RED_BUZZER_OFF_MS;
            buzzer_on = false;
            tone(BUZZER_GPIO_NUM, 0);
        }
      }
    }

    // Turn off everything and prepare to go to the next state
    tone(BUZZER_GPIO_NUM, 0);
    buzzer_on = false;
    digitalWrite(TL_GREEN_GPIO_NUM, LOW);
    tl_state = TL_STATE_YELLOW;
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

  // Initialize LEDC for PWM for tone
  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQ, BUZZER_RESOLUTION);
  ledcAttachPin(BUZZER_GPIO_NUM, BUZZER_CHANNEL);
}

bool app_button_pushed() {
  return digitalRead(BUTTON_GPIO_NUM) == 1;
}