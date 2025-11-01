#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#define BAUD_RATE               (PIO_BAUD_RATE)

// Traffic Light pin mappings
#define TL_RED_GPIO_NUM        (2)
#define TL_YELLOW_GPIO_NUM     (18)
#define TL_GREEN_GPIO_NUM      (4)
#define BUTTON_GPIO_NUM        (32)
#define BUZZER_GPIO_NUM        (12)

// Buzzer specific configurations 
#define BUZZER_FREQ            (440)
#define BUZZER_CHANNEL   0   // channel 0–15
#define BUZZER_RESOLUTION 8   // bits (1–16)


// ***** Traffic light delays *****

// Red state
#define TL_DELAY_RED_MS         (10000)
#define TL_DELAY_RED_BUZZER_MS  (250)

// Yellow and Yellow_Red State
#define TL_DELAY_YELLOW_MS      (2000)

// Green State
#define TL_DELAY_GREEN_MS       (5000)
#define TL_DELAY_RED_BUZZER_ON_MS   (500)
#define TL_DELAY_RED_BUZZER_OFF_MS  (1500)

typedef enum _TL_STATE {
    TL_STATE_RED,
    TL_STATE_RED_YELLOW,
    TL_STATE_YELLOW,
    TL_STATE_GREEN,
} TL_STATE;

#endif // __APP_CONFIG_H