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
#if !defined(_GLOBALS_H)
#define _GLOBALS_H
#define NUM_LEDS 60
#include <project.h>
    
typedef struct 
{
    float32 lux;
    float32 PRed;
    float32 PGreen;
    float32 PBlue;
    
 } SensorData;

typedef struct 
{
    
    float32 PhotonR; 
    float32 PhotonG;
    float32 PhotonB; 
    
 } light;

#endif
/* [] END OF FILE */
