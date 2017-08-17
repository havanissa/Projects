#if (HW_TYPE == 0)

  #define HAZ_BUZZER 1
  #define HAZ_BUTTON 1
  #define BUZZER 10
  #define BTN 11
  #define RF_OUT_INDICATOR A0
  #define PPM_IN 3
  #define SDO_pin 9
  #define SDI_pin 8
  #define SCLK_pin 7 
  #define IRQ_pin 2
  #define nSel_pin 4
  #define IRQ_interrupt 0
  #define RSSI_out_pin 5
  #define PPM_out 3
  #define GREEN_LED_pin 12
  #define RED_LED_pin 13

  #define PPM_Pin_Interrupt_Setup  PCMSK2 = 0b00001000;PCICR|=(1<<PCIE2);
  #define PPM_Signal_Interrupt PCINT2_vect
  #define PPM_Signal_Edge_Check (PIND & 0x08)==0x08
  
  #define  nIRQ_1 (PIND & 0x04)==0x04 
  #define  nIRQ_0 (PIND & 0x04)==0x00 

  #define  nSEL_on PORTD |= (1<<4) 
  #define  nSEL_off PORTD &= 0xEF 
  
  #define  SCK_on PORTD |= 0b10000000 
  #define  SCK_off PORTD &= 0b01111111 
  
  #define  SDI_on PORTB |= 0b00000001 
  #define  SDI_off PORTB &= 0b11111110 
  
  #define  SDO_1 (PINB & 0b00000010) == 0b00000010 
  #define  SDO_0 (PINB & 0b00000010) == 0b00000000 

  #define Red_LED_ON  digitalWrite(RED_LED_pin, HIGH);
  #define Red_LED_OFF  digitalWrite(RED_LED_pin, LOW);
  
  #define Green_LED_ON  digitalWrite(GREEN_LED_pin, HIGH)
  #define Green_LED_OFF  digitalWrite(GREEN_LED_pin, LOW)
  
  #define Servo_Ports_LOW digitalWrite(PPM_out,LOW);
  #define Serial_PPM_OUT_HIGH digitalWrite(PPM_out,HIGH);

#endif
#if (HW_TYPE == 1 || HW_TYPE == 2)
  #if (HW_ROLE == 0)
    #error Hardware cannot be used as TX
  #endif
  #define HAZ_BUZZER 0
  #define HAZ_BUTTON 0
  #define SDO_pin A0
  #define SDI_pin A1
  #define SCLK_pin A2
  #define IRQ_pin 2
  #define nSel_pin 4
  #define IRQ_interrupt 0
  #define RSSI_out_pin 3
  #define PPM_out 7
  #define GREEN_LED_pin 13
  #define RED_LED_pin A3
  
  #define  nIRQ_1 (PIND & 0x04)==0x04 //D2
  #define  nIRQ_0 (PIND & 0x04)==0x00 //D2
  
  #define  nSEL_on PORTD |= 0x10 //D4
  #define  nSEL_off PORTD &= 0xEF //D4
  
  #define  SCK_on PORTC |= 0x04 //C2
  #define  SCK_off PORTC &= 0xFB //C2
  
  #define  SDI_on PORTC |= 0x02 //C1
  #define  SDI_off PORTC &= 0xFD //C1
  
  #define  SDO_1 (PINC & 0x01) == 0x01 //C0
  #define  SDO_0 (PINC & 0x01) == 0x00 //C0
  
  #define Red_LED_ON  PORTC |= _BV(3);
  #define Red_LED_OFF  PORTC &= ~_BV(3);
  
  #define Green_LED_ON  PORTB |= _BV(5);
  #define Green_LED_OFF  PORTB &= ~_BV(5);
  
  #define Servo_Ports_LOW digitalWrite(PPM_out,LOW);
  #define Serial_PPM_OUT_HIGH digitalWrite(PPM_out,HIGH);
  
#endif
#if (HW_TYPE == 3)

  #if (HW_ROLE == 0)
    #error Hardware cannot be used as TX
  #endif
  #define HAZ_BUZZER 0
  #define HAZ_BUTTON 0
  #define SDO_pin 9
  #define SDI_pin 8
  #define SCLK_pin 7 
  #define IRQ_pin 2
  #define nSel_pin 4
  #define IRQ_interrupt 0
  #define RSSI_out_pin 3
  #define PPM_out 10
  #define GREEN_LED_pin 12
  #define RED_LED_pin 13
  
  #define  nIRQ_1 (PIND & 0x04)==0x04 
  #define  nIRQ_0 (PIND & 0x04)==0x00 

  #define  nSEL_on PORTD |= (1<<4) 
  #define  nSEL_off PORTD &= 0xEF 
  
  #define  SCK_on PORTD |= 0b10000000 
  #define  SCK_off PORTD &= 0b01111111 
  
  #define  SDI_on PORTB |= 0b00000001 
  #define  SDI_off PORTB &= 0b11111110 
  
  #define  SDO_1 (PINB & 0b00000010) == 0b00000010 
  #define  SDO_0 (PINB & 0b00000010) == 0b00000000 

  #define Red_LED_ON  digitalWrite(RED_LED_pin, HIGH);
  #define Red_LED_OFF  digitalWrite(RED_LED_pin, LOW);
  
  #define Green_LED_ON  digitalWrite(GREEN_LED_pin, HIGH)
  #define Green_LED_OFF  digitalWrite(GREEN_LED_pin, LOW)
  
  #define Servo_Ports_LOW digitalWrite(PPM_out,LOW);
  #define Serial_PPM_OUT_HIGH digitalWrite(PPM_out,HIGH);

#endif

  #define Servo1_OUT 3 //Servo1
  #define Servo3_OUT 6 //Servo3
