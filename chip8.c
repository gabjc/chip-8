#include "chip8.h"

void initialize(void) {
    // Initialize registers and memory once
    opcode = 0;
    I = 0;
    pc = 0x200;
    sp = 0;

    // Clear display	
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset
    for(int i = 0; i < 80; ++i) {
      memory[i] = chip8_fontset[i];	
    }

    // Reset Timers
    delay_timer = 0;
    sound_timer = 0;
  }

  
  
void emulateCycle() {
  // Fetch Opcode
  // Decode Opcode
  // Execute Opcode

  // Update timers
}


unsigned short fetchOpcode(unsigned char *memory[4096], unsigned short *pc) {
  unsigned short opcode = *memory[*pc] << 8 | *memory[*pc + 1];
  return opcode;
}

void decodeOpcode(unsigned short *opcode) {
  switch(*opcode & 0xF000) {
    case 0x0000:
    switch(*opcode & 0x000F) {
      case 0x0000: // 0x00E0: Clears the screen        
        // Execute opcode
      break;
 
      case 0x000E: // 0x00EE: Returns from subroutine          
        // Execute opcode
      break;
 
      default:
        printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
    }
  break;
  }
}


void executeOpcode() {

}


void updateTimers() {

}