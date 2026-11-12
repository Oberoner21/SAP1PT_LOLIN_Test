/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rulebook.h

    Copyright(C) 2026 by Oberoner
*/
#pragma once
#include <esp_heap_caps.h>
#include <esp32-hal-psram.h>
#include "symbols.h"

#define     LEFT            -1
#define     RIGHT           1
#define     CLKOUT          1
#define     RULES           0x400
#define     SYMBOLS         4
#define     INPUTS          2

#define     RULE_RESET      0
#define     RULE_FETCH      20
#define     RULE_DECODE     150
#define     RULE_NOP        200
#define     RULE_LDA        250
#define     RULE_ALU        350
#define     RULE_ADD        400
#define     RULE_SUB        500
#define     RULE_STA        600
#define     RULE_LDI        700
#define     RULE_JMP        750
#define     RULE_JC         800
#define     RULE_JZ         850
#define     RULE_OUT        900
#define     RULE_HLT        950

#define     RULE_END        1022
#define     RULE_ERROR      1023


struct rule_t {
    uint16_t next;
    int8_t direction;
    uint8_t writeSymbol;
    uint8_t serDATA;
    uint8_t outCLK;
    uint8_t addressCLK;
    uint8_t dataCLK;
    uint8_t mRead;
    uint8_t mWrite;
    uint8_t addressOUTCLK;
    uint8_t dataOUTCLK;
}; //ruleBook[RULES][SYMBOLS][INPUTS];

uint8_t controlLines = 0;


// Zeiger auf den Speicher im PSRAM
rule_t (*ruleBook)[SYMBOLS][INPUTS] = nullptr;

bool AllocateRuleBook()
{
    // Anzahl der benötigten Elemente
    size_t elements = (size_t)RULES * SYMBOLS * INPUTS;

    // Benötigte Speichergröße
    size_t memorySize = elements * sizeof(rule_t);

    // Prüfen, ob PSRAM vorhanden ist
    if (!psramFound()) {
        Serial.println("FEHLER: Kein PSRAM gefunden!");
        return false;
    }

    // Speicher im PSRAM reservieren
    ruleBook =(rule_t (*)[SYMBOLS][INPUTS]) ps_malloc(sizeof(rule_t) * RULES * SYMBOLS * INPUTS);

    if (ruleBook == nullptr) {
        Serial.println("FEHLER: PSRAM-Allokation fehlgeschlagen!");
        return false;
    }

    // Gesamten Speicher initialisieren
    memset(ruleBook, 0, memorySize);

    Serial.println("RuleBook erfolgreich im PSRAM angelegt!");

    Serial.print("Elemente: ");
    Serial.println(elements);

    Serial.print("Speicherbedarf: ");
    Serial.print(memorySize);
    Serial.println(" Byte");

    return true;
}


// Control line
void ResetControlLines(){

    controlLines = 0;
}

void SetSerDATA(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x01;
    else controlLines = controlLines & 0xFE;
}


void SetOutCLK(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x02;
    else controlLines = controlLines & 0xFD;
}

void SetAddrCLK(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x04;
    else controlLines = controlLines & 0xFB;
}

void SetDataCLK(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x08;
    else controlLines = controlLines & 0xF7;
}

void SetMREAD(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x10;
    else controlLines = controlLines & 0xEF;
}

void SetMWRITE(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x20;
    else controlLines = controlLines & 0xDF;
}

void SetAddrOUTCLK(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x40;
    else controlLines = controlLines & 0xBF;
}

void SetDataOUTCLK(uint8_t value){

    if(value == HIGH) controlLines = controlLines | 0x80;
    else controlLines = controlLines & 0x7F;
}

void SetRule(uint16_t rule, uint8_t readSymbol, uint8_t writeSymbol, int8_t direction, uint16_t nextRule)
{
    uint8_t numReadSymbol = symbol2num(readSymbol);

    ruleBook[rule][numReadSymbol][0].writeSymbol = symbol2num(writeSymbol);
    ruleBook[rule][numReadSymbol][1].writeSymbol = symbol2num(writeSymbol);
    ruleBook[rule][numReadSymbol][0].direction = direction;
    ruleBook[rule][numReadSymbol][1].direction = direction;
    ruleBook[rule][numReadSymbol][0].next = nextRule;
    ruleBook[rule][numReadSymbol][1].next = nextRule;
    // Set the control lines
    ruleBook[rule][numReadSymbol][0].serDATA = controlLines & 0x01 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].serDATA = controlLines & 0x01 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].outCLK = controlLines & 0x02 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].outCLK = controlLines & 0x02 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].addressCLK = controlLines & 0x04 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].addressCLK = controlLines & 0x04 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].dataCLK = controlLines & 0x08 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].dataCLK = controlLines & 0x08 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].mRead = controlLines & 0x10 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].mRead = controlLines & 0x10 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].mWrite = controlLines & 0x20 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].mWrite = controlLines & 0x20 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].addressOUTCLK = controlLines & 0x40 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].addressOUTCLK = controlLines & 0x40 ? 1 : 0;
    ruleBook[rule][numReadSymbol][0].dataOUTCLK = controlLines & 0x80 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].dataOUTCLK = controlLines & 0x80 ? 1 : 0;
}

void SetRuleDIN(uint16_t rule, uint8_t readSymbol, uint8_t writeSymbol, int8_t direction, uint16_t nextRule)
{
    uint8_t numReadSymbol = symbol2num(readSymbol);

    ruleBook[rule][numReadSymbol][1].writeSymbol = symbol2num(writeSymbol);
    ruleBook[rule][numReadSymbol][1].direction = direction;
    ruleBook[rule][numReadSymbol][1].next = nextRule;
    // Set the control lines
    ruleBook[rule][numReadSymbol][1].serDATA = controlLines & 0x01 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].outCLK = controlLines & 0x02 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].addressCLK = controlLines & 0x04 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].dataCLK = controlLines & 0x08 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].mRead = controlLines & 0x10 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].mWrite = controlLines & 0x20 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].addressOUTCLK = controlLines & 0x40 ? 1 : 0;
    ruleBook[rule][numReadSymbol][1].dataOUTCLK = controlLines & 0x80 ? 1 : 0;
}


uint16_t Set_RULE_END(uint16_t rule){

    SetRule(rule, '0', '0', RIGHT, rule +1);
    SetRule(rule, '1', '1', RIGHT, rule +1);
    SetRule(rule, '$', '$', RIGHT, rule +1);
    SetRule(rule, '_', '_', RIGHT, rule +1);
    rule++;

    SetRule(rule, '0', '0', LEFT, RULE_END);
    SetRule(rule, '1', '1', LEFT, RULE_END);
    SetRule(rule, '$', '$', LEFT, RULE_END);
    SetRule(rule, '_', '_', LEFT, RULE_END);
    rule++;
    
    return rule;
}

uint16_t nextLEFT(uint16_t rule){

    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);

    return rule+1;
}

uint16_t nextRIGHT(uint16_t rule){

    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    SetRule(rule, '_', '_', RIGHT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+1);

    return rule+1;
}

uint16_t next$RIGHT(uint16_t rule, int8_t endDirection = RIGHT) {

    SetRule(rule, '0', '0', RIGHT, rule);
    SetRule(rule, '1', '1', RIGHT, rule);
    SetRule(rule, '_', '_', RIGHT, rule);
    SetRule(rule, '$', '$', endDirection, rule+1);
    
    return rule+1;
}

uint16_t next$LEFT(uint16_t rule, int8_t endDirection = LEFT) {

    SetRule(rule, '0', '0', LEFT, rule);
    SetRule(rule, '1', '1', LEFT, rule);
    SetRule(rule, '_', '_', LEFT, rule);
    SetRule(rule, '$', '$', endDirection, rule+1);
    
    return rule+1;
}

uint16_t next_LEFT(uint16_t rule, int8_t endDirection = RIGHT){

    SetRule(rule, '0', '0', LEFT, rule);        
    SetRule(rule, '1', '1', LEFT, rule);
    SetRule(rule, '$', '$', LEFT, rule);
    SetRule(rule, '_', '_', endDirection, rule+1);

    return rule+1;
}

uint16_t next_RIGHT(uint16_t rule, int8_t endDirection = RIGHT){

    SetRule(rule, '0', '0', RIGHT, rule);        
    SetRule(rule, '1', '1', RIGHT, rule);
    SetRule(rule, '$', '$', RIGHT, rule);
    SetRule(rule, '_', '_', endDirection, rule+1);

    return rule+1;
}

#include "rule_markMemByte.h"
#include "rule_cpyPCtoMAR.h"
#include "rule_incPC.h"
#include "rule_setRAMaddress.h"
#include "rule_cpyINSTtoIR.h"
#include "rule_cpyMMBArgToPC.h"

#include "rule_reset.h"
#include "rule_fetch.h"
#include "rule_decode.h"
#include "rule_ldi.h"
#include "rule_sta.h"

void GenerateRuleBook()
{
    AllocateRuleBook();     // Allocates the rulebook in PSRAM

    uint16_t rule = 0;

    // Initialize all rules as Error State
    while(rule < RULES){

        SetRule(rule, '_', '_', RIGHT, RULE_ERROR);
        SetRule(rule, '0', '0', RIGHT, RULE_ERROR);  
        SetRule(rule, '1', '1', RIGHT, RULE_ERROR);
        SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
        rule++;
    }

    GenerateRuleRESET(RULE_RESET);
    GenerateRuleFETCH(RULE_FETCH);
    GenerateRuleDECODE(RULE_DECODE);
    GenerateRuleLDI(RULE_LDI);
    GenerateRuleSTA(RULE_STA);
}