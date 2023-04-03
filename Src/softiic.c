#include "softiic.h"

void iicioinit()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pins : PI7 PI6 */
    GPIO_InitStruct.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void SDA_READ2SET()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = GPIO_PIN_1;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void SDA_SET2READ()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = GPIO_PIN_1;
    GPIO_InitStruct.Mode             = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void delay_us(uint32_t us)
{
    uint32_t temp;
    SysTick->LOAD = us * 72; // 72MHz主频
    SysTick->VAL  = 0x00;
    SysTick->CTRL = 0x01;
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL = 0x00;
    SysTick->VAL  = 0X00;
}

unsigned short BD_Add_OddEven(unsigned short byte)
{
    unsigned char i;
    unsigned char n;
    unsigned short r;
    n = 0;
    for (i = 0; i < 10; i++) {
        if (((byte >> i) & 0x01) == 0x01) {
            n++;
        }
    }
    if ((n & 0x01) == 0x01) {
        r = byte | 0x400;
    } else {
        r = byte | 0x00;
    }
    return r;
}

unsigned short BD_Check_OddEven(unsigned short byte)
{
    unsigned char i;
    unsigned char n;
    unsigned char r;
    n = 0;
    for (i = 0; i < 10; i++) {
        if (((byte >> i) & 0x01) == 0x01) {
            n++;
        }
    }
    if ((byte >> 10) == (n & 0x01)) {
        r = 1;
    } else {
        r = 0;
    }
    return r;
}

void BD_I2C_start(void)
{
    SET_SDA = 1;
    SET_SCL = 1;
    delay_us(delaytime);
    SET_SDA = 0;
    delay_us(delaytime);
    SET_SCL = 0;
    delay_us(delaytime);
}

void BD_I2C_stop(void)
{
    //delay_us(delaytime);
    SET_SDA = 0;

    delay_us(delaytime);
    SET_SCL = 1;
    delay_us(delaytime);
    SET_SDA = 1;
    delay_us(delaytime);
}

uint16_t BD_I2C_read(void)
{
    uint16_t b = 1024;

    // BD_read_lock = 1;
    BD_I2C_start();

    SET_SDA = 1;
    SET_SCL = 1;
    delay_us(delaytime);
    SET_SCL = 0;

    delay_us(delaytime);
    b       = 0;
    SET_SDA = 1;
    // sda_gpio_in = gpio_in_setup(sda_pin, 1);
    for (unsigned char i = 0; i <= 10; i++) {
        b <<= 1;
        delay_us(delaytime);
        SET_SCL = 1;
        if (READ_SDA == 1)
            b |= 1;
        delay_us(delaytime);
        SET_SCL = 0;
    }
    BD_I2C_stop();
    if (BD_Check_OddEven(b) && (b & 0x3FF) < 1020)
        b = (b & 0x3FF);
    if (b > 1024)
        b = 1024;
    // BD_read_lock = 0;
    return b;
}

void BD_I2C_write(unsigned int addr)
{
    BD_I2C_start();
    //// write
    SET_SDA = 0;
    SET_SCL = 1;
    delay_us(delaytime);
    SET_SCL = 0;
    addr    = BD_Add_OddEven(addr);
    /// write address
    delay_us(delaytime);
    for (int i = 10; i >= 0; i--) {
        if ((addr >> i) & 0x01) {
            SET_SDA = 1;
        } else {
            SET_SDA = 0;
        }
        delay_us(delaytime);
        SET_SCL = 1;
        delay_us(delaytime);
        SET_SCL = 0;
        delay_us(delaytime);
        //     BD_setLow(sda_gpio);
        // ndelay_bd(delay_m);
        // BD_setHigh(scl_gpio);
        // ndelay_bd(delay_m);
        // BD_setLow(scl_gpio);
        // ndelay_bd(delay_m);
    }
    BD_I2C_stop();
}
