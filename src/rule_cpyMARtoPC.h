/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_cpyMARtoPC.h

    Copyright(C) 2026 by Oberoner

    Copied the value of MAR into interleaved PC
    Entry point     : NP pointer position P3 of PC
    Exit point      : NP pointer position I3 of instruction register IR 
    Rules           : ?? 
*/

uint16_t CopyMARtoPC(uint16_t rule) {

    // Mark P3
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '_', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // Seek to M3
    // Skip PC right
    rule = next$RIGHT(rule);
    // Skip IR
    rule = next$RIGHT(rule);

    // Copy MAR  to PC
    // 0 Read current bit of MAR mark it and branch
    SetRule(rule, '0', '_', LEFT, rule +5);
    SetRule(rule, '1', '_', LEFT, rule +1);
    SetRule(rule, '$', '$', LEFT, rule +9);  // End point
    rule++;

    // 1 Seek left to the marked bit of PC and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule);

    // 2 Skip Cx
    rule = nextRIGHT(rule);

    // 3 Mark the next bit right of PC
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 4 Seek right to the marked bit of MAR, write 1 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule-5);

    // 5 Seek left to the marked bit of PC and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule);

    // 6 Skip Cx
    rule = nextRIGHT(rule);

    // 7 Mark the next bit right of PC
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 8 Seek right to the marked bit of MAR, write 0 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule-9);

    // --- End point --

    // 9 Remark the last marked byte of PC
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);   

    return rule;
}