// =====================================================
// SMS1340 SEGMENT MAPPER
// =====================================================
//
// Purpose:
// Identify the physical LCD segment controlled by
// each INDEX of the original SMS1340 z_addr[] table.
//
// The program tests INDEX 0, 1, 2, 3... automatically.
// One segment is illuminated at a time.
//
// IMPORTANT DISCOVERY:
// INDEX 21 = D1
//
// This test uses the original SMS1340 communication
// routines (f_wd1, f_byte, f_byte_rrc, etc.).
//
// Hardware:
// Arduino Nano ATmega328P
// LCD SCL = D6
// LCD SDA = D7
//
// Serial Monitor: 9600 baud
//
// NO Si5351
// NO encoder
// NO STEP
// NO BFO
//
// =====================================================
/*
 SMS1340 LCD SEGMENT MAPPER
 Arduino Nano ATmega328P
 SCL=D6, SDA=D7
 Serial=9600 baud

 Purpose: test INDEX 0..49 of the original SMS1340 z_addr[]
 table, one segment every 2 seconds.

 Confirmed map:
 0 LOCK
 1 LSB
 2 USB
 3 AM
 4 CW
 5 FM
 6 SDR
 7 GENERATOR
 8 1 IF
 9 RIT
 10 SET
 11 SAVE
 12 MEM
 13 MEGAHERTZ
 14 MODE
 15 BFO A
 16 BFO B
 17 SPLIT
 18 TX
 19 RX
 20 OFF
 21 D1 (digit 1)
 22 decimal point 1
 23 decimal point 2
 24-30 STEP indicators
 31 VFO ON
 32 VFO OFF
 33 CHG
 34 VOLTAGE
 35 TO BE IDENTIFIED
 36-49 BAR-GRAPH segments

 IMPORTANT: INDEX 21 = physical D1.
*/

#define LCD_SCL 6
#define LCD_SDA 7

unsigned char l_data[20];

unsigned char z_addr[] = {
26,7,28,7,30,7,32,7,34,7,36,7,38,7,
0,3,0,2,0,0,0,1,24,7,22,7,20,7,0,6,
12,6,12,5,12,4,12,1,12,3,12,2,12,7,
2,4,0,7,12,0,2,5,2,7,2,2,2,0,0,4,0,5,
2,6,2,3,2,1,16,7,18,7,14,7,
10,4,10,5,10,6,10,7,10,3,10,2,10,1,10,0,
8,4,8,5,8,6,8,7,8,3,8,2,8,1,8,0,
6,4,6,5,6,6,6,7,6,3,6,2,6,1,6,0,
4,4,4,5,4,6,4,7,4,3,4,2,4,0,4,1
};

void f_start(){pinMode(LCD_SCL,OUTPUT);pinMode(LCD_SDA,OUTPUT);digitalWrite(LCD_SDA,HIGH);digitalWrite(LCD_SCL,HIGH);digitalWrite(LCD_SDA,LOW);digitalWrite(LCD_SCL,LOW);}
void f_stop(){pinMode(LCD_SCL,OUTPUT);pinMode(LCD_SDA,OUTPUT);digitalWrite(LCD_SDA,LOW);digitalWrite(LCD_SCL,HIGH);digitalWrite(LCD_SDA,HIGH);digitalWrite(LCD_SCL,LOW);}
byte IICCACK(){byte b;pinMode(LCD_SCL,OUTPUT);pinMode(LCD_SDA,OUTPUT);digitalWrite(LCD_SCL,HIGH);digitalWrite(LCD_SDA,HIGH);pinMode(LCD_SDA,INPUT);b=digitalRead(LCD_SDA);digitalWrite(LCD_SCL,LOW);return b;}
void f_bit(byte b){pinMode(LCD_SCL,OUTPUT);pinMode(LCD_SDA,OUTPUT);digitalWrite(LCD_SDA,b);digitalWrite(LCD_SCL,HIGH);digitalWrite(LCD_SCL,LOW);digitalWrite(LCD_SDA,LOW);}
void f_byte(unsigned char b){for(byte i=0;i<8;i++){f_bit((b&0x80)?1:0);b<<=1;}}
void f_byte_rrc(unsigned char b){for(byte i=0;i<8;i++){f_bit((b&1)?1:0);b>>=1;}}
void f_wc(unsigned char c,unsigned char d){f_start();f_byte(0x70);if(IICCACK()==0)f_byte(c);if(IICCACK()==0)f_byte(d);f_stop();}
void f_wd1(unsigned char a,unsigned char d){l_data[a/2]=d;f_start();f_byte(0x70);if(IICCACK()==0)f_byte(0xe0);if(IICCACK()==0)f_byte(a);if(IICCACK()==0)f_byte_rrc(d);f_stop();}
void Clear(unsigned char h){for(byte i=0;i<40;i+=2)f_wd1(i,h);}
void lcdreset(){f_wc(0xe0,0x48);f_wc(0xe0,0x70);}

void test_index(int index){
  byte address=z_addr[index*2];
  byte bit=z_addr[index*2+1];
  byte mask=(1<<bit);
  Clear(0x00);
  f_wd1(address,mask);
  Serial.println();
  Serial.println("--------------------------------");
  Serial.print("INDEX   = ");Serial.println(index);
  Serial.print("ADDRESS = ");Serial.println(address);
  Serial.print("BIT     = ");Serial.println(bit);
  Serial.print("MASK    = 0x");if(mask<0x10)Serial.print("0");Serial.println(mask,HEX);
  Serial.println("--------------------------------");
}

void setup(){
  Serial.begin(9600);
  pinMode(LCD_SCL,OUTPUT);pinMode(LCD_SDA,OUTPUT);
  delay(100);lcdreset();Clear(0x00);
  Serial.println();Serial.println("================================");
  Serial.println(" SMS1340 SEGMENT MAPPER");
  Serial.println("================================");
  Serial.println("One segment every 2 seconds.");
  Serial.println("Serial Monitor: 9600 baud");
}

void loop(){
  static int index=0;
  test_index(index);
  delay(2000);
  index++;
  if(index>=50){index=0;Serial.println();Serial.println("========== REPEATING ==========");}
}
