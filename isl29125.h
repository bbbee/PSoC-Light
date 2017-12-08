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
#if !defined(_ISL29125_H)
#define _ISL29125_H
    
#include <project.h>
#include "stdbool.h"
    
    
#define I2C_ADDRESS         0x44

/***************************************
*    ISL29125 Register assignment
***************************************/ 
    
#define DEVICE_ID_REG       0x00
#define CONFIG_REG_1        0x01
#define CONFIG_REG_2        0x02
#define CONFIG_REG_3        0x03
#define THRESHOLD_REG_LL    0x04
#define THRESHOLD_REG_LH    0x05
#define THRESHOLD_REG_HL    0x06
#define THRESHOLD_REG_HH    0x07
#define STATUS_REG          0x08 
#define GREEN_REG_L         0x09 
#define GREEN_REG_H         0x0A
#define RED_REG_L           0x0B
#define RED_REG_H           0x0C
#define BLUE_REG_L          0x0D
#define BLUE_REG_H          0x0E  
 
    
/***************************************
*       Configuration settings
***************************************/ 
    
// Configuration base settings
#define CONFIG_DEFAULT          0x00
    
// CONFIGURATION 1 REGISTER OPTIONS
// Pick color mode
#define CONFIG1_POWERDOWN       0x00
#define CONFIG1_GREEN_MODE      0x01
#define CONFIG1_RED_MODE        0x02
#define CONFIG1_BLUE_MODE       0x03
#define CONFIG1_STANDBY         0x04
#define CONFIG1_RGB_MODE        0x05
#define CONFIG1_RG_MODE         0x06
#define CONFIG1_GB_MODE         0x07
// Light intensity range
#define CONFIG1_375LUX          0x00
#define CONFIG1_10KLUX          0x08
// ADC accuracy
#define CONFIG1_16BIT           0x00
#define CONFIG1_12BIT           0x10
// interrupt pin setting
#define CONFIG1_NO_SYNC         0x00
#define CONFIG1_SYNC_TO_INT     0x20
    
// CONFIGURATION 2 REGISTER OPTIONS
// Range for IR filter
#define CONFIG2_IR_OFFSET_OFF   0x00  
#define CONFIG2_IR_OFFSET_ON    0x80   
// Sets amount of IR filtering, can use these presets or any value between 0x00 and 0x3F
// Consult datasheet for detailed IR filtering calibration
#define CONFIG2_IR_ADJUST_LOW   0x00
#define CONFIG2_IR_ADJUST_MID   0x20
#define CONFIG2_IR_ADJUST_HIGH  0x3F
    
// CONFIGURATION 2 REGISTER OPTIONS
// Chose what color fires an interupt
#define CONFIG3_NO_INT          0x00
#define CONFIG3_G_INT           0x01
#define CONFIG3_R_INT           0x02
#define CONFIG3_B_INT           0x03

// How many times a sensor sample must hit a threshold before triggering an interrupt
// More consecutive samples means more times between interrupts, but less triggers from short transients
#define CONFIG3_INT_1_TIME      0x00
#define CONFIG3_INT_2_TIME      0x04
#define CONFIG3_INT_4_TIME      0x08
#define CONFIG3_INT_8_TIME      0x0C
// Choose to have interrupt pin go low when a conversion is finished
#define CONFIG3_DISABLE_INT_ON_CONVERSION   0x00
#define CONFIG3_ENABLE_INT_ON_CONVERSION    0x10
    
    
/***************************************
*        Function Prototypes
***************************************/     



bool isl29125_init(void);    

    
uint16 isl29125_read_red(void);
uint16 isl29125_read_green(void);
uint16 isl29125_read_blue(void);





#endif
/* [] END OF FILE */
