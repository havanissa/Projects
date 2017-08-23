void waitforcomputer()
{
  unsigned long wait_timeout;
  byte pings = 0;
  bool timeout = true;
  wait_timeout = millis() + configtimeout;
  Serial.println(F("To enter AT mode, type +(pluss) four times"));
  while(timeout)
  {
    if(millis() > wait_timeout)
    {
      Serial.println(F("AT mode timed out!"));
      break;
    }
    if(Serial.available())
      if(Serial.read()=='+')
        pings++;
      else
        pings = 0;
    if(pings > 3)
    {
      Serial.println(F("AT mode enabled!"));
      at_main();
    }
  }
}

void at_main()
{
  bool run_loop;
  char command[30];
  int cmdpos;
  char c;
  run_loop = true;
  cmdpos = 0;
  // Start AT Loop
  while(run_loop)
  {
    if(Serial.available())
      while(Serial.available())
      {
        c = Serial.read();
        if(c==10)
        {
          
        }else if(c==13)
        {
          // Do something smart
          Serial.println();
          run_loop = at_process(command,cmdpos);
          cmdpos = 0;
        }else{
          command[cmdpos] = c;
          Serial.print(c); // write response back
          cmdpos += 1;
        } 
      }
  }
}

bool at_process(char command[], byte cmdlen)
{
  int i;
  char c;
  bool okrep = false;
  long freq;
  char buffer[6] = {0,0,0,0,0,0};
  if(command[0] == 65 && command[1] == 84 && cmdlen == 2)
  {
    at_ok();
    return true;
  }
  if(command[0] != 65 || command[1] != 84 || command[2] != 43)
  {
    at_error();
    return true;
  }
  if(command[3] == 'E' && command[4] == 'X' && command[5] == 'I' && command[6] == 'T' && cmdlen == 7)
  {
    at_ok();
    return false; // Continue bootup
  }
  // Show every settings
  if(command[3] == 'S' && command[4] == 'H' && command[5] == 'A' && command[6] == 'L' && cmdlen == 7)
  {
    Serial.print(F("Carrier frequency: "));
    Serial.print(CARRIER_FREQUENCY);
    Serial.println(F("kHz"));
    Serial.print(F("Stepping: "));
    Serial.print(HOPPING_STEP_SIZE*10);
    Serial.println(F("kHz"));
    Serial.print(F("Radio header: "));
    for(i=0;i<4;i++)
    {
      c = RF_Header[i];
      Serial.print(c);
    }
    Serial.println();
    Serial.print(F("Radio channels: "));
    for(i=0;i<3;i++)
    {
      Serial.print(hop_list[i]);
      if(i!=2)
        Serial.print(',');
    }
    Serial.println();
    okrep = true;
  }
  // Show carrier frequency
  if(command[3] == 'F' && command[4] == 'R' && command[5] == 'E' && command[6] == 'Q' && cmdlen == 7)
  {
    Serial.print(F("Carrier frequency: "));
    Serial.print(CARRIER_FREQUENCY);
    Serial.println(F("kHz"));
    okrep = true;
  }
  // Set carrier frequency
  if(command[3] == 'F' && command[4] == 'R' && command[5] == 'E' && command[6] == 'Q' && command[7] == '=' && cmdlen == 14)
  {
    for(i=0;i<6;i++) // Clean buffer
      buffer[i] = '0';
    for(i=8;i<14;i++)
      buffer[i-8] = command[i];
    freq = atol(buffer);
    if(freq > 413000 && freq < 453000)
    {
      CARRIER_FREQUENCY = freq;
      at_ok();
      okrep = true;
    }
  }
  // Show stepping size
  if(command[3] == 'S' && command[4] == 'T' && command[5] == 'E' && command[6] == 'P' && cmdlen == 7)
  {
    Serial.print(F("Stepping: "));
    Serial.print(HOPPING_STEP_SIZE*10);
    Serial.println(F("kHz"));
    okrep = true;
  }
  // Set stepping size
  if(command[3] == 'S' && command[4] == 'T' && command[5] == 'E' && command[6] == 'P' && command[7] == '=' && cmdlen > 7)
  {
    for(i=0;i<6;i++) // Clean buffer
      buffer[i] = '0';
    for(i=8;i<cmdlen;i++)
      buffer[i - 8 + 6 -(cmdlen-8)] = command[i];
    /* debugging
    for(i=0;i<6;i++)
    {
      c = buffer[i];
      Serial.print(c);
    }
    */
    freq = atol(buffer);
    if(freq > 9 && freq < 2560)
    {
      HOPPING_STEP_SIZE = floor(freq/10);
      at_ok();
      okrep = true;
    }
  }
  // Show header
  if(command[3] == 'H' && command[4] == 'E' && command[5] == 'A' && command[6] == 'D' && cmdlen == 7)
  {
    Serial.print(F("Radio header: "));
    for(i=0;i<4;i++)
    {
      c = RF_Header[i];
      Serial.print(c);
    }
    Serial.println();
    okrep = true;
  }
  // Set Header
  if(command[3] == 'H' && command[4] == 'E' && command[5] == 'A' && command[6] == 'D' && command[7] == '=' && cmdlen == 12)
  {
      RF_Header[0] = command[8];
      RF_Header[1] = command[9];
      RF_Header[2] = command[10];
      RF_Header[3] = command[11];
      okrep = true;
      at_ok();
  }
  // Show channels
  if(command[3] == 'C' && command[4] == 'H' && command[5] == 'A' && command[6] == 'N' && cmdlen == 7)
  {
    Serial.print(F("Radio channels: "));
    for(i=0;i<3;i++)
    {
      Serial.print(hop_list[i]);
      if(i!=2)
        Serial.print(',');
    }
    Serial.println();
    okrep = true;
  }
  // Set channels
  if(command[3] == 'C' && command[4] == 'H' && command[5] == 'A' && command[6] == 'N' && command[7] == '=' && cmdlen == 19)
  {
    for(i=0;i<6;i++) // Clean buffer
      buffer[i] = '0';
    // first channel
    for(i=0;i<4;i++)
      buffer[3+i] = command[i+8];
    hop_list[0] = atoi(buffer);
    for(i=0;i<6;i++) // Clean buffer
      buffer[i] = '0';
    // second channel
    for(i=0;i<4;i++)
      buffer[3+i] = command[i+13];
    hop_list[1] = atoi(buffer);
    for(i=0;i<6;i++) // Clean buffer
      buffer[i] = '0';
    // third channel
    for(i=0;i<4;i++)
      buffer[3+i] = command[i+17];
    hop_list[2] = atoi(buffer);
    at_ok();
    okrep = true;
    /* Something overcomplicated
    int tmpchan[3] = {-1,-1,-1};
    byte chancnt = 0;
    byte strcnt = 0;
    for(i=0;i<6;i++) // Clean buffer
      buffer[i] = '0';
    for(i=8;i<cmdlen;i++)
    {
      buffer[strcnt] = command[i];
      strcnt++;
      if(command[i] == ',' || i == cmdlen-1)
      {
        
        chancnt++;
      }
    }
    */
  }

  // Commit to memory
  if(command[3] == 'C' && command[4] == 'M' && command[5] == 'I' && command[6] == 'T' && cmdlen == 7)
  {
    write_eeprom();
    okrep = true;
    at_ok();
  }
  // Roload from memory
  if(command[3] == 'R' && command[4] == 'E' && command[5] == 'S' && command[6] == 'T' && cmdlen == 7)
  {
    read_eeprom();
    okrep = true;
    at_ok();
  }
  // Reboot controller
  if(command[3] == 'R' && command[4] == 'E' && command[5] == 'B' && command[6] == 'T' && cmdlen == 7)
  {
    asm volatile ("jmp 0");
  }
  // Memory check
  if(command[3] == 'M' && command[4] == 'C' && command[5] == 'H' && command[6] == 'K' && cmdlen == 7)
  {
    mem_dump();
    okrep = true;
  }
  if(!okrep)
    at_error();
  return true;
}

void at_error()
{
  Serial.println(F("AT+ERROR"));
}
void at_ok()
{
  Serial.println(F("AT+OK"));
}








