/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_sta.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for STA
    Entry conditions: None
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit conditions : None
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 92 
*/
void GenerateRuleSTA(uint16_t rule) {

    // Serial data out of SRAM address, stored in MAR
    // Clock out address
    // Serial data out of Register A
    // Clock out data
    // Set MWRITE
    // Write data into SRAM
    // Go back to RULE_FETCH

    // Seek to I3
    rule = next$LEFT(rule, RIGHT);

    // Set SRAM address stored in MAR
    rule = SetRAMAddress(rule);

    // Skip LEFT MAR
    rule = next$LEFT(rule);
    // Skip LEFT IR
    rule = next$LEFT(rule);
    // Skip LEFT PC
    rule = next$LEFT(rule);
    // Seek to A7
    rule = next$LEFT(rule, RIGHT);

    // ++++ Set SRAM data +++
    // Write value of A into SRAM data shift register
    // 0
    ResetControlLines();
    // 0
    SetDataCLK(LOW);
    SetSerDATA(LOW);
    SetRule(rule, '0', '0', RIGHT, rule +1);
    SetSerDATA(HIGH);
    SetRule(rule, '1', '1', RIGHT, rule +2);
    SetSerDATA(LOW);
    SetDataCLK(LOW);
    SetRule(rule, '$', '$', RIGHT, rule +3);
    rule++;
    // 1 Skip Bx and clock in LOW
    SetSerDATA(LOW);
    SetDataCLK(HIGH);
    SetRule(rule, '0', '0', RIGHT, rule -1);
    SetRule(rule, '1', '1', RIGHT, rule -1);
    rule++;
    // 2 Skip Bx and clock in HIGH
    SetSerDATA(HIGH);
    SetDataCLK(HIGH);
    SetRule(rule, '0', '0', RIGHT, rule -2);
    SetRule(rule, '1', '1', RIGHT, rule -2);
    rule++;

    // Clock out the data shift register and set the SRAM data
    // The chip is not yet output enabled
    ResetControlLines();
    SetDataOUTCLK(HIGH);
    SetRule(rule, '0', '0', LEFT, rule +1);
    SetRule(rule, '1', '1', LEFT, rule +1);
    rule++;
    SetDataOUTCLK(LOW);
    SetRule(rule, '$', '$', RIGHT, rule +1);
    rule++;

    // Set MWrite to store data into SRAM
    ResetControlLines();
    SetMWRITE(HIGH);
    SetRule(rule, '0', '0', LEFT, rule +1);
    SetRule(rule, '1', '1', LEFT, rule +1);
    rule++;
    SetMWRITE(LOW);
    SetRule(rule, '$', '$', RIGHT, rule +1);
    rule++;   

    // Seek to M0 and go to RULE_FETCH

    // Skip RIGHT PC
    rule = next$RIGHT(rule);
    // Skip RIGHT IR
    rule = next$RIGHT(rule); 
    // Skip RIGHT MAR and goto RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);






    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR (is yet market) and PC
    rule = next$LEFT(rule);

    // Skip left A/B register and mark it
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // Seek to IR marker and remark the register
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Seek to MAR to bit M3
    rule = next$RIGHT(rule);

    // Mark M3 and skip right
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // Seek to bit7 of the marked memory byte
    rule = next_RIGHT(rule);

    // Copy low nibble of MMB into MAR
    //rule = CopyMMBArgToMAR(rule);

    // // Seek back to bit3 of MAR
    rule = next_LEFT(rule);

    // Mark the memory byte, addressed by register MAR
    rule = MarkMemoryByte(rule);
    // Change the last rule to remark MMB
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Mark bit7 of MMB
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+1);
    rule++;

    // Seek to A7 of marked register A/B
    rule = next_LEFT(rule); 

    // Copy A into MMB
    //rule = CopyAtoMMB(rule);

    // Skip right register A/B
    rule = next$RIGHT(rule);
    // Skip right PC
    rule = next$RIGHT(rule); 
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);    
    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);  

}