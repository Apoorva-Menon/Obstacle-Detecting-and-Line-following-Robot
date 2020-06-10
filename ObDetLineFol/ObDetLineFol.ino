#include <avr/io.h>
#include <util/delay.h>

void init_adc() // ADC initialization
{
ADCSRA = ADCSRA | (0b00000001 << ADEN); //Enable the ADC
ADCSRA = ADCSRA | (0b00000001 << ADPS2); // Set sampling rate
ADMUX = ADMUX | (0b00000001 << REFS0); // Set reference voltage


}
unsigned int read_adc_ldr()
{
int p = 0b00000000; // select the input pin for ldr1 ADC0 - assuming left ldr
ADMUX = ADMUX | p; // ADMUX = 01000000
ADCSRA = ADCSRA | (0b00000001 << ADSC); // Start conversion
while ((ADCSRA & (0b00000001 << ADIF)) == 0) // Check for end conversion
{}

ADCSRA = ADCSRA | (0b00000001 << ADIF); //clear ADIF bit
return(ADC); // get the output from ADC

}

void main()
{
DDRD =0b00000000; // part d as input
DDRB =0b11111111; // part b as output
PORTD =0b11111111; // pull up all pins in part d
int c;
init_adc(); // call the ADC initializing function

_delay_ms(500); 
unsigned int ldr; //Initialized as unsigned int as it relates to analog input
unsigned int p;
unsigned int T;
unsigned int TON;
unsigned int TOFF;

while (1)
{
p = read_adc_ldr(); // read the ADC input from ADC - left ldr
c=PIND;

ldr = (p/40); //Scaling the total time period

T = 25.6; // TOtal time period = (1024/40)
TOFF = ldr; // ON time based on the current Analog input
TON = T - TOFF; // On time = Total Time period - Off Time

if (p<717) // Vin = 3.5v from multimeter
{
  if (c==0b11111011)             // Line on Right 
    {                     
    PORTB = 0b00001001;         // Move Left wheel
        }                    
      
  if (c==0b11111110)           //Line on Left
        {     
    PORTB = 0b00000110;   // Move Right wheel
    }
  if (c==0b11111010)        // no black line
    {
    PORTB = 0b00001010;   //Move forward
    }
  if (c==0b11111111)            //Both Black
      {     
    PORTB=0b00011010;  // Move forward and set buzzer high
    _delay_ms(100);
    PORTB=0b00001010;
        } 
    else if(c==0b11111001 | c==0b11111100 | c==0b11111101)
        {
        PORTB=0b00010000;         
    } 
}
else
{
 if (c==0b11111011)             // Line on Right 
  {                     
  PORTB = 0b00001001;         // Move Left wheel
  _delay_ms(TOFF);
  PORTB = 0b00001010;
  _delay_ms(TON);
    }                    
      
if (c==0b11111110)           //Line on Left
    {     
  PORTB = 0b00000110;   // Move Right wheel
  _delay_ms(TOFF);
  PORTB = 0b00001010;
  _delay_ms(TON);
  
  }
if (c==0b11111010)        // no black line
  {
  PORTB = 0b00001010;   //Move forward
  _delay_ms(TOFF);
  PORTB = 0b00001010;
  _delay_ms(TON);
  }  
if (c==0b11111111)            //Both Black
  {     
  PORTB=0b00011010;
    }
else if(c==0b11111001 | c==0b11111100 | c==0b11111101)
    {
    PORTB=0b00010000;         
  } 
}
}
}

    
    
    
    
    
    
    
