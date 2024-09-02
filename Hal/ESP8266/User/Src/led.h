#ifndef _LED_H__
#define _LED_H__


#include <stdint.h>

#ifndef LED_PIN
    #define LED_Toggle()     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    #define LED_ON()         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    #define LED_OFF()        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
#else
#endif


// void Led_On(void);
// void Led_Off(void);

#endif

