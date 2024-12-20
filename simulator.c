#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

struct Block {
  uint8_t buffer[1 << 16];
};

struct Memory {
  struct Block *blocks[1 << 16];
} memory;

struct RegisterFile {
  int PC;
  int X[32];
} registerFile;

struct ResultsFile {
  int valid;
  int X[32];
} resultsFile;

struct Rtype
{
  int rd;
  int funct3;
  int rs1;
  int rs2;
  int funct7;
};

struct Itype
{
  int rd;
  int funct3;
  int rs1;
  int imm;
};

struct Stype
{
  int funct3;
  int rs1;
  int rs2;
  int imm;
};

struct Btype
{
  int funct3;
  int rs1;
  int rs2;
  int imm;
};

struct Utype
{
  int rd;
  int imm;
};

struct Jtype
{
  int rd;
  int imm;
};

uint32_t memReadWord(uint32_t address) {
  int addr_hi = address >> 16;
  int addr_lo = address & 0xffff;
  struct Block *block = memory.blocks[addr_hi];
  if (block == NULL) {
    return 0;
  }
  return * (uint32_t *) &block->buffer[addr_lo];
}

uint8_t *getBufferAddress(uint32_t address) {
  int addr_hi = address >> 16;
  int addr_lo = address & 0xffff;
  struct Block *block = memory.blocks[addr_hi];
  if (block == NULL) {
    block = calloc(1, sizeof(struct Block));
    memory.blocks[addr_hi] = block;
  }
  return &block->buffer[addr_lo];
}

void memWriteWord(uint32_t address, uint32_t value) {
  * (uint32_t *) getBufferAddress(address) = value;
}

void memWriteHalfWord(uint32_t address, uint16_t value) {
  * (uint16_t *) getBufferAddress(address) = value;
}

void memWriteByte(uint32_t address, uint8_t value) {
  * (uint8_t *) getBufferAddress(address) = value;
}

int regRead(uint32_t index) {
  if (index == 0) {
    return 0;
  }
  if (index < 32) {
    return registerFile.X[index];
  }
}

void regWrite(uint32_t index, int value) {
  if (index > 0 && index < 32) {
    registerFile.X[index] = value;
  }
}

int  bits(int source, int high, int low) {
  int width = high - low + 1;
  int mask = (1L << width) - 1;
  return (source >> low) & mask;
}

int bit(int source, int index) {
  return (source >> index) & 1;
}

int sbits(int source, int high, int low) {
  return bits(source, high - 1, low) - (bit(source, high) << (high - low));
}

void initInstMem(char *fileName) {
  FILE *file = fopen(fileName, "rb");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);
  int *buffer = (int *) malloc(length);
  int i = fread(buffer, 1, length, file);
  if (i != length) {
    perror("Error reading file");
    exit(1);
  }
  for (i = 0; i < length; i += 4) {
    memWriteWord(i, buffer[i/4]);
  }
}

void prepareResults(char *fileName) {
  resultsFile.valid = 0;
  char *point = strrchr(fileName, '.');
  if (point == NULL) {
    printf("[WARNING] No results file found\n\n");
    return;
  }
  *point = '\0';
  char resultFileName[100];
  sprintf(resultFileName, "%s%s", fileName, ".res");
  FILE *file = fopen(resultFileName, "rb");
  if (file == NULL) {
    printf("[WARNING] No results file found\n\n");
    return;
  }
  int i = fread(resultsFile.X, 1, sizeof(resultsFile.X), file);
  if (i != sizeof(resultsFile.X)) {
    perror("Error reading results file");
    exit(1);
  }
  resultsFile.valid = 1;
}

void printRegisterFile() {
  printf("\nRegister Content:\n PC=0x%08x\n", registerFile.PC);
  for (int j = 0; j < 32; j += 4) {
    for (int i = 0; i < 4; i ++) { 
      int n = i + j;
      printf("%sx%d=0x%08x   ", n < 10 ? " " : "", n, regRead(n));
    }
    printf("\n");
  }
}

void saveRegisterFile() {
  FILE *file = fopen("output.res", "wb");
  if (file == NULL) {
    perror("Error opening output.res file");
    exit(1);
  }
  int i = fwrite(registerFile.X, 1, sizeof(registerFile.X), file);
  if (i != sizeof(registerFile.X)) {
    perror("Error writing to output.res");
    exit(1);
  }
}

void successfullExit() {
  if (!resultsFile.valid) {
    exit(0);
  }
  if (memcmp(registerFile.X, resultsFile.X, sizeof(resultsFile.X)) == 0) {
    printf("\nRegisterfile is identical to expected result\n");
    exit(0);
  } else {
    printf("\nRegisterfile is DIFFERENT from expected result\nExpected:\n");
    for (int j = 0; j < 32; j += 4) {
      for (int i = 0; i < 4; i ++) { 
        int n = i + j;
        printf("%sx%d=0x%08x   ", n < 10 ? " " : "", n, resultsFile.X[n]);
      }
      printf("\n");
    }
    exit(1);
  }
}

struct Rtype parseRtype(int instruction){
  struct Rtype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.funct3 = bits(instruction,14,12);
  parsed.rs1 = bits(instruction,19,15);
  parsed.rs2 = bits(instruction,24,20);
  parsed.funct7 = bits(instruction,31,25);
  return parsed;
}

struct Itype parseItype(int instruction){
  struct Itype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.funct3 = bits(instruction,14,12);
  parsed.rs1 = bits(instruction,19,15);
  parsed.imm = sbits(instruction, 31, 20);
  return parsed;
}

struct Stype parseStype(int instruction){
  struct Stype parsed;
  parsed.funct3 = bits(instruction,14,12);
  parsed.rs1 = bits(instruction,19,15);
  parsed.rs2 = bits(instruction,24,20);
  parsed.imm = (sbits(instruction,31,25) << 5) | (bits(instruction, 11, 7));
  return parsed;
}

struct Btype parseBtype(int instruction){
  struct Btype parsed;
  parsed.funct3 = bits(instruction,14,12);
  parsed.rs1 = bits(instruction,19,15);
  parsed.rs2 = bits(instruction,24,20);
  parsed.imm = -(bit(instruction, 31) << 12) | (bit(instruction, 7) << 11) | (bits(instruction, 30, 25) << 5) | (bits(instruction, 11, 8) << 1);
  return parsed;
}

struct Utype parseUtype(int instruction){
  struct Utype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.imm = sbits(instruction,31,12) << 12;
  return parsed;
}

struct Jtype parseJtype(int instruction){
  struct Jtype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.imm = -(bit(instruction, 31) << 20) | (bits(instruction, 19, 12) << 12) | (bit(instruction, 20) << 11) | (bits(instruction, 30, 21) << 1);
  return parsed;
}

int ALU(int funct3, int funct7, int operand1, int operand2) {
  switch (funct3) {
    case 0b000: // add / sub
      return bit(funct7, 5) ? operand1 - operand2 : operand1 + operand2;
    case 0b001: // sll (shift left)
      return operand1 << bits(operand2, 5, 0);
    case 0b010: // slt (set less than)
      return (operand1 < operand2) ? 1 : 0;
    case 0b011: // sltu (set less than unsigned)
      return ((unsigned int) operand1 < (unsigned int) operand2) ? 1 : 0;
    case 0b100: // xor
      return operand1 ^ operand2;
    case 0b101: // srl / sra
      return bit(funct7, 5) ? operand1 >> bits(operand2, 5, 0) : (unsigned int) operand1 >> bits(operand2, 5, 0);
    case 0b110: // or
      return operand1 | operand2;
    case 0b111: // and
      return operand1 & operand2;
  }
}

void executeMathReg(struct Rtype instruction) {
  int operand1 = regRead(instruction.rs1);
  int operand2 = regRead(instruction.rs2);
  int result = ALU(instruction.funct3, instruction.funct7, operand1, operand2);
  regWrite(instruction.rd, result);
}

void executeMathImm(struct Itype instruction) {
  int operand1 = regRead(instruction.rs1);
  int operand2 = instruction.imm;
  // In case of srli / srai instruction:
  int funct7 = instruction.funct3 == 0b101 ? bits(instruction.imm, 11, 5) : 0;
  int result = ALU(instruction.funct3, funct7, operand1, operand2);
  regWrite(instruction.rd, result);
}

void executeLoad(struct Itype instruction) {
  int address = regRead(instruction.rs1)+instruction.imm;
  int result = memReadWord(address);
  switch(instruction.funct3){
    case 0b000: // lb
      result = sbits(result,7,0); break;
    case 0b001: // lh
      result = sbits(result,15,0); break;
    case 0b010: // lw
      result = bits(result,31,0); break;
    case 0b100: // lbu
      result = bits(result,7,0); break;
    case 0b101: // lhu
      result = bits(result,15,0); break;
    default: printf("Unkown Load funct3=%d\n", instruction.funct3); exit(1);
  }
  regWrite(instruction.rd, result);
}

void executeStore(struct Stype instruction) {
  int address = regRead(instruction.rs1)+instruction.imm;
  int data = regRead(instruction.rs2);
  switch(instruction.funct3){
    case 0b000: // sb
      return memWriteByte(address, data);
    case 0b001: // sh
      return memWriteHalfWord(address, data);
    case 0b010: // sw
      return memWriteWord(address, data);
    default: printf("Unkown Store funct3=%d\n", instruction.funct3); exit(1);
  } 
}

void executeBranch(struct Btype instruction) {
  int value1 = regRead(instruction.rs1);
  int value2 = regRead(instruction.rs2);
  int imm = instruction.imm;
  switch (instruction.funct3) {
    case 0b000: // BEQ
      if (value1 == value2) registerFile.PC += imm;
      break;
    case 0b001: // BNE
      if (value1 != value2) registerFile.PC += imm;
      break;
    case 0b100: // BLT
      if (value1 < value2) registerFile.PC += imm;
      break;
    case 0b101: // BGE
      if (value1 >= value2) registerFile.PC += imm;
      break;
    case 0b110: // BLTU
      if ((unsigned int)value1 < (unsigned int)value2) registerFile.PC += imm;
      break;
    case 0b111: // BGEU
      if ((unsigned int)value1 >= (unsigned int)value2) registerFile.PC += imm;
      break;
    default:
      printf("Unknown branch funct3: 0x%02x\n", instruction.funct3);
      exit(1);
  }
}

void executeAUIPC(struct Utype instruction) {
  regWrite(instruction.rd, registerFile.PC + instruction.imm);
}

void executeLUI(struct Utype instruction) {
  regWrite(instruction.rd, instruction.imm);
}

void executeJAL(struct Jtype instruction) {
  regWrite(instruction.rd, registerFile.PC + 4);
  registerFile.PC += instruction.imm;
}

void executeJALR(struct Itype instruction) {
  regWrite(instruction.rd, registerFile.PC + 4);
  registerFile.PC = regRead(instruction.rs1) + instruction.imm;
}

void executeEcall(struct Itype instruction) {
  int a7 = regRead(17);
  int a0 = regRead(10);
  switch (a7) {
    case 1:
      printf("%d", a0);
      break;
    case 2:
      printf("%f", * (float *) &a0);
      break;
    case 4:
      printf("%s", (char *) getBufferAddress(a0));
      break;
    case 10: 
      successfullExit(); 
    case 11:
      printf("%c", (char) a0);
      break;
    case 34:
      printf("%x", a0);
      break;
    case 35:
      for (int i = 31; i >= 0; i --) {
        printf("%d", (a0 >> i) & 1);
      }
      break;
    case 36:
      printf("%u", a0);
      break;
    case 93:
      printf("Program exited with status code: %d\n", a0);
      successfullExit();
    default: 
      printf("Unkown Ecall: a7=%d\n", a7); 
      exit(1);
  }
}

void execute(int instruction){
  int opcode = bits(instruction,6,0);
  switch (opcode) {
    // R Type:
    case 0b0110011: return executeMathReg(parseRtype(instruction));
    // I Type:
    case 0b0000011: return executeLoad(parseItype(instruction));
    case 0b0010011: return executeMathImm(parseItype(instruction));
    case 0b1100111: return executeJALR(parseItype(instruction));
    case 0b1110011: return executeEcall(parseItype(instruction));
    // S Type:
    case 0b0100011: return executeStore(parseStype(instruction));
    // B Type:
    case 0b1100011: return executeBranch(parseBtype(instruction));
    // U Type:
    case 0b0010111: return executeAUIPC(parseUtype(instruction));
    case 0b0110111: return executeLUI(parseUtype(instruction));
    // J Type:
    case 0b1101111: return executeJAL(parseJtype(instruction));
    
    default: printf("Illegal opcode: 0x%02x of instruction: 0x%08x\n", opcode, instruction); exit(1);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("please input file\n");
    return 1;
  }

  initInstMem(argv[1]);
  prepareResults(argv[1]);
  registerFile.PC = 0;

  atexit(printRegisterFile);
  atexit(saveRegisterFile);

  int i = 0;
  while (1) {
    int PC = registerFile.PC;
    int instruction = memReadWord(PC);
    printf("%4x: 0x%08x\n", PC, instruction);
    execute(instruction);
    if (PC == registerFile.PC) {
      registerFile.PC += 4;
    }

    if (++i > 100000) {
      printf("Maximum clock cycles reached!\n");
      exit(1);
    }
  }
  return 0;
}
