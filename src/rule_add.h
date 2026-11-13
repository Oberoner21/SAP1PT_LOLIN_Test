/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_add.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for ADD
    Entry conditions: None
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position P3 of Program Counter
    Rules           : ??
*/
void GenerateRuleADD(uint16_t rule) {

    // Set SRAM address stored in MAR and clock it out
    // Seek to B0
    // Read SRAM value into Register B
    // Go to RULE_ALU

    // Seek to I3
    rule = next$LEFT(rule, RIGHT);

    // Set the SRAM address
    rule = SetRAMAddress(rule);

    // +++ Seek to B0 +++
    // Skip LEFT MAR
    rule = next$LEFT(rule);
    // Skip LEFT IR
    rule = next$LEFT(rule);
    // Skip PC
    rule = next$LEFT(rule);

    // +++ Read SRAM value into B

    // Clock MREAD to store SRAM value into parallel in
    // serial out shift register 74LS165 and give out bit 7
    ResetControlLines();
    SetMREAD(HIGH);
    rule = nextLEFT(rule);
    SetMREAD(LOW);
    rule = nextRIGHT(rule);

    // +++ Read SRAM value into B +++
    // First byte is just clocked out with MREAD clock
    // 0 Read bit and write Bx
    SetDataCLK(LOW);
    SetRule(rule, '0', '0', LEFT, rule +1);
    SetRule(rule, '1', '0', LEFT, rule +1);
    SetRule(rule, '$', '$', RIGHT, rule +2);
    SetRuleDIN(rule, '0', '1', LEFT, rule +1);
    SetRuleDIN(rule, '1', '1', LEFT, rule +1);
    SetRuleDIN(rule, '$', '$', RIGHT, rule +2);
    rule++;
    // 2 Skip Ax and clock the shift register
    SetDataCLK(HIGH);
    SetRule(rule, '0', '0', LEFT, rule -1);
    SetRule(rule, '1', '1', LEFT, rule -1);
    rule++;

    // +++ Seek to P3 +++
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', RIGHT, RULE_ALU);

}