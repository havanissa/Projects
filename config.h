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



#define POWER 7
#define configtimeout 2000    // Wait for x amout of ms before continue normal operation



void RF22B_init_parameter(void);
void _spi_write(unsigned char address, unsigned char data);
unsigned char _spi_read(unsigned char address);
unsigned char _spi_read(unsigned char address);
void send_read_address(unsigned char i);
unsigned char read_8bit_data(void);
void to_tx_mode(void);
void  Hopping(void);

#define FIFO_SIZE 1000 // secret ingredient
unsigned char Input_Fifo[FIFO_SIZE];
int writepos = 0;
int readpos = 0;
int fifo_len = 0;
long rssi_timer = 0;

enum rf_rate_enum
{
  rate_57K,
  rate_125K,
  rate_125KOOK
};

rf_rate_enum rf_rate = rate_57K;


enum serial_ppm_type_enum
{
  CH8,
  MIXED
};

const int low_pulse_width = 300;
serial_ppm_type_enum ppm_type = MIXED;
#if (HW_ROLE == 1)
  #define SERIAL_BAUD_RATE 19200
  #define RF_PACK_SIZE 64
#else
  #define SERIAL_BAUD_RATE 57600
  #define RF_PACK_SIZE 40
#endif

#define Lost_Package_Alert 3
#define RC_CHANNEL_COUNT 18

unsigned char RF_Rx_Buffer[RF_PACK_SIZE];
unsigned char RF_Tx_Buffer[RF_PACK_SIZE];
unsigned char RS232_Tx_Buffer[RF_PACK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  
unsigned int Servo_Position[RC_CHANNEL_COUNT] = {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000}; 
unsigned char Telemetry_Buffer[8];

volatile unsigned int Servo_Buffer[RC_CHANNEL_COUNT];

volatile unsigned char channel_no = 0;
volatile unsigned int transmitted = 1;
volatile unsigned char channel_count = 0;

static unsigned char hopping_channel = 1;
unsigned long time, old_time = 0, old_rssi = 0;
unsigned long last_pack_time ;
unsigned long last_hopping_time;
unsigned char failsafe_mode = 0;

unsigned char Rx_Pack_Received = 0;
unsigned char Rx_RSSI = 110;
unsigned char Tx_RSSI = 110;

static unsigned char receiver_mode = 0;

volatile unsigned char RF_Mode = 0;
#define Available 0
#define Transmit 1
#define Transmitted 2
#define Receive 3
#define Received 4

volatile bool Preamble = false;

unsigned char loop_counter = 0;

unsigned char temp_char;
unsigned int temp_int;
volatile unsigned char current = 0;

static unsigned char Servo_Number = 0;
unsigned int total_ppm_time = 0;
















