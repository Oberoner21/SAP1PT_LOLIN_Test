/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for LDI
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I1 of Instruction Register IR
    Entry conditions: Nothing is marked, jump to RULE_FETCH
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 33 
*/
void GenerateRuleLDI(uint16_t rule) {

    uint8_t i, j;

    // Skip left IR
    rule = next$LEFT(rule);
    // Skip left PC
    rule = next$LEFT(rule);
    // Skip left A/B register
    rule = next$LEFT(rule, RIGHT);

    // Set A7 to A4 to 0
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B7
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B6
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B5
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B4
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;

    // Mark A3
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    rule++;

    // Skip RIGHT A/B
    rule = next$RIGHT(rule);
    // Skip RIGHT PC
    rule = next$RIGHT(rule);
    // Skip RIGHT IR
    rule = next$RIGHT(rule);

    // 0 Read current bit of MAR mark it and branch
    SetRule(rule, '0', '_', LEFT, rule+5);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+9);  // End point
    rule++;

    // 1 Seek left to the marked bit of AB and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule);

    // 2 Skip Bx
    rule = nextRIGHT(rule);

    // 3 Mark the next bit right of AB
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 4 Seek right to the marked bit of MAR, write 1 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule-5);

    // 5 Seek left to the marked bit of AB and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule);

    // 6 Skip Bx
    rule = nextRIGHT(rule);

    // 7 Mark the next bit right of AB
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 8 Seek right to the marked bit of MMB, write 0 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule-9);   // The rule are just incrementet !!!!

    // ----- Done copy -----

    // 9 Remark the last marked byte of AB
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Skip PC
    rule = next$RIGHT(rule);
    // Skip IR
    rule = next$RIGHT(rule);
    // Seek to M0 and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);

}