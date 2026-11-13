/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_jmp.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Instruction rule for JMP
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : ?? 
*/
void GenerateRuleJMP(uint16_t rule) {

    // --- Seek to P3 ------
    // Seek left to IR
    rule = next$LEFT(rule);
    // Seek to P3
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '$', RIGHT, rule);

    // Copy MAR into PC
    rule = CopyMARtoPC(rule);

    // --- Seek to M0 and goto RULE_FETCH
    // Skip IR
    rule = next$RIGHT(rule);
    // Seek to M0 and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH); 

}