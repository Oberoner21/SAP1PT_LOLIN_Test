/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_lda.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for LDA
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : ?? 
*/
void GenerateRuleLDA(uint16_t rule) {

    // Serial data out of SRAM address, stored in MAR
    // Clock out address

    // Seek to I3
    rule = next$LEFT(rule, RIGHT);

    // Set SRAM address stored in MAR
    rule = SetRAMAddress(rule);

    // ---- Seek to A0 ------
    // Skip LEFT MAR
    rule = next$LEFT(rule);
    // Skip LEFT IR
    rule = next$LEFT(rule);
    // Skip LEFT PC
    rule = next$LEFT(rule);
    // Skip B0
    rule = nextLEFT(rule);

    // +++ Read SRAM value into B

    // Clock MREAD to store SRAM value into parallel in
    // serial out shift register 74LS165 and give out bit 7
    ResetControlLines();
    SetMREAD(HIGH);
    rule = nextLEFT(rule);
    SetMREAD(LOW);
    rule = nextRIGHT(rule);

    // +++ Read SRAM value into A +++
    // First byte is just clocked out with MREAD clock
    // 0 Read bit and write Ax
    SetDataCLK(LOW);
    SetRule(rule, '0', '0', LEFT, rule +1);
    SetRule(rule, '1', '0', LEFT, rule +1);
    SetRuleDIN(rule, '0', '1', LEFT, rule +1);
    SetRuleDIN(rule, '1', '1', LEFT, rule +1);
    rule++;
    // 2 Skip Bx and clock the shift register
    SetDataCLK(HIGH);
    SetRule(rule, '0', '0', LEFT, rule -1);
    SetRule(rule, '1', '1', LEFT, rule -1);
    SetRule(rule, '$', '$', RIGHT, rule +1);
    rule++;

    // ---- Seek to M0 and goto RULE_FETCH
    // Skip right A/B
    rule = next$RIGHT(rule);
    // Skip right PC
    rule = next$RIGHT(rule);
    // Skip right IR
    rule = next$RIGHT(rule);
    // Skip right MAR
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);

}