/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_cpyINSTtoIR.h

    Copyright(C) 2026 by Oberoner

    Copies the instruction of the current program step into the IR and
    the argument of instruction into memory address register MAR
    The instruction is stored in the high nibble of the opcode, 
    the argument in the low nibble.
    Entry conditions: None
    Entry point     : NP pointer position P3 of Program Counter PC
    Exit conditions : IR contains the current instruction, the argument is stored in MAR
    Exit point      : NP pointer position I3 of Instruction Registers IR
    Rules           : 64 
*/

void CopyINSTtoIR(uint16_t rule){

    // Seek to I3
    rule = next$RIGHT(rule);

    // Set the SRAM address
    rule = SetRAMAddress(rule);

    // +++++ Read the low nibble of SRAM value into MAR. This is the argument ++++

    // Clock MREAD to store SRAM value into parallel in
    // serial out shift register 74LS165 and give out bit 7
    ResetControlLines();
    SetMREAD(HIGH);
    rule = nextLEFT(rule);
    SetMREAD(LOW);
    rule = nextRIGHT(rule);
    // 0
    SetRule(rule, '0', '0', LEFT, rule +1);
    SetRule(rule, '1', '0', LEFT, rule +1);
    SetRuleDIN(rule, '0', '1', LEFT, rule +1);
    SetRuleDIN(rule, '1', '1', LEFT, rule +1);
    rule++;
    // 1
    SetDataCLK(HIGH);
    SetRule(rule, '0', '0', RIGHT, rule +1);
    SetRule(rule, '1', '1', RIGHT, rule +1);
    SetDataCLK(LOW);
    SetRule(rule, '$', '$', LEFT, rule +2);
    rule++;
    // 2
    SetDataCLK(LOW);
    SetRule(rule, '0', '0', LEFT, rule -2);
    SetRule(rule, '1', '1', LEFT, rule -2);
    rule++;

    // +++ Read the high nibble of SRAM value into IR. This is the instruction +++
    SetDataCLK(HIGH);
    rule = nextLEFT(rule);
    SetDataCLK(LOW);
    rule = nextRIGHT(rule);
    // 0
    SetRule(rule, '0', '0', LEFT, rule +1);
    SetRule(rule, '1', '0', LEFT, rule +1);
    SetRuleDIN(rule, '0', '1', LEFT, rule +1);
    SetRuleDIN(rule, '1', '1', LEFT, rule +1);
    rule++;
    // 1
    SetDataCLK(HIGH);
    SetRule(rule, '0', '0', RIGHT, rule +1);
    SetRule(rule, '1', '1', RIGHT, rule +1);
    SetDataCLK(LOW);
    SetRule(rule, '$', '$', RIGHT, RULE_DECODE);
    rule++;
    // 2
    SetDataCLK(LOW);
    SetRule(rule, '0', '0', LEFT, rule -2);
    SetRule(rule, '1', '1', LEFT, rule -2);
    rule++;
}
