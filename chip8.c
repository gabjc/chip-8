#include "chip8.h"
#include "stdint.h"
#include <stdio.h>

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

void loadGame(char game[]) {
  // Have to use fopen in binary mode to fill the memory location at 0x200 == 512
  FILE *file = fopen(game, "rb");

  for(int i = 0; i < bufferSize; ++i) {
    memory[i + 512] = buffer[i];
  }

}
  
void emulateCycle() {
  // Fetch Opcode
  fetchOpcode();

  // Decode Opcode
  decodeOpcode(opcode);
  
  // Update timers
  updateTimers();
}


unsigned short fetchOpcode() {
  opcode = memory[pc] << 8 | memory[pc + 1];
  return opcode;
}

void decodeOpcode() {
  switch(opcode & 0xF000) {
    // ANNN: Sets I to the address NNN
    case 0xA000: 
      // Execute opcode
      I = opcode & 0x0FFF;
      pc += 2;
    break;
    
    // 2NNN
    case 0x2000:
      stack[sp] = pc;
      ++sp;
      pc = opcode & 0x0FFF;
    break;
    
    //8XY4
    case 0x0004: 
      if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
        V[0xF] = 1; // carry
      }
      else {
        V[0xF] = 0;
      }
      V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
      pc += 2;          
    break;
    
    // FX33
    case 0x0033: 
      memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
      memory[I + 1] = (V[(opcode & 0x0F00) >> 8 / 10] % 10);
      memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
      pc += 2;
    break;

    
    switch(opcode & 0xF000) {    
      case 0x0000:
        switch(opcode & 0x000F)
        {
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
  break;
  }
}


void executeOpcode() {

}


void updateTimers() {
  if (delay_timer > 0) {
    --delay_timer;
  }

  if (sound_timer > 0) {
    if (sound_timer == 1) {
      printf("BEEP!\n");
    }
    --sound_timer;
  }
}