# Ultimate LRS
This fork was made to simplify the process of configuring and compiling ulrs by combining rx and tx project into a single project and add compatibility for more devices.

## How to configure

Start by defining hardware role, which can be either receiver or transmitter

### Transmitter
```
#define HW_ROLE 0
```
### Receiver 
```
#define HW_ROLE 1
```

Next, start by defining which hardware you have

## Hobbyking ORange OpenLRS TX 1W
```
#define HW_TYPE 0
```

## Hobbyking ORange OpenLRS RX 100mW v1
```
#define HW_TYPE 1
```

## Hobbyking ORange OpenLRS RX 100mW v2
```
#define HW_TYPE 2
```

## Hobbyking ORange OpenLRSng RX 1W
```
#define HW_TYPE 3
```

If you want to be able to change settings via serial commands, uncomment the following line
```
#define HARD_CODE_SETTINGS
```
If you want to have hardcoded configuration, remember to edit these lines to your liking
```
unsigned long CARRIER_FREQUENCY = 433575; // Carrier frequency in kHz
unsigned char HOPPING_STEP_SIZE = 5; // Stepping size in kHz
static unsigned char hop_list[4] = { 1, 5, 10}; // Hopping channels
static unsigned char RF_Header[4] = {'F', 'L', 'I', 'P'}; // Radio header
```
Now you just have to compile and upload sketch.
## Startup configuration

Open serial console, when you see the message “To enter AT mode, type +(pluss) four times”, press + four times, this will enable AT mode for configuration.
If you have enabled hardcode settings, this will not work.

### First time setup
It is important to fill in all settings before use, and write them to eeprom.

Start by setting you carrier frequency by using the command AT+FREQ=XXXXXX
```
AT+FREQ=433575
```
This example will set the carrier frequency to 433.575MHz

Next you set the stepping size by using the command AT+STEP=X..XXXXXX
```
AT+STEP=5
```
This will set your stepping size to 5kHz

Next you set the channels by using the command AT+CHAN=X,X,X
```
AT+CHAN=1,5,10
```
This will set the channels to 1,5 and 10. Channel frequency is CARRIER + STEP*CHAN

Last setting is to set you Radio Header by using the command AT+HEAD=XXXX
```
AT+HEAD=FLIP
```
Receiver and transmitter must have matching header to be able to communicate.

You can review your settings by using the following command
```
AT+SHAL
```


Last important step is to commit settings to eeprom by using the following command
```
AT+CMIT
```

To reboot device
```
AT+REBT
```

To discard changes and reload form memory
```
AT+REST
```

To exit AT mode
```
AT+EXIT
```

## Notice
This fork has not been tested in flight, only on the bench


