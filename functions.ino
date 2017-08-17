/*****************************************************************************************************
***         Ultimate LRS - Long Range RC & Data Link using 2 x 1W 433 MHz OrangeRX modules         ***
***                                                                                                ***
*** Copyright 2014-2015 Benoit Joassart    benoit@joassart.com                                     ***
***                                        rcgroups.com : flipflap                                 ***
***                     Project website : http://www.itluxembourg.lu/site/                         ***
***                                                                                                ***
*** Contains code from OpenLRS ( http://code.google.com/p/openlrs/ )                               ***
***   Copyright 2010-2012 Melih Karakelle ( http://www.flytron.com ) (forum nick name: Flytron)    ***
***                     Jan-Dirk Schuitemaker ( http://www.schuitemaker.org/ ) (CrashingDutchman)  ***
***                     Etienne Saint-Paul ( http://www.gameseed.fr ) (forum nick name: Etienne)   ***
***                     thUndead (forum nick name: thUndead)                                       ***
***                                                                                                ***
******************************************************************************************************

          This file is part of Ultimate LRS.

          Ultimate LRS is free software: you can redistribute it and/or modify
          it under the terms of the GNU General Public License as published by
          the Free Software Foundation, either version 3 of the License, or
          (at your option) any later version.

          Ultimate LRS is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
          GNU General Public License for more details.

          You should have received a copy of the GNU General Public License
          along with Ultimate LRS.  If not, see <http://www.gnu.org/licenses/>.

          This project must be compiled with Arduino 1.5.8.
*/

void INIT_SERVO_DRIVER(void)
{
  TCCR1B   =   0x00;
  TCNT1H   =   0x00;
  TCNT1L   =   0x00;
  ICR1   =   40000;
  TCCR1A   =   0x02;
  TCCR1B   =   0x1A;
  TIMSK1 = _BV(TOIE1);
}

void RFM22B_Int()
{
  if (RF_Mode == Transmit)
  {
    RF_Mode = Transmitted;
    Preamble = false;
  }

  if (RF_Mode == Receive)
  {
    RF_Mode = Received;
    Preamble = false;
  }
}
void Red_LED_Blink(unsigned short blink_count)
{
  unsigned char i;

  for (i = 0; i < blink_count; i++)
  {
    delay(100);
    Red_LED_ON;
    delay(100);
    Red_LED_OFF;
  }
}
void Green_LED_Blink(unsigned short blink_count)
{
  unsigned char i;

  for (i = 0; i < blink_count; i++)
  {
    delay(100);
    Green_LED_ON;
    delay(100);
    Green_LED_OFF;
  }
}
unsigned char check_modes(void)
{
  if (debug)
      return 1;
  if(HW_TYPE == 3) // ng board
    return 0; // no servos
  if(HW_TYPE == 2)
  {
    pinMode(Servo3_OUT, INPUT);
    digitalWrite(Servo3_OUT, HIGH);
    digitalWrite(Servo1_OUT, HIGH);
    delayMicroseconds(1);

    if (digitalRead(Servo3_OUT) == HIGH)
    {
      digitalWrite(Servo1_OUT, LOW);
      delayMicroseconds(1);

      if (digitalRead(Servo3_OUT) == LOW)
      {
        pinMode(Servo3_OUT, OUTPUT);
        return  1;
      }
    }

    pinMode(Servo3_OUT, OUTPUT);
    return  0;
  }
  // TX : only serial PPM
  return  1;
}
void Hopping(void)
{
  hopping_channel++;
  if (hopping_channel > 2)
    hopping_channel = 0;
  _spi_write(0x79, hop_list[hopping_channel]);
}
void Power_Set(unsigned short level)
{
  if (level < 8)
    _spi_write(0x6d, level | 0b00001000);
}

#if (HAZ_BUTTON)
void Check_Button(void)
{
  unsigned long loop_time;
  byte btn_mode = 0;

  if (digitalRead(BTN) == 0)
  {
    delay(1000);
    #if (HAZ_BUZZER)
    digitalWrite(BUZZER, LOW);
    #endif
    time = millis();
    loop_time = time;

    while (digitalRead(BTN) == 0)
    {
      loop_time = millis();

      if (loop_time > time + 1000)
      {
        time = millis();
        btn_mode++;
        #if (HAZ_BUZZER)
        digitalWrite(BUZZER, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        #endif
      }
    }

    if (btn_mode == 0)
      Power_Set(6);

    if (btn_mode == 3)
      Power_Set(0);
    #if (HAZ_BUZZER)
    delay(500);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    #endif
  }
}
#endif
void Telemetry_Bridge_Write(void)
{
  blue_led(true);
  RF_Tx_Buffer[0] = 'B';
  fill_fifo();
  byte total_rx_byte = fifo_available();

  if (total_rx_byte > RF_PACK_SIZE - 2)
    total_rx_byte = RF_PACK_SIZE - 2;

  if (total_rx_byte > 0)
  {
    RF_Tx_Buffer[1] = total_rx_byte;
    for (byte i = 0; i < total_rx_byte; i++)
      RF_Tx_Buffer[2 + i] = fifo_read();
  }

  to_tx_mode();
  rx_reset();
  RF_Tx_Buffer[1] = 0;
  RF_Tx_Buffer[2] = 0;
  RF_Tx_Buffer[3] = 0;
  RF_Tx_Buffer[4] = 0;
  RF_Tx_Buffer[5] = 0;
  RF_Tx_Buffer[6] = 0;
  RF_Tx_Buffer[7] = 0;
  RF_Tx_Buffer[8] = 0;
  RF_Tx_Buffer[9] = 0;
  RF_Tx_Buffer[10] = 0;
  RF_Tx_Buffer[11] = 0;
  RF_Tx_Buffer[12] = 0;
  RF_Tx_Buffer[13] = 0;
  RF_Tx_Buffer[14] = 0;
  RF_Tx_Buffer[15] = 0;
  RF_Tx_Buffer[16] = 0;
  blue_led(false);
}
unsigned char fifo_read()
{
  if (fifo_len > 0)
  {
    readpos++;
    readpos %= FIFO_SIZE;
    fifo_len--;
    return(Input_Fifo[readpos]);
  }

  else
  {
    return 0;
  }
}

void fifo_write(unsigned char b)
{
  if (fifo_len < FIFO_SIZE)
  {
    writepos++;
    writepos %= FIFO_SIZE;
    fifo_len++;
    Input_Fifo[writepos] = b;
  }
}

int fifo_available()
{
  return fifo_len;
}

void fill_fifo()
{
  int a = Serial.available();
  while (a > 0)
  {
    int b = Serial.read();
    if (b >= 0)
      fifo_write((unsigned char) b);
    else
      break;
    a--;
  }
}

void red_led(bool status)
{
  if (status)
  {
    Red_LED_ON;
  }else{
    Red_LED_OFF;
  }
}

void blue_led(bool status)
{
  if (status)
  {
    Green_LED_ON;
  }else{
    Green_LED_OFF;
  }
}
void SetServoPos(unsigned char channel, int value)
{
  unsigned char ch = channel * 2;
  Servo_Buffer[ch] = value / 256;
  Servo_Buffer[ch + 1] = value % 256;
}


































































