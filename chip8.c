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

