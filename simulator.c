#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct Memory {
  int length;
  uint8_t buffer[];
};

struct Memory *instructionMemory;

void initInstructionMemory(char *fileName) {
  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);
  instructionMemory = (struct Memory*) malloc(sizeof(struct Memory) + length);
  instructionMemory->length = length;
  int i = fread(instructionMemory->buffer, 1, length, file);
  if (i != length) {
    perror("Error reading file");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("please input file\n");
    return 1;
  }

  initInstructionMemory(argv[1]);

  for (int i = 0; i < instructionMemory->length; i ++) {
    printf("%4d: 0x%08x\n", i, instructionMemory->buffer[i]);
  }

  return 0;
}
