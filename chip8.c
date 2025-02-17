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
  unsigned short x = (opcode & 0x0F00) >> 8;
  unsigned short y = (opcode & 0x00F0) >> 4;


  switch(opcode & 0xF000) {
    case 0x0000:
      switch(opcode & 0x00FF) {    
        case 0x00E0:

        break;
  
        case 0x00EE: // 0x00EE: Returns from subroutine          
          // Execute opcode
          return;
        break;
      
        default:
          printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
      }
    break;

    
    // 1NNN
    case 0x1000:
      // stack[sp] = pc;
      // ++sp;
      pc = opcode & 0x0FFF;
    break;

    // 2NNN
    case 0x2000:
      stack[sp] = pc;
      ++sp;
      pc = opcode & 0x0FFF;
    break;
    
    // 3XNN
    case 0x3000:
      if (V[x] == (opcode & 0x00FF)) {
        pc += 2;
      }
      pc += 2;
    break;
    
    // 4XNN
    case 0x4000:
      if (V[x] != (opcode & 0x00FF)) {
        pc += 2;
      }
      pc += 2;
    break;

    // 5XY0
    case 0x5000:
      if (V[x] == V[y]) {
        pc += 2;
      }
      pc += 2;
    break;

    // 6XNN
    case 0x6000:
      V[x] = opcode & 0x00FF;
      pc += 2;
    break;

    //7XNN
    case 0x7000:
      V[x] += opcode & 0x00FF;
      pc += 2;
    break;

    // 8XYN
    case 0x8000:
      switch (opcode & 0x000F) {
        // 8XY0
        case 0x0000: 

        break;

        // 8XY1
        case 0x0001: 

        break;
        
        // BXY2
        case 0x0002:

        break;
        
        // BXY3
        case 0x0003:

        break;

        // 8XY4
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
        
        // 8XY5
        case 0x0005: 
          //figure out "underflow"
          if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
            V[0xF] = 0; // carry
          }
          else {
            V[0xF] = 1;
          }
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
          pc += 2;          
        break;
        
        // BXY6
        case 0x0006:

        break;

        // BXY7
        case 0x0007:

        break;

        // BXYE
        case 0x000E:

        break;
    }
    break;
    
    // 9XY0
    case 0x9000:

    break;
  
    // ANNN: Sets I to the address NNN
    case 0xA000: 
      // Execute opcode
      I = opcode & 0x0FFF;
      pc += 2;
    break;
    
    // FX33
    case 0x0033: 
      memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
      memory[I + 1] = (V[(opcode & 0x0F00) >> 8 / 10] % 10);
      memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
      pc += 2;
    break;
  break;
  }
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