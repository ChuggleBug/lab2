#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#define BAUD_RATE               (PIO_BAUD_RATE)

#define TL_RED_GPIO_NUM        (2)
#define TL_YELLOW_GPIO_NUM     (18)
#define TL_GREEN_GPIO_NUM      (4)

#define BUTTON_GPIO_NUM        (32)

#define BUZZER_GPIO_NUM        (12)
#define BUZZER_FREQ            (440)

typedef enum _TL_STATE {
    TL_STATE_RED,
    TL_STATE_RED_YELLOW,
    TL_STATE_YELLOW,
    TL_STATE_GREEN,
} TL_STATE;

#endif // __APP_CONFIG_H