#include 
#include   // OpenGL graphics and input
#include "chip8.h" // Your cpu core implementation

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
  // Set up render system and register input callbacks
  setupGraphics();
  setupInput();
  
   // Initialize the Chip8 system and load the game into the memory  
  initialize();
  
  int error = loadGame("pong");
  if (error) {
    if (error == -1) {
      perror("FAILED fread()");
    }
    else {
      perror("Error loading rom/game");
    }
    return 1;
  }

   // Emulation loop
  for(;;) {
    // Emulate one cycle
    emulateCycle();
 
    // If the draw flag is set, update the screen
    if(drawFlag)
      drawGraphics();
 
    // Store key press state (Press and Release)
    setKeys();	
  }
  return 0;
}
