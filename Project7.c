/* 
 * File:   Project7.c
 * Author: kouji
 *
 * Created on December 4, 2022, 5:13 PM
 */

#include "mcc_generated_files/mcc.h"
#include "buttons.h"
#include "myUART.h"
#include "putty.h"
#include <stdbool.h>



int main(void) 
{ 
    // Initialize the device
    SYSTEM_Initialize();
    clearPuTTY();
  
    unsigned int has_switch1_changed = 0;
    unsigned int counter = 0, n = 0;
    uint32_t time1=1, time2=2 ,distance = 3;
    
    PWM6_LoadDutyValue(0); // change CCPR1H:CCPRxL
                                   
    PWM5_LoadDutyValue(568); // change CCPR1H:CCPRxL
    T2CONbits.CKPS = 2;
    T2PR = 0x8D;// change PWM5DCH:PWM5DCL
    n = T2CONbits.CKPS;     // prescaler setting, N = 2^n                                       
    PWM7_LoadDutyValue(0);
    
    SMT1CON0bits.EN = 1;             // enable SMT peripheral
    SMT1_DataAcquisitionEnable();    // allows acquisitions/captures  
    SMT1_RepeatDataAcquisition();    // allow repeated measurements
     printf("if distance is more than 60cm: shows fine and no sounds\n\r"
                  "if distance is between 20cm and 60cm: caution and make sounds\n\r"
                  "if getting closer: increase frequency\n\r"
                  "if distance is less than 20cm: shows alert and make high frequency sounds\n\r");
    while(1) 
    { 
        
      has_switch1_changed = poll_switch1_for_edges(IO_RD0_GetValue());
      while(!IO_RD0_GetValue()){
          
            PWM5_LoadDutyValue(505); // change CCPR1H:CCPRxL   // green blink                            
            PWM6_LoadDutyValue(566); // change CCPR1H:CCPRxL  
            DELAY_milliseconds(200);
            PWM6_LoadDutyValue(0); // change CCPR1H:CCPRxL                                  
            PWM5_LoadDutyValue(568); // change CCPR1H:CCPRxL
            DELAY_milliseconds(200);
            has_switch1_changed = poll_switch1_for_edges(IO_RD0_GetValue()); 
                                   
      }       
      while(has_switch1_changed == 1){
          // capture 1 pulse width          
            PWM5_LoadDutyValue(440); // change CCPR1H:CCPRxL                               
            PWM6_LoadDutyValue(566); // change CCPR1H:CCPRxL             
            IO_RC1_SetDigitalOutput();
            IO_RC1_SetHigh(); 
            DELAY_microseconds(20);
            IO_RC1_SetLow();
            IO_RC1_SetDigitalInput();
            SMT1_ManualTimerReset();  // zero SMT1 counter/timer  
            SMT1PWAIF = 0;            // clear pulse width (FE) interrupt flag
            while(SMT1PWAIF == 0);    // wait for pulse width buffer to fill           
            time1 = SMT1_GetCapturedPulseWidth();  // read buffer                               
            printf("Pulse width = %lu musec\n\r", time1);
            
            distance = time1*170/1000;
            printf("distance = %lu cm\n\r", distance);
            
            
      while(distance > 60){
                PWM5_LoadDutyValue(505); // change CCPR1H:CCPRxL                               
                PWM6_LoadDutyValue(566); // change CCPR1H:CCPRxL 
                PWM7_LoadDutyValue(0);
                printf("status: fine\n\r");
                
                DELAY_microseconds(20);
                IO_RC1_SetDigitalOutput();
                IO_RC1_SetHigh(); 
                DELAY_microseconds(20);
                IO_RC1_SetLow();
                IO_RC1_SetDigitalInput();
                SMT1_ManualTimerReset();  // zero SMT1 counter/timer  
                SMT1PWAIF = 0;            // clear pulse width (FE) interrupt flag
                while(SMT1PWAIF == 0);    // wait for pulse width buffer to fill
               
                time1 = SMT1_GetCapturedPulseWidth();  // read buffer                               
                printf("Pulse width = %lu musec\n\r", time1);

                distance = time1*170/1000;
                printf("distance = %lu cm\n\r", distance);

            }
            
            while((distance >= 20) && (distance <= 60) ){
                
                 PWM5_LoadDutyValue(425); // change CCPR1H:CCPRxL                                      
                 PWM6_LoadDutyValue(311); // change CCPR1H:CCPRxL
                 printf("status: caution\n\r");
                 
                while(distance > 50){
                     PWM7_LoadDutyValue(477); // change CCPR1H:CCPRxL
                     T2CONbits.CKPS = 2;
                     T2PR = 0xEE;// change PWM5DCH:PWM5DCL
                     n = T2CONbits.CKPS;     // prescaler setting, N = 2^n
                     IO_RC1_SetDigitalOutput();
                     IO_RC1_SetDigitalOutput();
                     IO_RC1_SetHigh(); 
                     DELAY_microseconds(20);
                     IO_RC1_SetLow();
                     IO_RC1_SetDigitalInput();
                     SMT1_ManualTimerReset();  // zero SMT1 counter/timer  
                     SMT1PWAIF = 0;            // clear pulse width (FE) interrupt flag
                     while(SMT1PWAIF == 0);    // wait for pulse width buffer to fill
                     
                    time1 = SMT1_GetCapturedPulseWidth();  // read buffer                               
                    printf("Pulse width = %lu musec\n\r", time1);

                    distance = time1*170/1000;
                    printf("distance = %lu cm\n\r", distance);
                     
                }               
                  while(distance > 30 ){
                      PWM7_LoadDutyValue(377); // change CCPR1H:CCPRxL
                      T2CONbits.CKPS = 2;
                      T2PR = 0xBD;// change PWM5DCH:PWM5DCL
                      n = T2CONbits.CKPS;     // prescaler setting, N = 2^n
                      
                      IO_RC1_SetDigitalOutput();
                      IO_RC1_SetHigh(); 
                      DELAY_microseconds(20);
                      IO_RC1_SetLow();
                      IO_RC1_SetDigitalInput();
                      SMT1_ManualTimerReset();  // zero SMT1 counter/timer  
                      SMT1PWAIF = 0;            // clear pulse width (FE) interrupt flag
                      while(SMT1PWAIF == 0);    // wait for pulse width buffer to fill
                      time1 = SMT1_GetCapturedPulseWidth();  // read buffer                               
                      printf("Pulse width = %lu musec\n\r", time1);
                      distance = time1*170/1000;
                      printf("distance = %lu cm\n\r", distance);
                 }                  
                    DELAY_microseconds(20);
                   IO_RC1_SetDigitalOutput();
                   IO_RC1_SetHigh(); 
                   DELAY_microseconds(20);
                   IO_RC1_SetLow();
                   IO_RC1_SetDigitalInput();
                   SMT1_ManualTimerReset();  // zero SMT1 counter/timer  
                   SMT1PWAIF = 0;            // clear pulse width (FE) interrupt flag
                   while(SMT1PWAIF == 0);    // wait for pulse width buffer to fill

                   time1 = SMT1_GetCapturedPulseWidth();  // read buffer                               
                   printf("Pulse width = %lu musec\n\r", time1);

                   distance = time1*170/1000;
                   printf("distance = %lu cm\n\r", distance);
                                                                
            }
            
            while(distance < 20){
                // red blink
                PWM6_LoadDutyValue(0); // change CCPR1H:CCPRxL                   
                PWM5_LoadDutyValue(1133); // change CCPR1H:CCPRxL 
                DELAY_milliseconds(200);                    
                PWM6_LoadDutyValue(0); // change CCPR1H:CCPRxL                                  
                PWM5_LoadDutyValue(568); // change CCPR1H:CCPRxL
                
                PWM7_LoadDutyValue(317); // change CCPR1H:CCPRxL
                    T2CONbits.CKPS = 1;
                    T2PR = 0x9E;// change PWM5DCH:PWM5DCL
                    n = T2CONbits.CKPS;     // prescaler setting, N = 2^n  
                    row4_RC7_GetValue();
                printf("status: alert\n\r");
                
                DELAY_microseconds(20);
                IO_RC1_SetDigitalOutput();
                IO_RC1_SetHigh(); 
                DELAY_microseconds(20);
                IO_RC1_SetLow();
                IO_RC1_SetDigitalInput();
                SMT1_ManualTimerReset();  // zero SMT1 counter/timer  
                SMT1PWAIF = 0;            // clear pulse width (FE) interrupt flag
                while(SMT1PWAIF == 0);    // wait for pulse width buffer to fill
           
                time1 = SMT1_GetCapturedPulseWidth();  // read buffer                               
                printf("Pulse width = %lu musec\n\r", time1);

                distance = time1*170/1000;
                printf("distance = %lu cm\n\r", distance);
                                           
            }
           
      }       
      
    } 
    return 0;
} 

