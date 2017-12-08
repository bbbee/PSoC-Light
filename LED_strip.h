/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#if !defined(_LED_STRIP_H)
#define _LED_STRIP_H
#include <project.h>
#include "stdbool.h"
#define NUM_LEDS 120 

typedef struct{
    uint8 Blue;
    uint8 Green;
    uint8 Red;
}LED;

typedef struct {
    LED leds[NUM_LEDS];
    uint8 global_scale;
}strip;



strip LED_strip;

void write_SPIM(strip LED_strip);
void write_uniform_strip(uint8 blue, uint8 green, uint8 red);
void write_SPIM(strip LED_strip);
void send_strip_Start(void);
void send_strip_Stop(void);   
    
#endif
/* [] END OF FILE */
