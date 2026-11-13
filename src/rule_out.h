/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_out.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for OUT
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit conditions : No markers
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 27 
*/
void GenerateRuleOUT(uint16_t rule) {

    // Skip one position to LEFT (Unique rule OUT)
    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR
    rule = next$LEFT(rule);
    // Skip left PC
    rule = next$LEFT(rule);
    // Seek to A7
    rule = next$LEFT(rule, RIGHT);

    // Out A
    // Output of bit7 to bit1 (A register)
    SetOutCLK(HIGH);
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    // Latch shift register 74xx595 to output register 
    SetOutCLK(LOW);
    SetRule(rule, '$', '$', RIGHT, rule+2);
    rule++;
    // Skip Bx, set CLKOUT LOW and repeat next bit if A
    SetOutCLK(LOW);
    SetRule(rule, '0', '0', RIGHT, rule-1);
    SetRule(rule, '1', '1', RIGHT, rule-1);
    rule++;

    // Skip right PC
    rule = next$RIGHT(rule);
    // Skip right IR
    rule = next$RIGHT(rule);   
    // Skip right MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH); 

}