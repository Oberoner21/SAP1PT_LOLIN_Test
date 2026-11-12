/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_reset.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Reset procedure to start the machine
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position 0
    Exit point      : NP pointer position 0 
    Rules           : 87 
*/

void GenerateRuleRESET(uint16_t rule) {

    // ------------ Skip to position MAR0 -------------

    rule = next$RIGHT(rule);                        // Skip first $
    rule = next$RIGHT(rule);                        // Skip second $
    rule = next$RIGHT(rule);                        // Skip the flags
    rule = next$RIGHT(rule);                        // Skip the A/B Register
    rule = next$RIGHT(rule);                        // Skip the Program Counter
    rule = next$RIGHT(rule);                        // Skip the Instruction Register

    rule = next$RIGHT(rule);                        // Skip the MAR and go to rule FETCH
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);
}