#ifndef __SIIC_H
#define __SIIC_H
#include "stdint.h"
#include "stm32f10x.h"

#define HIGH 1
#define LOW 0

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOD_ODR_Addr    (GPIOD_BASE+12)     
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) 

#define SCL_IOGroup GPIOD
#define SCL_PIN 0
#define SDA_IOGroup GPIOD
#define SDA_PIN 1

#define SDA_IN() 			{SDA_IOGroup->CRL &= 0x0FFFFFFF;SDA_IOGroup->CRL |=(u32)8<<28;}
#define SDA_OUT() 			{SDA_IOGroup->CRL &= 0x0FFFFFFF;SDA_IOGroup->CRL |=(u32)3<<28;}

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入


#define SET_SCL				PDout(SCL_PIN)
#define READ_SCL			PDin(SCL_PIN)
#define SET_SDA				PDout(SDA_PIN)//输出数据
#define READ_SDA			PDin(SDA_PIN)//读数据

//extern void sw_i2c_start();


#endif 
