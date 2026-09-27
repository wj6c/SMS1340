# SMS1340

SUNMAN SMS1340 LCD mapping, Arduino Nano tests, and firmware development.

## Project

This repository documents the development and testing of the SUNMAN SMS1340 LCD using an Arduino Nano ATmega328P.

The project started by mapping the physical LCD segments controlled by the original `z_addr[]` address table. The goal is to create a reliable reference for future SMS1340 firmware development.

## Hardware

- Arduino Nano ATmega328P
- SUNMAN SMS1340 LCD
- LCD SCL: Arduino D6
- LCD SDA: Arduino D7

## Segment Mapping

The SMS1340 address table contains 50 indexed elements, from INDEX 0 to INDEX 49.

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

## Tests

The `Tests` directory contains experimental sketches used to identify and verify the SMS1340 LCD segments.

### SMS1340 Segment Mapper

`Tests/SMS1340_Segment_Mapper.ino`

This program automatically tests INDEX 0 through INDEX 49, illuminating one LCD segment at a time.

Serial Monitor:

**9600 baud**

The test uses the original SMS1340 LCD communication routines.

## Project Status

The SMS1340 segment map is being documented progressively.

Additional segments and functions will be verified as the firmware development continues.

## License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.
