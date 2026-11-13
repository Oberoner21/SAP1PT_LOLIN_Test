/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_jc.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for JC
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 32 
*/
void GenerateRuleJC(uint16_t rule) {

    // Seek to carry flag
    // If not carry -> END 
    // If carry
    //  - Seek to PC and mark it
    //  - Copy low nibble MMB into interleaved PC
    //  - Remark PC and END
    //  END
    //  - Remark MMB,
    //  - Remark IR
    //  - Seek to MAR0
    //  - Go to RULE_FETCH

    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR
    rule = next$LEFT(rule);
    // Skip left PC
    rule = next$LEFT(rule);
    // Skip left A/B register
    rule = next$LEFT(rule);

    // Read carry and branch
    SetRule(rule, '0', '0', RIGHT, rule +1);     // Carry not set
    SetRule(rule, '1', '1', RIGHT, rule +5);     // Carry set
    rule++;

    // +++ Carry not set +++

    // Skip RIGHT $
    rule = nextRIGHT(rule);
    // Skip A/B
    rule = next$RIGHT(rule);
    // Skip PC
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', RIGHT, rule +16);

    // ++++++++ Carry is set +++++++++++++

    // Seek to P3
    rule = next$RIGHT(rule);
    rule = next$RIGHT(rule);
    rule = next$RIGHT(rule);

    // Copy MAR into PC
    rule = CopyMARtoPC(rule);

    // --- Exit point ---

    // Skip IR
    rule = next$RIGHT(rule);
    // Seek to M0 and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH); 

}