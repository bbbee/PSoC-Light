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
#include "LED_strip.h"
#include "stdio.h"

 

void write_uniform_strip(uint8 blue, uint8 green, uint8 red)
{    
   for (uint8 i = 0; i<=NUM_LEDS; i++) 
    {
        LED_strip.leds[i].Blue = blue;
        LED_strip.leds[i].Green = green;
        LED_strip.leds[i].Red = red;
    }
    LED_strip.global_scale = 0xF0;
}

void write_SPIM(strip LED_strip) {
    send_strip_Start();
    for (uint8 i=0; i<=NUM_LEDS; i++) {
        SPIM_WriteTxData(LED_strip.global_scale);
        SPIM_WriteTxData(LED_strip.leds[i].Blue);
        SPIM_WriteTxData(LED_strip.leds[i].Green);
        SPIM_WriteTxData(LED_strip.leds[i].Red);
        
    }
   
    send_strip_Stop();

}
void send_strip_Start(void) {
    SPIM_WriteTxData(0x00);
    SPIM_WriteTxData(0x00);
    SPIM_WriteTxData(0x00);
    SPIM_WriteTxData(0x00);
}

void send_strip_Stop(void) {
    SPIM_WriteTxData(0xFF);
    SPIM_WriteTxData(0xFF);
    SPIM_WriteTxData(0xFF);
    SPIM_WriteTxData(0xFF);     
    
}
/* [] END OF FILE */
