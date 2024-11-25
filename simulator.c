#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

struct Memory {
  size_t length;
  uint8_t buffer[];
};

struct RegisterFile {
  int PC;
  int GP[32];
};

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
  int rd;
  int funct3;
  int rs1;
  int rs2;
  int imm;
};
struct Btype
{
  int rd;
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

struct Instruction
{
  int opcode;
  int opcodeInstruction[7];
  int instruction[32];
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
  if (index == 0) {
    return 0;
  }
  if (index < 32) {
    return rf->GP[index];
  }
}

void regWrite(size_t index, int value) {
  if (index > 0 && index < 32) {
    rf->GP[index] = value;
  }
}

int bits(int source, int high, int low) {
  int width = high - low + 1;
  int mask = (1 << width) - 1;
  return (source >> low) & mask;
}

int bit(int source, int index) {
  return (source >> index) & 1;
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
  for (int j = 0; j < 32; j += 4) {
    for (int i = 0; i < 4; i ++) { 
      int n = i + j;
      printf("%sx%d=0x%08x   ", n < 10 ? " " : "", n, regRead(n));
    }
    printf("\n");
  }
}

struct Instruction getInstruction(struct Memory* mem, int pc){
  struct Instruction current;
  int num = mem->buffer[0+pc]+(mem->buffer[1+4*pc]<<8)+(mem->buffer[2+4*pc]<<16)+(mem->buffer[3+4*pc]<<24);
  for(int i = 0;i<=31;i++){
    current.instruction[i] = (num >> i) & 1;
  }
  for(int i = 0 ;i<=6;i++){
    current.opcodeInstruction[i] = (num >> i) & 1;
  }
  return current;
} 

void printInstruction(struct Instruction current){
  printf("Full instruction:\n");
  for(int i = 31; i >= 0 ;i--) {
     printf("%d",current.instruction[i]);
  }
  printf("\nOpcode: \n");
  for(int i = 6; i >= 0 ;i--) {
     printf("%d",current.opcodeInstruction[i]);
  }
  printf("\n");
}

int binToDec(int bin[],int size){
  int sum = 0;
  for(int i = 0; i<size;i++){
    sum += bin[i]*pow(2,i);
  }
  return sum;
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
  parsed.imm = bits(instruction,30,20) - (bit(instruction,31) << 11);
  return parsed;
}
struct Stype parseStype(int instruction){
  struct Stype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.funct3 = bits(instruction,14,12);
  parsed.rs1 = bits(instruction,19,15);
  parsed.rs2 = bits(instruction,24,20);
  parsed.imm = bits(instruction,31,25);
  return parsed;
}
struct Btype parseBtype(int instruction){
  struct Btype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.funct3 = bits(instruction,14,12);
  parsed.rs1 = bits(instruction,19,15);
  parsed.rs2 = bits(instruction,24,20);
  parsed.imm = bits(instruction,31,25);
  return parsed;
}
struct Utype parseUtype(int instruction){
  struct Utype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.imm = bits(instruction,31,12);
  return parsed;
}
struct Jtype parseJtype(int instruction){
  struct Jtype parsed;
  parsed.rd = bits(instruction,11,7);
  parsed.imm = bits(instruction,31,12);
  return parsed;
}

int ALU(int funct3, int funct7, int operand1, int operand2) {
  switch (funct3) {
    // add / sub
    case 0b000: return bit(funct7, 5) ? operand1 - operand2 : operand1 + operand2;
    // sll (shift left)
    case 0b001: return operand1 << operand2;
    // slt (set less than)
    case 0b010: return (operand1 < operand2) ? 1 : 0;
    // sltu (set less than unsigned)
    case 0b011: return ((unsigned int) operand1 < (unsigned int) operand2) ? 1 : 0;
    // xor
    case 0b100: return operand1 ^ operand2;
    // srl / sra
    case 0b101: return bit(funct7, 5) ? operand1 >> operand2 : (unsigned int) operand1 >> (unsigned int) operand2;
    // or
    case 0b110: return operand1 | operand2;
    // and
    case 0b111: return operand1 & operand2;
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

}

void executeJALR(struct Itype instruction) {

}

void executeEcall(struct Itype instruction) {
  int a7 = regRead(17);
  switch (a7) {
    case 10: exit(0); 
    default: printf("Unkown Ecall: a7=%d\n", a7); exit(1);
  }
}

void executeStore(struct Stype instruction) {

}

void executeBranch(struct Btype instruction) {

}

void executeAUIPC(struct Utype instruction) {

}

void executeLUI(struct Utype instruction) {

}

void executeJAL(struct Jtype instruction) {

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
    // case 105: break; Eksisterer ikke
    // case 23: break; Eksisterer ikke
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

  initIMem(argv[1]);
  rf = (struct RegisterFile *) calloc(sizeof(struct RegisterFile), 1);
  rf->PC = 0;

  atexit(printRegisterFile);

  while (rf->PC < iMem->length) {
    int instruction = memRead(iMem, rf->PC);
    printf("%4d: 0x%08x\n", rf->PC, instruction);

    execute(instruction);

    rf->PC += 4;
  }

  printf("Reached end of file\n");
  
  return 0;
}
