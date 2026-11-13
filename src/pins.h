/*
    TSAP1 - Turing SAP1
    Modul: pins.h

    PIN-Definitionen ESP32-WROVER-E LOLIN D32 PRO

*/

#pragma once
#include <Arduino.h>

const uint8_t CLKRAW        = 23;
const uint8_t MWRITE        = 22;
const uint8_t MREAD         = 21;
const uint8_t DIN           = 19;
const uint8_t D0            = 18;
const uint8_t D1            = 5;
const uint8_t OUTCLK        = 4;
const uint8_t GPIO15        = 15;

const uint8_t GPIO34        = 34; // Input only
const uint8_t SERDATA       = 32;
const uint8_t DATACLK       = 33;
const uint8_t ADDRCLK       = 25;
const uint8_t ADDR_OUTCLK   = 26;
const uint8_t DATA_OUTCLK   = 27;
const uint8_t GPIO14        = 14;

const uint8_t BUTTON        = 13;


void InitGPIOPins(){

    pinMode(CLKRAW, OUTPUT);
    digitalWrite(CLKRAW, LOW);

    pinMode(MWRITE, OUTPUT);
    digitalWrite(MWRITE, LOW);

    pinMode(MREAD, OUTPUT);
    digitalWrite(MREAD, LOW);

    pinMode(DIN, INPUT);

    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(OUTCLK, OUTPUT);

    pinMode(GPIO15, INPUT);

    pinMode(GPIO34, INPUT);
    pinMode(SERDATA, OUTPUT);
    pinMode(DATACLK, OUTPUT);
    pinMode(ADDRCLK, OUTPUT);
    pinMode(ADDR_OUTCLK, OUTPUT);
    pinMode(DATA_OUTCLK, OUTPUT);
    pinMode(GPIO14, INPUT);

}