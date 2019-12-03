#include "config.h"
#include "byteToBin.h"
#include "PCF8583Write.h"
#include "decToBcd.h"
#include "clearRow.h"
#include <stdio.h>
void setupCMgr(void)
{
    uchar LsbH, MsbH, LsbM, MsbM = 0;
    static uchar disableHours = 0;
    static uchar disableMins = 0;
    
    uchar result;
    switch(state)
    {
        case 1:
            
            if(UP == 0)
            {
                state = 2;
            }
            else if(UP == 1)
            {
                state = 1;
            }
            else if (DOWN == 0)
            {
                state = 3;
            }
            break;
            
        case 2:
            if(UP == 0 && hour <23) // ensures hours are never more than 23
            { 
                hour++;
                __delay_ms(250);
                state = 2;
            }
            if(DOWN == 0)
            {
                state = 3;
            }
                
            else if(UP == 1 && ENT == 0)
            {
                state = 4;
            }
            break;
            
        case 3:
            if(DOWN == 0 && hour >=1)   // ensures hours can never be negative
            {
                hour--;
                __delay_ms(250);
                state = 3;
                
            }
            else if(UP == 0)
            {
                state = 2;
            }
            else if(UP == 1 && ENT == 0)
            {
                state = 4;
            }
            break;
         
        case 4:
            if((DOWN == 1 || UP == 1) && ENT == 0)
            {
                result = decToBcd(hour);
                
                PCF8583Write(0xa0,0x04,result);// write hours
    
            }
            state = 5;
            
            break;
            
        case 5:
            
            if(UP == 1 && DOWN == 1)
            {
                state = 5;
            }
            else if(UP == 0)
            {
                state = 6;
            }
            break;
            
        case 6:
            
            if(UP == 0 && min <59)   // ensures minutes can never be greater than 59
            {
                min++;
                __delay_ms(250);
                state = 6;
                
            }
            else if(DOWN == 0)
            {
                state = 7;  
            }
            else if (UP == 1 && ENT == 0)
            {
                state = 8;
            }
            break;
            
        case 7:
            if(DOWN == 0 && min >=1)   //ensures minutes can never have a -ve value, 
            {
                min--;
                __delay_ms(250);
                state = 7;
                
            }
            else if(UP == 0)
            {
                state = 6;
            }
            else if((UP == 1 || DOWN == 1) && ENT == 0)
            {
                state = 8;
            }
            break;
            
        case 8:
            result = decToBcd(min);   
            PCF8583Write(0xa0,0x03,result);// write minutes
            
            state = 1;
            break;
           
    }
   /* printf(" state %d\n", state);
      clearRow(0x94, 0xa7);
      clearRow(0xc0, 0xd3);
      clearRow(0xd4, 0xe7);*/
    
    
        LsbH = hour%10;
        MsbH = hour/10;
        byteToBin(0, 0x9b);
        byteToBin(1, MsbH + 0x30);
        byteToBin(1, LsbH + 0x30);
        

        LsbM = min%10;          // display mins set value on line 4
        MsbM = min/10;
        byteToBin(0, 0xdd);
        byteToBin(1, MsbM + 0x30);
        byteToBin(1, LsbM + 0x30);
    
}
