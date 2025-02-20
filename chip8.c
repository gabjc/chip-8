#include "chip8.h"
#include "stdint.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>


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

void loadGame(char* game) {
  // Have to use fopen in binary mode to fill the memory location at 0x200 == 512
  FILE *file = fopen(game, "rb");

  if (file == NULL) {
    return errno;
  }

  struct stat st;
  stat(game, &st);
  size_t fsize = st.st_size;

  size_t bytes_read = fread(memory + 0x200, 1, sizeof(memory) - 0x200, file);

  fclose(file);

  if (bytes_read != fsize) {
    return -1;
  }
  return 0;
}
  
void emulateCycle() {
  drawFlag = 0;
  soundFlag = 0;
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
          V[x] = V[y];
          pc += 2;
          break;

        // 8XY1
        case 0x0001: 
          V[x] = V[x] | V[y];
          pc += 2;
          break;
        
        // 8XY2
        case 0x0002:
          V[x] = V[x] & V[y];
          pc += 2;
          break;
        
        // 8XY3
        case 0x0003:
          V[x] = V[x] ^ V[y];
          pc += 2;
          break;

        // 8XY4
        case 0x0004: 
          // Overflow
          if(V[x] + V[y] > 0xFF) {
            V[0xF] = 1; // carry
          }
          else {
            V[0xF] = 0;
          }
          V[x] += V[y];
          pc += 2;          
          break;
        
        // 8XY5
        case 0x0005: 
          //figure out "underflow"
          if(V[x] > V[y]) {
            V[0xF] = 0; // carry
          }
          else {
            V[0xF] = 1;
          }
          V[x] -= V[y];
          pc += 2;          
          break;
        
        // 8XY6
        case 0x0006:
          V[0xF] = V[x] & 0x1;
          V[x] = V[x] >> 1;

          pc += 2;
          break;

        // 8XY7
        case 0x0007:
          if (V[x] < V[y]) {
            V[0xF] = 0;
          }
          else {
            V[0xF] = 1;
          }

          V[x] = V[y] - V[x];
          pc += 2;
          break;

        // 8XYE (READ MORE ON THIS ONE)
        case 0x000E:
          V[0xF] = (V[x] >> 7) & 0x1;
          V[x] = V[x] << 1;
          pc += 2;
          break;
    }
      break;
    
    // 9XY0
    case 0x9000:
      if (V[x] != V[y]) {
        pc += 2;
      }
      pc += 2;
      break;
  
    // ANNN: Sets I to the address NNN
    case 0xA000: 
      // Execute opcode
      I = opcode & 0x0FFF;
      pc += 2;
      break;

    // BNNN
    case 0xB000:
      pc = (opcode & 0x0FFF) + V[0];
      break;

    // CXNN
    case 0xC000:
      srand(time(NULL));
      V[x] = (rand() % 256) & (opcode & 0x00FF);
      pc += 2;
      break;
    
    //DXYN
    case 0xD000: 
      unsigned short x = V[x];
      unsigned short y = V[y];
      unsigned short height = opcode & 0x000F;
      unsigned short pixel;

      V[0xF] = 0;
      for (int yline = 0; yline < height; yline++) {
        pixel = memory[I + yline];
        for (int xline = 0; xline < 0; xline++) {
          if ((pixel & (0x80 >> xline)) != 0) {
            if (gfx[(x + xline + ((y + yline) * 64))] == 1) {
              V[0xF] = 1;
            }
            gfx[x + xline + ((y + yline) * 64)] ^= 1;
          }
        }
      }

      drawFlag = 1;
      pc += 2;

    
      break;

    case 0xE000:
      switch(opcode & 0x00FF) {
        // EX9E
        case 0x009E:
          if (key[V[x]]) {
            pc += 2;
          }
          pc += 2;
        break;
      
        // EXA1
        case 0x00A1:
          if (!key[V[x]]) {
            pc += 2;
          }
          pc += 2;
          break;
  
      }
      break;
    
    case 0xF000:
      switch(opcode & 0x00FF){
        // FX07
        case 0x0007:
          V[x] = delay_timer;
          pc += 2;
          break;

        // FX0A
        case 0x000A:
          for (int i = 0; i < 16; i++) {
            if (key[i]) {
              V[x] = i;
              pc += 2;
              break;
            }
          }
          break;

        // FX15
        case 0x0015:
          delay_timer = V[x];
          pc += 2;
          break;

        // FX18
        case 0x0018:
          sound_timer = V[x];
          pc += 2;
          break;
        
        // FX1E
        case 0x001E:
          I += V[x];
          pc += 2;
          break;

        // FX29
        case 0x0029:
          I = V[x] * 5;
          pc += 2;
          break;

        // FX33
        case 0x0033: 
          memory[I] = V[x] / 100;
          memory[I + 1] = (V[x] % 100) / 10;
          memory[I + 2] = (V[x] % 10);
          pc += 2;
          break;
        
        // FX55
        case 0x0055:
          for (int i = 0; i <= x; i++) {
            memory[I + i] = V[i];
          }
          pc += 2;
          break;
          
        // FX65
        case 0x0065:
          for (int i = 0; i <= x; i++) {
            V[i] = memory[I + i];
          }
          pc += 2;
          break;
          }
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
      soundFlag = 1;
    }
    --sound_timer;
  }
}