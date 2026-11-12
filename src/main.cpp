#include <Arduino.h>
#include <Pushbutton.h>
#include "pins.h"
#include "notepad.h"
#include "rulebook.h"

// Objects
Pushbutton stepButton(BUTTON);   // Wire the step button on the button pin to GND

void setup() {

    Serial.begin(115200);
    Serial.println(); 
    
    InitGPIOPins();
    GenerateRuleBook();
    BuildNotepad();
}

const uint16_t CLK_DELAY = 50;

// Variables
uint8_t notePadPointer = 0;
uint8_t symbol;
uint16_t nextRule;
uint8_t wSymbol;
uint8_t dataIn = 0;
uint16_t rule = 0;
boolean halt = false;
uint8_t rSymbol;
uint16_t lastRule;
int8_t direction;
uint16_t lastNotePadPointer;
long cykles = 0;
long cyklesBefore = 0;
uint16_t address = 0;

void loop() {
  
    delay(1);

    if(!halt) {

            cykles++;
            lastRule = rule;

            dataIn = digitalRead(DIN) == HIGH ? 1 : 0;  // Read the data in bit
            symbol = notePad[notePadPointer];
            nextRule = ruleBook[rule][symbol][dataIn].next;
            wSymbol = ruleBook[rule][symbol][dataIn].writeSymbol;
            notePad[notePadPointer] = wSymbol;
            direction = ruleBook[rule][symbol][dataIn].direction;
            notePadPointer += direction;

            // Set the control lines
            digitalWrite(SERDATA, ruleBook[rule][symbol][dataIn].serDATA);
            digitalWrite(DATACLK, ruleBook[rule][symbol][dataIn].dataCLK);
            digitalWrite(DATA_OUTCLK, ruleBook[rule][symbol][dataIn].dataOUTCLK);
            digitalWrite(ADDRCLK, ruleBook[rule][symbol][dataIn].addressCLK);
            digitalWrite(ADDR_OUTCLK, ruleBook[rule][symbol][dataIn].addressOUTCLK);
            digitalWrite(MREAD, ruleBook[rule][symbol][dataIn].mRead);
            digitalWrite(MWRITE, ruleBook[rule][symbol][dataIn].mWrite);

            // Clock the external mashine
            digitalWrite(CLKRAW, HIGH);
            delayMicroseconds(CLK_DELAY);
            digitalWrite(CLKRAW, LOW);

            // Make next rule to curent rule
            rule = nextRule;

            if(nextRule == RULE_ERROR) {

                Serial.println();
                Serial.println("ERROR state");
                Serial.print("NPP: ");
                Serial.print(notePadPointer);
                Serial.print("  RSymbol: ");
                Serial.print(cSymbols[notePad[notePadPointer]]);
                Serial.print("  Rule: ");
                Serial.println(lastRule);

                for(uint8_t i = 0; i < MEMSTART; i++){
                    Serial.print(cSymbols[notePad[i]]);
                }
                Serial.println();
                for(uint8_t j = 0; j < notePadPointer; j++){
                    Serial.print(".");
                }
                Serial.println("|");

                halt = true;
            }

            if(nextRule == RULE_END) {

                Serial.println();
                Serial.println("END state");
                Serial.print("NPP: ");
                Serial.print(notePadPointer);
                Serial.print("  RSymbol: ");
                Serial.print(cSymbols[notePad[notePadPointer]]);
                Serial.print("  Rule: ");
                Serial.println(lastRule);

                for(uint8_t i = 0; i < MEMSTART; i++){
                    Serial.print(cSymbols[notePad[i]]);
                }
                Serial.println();
                for(uint8_t j = 0; j < notePadPointer; j++){
                    Serial.print(".");
                }
                Serial.println("|");

                halt = true;
            }

            if(rule == RULE_OUT) {

                Serial.println(cykles - cyklesBefore);
                cyklesBefore = cykles;
            }

    }
}
