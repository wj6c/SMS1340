// =====================================================
// SMS1340 COMPLETE SEGMENT MAPPER - FAST TEST
// =====================================================
// SUNMAN SMS1340 + ARDUINO NANO
//
// COMPLETE TEST:
//   PASS 1: INDEX 0 -> INDEX 68
//   PASS 2: Z49 -> Z66
//
// IMPORTANT:
// The scan speed is controlled by ONE constant:
//
//     SCAN_DELAY_MS
//
// CHANGE ONLY THIS VALUE to make the scan faster/slower.
//
// Examples:
//     1000 = 1 second
//      500 = 0.5 second
//      250 = 0.25 second
//      100 = 0.10 second
//       50 = 0.05 second
//
// Current test setting:
//     250 ms
//
// =====================================================
// VERIFIED RELATIONSHIP
//
// INDEX 51 = Z49
// INDEX 52 = Z50
// INDEX 53 = Z51
// INDEX 54 = Z52
// ...
// INDEX 66 = Z64
// INDEX 67 = Z65 = IF:-
// INDEX 68 = Z66 = IF:+
//
// USER-VERIFIED DISPLAY MAP:
//
// INDEX 34 = voltage number
// INDEX 35 = V
// INDEX 36 = ADIBA
// INDEX 37 = beginning of bar graph
//
// Z49-Z64 = 16 bar-graph segments
// Z65     = IF:-
// Z66     = IF:+
//
// INDEX and Z are DIFFERENT numbering systems.
// =====================================================

#define LCD_SCL 6
#define LCD_SDA 7

// =====================================================
// SCAN SPEED
//
// >>> CHANGE THIS NUMBER TO CONTROL SPEED <<<
//
// 250 = fast test
// 500 = medium
// 1000 = slow
// =====================================================

const unsigned int SCAN_DELAY_MS = 250;

// =====================================================
// LCD DATA
// =====================================================

unsigned char l_data[20];

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
// COMPLETE LOGICAL INDEX MAP
//
// INDEX 0 ... INDEX 68
// =====================================================

const byte indexMap[] =
{
  26,7,   // INDEX 0
  28,7,   // INDEX 1
  30,7,   // INDEX 2
  32,7,   // INDEX 3
  34,7,   // INDEX 4
  36,7,   // INDEX 5
  38,7,   // INDEX 6

  0,3,    // INDEX 7
  0,2,    // INDEX 8
  0,0,    // INDEX 9
  0,1,    // INDEX 10

  24,7,   // INDEX 11
  22,7,   // INDEX 12
  20,7,   // INDEX 13
  0,6,    // INDEX 14

  12,6,   // INDEX 15
  12,5,   // INDEX 16
  12,4,   // INDEX 17
  12,1,   // INDEX 18
  12,3,   // INDEX 19
  12,2,   // INDEX 20
  12,7,   // INDEX 21

  2,4,    // INDEX 22
  0,7,    // INDEX 23
  12,0,   // INDEX 24

  2,5,    // INDEX 25
  2,7,    // INDEX 26
  2,2,    // INDEX 27
  2,0,    // INDEX 28
  0,4,    // INDEX 29
  0,5,    // INDEX 30
  2,6,    // INDEX 31

  2,3,    // INDEX 32
  2,1,    // INDEX 33

  // USER VERIFIED
  16,7,   // INDEX 34 = voltage number
  18,7,   // INDEX 35 = V
  14,7,   // INDEX 36 = ADIBA

  // BAR GRAPH START
  10,4,   // INDEX 37
  10,5,   // INDEX 38
  10,6,   // INDEX 39
  10,7,   // INDEX 40
  10,3,   // INDEX 41
  10,2,   // INDEX 42
  10,1,   // INDEX 43
  10,0,   // INDEX 44

  8,4,    // INDEX 45
  8,5,    // INDEX 46
  8,6,    // INDEX 47
  8,7,    // INDEX 48
  8,3,    // INDEX 49
  8,2,    // INDEX 50
  8,1,    // INDEX 51 = Z49
  8,0,    // INDEX 52 = Z50

  6,4,    // INDEX 53 = Z51
  6,5,    // INDEX 54 = Z52
  6,6,    // INDEX 55 = Z53
  6,7,    // INDEX 56 = Z54
  6,3,    // INDEX 57 = Z55
  6,2,    // INDEX 58 = Z56
  6,1,    // INDEX 59 = Z57
  6,0,    // INDEX 60 = Z58

  4,4,    // INDEX 61 = Z59
  4,5,    // INDEX 62 = Z60
  4,6,    // INDEX 63 = Z61
  4,7,    // INDEX 64 = Z62
  4,3,    // INDEX 65 = Z63
  4,2,    // INDEX 66 = Z64
  4,0,    // INDEX 67 = Z65 = IF:-
  4,1     // INDEX 68 = Z66 = IF:+
};

const byte INDEX_COUNT = 69;

// =====================================================
// MANUFACTURER Z49-Z66 MAP
// =====================================================

const byte zMap[] =
{
  8,1,    // Z49
  8,0,    // Z50
  6,4,    // Z51
  6,5,    // Z52
  6,6,    // Z53
  6,7,    // Z54
  6,3,    // Z55
  6,2,    // Z56
  6,1,    // Z57
  6,0,    // Z58
  4,4,    // Z59
  4,5,    // Z60
  4,6,    // Z61
  4,7,    // Z62
  4,3,    // Z63
  4,2,    // Z64
  4,0,    // Z65 = IF:-
  4,1     // Z66 = IF:+
};

const byte Z_FIRST = 49;
const byte Z_LAST  = 66;

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
// SEND BIT
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
      f_bit(1);
    else
      f_bit(0);

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
      f_bit(1);
    else
      f_bit(0);

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
    f_byte(l_ICCS);

  if (IICCACK() == 0)
    f_byte(l_ICDATA);

  f_stop();
}

// =====================================================
// WRITE DISPLAY DATA
// =====================================================

void f_wd1(unsigned char l_ICADDR,
           unsigned char l_ICDATA)
{
  l_data[l_ICADDR / 2] = l_ICDATA;

  f_start();

  f_byte(0x70);

  if (IICCACK() == 0)
    f_byte(0xe0);

  if (IICCACK() == 0)
    f_byte(l_ICADDR);

  if (IICCACK() == 0)
    f_byte_rrc(l_ICDATA);

  f_stop();
}

// =====================================================
// CLEAR DISPLAY
// =====================================================

void Clear(unsigned char h)
{
  unsigned char i;

  for (i = 0; i < 40; i += 2)
    f_wd1(i, h);
}

// =====================================================
// LCD RESET
// =====================================================

void lcdreset()
{
  f_wc(0xe0, 0x48);
  f_wc(0xe0, 0x70);
}

// =====================================================
// TEST ONE LOCATION
// =====================================================

void testLocation(byte address, byte bitNumber)
{
  byte mask = Mask2[bitNumber];

  l_data[address / 2] |= mask;

  f_wd1(address, l_data[address / 2]);

  Serial.print("LCDRAM ");
  Serial.print(address);
  Serial.print(" D");
  Serial.print(bitNumber);
  Serial.print(" MASK 0x");

  if (mask < 0x10)
    Serial.print("0");

  Serial.println(mask, HEX);
}

// =====================================================
// TEST ONE INDEX
// =====================================================

void testIndex(byte index)
{
  byte p = index * 2;

  byte address = indexMap[p];
  byte bitNumber = indexMap[p + 1];

  Clear(0x00);

  Serial.print("INDEX ");
  Serial.print(index);
  Serial.print(" -> ");

  testLocation(address, bitNumber);

  delay(SCAN_DELAY_MS);
}

// =====================================================
// TEST ONE Z
// =====================================================

void testZ(byte z)
{
  byte p = (z - Z_FIRST) * 2;

  byte address = zMap[p];
  byte bitNumber = zMap[p + 1];

  Clear(0x00);

  Serial.print("Z");
  Serial.print(z);
  Serial.print(" -> ");

  testLocation(address, bitNumber);

  delay(SCAN_DELAY_MS);
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(9600);

  pinMode(LCD_SCL, OUTPUT);
  pinMode(LCD_SDA, OUTPUT);

  delay(100);

  lcdreset();
  Clear(0x00);

  Serial.println();
  Serial.println("================================================");
  Serial.println("     SMS1340 COMPLETE SEGMENT MAPPER - FAST");
  Serial.println("================================================");

  Serial.print("SCAN_DELAY_MS = ");
  Serial.print(SCAN_DELAY_MS);
  Serial.println(" ms");

  Serial.println();
  Serial.println("PASS 1: INDEX 0 -> INDEX 68");
  Serial.println("PASS 2: Z49 -> Z66");

  Serial.println();
  Serial.println("VERIFIED:");
  Serial.println("INDEX 34 = voltage number");
  Serial.println("INDEX 35 = V");
  Serial.println("INDEX 36 = ADIBA");
  Serial.println("INDEX 37 = bar graph start");

  Serial.println();
  Serial.println("Z49-Z64 = 16 bar segments");
  Serial.println("Z65 = IF:-");
  Serial.println("Z66 = IF:+");

  Serial.println();
  Serial.println("INDEX 51 = Z49");
  Serial.println("INDEX 52 = Z50");
  Serial.println("INDEX 53 = Z51");
  Serial.println("...");
  Serial.println("INDEX 67 = Z65 = IF:-");
  Serial.println("INDEX 68 = Z66 = IF:+");

  Serial.println("================================================");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  // -------------------------------------------------
  // PASS 1
  // -------------------------------------------------

  Serial.println();
  Serial.println("******** INDEX MAP: 0 -> 68 ********");

  for (byte index = 0; index < INDEX_COUNT; index++)
  {
    testIndex(index);
  }

  // -------------------------------------------------
  // SHORT PAUSE
  // -------------------------------------------------

  Clear(0x00);

  Serial.println();
  Serial.println("******** Z MAP: Z49 -> Z66 ********");

  delay(SCAN_DELAY_MS);

  // -------------------------------------------------
  // PASS 2
  // -------------------------------------------------

  for (byte z = Z_FIRST; z <= Z_LAST; z++)
  {
    testZ(z);
  }

  // -------------------------------------------------
  // RESTART
  // -------------------------------------------------

  Clear(0x00);

  Serial.println();
  Serial.println("******** COMPLETE MAP RESTART ********");

  delay(SCAN_DELAY_MS);
}

