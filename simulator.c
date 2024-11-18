#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct Memory {
  size_t length;
  uint8_t buffer[];
};

struct RegisterFile {
  int PC;
  int GP[32];
};

struct Memory *iMem;

struct RegisterFile *rf;

int memRead(struct Memory *memory, size_t index) {
  if (index < memory->length) {
    return * (int *) &memory->buffer[index];
  }
}

void memWrite(struct Memory *memory, size_t index, int value) {
  if (index < memory->length) {
    * (int *) &memory->buffer[index] = value;
  }
}

int regRead(size_t index) {
  if (index < 32) {
    return rf->GP[index];
  }
}

void regWrite(size_t index, int value) {
  if (index < 32) {
    rf->GP[index] = value;
  }
}

void initIMem(char *fileName) {
  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);
  iMem = (struct Memory*) malloc(sizeof(struct Memory) + length);
  iMem->length = length;
  int i = fread(iMem->buffer, 1, length, file);
  if (i != length) {
    perror("Error reading file");
    exit(1);
  }
}

void printRegisterFile() {
  printf("\nRegister Content:\n PC=0x%08x\n", rf->PC);
  for (int i = 0; i < 4; i ++) {
    for (int j = 0; j < 16; j += 4) {
      int n = i + j;
      printf("%sx%d=0x%08x   ", n < 10 ? " " : "", n, regRead(n));
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("please input file\n");
    return 1;
  }

  initIMem(argv[1]);
  rf = (struct RegisterFile *) malloc(sizeof(struct RegisterFile));

  for (rf->PC = 0; rf->PC < iMem->length; rf->PC += 4) {
    printf("%4d: 0x%08x\n", rf->PC, memRead(iMem, rf->PC));
  }

  printRegisterFile();

  return 0;
}
