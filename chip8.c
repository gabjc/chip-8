#include "chip8.h"

void initialize(void) {
    // Initialize registers and memory once
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];
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