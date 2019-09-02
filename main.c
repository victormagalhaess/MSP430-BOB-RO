/************************************************************************************************************************************************************\
author: Victor Hugo Faria
date: 10/2017

                              MSP430G2553 OR MSP430F2132
                              __________________________
                             |                          |                           ______________________________
        LED------------------| P1.0                     |                          |__________SUBTITLE____________|
                             | P1.1                     |                          |RESISTOR     ||     ---mmmm-- |
                             | P1.2                     |                          |BUTTON       ||        //-*   |   
         (ON)//-*----------- | P1.3                     |                          |_____________||_______________|
(RIGHT SENSOR)//-*-----------| P1.4                     |
(LEFT SENSOR)//-*------------| P1.5                     |
                   BUZZER----| P1.6                     |                                       
                             | P1.7                     |
                             |                          |
               VCC ---mmmm-- | RST                      |
                    |        |                          |
                    |        |                    P2.0  |
                   //-*      |                    P2.1  |-----------------IN4(PONTE H)
                    |        |                    P2.2  |----------------------------------IN3(PONTE H)------LEFT LED
                    |        |                    P2.3  |-----------------IN2(PONTE H)
                    GND      |                    P2.4  |----------------------------------IN1(PONTE H)------RIGHT LED
                             |                    P2.5  |
                             |                    P2.6  |
                             |                    P2.7  |
                             |__________________________|


\************************************************************************************************************************************************************/

#include "msp430.h" 

#define TIME_UNITY 25000
#define DEBOUNCE_TIME 18000
#define MOTORS 0X14
#define LEDS 0X01
#define STOP_SENSOR1 0X10 
#define STOP_SENSOR2 0X20
#define TURN_OFF 0X00
#define BUZZERS 0X40
#define BACK_MOTORS 0X0A
#define LEFT_MOTOR 0X10
#define RIGHT_MOTOR 0X04
#define BUTTON_UNPRESSED 0X08


int frente (long int temp) //straight movement function
{
  int test=0;
  
  for (long int i=0; i<temp; i++) 
  {
    P2OUT=MOTORS;
    P1OUT|=LEDS;
    
    if (((~P1IN & STOP_SENSOR1)==STOP_SENSOR1)||((~P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
      __delay_cycles(DEBOUNCE_TIME);//
      test=1;
    }
    while (test==1){ 
      
      P2OUT=TURN_OFF;  //turn off leds and motors if stop_sensor are active and turn on the buzzer 
      P1OUT|=BUZZERS; 
      P1OUT&=~LEDS;
      
      if (((P1IN & STOP_SENSOR1)==STOP_SENSOR1)&&((P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
        __delay_cycles(DEBOUNCE_TIME);// Debounce
        test=0;
        P1OUT&=~BUZZERS;
      }     
    }
  }
  
  
  P2OUT=TURN_OFF;//turn off motors
  P1OUT&=~LEDS;//turn off leds
  return 0;
}

int andar_re (long int temp) //Backwards movement function
{
  int test=0;
  
  for ( long int i=0; i<temp; i++)
  {
    P2OUT=BACK_MOTORS;//reverse the motors movement
    
    if (((~P1IN & STOP_SENSOR1)==STOP_SENSOR1)||((~P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
      __delay_cycles(DEBOUNCE_TIME);// Debounce
      test=1;
    }
    while (test==1){
      
      P2OUT=TURN_OFF;  //turn off leds and motors if stop_sensor are active and turn on the buzzer 
      P1OUT|=BUZZERS; 
      P1OUT&=~LEDS;
      
      if (((P1IN & STOP_SENSOR1)==STOP_SENSOR1)&&((P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
        __delay_cycles(DEBOUNCE_TIME);// Debounce
        test=0;
        P1OUT&=~BUZZERS;
      }
    }
  }
  P2OUT=TURN_OFF;
  return 0;
}

int esquerda (long int temp) //turn left movement function
{
  int test=0;
  
  for ( long int i=0; i<temp; i++)
  {
    P2OUT= LEFT_MOTOR;
    
    if (((~P1IN & STOP_SENSOR1)==STOP_SENSOR1)||((~P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
      __delay_cycles(DEBOUNCE_TIME);// Debounce
      test=1;
    }
    while (test==1){
      
      P2OUT=TURN_OFF;  //turn off leds and motors if stop_sensor are active and turn on the buzzer 
      P1OUT|=BUZZERS; 
      P1OUT&=~LEDS;
      
      if (((P1IN & STOP_SENSOR1)==STOP_SENSOR1)&&((P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
        __delay_cycles(DEBOUNCE_TIME);// Debounce
        test=0;
        P1OUT&=~BUZZERS;
      }
    }
  }
  P2OUT=TURN_OFF;
  return 0;
}

int direita (long int temp) // turn right movement function
{
  int test=0;
  
  for (long int i=0; i<temp; i++) 
  {
    P2OUT=RIGHT_MOTOR;
    
    if (((~P1IN & STOP_SENSOR1)==STOP_SENSOR1)||((~P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
      __delay_cycles(DEBOUNCE_TIME);// Debounce
      test=1;
    }
    while (test==1){
      
      P2OUT=TURN_OFF;  //turn off leds and motors if stop_sensor are active and turn on the buzzer 
      P1OUT|=BUZZERS;   
      P1OUT&=~LEDS;
      
      if (((P1IN & STOP_SENSOR1)==STOP_SENSOR1)&&((P1IN & STOP_SENSOR2)==STOP_SENSOR2)){ 
        __delay_cycles(DEBOUNCE_TIME);// Debounce
        test=0;        
        P1OUT&=~BUZZERS;
      } 
    }
  }
  P2OUT=TURN_OFF;
  return 0;
}

void parado (void) //stop fucntion
{
  while (P1IN&BUTTON_UNPRESSED)//Polling while that holds the program execution until the start buttons is pressed
  {
    __delay_cycles(DEBOUNCE_TIME);// Debounce
  } 
}


void trajeto(){     //Traject function   

  //Use X * TIME_UNITY to change the distance 
  //sample traject:
    parado();//waits until start button is pressed
    frente(4*TIME_UNITY); 
    direita(2*TIME_UNITY);
    frente(TIME_UNITY);  
    esquerda(TIME_UNITY);
    frente(3*TIME_UNITY);
    direita(TIME_UNITY);
    frente(2*TIME_UNITY);
    direita(2*TIME_UNITY);
    frente(TIME_UNITY);

}


void main()//main
{
  WDTCTL = WDTPW + WDTHOLD; // STOPS Watchdog times
  P1SEL=0x00; //configurations of the pins and ports
  P1SEL2=0x00;//  
  P1DIR=0xC7; //  
  P1REN=0x38; //
  P1OUT=0x38; //
  P2SEL=0x00; //   
  P2SEL2=0x00;//  
  P2DIR=0xFF; //
  P2REN=0x00; //     
  P2OUT=0x00; //
   
  while(1)
    {
    trajeto(); 
    }
}
