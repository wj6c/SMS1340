# SMS1340

SUNMAN SMS1340 LCD mapping, Arduino Nano tests, and firmware development.

## Project

This repository documents the development and testing of the SUNMAN
SMS1340 LCD using an Arduino Nano ATmega328P.

The project started by mapping the physical LCD segments controlled
by the original `z_addr[]` address table. The goal is to create a
reliable reference for future SMS1340 firmware development.

The project is being developed progressively, with each verified
stage saved as a separate reference sketch.

## Hardware

- Arduino Nano ATmega328P
- SUNMAN SMS1340 LCD
- LCD SCL: Arduino D6
- LCD SDA: Arduino D7
- Rotary Encoder A: Arduino D11
- Rotary Encoder B: Arduino D12
- STEP Button: Arduino D10

## Segment Mapping

The SMS1340 address table contains 50 indexed elements, from
INDEX 0 to INDEX 49.

### Confirmed Map

| INDEX | Function |
|------:|----------|
| 0 | LOCK |
| 1 | LOWER SIDEBAND (LSB) |
| 2 | UPPER SIDEBAND (USB) |
| 3 | AM |
| 4 | CW |
| 5 | FM |
| 6 | SDR |
| 7 | GENERATOR |
| 8 | 1 IF |
| 9 | RIT |
| 10 | SET |
| 11 | SAVE |
| 12 | MEM |
| 13 | MEGAHERTZ (MHz) |
| 14 | MODE |
| 15 | BFO A |
| 16 | BFO B |
| 17 | SPLIT |
| 18 | TX |
| 19 | RX |
| 20 | OFF |
| 21 | DIGIT 1 |
| 22 | DECIMAL POINT 1 |
| 23 | DECIMAL POINT 2 |
| 24 | STEP 1 |
| 25 | STEP 2 |
| 26 | STEP 3 |
| 27 | STEP 4 |
| 28 | STEP 5 |
| 29 | STEP 6 |
| 30 | STEP 7 |
| 31 | VFO ON |
| 32 | VFO OFF |
| 33 | CHG |
| 34 | VOLTAGE |
| 35 | TO BE IDENTIFIED |
| 36 | BAR 1 |
| 37 | BAR 2 |
| 38 | BAR 3 |
| 39 | BAR 4 |
| 40 | BAR 5 |
| 41 | BAR 6 |
| 42 | BAR 7 |
| 43 | BAR 8 |
| 44 | BAR 9 |
| 45 | BAR 10 |
| 46 | BAR 11 |
| 47 | BAR 12 |
| 48 | BAR 13 |
| 49 | BAR 14 |

## Important Discovery

**INDEX 21 corresponds to the physical first frequency digit (D1).**

This was confirmed using the `SMS1340_Segment_Mapper.ino` test program.

The physical frequency digit addresses are:

| Digit | SMS1340 Address |
|------:|---------------:|
| D1 | 26 |
| D2 | 28 |
| D3 | 30 |
| D4 | 32 |
| D5 | 34 |
| D6 | 36 |
| D7 | 38 |

## Decimal Points

The frequency display uses two decimal points to produce the format:

**XX.XXX.XX**

The verified LCD locations are:

- First decimal point: LCD address `0`, bit `7`
- Second decimal point: LCD address `2`, bit `4`

These locations are used by the working frequency display firmware.

## Tests

The `LCD_Tests` directory contains experimental and verified
sketches used to identify and test the SMS1340 LCD.

### SMS1340 Segment Mapper

`LCD_Tests/SMS1340_Segment_Mapper.ino`

This program automatically tests INDEX 0 through INDEX 49,
illuminating one LCD segment at a time.

Serial Monitor:

**9600 baud**

The test uses the original SMS1340 LCD communication routines.

This sketch was used to create the SMS1340 segment map documented
in this repository.

## Working Firmware

### SMS1340 Encoder + STEP

`LCD_Tests/SMS1340_Encoder_STEP_Working.ino`

This is the first verified working firmware version combining:

- SMS1340 7-digit frequency display
- Arduino Nano ATmega328P
- Rotary encoder
- Frequency adjustment
- 7-position STEP selection
- STEP indicator under the selected frequency digit

### Frequency STEP Positions

| STEP | Frequency Increment | Display Position |
|-----:|--------------------:|------------------|
| 0 | 1 Hz | D7 |
| 1 | 10 Hz | D6 |
| 2 | 100 Hz | D5 |
| 3 | 1 kHz | D4 |
| 4 | 10 kHz | D3 |
| 5 | 100 kHz | D2 |
| 6 | 1 MHz | D1 |

The STEP indicator positions were identified from the SMS1340
segment map:

| STEP Position | SMS1340 INDEX |
|--------------|--------------:|
| D1 | 24 |
| D2 | 25 |
| D3 | 26 |
| D4 | 27 |
| D5 | 28 |
| D6 | 29 |
| D7 | 30 |

The firmware uses these indexes to place the STEP indicator under
the correct frequency digit.

## Voltage Display

A separate test was performed using the original SMS1340 voltage
display routine.

The voltage numeric display does **not** use the VOLTAGE indicator
address itself. The verified numeric voltage display uses the
following LCD addresses:

| Voltage Digit | LCD Address |
|--------------|------------:|
| First digit | 14 |
| Second digit | 16 |
| Third digit | 18 |

The original voltage routine uses:

- `Mask1[]` for the first voltage digit
- `Mask3[]` for the second and third voltage digits

A fixed test value of:

**13.5 V**

was successfully displayed on the physical SMS1340 LCD.

The test value is represented internally as:

```text
135
