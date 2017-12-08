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
#include "isl29125.h"
#include "stdio.h"

uint8 read_buffer[16];
uint8 write_buffer[16];

extern char LCD_str[40];

static bool isl29125_reset(void);
static bool isl29125_config(uint8 config1, uint8 config2, uint8 config3);
static void isl29125_write8(uint8 _register, uint8 data);
static uint8 isl29125_read8(uint8 _register);
static uint16 isl29125_read16(uint8 _register);

bool isl29125_init(void) {
    uint8 data;
    data = isl29125_read8(DEVICE_ID_REG);
    if (data != 0x7D) {
        return false;
    }
    if (!isl29125_reset()) {
        return false;
    }
    
    if (!isl29125_config(CONFIG1_RGB_MODE | CONFIG1_10KLUX, CONFIG2_IR_ADJUST_HIGH, CONFIG_DEFAULT)) {
        return false;
    }
    return true;
}

static bool isl29125_reset(void) {
    uint8 data = 0x00;
    
    // Reset the registers
    isl29125_write8(DEVICE_ID_REG, 0x46);
    
    // Check reset
    data = isl29125_read8(CONFIG_REG_1);
    data |= isl29125_read8(CONFIG_REG_2);
    data |= isl29125_read8(CONFIG_REG_3);
    data |= isl29125_read8(STATUS_REG);
    if (data == 0x00) {
        return true;
    }
    return false;
}

static bool isl29125_config(uint8 config1, uint8 config2, uint8 config3) {
    uint8 data;  
    // set up the registers
    isl29125_write8(CONFIG_REG_1, config1);
    isl29125_write8(CONFIG_REG_2, config2);
    isl29125_write8(CONFIG_REG_3, config3);
    // confirm the registers are correct
    data = isl29125_read8(CONFIG_REG_1);
    if (data != config1) {
        return false;
    }
    data = isl29125_read8(CONFIG_REG_2);
    if (data != config2) {
        return false;
    }
    data = isl29125_read8(CONFIG_REG_3);
    if (data != config3) {
        return false;
    }
    return true;
}

uint16 isl29125_read_red(void) {
    return isl29125_read16(RED_REG_L);
  
}
uint16 isl29125_read_green(void) {
    return isl29125_read16(GREEN_REG_L);
  
}
uint16 isl29125_read_blue(void) {
    return isl29125_read16(BLUE_REG_L);
  
}

static void isl29125_write8(uint8 _register, uint8 data) {
    
    uint8 status = 0;
    write_buffer[0] = _register;
    write_buffer[1] = data;
    do {
        status = I2C_MasterWriteBuf(I2C_ADDRESS, (uint8 *)write_buffer, 2, I2C_MODE_COMPLETE_XFER);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while(status != I2C_MSTR_NO_ERROR);
}


static uint8 isl29125_read8(uint8 _register) {
    uint8 data;
    uint8 temp;
    do {
        temp = I2C_MasterWriteBuf(I2C_ADDRESS, &_register, 1, I2C_MODE_NO_STOP);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    I2C_MasterClearWriteBuf();

    do {
        temp = I2C_MasterReadBuf(I2C_ADDRESS, (uint8 *)read_buffer, 1, I2C_MODE_REPEAT_START);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    data = read_buffer[0];
    I2C_MasterClearReadBuf();
    return data;

}

static uint16 isl29125_read16(uint8 _register) {
    uint16 data = 0x0000;
    uint8 temp;
    do {
        temp = I2C_MasterWriteBuf(I2C_ADDRESS, &_register, 1, I2C_MODE_NO_STOP);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    I2C_MasterClearWriteBuf();
    do {
        temp = I2C_MasterReadBuf(I2C_ADDRESS, (uint8 *)read_buffer, 2, I2C_MODE_REPEAT_START);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    data = read_buffer[0];
    data |= (read_buffer[1] << 8);
    //LCD_Position(1, 0);
    //sprintf(LCD_str, "data: %02X, %02X", read_buffer[0], read_buffer[1]);
    //LCD_PrintString(LCD_str);
    I2C_MasterClearReadBuf();
    return data;
}



/* [] END OF FILE */
