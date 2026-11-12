/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_setRAMaddress.h

    Copyright(C) 2026 by Oberoner

    Set the SRAM address stored in register MAR (low nibble, high nibble is zero)
    Entry conditions: None
    Entry point     : NP pointer position I3 of the instruction register IR
    Exit conditions : None
    Exit point      : NP pointer position M0 of Memory Address Register MAR
    Rules           : ?? 
*/

uint16_t SetRAMAddress(uint16_t rule) {

    // Set the MAR high nibble to zero
    // 0
    SetRule(rule, '0', '0', RIGHT, rule + 1);
    SetRule(rule, '1', '1', RIGHT, rule + 1);
    SetRule(rule, '$', '$', RIGHT, rule + 3);
    rule++;
    // 1
    SetSerDATA(LOW);
    SetAddrCLK(HIGH);
    SetRule(rule, '0', '0', LEFT, rule + 1);
    SetRule(rule, '1', '1', LEFT, rule + 1);
    SetRule(rule, '$', '$', RIGHT, rule + 2);
    rule++;
    // 2
    ResetControlLines();
    SetRule(rule, '0', '0', RIGHT, rule - 2);
    SetRule(rule, '1', '1', RIGHT, rule - 2);
    SetRule(rule, '$', '$', RIGHT, rule + 1);
    rule++;

    // Write value of MAR to bit 0 to 3
    // of SRAM address (low nibble)
    // 0
    ResetControlLines();
    SetAddrCLK(LOW);
    SetSerDATA(LOW);
    SetRule(rule, '0', '0', RIGHT, rule + 1);
    SetSerDATA(HIGH);
    SetRule(rule, '1', '1', RIGHT, rule + 2);
    //SetSerDATA(LOW);
    //SetRule(rule, '$', '$', RIGHT, rule + 4);
    rule++;
    // 1 Clock a LOW into address shift register and move to left
    SetSerDATA(LOW);
    SetAddrCLK(HIGH);
    SetRule(rule, '0', '0', LEFT, rule + 2);
    SetRule(rule, '1', '1', LEFT, rule + 2);
    SetRule(rule, '$', '$', LEFT, rule + 3);
    rule++;
    // 2 Clock a HIGH into address shift register and move to left
    SetSerDATA(HIGH);
    SetAddrCLK(HIGH);
    SetRule(rule, '0', '0', LEFT, rule + 1);
    SetRule(rule, '1', '1', LEFT, rule + 1);
    SetRule(rule, '$', '$', LEFT, rule + 2);
    rule++;
    // 3 Clear the control lines and move back to right
    ResetControlLines();
    SetRule(rule, '0', '0', RIGHT, rule -3);
    SetRule(rule, '1', '1', RIGHT, rule -3);
    SetRule(rule, '$', '$', RIGHT, rule -3);
    rule++;

    // Clock out the address shift register and set the SRAM address
    ResetControlLines();
    SetAddrOUTCLK(HIGH);
    SetRule(rule, '0', '0', RIGHT, rule +1);
    SetRule(rule, '1', '1', RIGHT, rule +1);
    rule++;
    SetAddrOUTCLK(LOW);
    SetRule(rule, '$', '$', LEFT, rule +1);
    rule++;

    return rule;
}

