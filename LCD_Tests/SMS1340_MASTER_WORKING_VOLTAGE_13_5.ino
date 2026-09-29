// =====================================================
// SMS1340 + ARDUINO NANO
// DISPLAY + ENCODER + STEP
// CORREGIDO SEGUN EL MAPA REAL DEL SMS1340
// =====================================================
//
// LCD:
//   D1 = 26
//   D2 = 28
//   D3 = 30
//   D4 = 32
//   D5 = 34
//   D6 = 36
//   D7 = 38
//
// STEP INDICATORS DEL MAPA:
//   INDEX 24 = D1
//   INDEX 25 = D2
//   INDEX 26 = D3
//   INDEX 27 = D4
//   INDEX 28 = D5
//   INDEX 29 = D6
//   INDEX 30 = D7
//
// STEP:
//   1 Hz       -> D7 -> INDEX 30
//   10 Hz      -> D6 -> INDEX 29
//   100 Hz     -> D5 -> INDEX 28
//   1 kHz      -> D4 -> INDEX 27
//   10 kHz     -> D3 -> INDEX 26
//   100 kHz    -> D2 -> INDEX 25
//   1 MHz      -> D1 -> INDEX 24
//
// NO Si5351
// NO BFO
// NO VFO
// NO MODE
// =====================================================

#include <Rotary.h>

// =====================================================
// PINS
// =====================================================

#define LCD_SCL 6
#define LCD_SDA 7

#define ROT_A 11
#define ROT_B 12

#define STEP_BUTTON 10

// =====================================================
// ROTARY
// =====================================================

Rotary r = Rotary(ROT_A, ROT_B);

// =====================================================
// FREQUENCY
// =====================================================

volatile unsigned long freq = 7050000UL;

unsigned long _freq = 7050000UL;

// =====================================================
// STEP
// =====================================================

unsigned long fstep = 1000UL;

byte Step = 3;

// =====================================================
// LCD DATA
// =====================================================

unsigned char l_data[20];

// =====================================================
// DIGITS
// =====================================================

byte D1;
byte D2;
byte D3;
byte D4;
byte D5;
byte D6;
byte D7;

// =====================================================
// DIGIT MASK
// =====================================================

unsigned char Mask1[] =
{
  0x5f,
  0x50,
  0x6b,
  0x79,
  0x74,
  0x3d,
  0x3f,
  0x58,
  0x7f,
  0x7d
};


// =====================================================
// VOLTAGE DIGIT MASK
// Taken from the original working SMS1340 voltage routine
// =====================================================

unsigned char Mask3[] =
{
  0xdf,
  0xd0,
  0xeb,
  0xf9,
  0xf4,
  0xbd,
  0xbf,
  0xd8,
  0xff,
  0xfd
};


// =====================================================
// BIT MASK
// =====================================================

unsigned char Mask2[] =
{
  0x01,
  0x02,
  0x04,
  0x08,
  0x10,
  0x20,
  0x40,
  0x80
};

// =====================================================
// ORIGINAL SMS1340 ADDRESS TABLE
// INDEX 0 ... INDEX 49
// =====================================================

unsigned char z_addr[] =
{
  26,7,
  28,7,
  30,7,
  32,7,
  34,7,
  36,7,
  38,7,

  0,3,
  0,2,
  0,0,
  0,1,

  24,7,
  22,7,
  20,7,

  0,6,

  12,6,
  12,5,
  12,4,
  12,1,
  12,3,
  12,2,
  12,7,

  2,4,
  0,7,

  12,0,

  // INDEX 24 = STEP D1
  2,5,

  // INDEX 25 = STEP D2
  2,7,

  // INDEX 26 = STEP D3
  2,2,

  // INDEX 27 = STEP D4
  2,0,

  // INDEX 28 = STEP D5
  0,4,

  // INDEX 29 = STEP D6
  0,5,

  // INDEX 30 = STEP D7
  2,6,

  // INDEX 31 = VFO ON
  2,3,

  // INDEX 32 = VFO OFF
  2,1,

  // INDEX 33 = CHG
  16,7,

  // INDEX 34 = VOLTAGE
  18,7,

  // INDEX 35
  14,7,

  // INDEX 36 ... 49 = BAR GRAPH
  10,4,
  10,5,
  10,6,
  10,7,
  10,3,
  10,2,
  10,1,
  10,0,

  8,4,
  8,5,
  8,6,
  8,7,
  8,3,
  8,2,
  8,1,
  8,0,

  6,4,
  6,5,
  6,6,
  6,7,
  6,3,
  6,2,
  6,1,
  6,0,

  4,4,
  4,5,
  4,6,
  4,7,
  4,3,
  4,2,
  4,0,
  4,1
};

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(9600);

  // LCD
  pinMode(LCD_SCL, OUTPUT);
  pinMode(LCD_SDA, OUTPUT);

  // STEP
  pinMode(STEP_BUTTON, INPUT_PULLUP);

  // ENCODER
  pinMode(ROT_A, INPUT_PULLUP);
  pinMode(ROT_B, INPUT_PULLUP);

  // Pin Change Interrupt
  PCICR |= (1 << PCIE0);

  // D11 = PB3 = PCINT3
  PCMSK0 |= (1 << PCINT3);

  // D12 = PB4 = PCINT4
  PCMSK0 |= (1 << PCINT4);

  sei();

  delay(100);

  // Reset LCD
  lcdreset();

  // Clear LCD
  Clear(0x00);

  // Initial frequency
  show_freq(freq);

  // Initial step
  show_step();

  // Permanent MHz indicator
  // INDEX 13 -> MHz
  set_indicator(13);

  // Permanent BFOA indicator
  // INDEX 15 -> BFOA
  set_indicator(15);

  // Permanent RX indicator
  // INDEX 19 -> RX / reception
  set_indicator(19);

  // Permanent LSB indicator
  // INDEX 1 -> LSB (Lower Side Band)
  set_indicator(1);

  // Permanent BFO ON indicator
  // INDEX 31 -> BFO ON
  set_indicator(31);

  // Permanent voltage indicator
  // INDEX 34 -> VOLTAGE
  // This indicator will remain fixed for the voltage-divider test.
  set_indicator(34);

  // -------------------------------------------------
  // VOLTAGE TEST
  // Fixed test value: 13.5 V
  // Uses the original SMS1340 voltage positions:
  // 14, 16, 18
  // -------------------------------------------------
  show_volt_13_5();

  Serial.println();
  Serial.println("==============================");
  Serial.println("SMS1340 TEST - CORRECTED");
  Serial.println("==============================");

  Serial.print("Frequency: ");
  Serial.println(freq);

  Serial.print("Step: ");
  Serial.println(fstep);
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  // -------------------------------------------------
  // STEP BUTTON
  // -------------------------------------------------

  if (digitalRead(STEP_BUTTON) == LOW)
  {
    delay(20);

    if (digitalRead(STEP_BUTTON) == LOW)
    {
      set_step();
    }
  }

  // -------------------------------------------------
  // FREQUENCY CHANGED
  // -------------------------------------------------

  if (freq != _freq)
  {
    show_freq(freq);

    // Volvemos a dibujar la flecha después
    // de actualizar los dígitos.
    show_step();

    Serial.print("Frequency = ");
    Serial.print(freq);

    Serial.print("   Step = ");
    Serial.println(fstep);

    _freq = freq;
  }
}

// =====================================================
// SET STEP
// =====================================================

void set_step()
{
  Step++;

  if (Step > 6)
  {
    Step = 0;
  }

  show_step();

  // Wait for button release
  while (digitalRead(STEP_BUTTON) == LOW)
  {
    delay(5);
  }
}

// =====================================================
// SHOW STEP
//
// MAPA REAL:
//
// Step 0 = 1 Hz       -> INDEX 30 -> D7
// Step 1 = 10 Hz      -> INDEX 29 -> D6
// Step 2 = 100 Hz     -> INDEX 28 -> D5
// Step 3 = 1 kHz      -> INDEX 27 -> D4
// Step 4 = 10 kHz     -> INDEX 26 -> D3
// Step 5 = 100 kHz    -> INDEX 25 -> D2
// Step 6 = 1 MHz      -> INDEX 24 -> D1
// =====================================================

void show_step()
{
  clear_step_indicators();

  if (Step == 0)
  {
    fstep = 1UL;

    // D7
    set_indicator(30);

    Serial.println("STEP = 1 Hz   -> D7");
  }

  else if (Step == 1)
  {
    fstep = 10UL;

    // D6
    set_indicator(29);

    Serial.println("STEP = 10 Hz  -> D6");
  }

  else if (Step == 2)
  {
    fstep = 100UL;

    // D5
    set_indicator(28);

    Serial.println("STEP = 100 Hz -> D5");
  }

  else if (Step == 3)
  {
    fstep = 1000UL;

    // D4
    set_indicator(27);

    Serial.println("STEP = 1 kHz  -> D4");
  }

  else if (Step == 4)
  {
    fstep = 10000UL;

    // D3
    set_indicator(26);

    Serial.println("STEP = 10 kHz -> D3");
  }

  else if (Step == 5)
  {
    fstep = 100000UL;

    // D2
    set_indicator(25);

    Serial.println("STEP = 100 kHz -> D2");
  }

  else if (Step == 6)
  {
    fstep = 1000000UL;

    // D1
    set_indicator(24);

    Serial.println("STEP = 1 MHz -> D1");
  }
}

// =====================================================
// CLEAR STEP INDICATORS
// =====================================================

void clear_step_indicators()
{
  clear_indicator(24);
  clear_indicator(25);
  clear_indicator(26);
  clear_indicator(27);
  clear_indicator(28);
  clear_indicator(29);
  clear_indicator(30);
}

// =====================================================
// SET INDICATOR
//
// IMPORTANT:
// index is the INDEX 0..49 of z_addr[]
// NOT the raw array position.
// =====================================================

void set_indicator(byte index)
{
  byte address;
  byte bitmask;

  address = z_addr[index * 2];

  bitmask = Mask2[z_addr[index * 2 + 1]];

  l_data[address / 2] |= bitmask;

  f_wd1(address, l_data[address / 2]);
}

// =====================================================
// CLEAR INDICATOR
// =====================================================

void clear_indicator(byte index)
{
  byte address;
  byte bitmask;

  address = z_addr[index * 2];

  bitmask = Mask2[z_addr[index * 2 + 1]];

  l_data[address / 2] &= ~bitmask;

  f_wd1(address, l_data[address / 2]);
}

// =====================================================
// SHOW FREQUENCY
//
// D1 = millions
// D2 = hundred-thousands
// D3 = ten-thousands
// D4 = thousands
// D5 = hundreds
// D6 = tens
// D7 = units
// =====================================================

// =====================================================
// SHOW VOLTAGE - FIXED TEST 13.5 V
// =====================================================
//
// Original SMS1340 voltage routine:
//
//   14 -> first digit
//   16 -> second digit
//   18 -> third digit
//
//   Mask1 -> first digit
//   Mask3 -> second and third digits
//
// 135 represents 13.5 V.
//
// =====================================================

void show_volt_13_5()
{
  byte V1;
  byte V2;
  byte V3;

  unsigned int Volt_to_Disp = 135;

  V1 = (Volt_to_Disp / 100);
  V2 = ((Volt_to_Disp / 10) % 10);
  V3 = ((Volt_to_Disp / 1) % 10);

  f_wd1(14, Mask1[V1]);
  f_wd1(16, Mask3[V2]);
  f_wd1(18, Mask3[V3]);
}


void show_freq(unsigned long data)
{
  D1 = (data / 1000000UL) % 10UL;
  D2 = (data / 100000UL)  % 10UL;
  D3 = (data / 10000UL)   % 10UL;
  D4 = (data / 1000UL)    % 10UL;
  D5 = (data / 100UL)     % 10UL;
  D6 = (data / 10UL)      % 10UL;
  D7 = data % 10UL;

  f_wd1(26, Mask1[D1]);
  f_wd1(28, Mask1[D2]);
  f_wd1(30, Mask1[D3]);
  f_wd1(32, Mask1[D4]);
  f_wd1(34, Mask1[D5]);
  f_wd1(36, Mask1[D6]);
  f_wd1(38, Mask1[D7]);

  // -------------------------------------------------
  // DECIMAL POINTS - SMS1340 FACTORY MAP
  //
  // P2 = LCDRAM 0, D7  -> address 0, bit 7
  // P1 = LCDRAM 2, D4  -> address 2, bit 4
  //
  // Display format: XX.XXX.XX
  // -------------------------------------------------

  l_data[0] |= Mask2[7];
  f_wd1(0, l_data[0]);

  l_data[1] |= Mask2[4];
  f_wd1(2, l_data[1]);
}

// =====================================================
// ROTARY ENCODER INTERRUPT
// =====================================================

ISR(PCINT0_vect)
{
  unsigned char result;

  result = r.process();

  if (result)
  {
    // -------------------------------------------------
    // CLOCKWISE
    // -------------------------------------------------

    if (result == DIR_CW)
    {
      freq += fstep;

      if (freq > 29999999UL)
      {
        freq = 29999999UL;
      }
    }

    // -------------------------------------------------
    // COUNTER CLOCKWISE
    // -------------------------------------------------

    else
    {
      if (freq > fstep)
      {
        freq -= fstep;
      }
      else
      {
        freq = 540000UL;
      }

      if (freq < 540000UL)
      {
        freq = 540000UL;
      }
    }
  }
}

// =====================================================
// LCD START
// =====================================================

void f_start()
{
  pinMode(LCD_SCL, OUTPUT);
  pinMode(LCD_SDA, OUTPUT);

  digitalWrite(LCD_SDA, HIGH);
  digitalWrite(LCD_SCL, HIGH);

  digitalWrite(LCD_SDA, LOW);
  digitalWrite(LCD_SCL, LOW);
}

// =====================================================
// LCD STOP
// =====================================================

void f_stop()
{
  pinMode(LCD_SCL, OUTPUT);
  pinMode(LCD_SDA, OUTPUT);

  digitalWrite(LCD_SDA, LOW);
  digitalWrite(LCD_SCL, HIGH);

  digitalWrite(LCD_SDA, HIGH);
  digitalWrite(LCD_SCL, LOW);
}

// =====================================================
// ACK
// =====================================================

byte IICCACK()
{
  byte l_bit;

  pinMode(LCD_SCL, OUTPUT);
  pinMode(LCD_SDA, OUTPUT);

  digitalWrite(LCD_SCL, HIGH);
  digitalWrite(LCD_SDA, HIGH);

  pinMode(LCD_SDA, INPUT);

  l_bit = digitalRead(LCD_SDA);

  digitalWrite(LCD_SCL, LOW);

  return l_bit;
}

// =====================================================
// SEND ONE BIT
// =====================================================

void f_bit(byte l_bit)
{
  pinMode(LCD_SCL, OUTPUT);
  pinMode(LCD_SDA, OUTPUT);

  digitalWrite(LCD_SDA, l_bit);

  digitalWrite(LCD_SCL, HIGH);
  digitalWrite(LCD_SCL, LOW);

  digitalWrite(LCD_SDA, LOW);
}

// =====================================================
// SEND BYTE
// =====================================================

void f_byte(unsigned char l_byte)
{
  unsigned char i;

  for (i = 0; i < 8; i++)
  {
    if (l_byte & 0x80)
    {
      f_bit(1);
    }
    else
    {
      f_bit(0);
    }

    l_byte <<= 1;
  }
}

// =====================================================
// SEND BYTE REVERSE
// =====================================================

void f_byte_rrc(unsigned char l_byte)
{
  unsigned char i;

  for (i = 0; i < 8; i++)
  {
    if (l_byte & 0x01)
    {
      f_bit(1);
    }
    else
    {
      f_bit(0);
    }

    l_byte >>= 1;
  }
}

// =====================================================
// WRITE COMMAND
// =====================================================

void f_wc(unsigned char l_ICCS,
          unsigned char l_ICDATA)
{
  f_start();

  f_byte(0x70);

  if (IICCACK() == 0)
  {
    f_byte(l_ICCS);
  }

  if (IICCACK() == 0)
  {
    f_byte(l_ICDATA);
  }

  f_stop();
}

// =====================================================
// WRITE LCD DATA
// =====================================================

void f_wd1(unsigned char l_ICADDR,
           unsigned char l_ICDATA)
{
  l_data[l_ICADDR / 2] = l_ICDATA;

  f_start();

  f_byte(0x70);

  if (IICCACK() == 0)
  {
    f_byte(0xe0);
  }

  if (IICCACK() == 0)
  {
    f_byte(l_ICADDR);
  }

  if (IICCACK() == 0)
  {
    f_byte_rrc(l_ICDATA);
  }

  f_stop();
}

// =====================================================
// CLEAR DISPLAY
// =====================================================

void Clear(unsigned char h)
{
  unsigned char i;

  for (i = 0; i < 40; i += 2)
  {
    f_wd1(i, h);
  }
}

// =====================================================
// LCD RESET
// =====================================================

void lcdreset()
{
  f_wc(0xe0, 0x48);

  f_wc(0xe0, 0x70);
}