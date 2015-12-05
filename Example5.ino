/***
 * Millenium Library - Arduino Workshop
 * 
 * TITLE:        Example #5
 * DESCRIPTION:  Input Interrupt.
 * AUTHOR:       Troy Denton (trdenton@gmail.com)
 * DATE:         December 5, 2015
 *
 * Toggle an LED using only interrupts
 * 
 **/ 

/***
 * setup()
 * Description:  Initialize interrupts, pins
 **/
void setup()
{
 // Serial debugging
  Serial.begin(9600);


  //configure pin 13 as our output
  pinMode(13,OUTPUT);

  //configure pin 3 as an input
  pinMode(3,INPUT_PULLUP);
  
  //attach interrupt functions to pin 3
  //replace RISING with FALLING - what's the difference?
  attachInterrupt(0,pin3Interrupt,RISING); 
}


//our input pin interrupt
void pin3Interrupt()
{

  //make the interrupt toggle the state of the output LED

}

/***
 * loop()
 * Description:  Read a key and print a msg to the LCD.
 **/
void loop()
{
}

