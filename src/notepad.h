/*
    TSAP1 - Turing SAP1
    Modul: notepad.h

    Notepad SAP1PT

*/

#pragma once
#include <Arduino.h>
#include "symbols.h"

#define     NOTEPADSIZE     0x100       // SAP1PT Notepad size 
#define     REG_A_START     5           // Notepad start position of register A/B
#define     MEMSTART        41          // Notepad start position of 16 byte SAP1 Memory

const char SAP1_REGISTERS[] = (

    // The PC is preloaded with te value 3. Program starts at address 0x3
    // ZC A/B              PC       IR   MAR
    "$$10$0000000000000000$00001011$0000$0000$"
);

uint16_t notePad[NOTEPADSIZE];


// Fibonacci 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233
//           0x01, 0x02, 0x03, 0x05, 0x08, 0x0D, 0x15, 0x22, 0x37, 0x59, 0x90, 0xE9
uint8_t Program[16] = {
                    // Fibonacci
                    // .org 0x3
	0x00,
	0x00,
	0x00,
	0x51,			//	LDI 0x1     // 51
	0x41,			//	STA [0x1]
	0x50,			//	LDI 0x0
	0x21,			//	ADD [0x1]
	0x73,			//	JC 0x3
	0xE0,			//	OUT
	0x42,			//	STA [0x2]
	0x11,			//	LDA [0x1]
	0x40,			//	STA [0x0]
	0x12,			//	LDA [0x2]
	0x41,			//	STA [0x1]
	0x10,			//	LDA [0x0]
	0x66 			//	JMP 0x6
};

// uint8_t Program[16] = {
//                     // Sub [0x1] - [0x2]
//                     // .org 0x3
// 	0x00,
// 	0x28,
// 	0x14,
// 	0x11,			//	LDA [0x1]
// 	0x32,			//	SUB [0x2]
// 	0xE0,			//	OUT
// 	0xF0,			//	HLT
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00
// };

// uint8_t Program[16] = {
//             // UP/Down counter
//             // .org 0x3
//     0x00,   // 0x0 Data0
// 	0xFF,   // 0x1 Data1
// 	0x01,   // 0x2 Data2
// 	0x11,	// 0x3 LDA [0x1]
// 	0x32,	// 0x4 SUB [0x2]
// 	0xE0,	// 0x5 OUT
//     0x88,   // 0x6 JZ 0x8
//     0x64,   // 0x7 JMP 0x4
// 	0x22,   // 0x8 ADD [0x2]
// 	0x74,   // 0x9 JC 0x4
// 	0xE0,   // 0xA OUT
// 	0x68,   // 0xB JMP 0x8
// 	0x00,
// 	0x00,
// 	0x00,
// 	0x00

// };

void AddressOut(uint8_t address){

    uint8_t i;

    // Send 8 bit address, MSB first
    for(i=0; i<8; i++) {

        digitalWrite(SERDATA, (address >> (7 - i)) & 0x01);
        digitalWrite(ADDRCLK, HIGH);
        delayMicroseconds(50);
        digitalWrite(ADDRCLK, LOW);
    }

    digitalWrite(ADDR_OUTCLK, HIGH);
    delayMicroseconds(50);
    digitalWrite(ADDR_OUTCLK, LOW);
}

void DataOut(uint8_t data){

    uint8_t i;

    // Send 8 bit data byte, MSB first
    for(i=0; i<8; i++) {

        digitalWrite(SERDATA, (data >> (7 - i)) & 0x01);
        digitalWrite(DATACLK, HIGH);
        delayMicroseconds(50);
        digitalWrite(DATACLK, LOW);
    }

    digitalWrite(DATA_OUTCLK, HIGH);
    delayMicroseconds(50);
    digitalWrite(DATA_OUTCLK, LOW);
}

void CopyProgram(){

    for(uint8_t i = 0; i<16; i++){

        AddressOut(i);
        DataOut(Program[i]);
        digitalWrite(MWRITE, HIGH);
        digitalWrite(CLKRAW, HIGH);
        delayMicroseconds(50);
        digitalWrite(CLKRAW, LOW);
        digitalWrite(MWRITE, LOW);
    }
}

void BuildNotepad()
{
    uint16_t i;

    // Clear the Notepad
    for(i=0; i<NOTEPADSIZE; i++)
    {
        notePad[i] = 0;
    }

    for(i=0; i<MEMSTART+1; i++)
    {
        notePad[i] = symbol2num(SAP1_REGISTERS[i]);
    }

    CopyProgram();
}