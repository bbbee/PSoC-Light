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
#include "project.h"
// standard libraries
#include "math.h"
#include "stdio.h"

// local files
#include "isl29125.h"
#include "TSL2561.h"
#include "stdbool.h"
#include "usb_protocolc.h"
#include "globals.h"
#include "LED_strip.h"

float32 Red_Radiation;
float32 Green_Radiation;
float32 Blue_Radiation;

uint8 OUT_buffer[40];
uint8 usb_data = 0;
uint8 count = 0;
uint8 TSL2561_ready = true;
char LCD_str[40];


/***************************************
*        Function Prototypes
***************************************/ 

void get_color_values(void);
uint16 Convert2Dec(uint8 array[], uint8 len);
void Start_ISL_Power(void);
void Stop_ISL_Power(void);

float32 tolerance = 1.0 ;
float32 PID_alpha = 3.0;
float32 red_error;
float32 green_error;
float32 blue_error;

strip LED_strip;
LED LED_settings;
light Photon;

SensorData all_data;
int i;

bool get_color_flag = false;
bool lights_on = false;
bool Set_Radiation_Levels = false;
CY_ISR(Pin_SW2_Handler)
{
    if (get_color_flag) {
//        LCD_ClearDisplay(); 
//        LCD_Position(0,0);
//        LCD_PrintString("Off");
        get_color_flag = false;
        Pin_SW2_ClearInterrupt();
        
    }
    else {
//        LCD_ClearDisplay(); 
//        LCD_Position(0,0);
//        LCD_PrintString("Read");
        get_color_flag = true;
        Pin_SW2_ClearInterrupt();
        
    }

}
        

int main(void) {
//    Pin_SW2_Int_StartEx( Pin_SW2_Handler );
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    
    USBFS_Start(0, USBFS_DWR_VDDD_OPERATION);  // initialize the USB
    while(!USBFS_bGetConfiguration());  //Wait till it the usb gets its configuration from the PC
    USBFS_EnableOutEP(OUT_ENDPOINT);
    i = 1;
    
    SPIM_Start();
    write_uniform_strip(10, 10, 10);
    write_SPIM(LED_strip);
    
    LCD_Start();
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Start");
    I2C_Start();
    isl29125_init();
    TSL2561_enable();
//    Start_ISL_Power();
    
    for(;;){
        uint8 input = USB_CheckInput(OUT_buffer);
        
        if ( input == true ) {
            
            switch( OUT_buffer[0] ) {
            //if (OUT_buffer[0] == 'E' ) {
                case 'O': ;
                    Set_Radiation_Levels = false;
                    write_uniform_strip(0, 0, 0);
                    write_SPIM(LED_strip);
                    break;
                case 'E': ;
                    uint8 red = Convert2Dec(&OUT_buffer[2], 3); 
                    uint8 green = Convert2Dec(&OUT_buffer[6], 3); 
                    uint8 blue = Convert2Dec(&OUT_buffer[10], 3); 
                    write_uniform_strip(blue, green, red);
                    write_SPIM(LED_strip);
                    break;
                case 'S': ;
                    get_color_flag = false;
                    break;
                case 'T': ;
                    get_color_flag = true;

                    break;
                case 'R':;
                    LCD_Position(0,0);
                    LCD_PrintString("Rad level");
                    Red_Radiation = Convert2Dec(&OUT_buffer[2],5)/100.0; 
                    Green_Radiation = Convert2Dec(&OUT_buffer[8],5)/100.0; 
                    Blue_Radiation = Convert2Dec(&OUT_buffer[14],5)/100.0; 
                    Set_Radiation_Levels = true;
                    CyDelay(500);
                    LCD_ClearDisplay();
                    break;
                case 'P':;
                    Set_Radiation_Levels = false;
                    break;
                }

            OUT_buffer[0] = 0;
        }  
        if(get_color_flag == true){
            LCD_Position(0,0);
            LCD_PrintString("Read");
            get_color_values();      
        }
        
        if(Set_Radiation_Levels == true){
            
            red_error = (Red_Radiation - Photon.PhotonR);
            if (fabs(red_error) > tolerance)
            {
                uint16 check = LED_settings.Red + (int8) red_error*PID_alpha;
//                LCD_Position(1,0);
//                sprintf(LCD_str,"c:%i|%2.2f|%2.2f", check, red_error, Red_Radiation); 
//                LCD_PrintString(LCD_str);
                if (check > 255) {
                    LED_settings.Red = 255;
                }
                else if ( check < 0 ){
                    LED_settings.Red = 0;
                }
                else {
                    LED_settings.Red = (uint8) check;  
                }
            }
            
            green_error = (Green_Radiation - Photon.PhotonG);
            if (fabs(green_error) > tolerance)
            {
                uint16 check = LED_settings.Green + (int8) green_error*PID_alpha;
                LCD_Position(1,0);
                sprintf(LCD_str,"c:%i|%2.2f|%2.2f", check, green_error, Green_Radiation); 
                LCD_PrintString(LCD_str);
                if (check > 255) {
                    LED_settings.Green = 255;
                }
                else if ( check < 0 ){
                    LED_settings.Green = 0;
                }
                else {
                    LED_settings.Green = (uint8) check;  
                }
            }

            blue_error = (Blue_Radiation - Photon.PhotonB);
            if (fabs(blue_error) > tolerance)
            {
                uint16 check = LED_settings.Blue + (int8) blue_error*PID_alpha;
//                LCD_Position(1,0);
//                sprintf(LCD_str,"c:%i|%2.2f|%2.2f", check, blue_error, Blue_Radiation); 
//                LCD_PrintString(LCD_str);
                if (check > 255) {
                    LED_settings.Blue = 255;
                }
                else if ( check < 0 ){
                    LED_settings.Blue = 0;
                }
                else {
                    LED_settings.Blue = (uint8) check;  
                }
            }
            
            write_uniform_strip(LED_settings.Blue, LED_settings.Green, LED_settings.Red);
            write_SPIM(LED_strip);
            CyDelay(100);
        }
        
        
    }
}
    
void get_color_values(void){
    uint16 red = isl29125_read_red();
    uint16 green = isl29125_read_green();
    uint16 blue = isl29125_read_blue();
 
    float32 luxRGB =  (float32)0.0286*(float32)red+(float32)0.0423*(float32)green+(float32)0.0825*(float32)blue;
    Photon.PhotonR = (float32)red*(float32)0.0004187; 
    Photon.PhotonG = (float32)green*(float32)0.0030964;
    Photon.PhotonB = (float32)blue*(float32)0.0012079;
    
    all_data.lux = luxRGB;
    all_data.PRed = Photon.PhotonR;
    all_data.PGreen = Photon.PhotonG;
    all_data.PBlue = Photon.PhotonB;

//    LCD_Position(0,0);
//    sprintf(LCD_str,"lux:%f",luxRGB);
//    LCD_PrintString(LCD_str);
//
//    LCD_Position(0,8);
//    sprintf(LCD_str,"PR:%f ",Photon.PhotonR );
//    LCD_PrintString(LCD_str);
//     
//    LCD_Position(1,0);
//    sprintf(LCD_str,"PG:%f ",Photon.PhotonG);
//    LCD_PrintString(LCD_str);
//     
//    LCD_Position(1,8);
//    sprintf(LCD_str,"PB:%f ",Photon.PhotonB);
//    LCD_PrintString(LCD_str);
//    CyDelay(250);
  
    USB_Export_Data(all_data, 16);
    usb_data++;
    count++;
    CyDelay(1000);

}

uint16 Convert2Dec(uint8 array[], uint8 len){
    uint16 num = 0;
    for (int i = 0; i < len; i++){
        num = num * 10 + (array[i] - '0');
    }
    return num;
}

//void Start_ISL_Power(void) {
//    
//    VDAC_3_3V_Start();
//    Opamp_3_3V_Start();
//    
//}
//void Stop_ISL_Power(void) {
//    
//    VDAC_3_3V_Stop();
//    Opamp_3_3V_Stop();
//    
//}

/* [] END OF FILE */
