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
  int opcode;
  int rd;
  int fucnt3;
  int rs1;
  int rs2;
  int funct7;
};
struct Itype
{
  int opcode;
  int rd;
  int fucnt3;
  int rs1;
  int imm;
};
struct Stype
{
  int opcode;
  int rd;
  int fucnt3;
  int rs1;
  int rs2;
  int imm;
};
struct Btype
{
  int opcode;
  int rd;
  int fucnt3;
  int rs1;
  int rs2;
  int imm;
};
struct Utype
{
  int opcode;
  int rd;
  int imm;
};
struct Jtype
{
  int opcode;
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
  if (index < 32) {
    return rf->GP[index];
  }
}

void regWrite(size_t index, int value) {
  if (index < 32) {
    rf->GP[index] = value;
  }
}

int bits(int source, int high, int low) {
  int width = high - low + 1;
  int mask = (1 << width) - 1;
  return (source >> low) & mask; 
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

struct Instruction parsing (struct Memory* mem, int pc){
  struct Instruction current;
  current = getInstruction(mem,pc);
  printInstruction(current);
  current.opcode = binToDec(current.opcodeInstruction,7);
  printf("\n%d\n",current.opcode);
  if (current.opcode == 51){
  //Opcode R
  }
  else if (current.opcode == 19 || current.opcode == 3 || current.opcode == 103 || current.opcode ==105 || current.opcode == 23){
  //opcode I
  }
  else if (current.opcode == 35){
  //opcode S
  }
  else if(current.opcode == 99){
  //opcode B
  }
  else if (current.opcode == 55){
  //upcode U
  }
  else if (current.opcode == 111){
  //upcode J
  }
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("please input file\n");
    return 1;
  }

  initIMem(argv[1]);
  rf = (struct RegisterFile *) malloc(sizeof(struct RegisterFile));
  rf->PC = 0;

  while (rf->PC < iMem->length) {
    int instruction = memRead(iMem, rf->PC);
    printf("%4d: 0x%08x\n", rf->PC, instruction);
    rf->PC += 4;
  }

  printRegisterFile();
  printf("%08x\n",memRead(iMem,0));

  rf->PC = 0;
  parsing(iMem,rf->PC);
  
  // if (current.opcode == 51){
  //   //Opcode R
  //   int rd[5];
  //   for(int i = 7 ;i<=11;i++){
  //     rd[i-7] = current.instruction[i];
  //   }
  //   int fucnt3[3];
  //   for(int i = 12 ;i<=14;i++){
  //     fucnt3[i-12] = current.instruction[i];
  //   }
  //   int rs1[5];
  //   for(int i = 15 ;i<=19;i++){
  //     rs1[i-15] = current.instruction[i];
  //   }
  //   int rs2[5];
  //   for(int i = 20 ;i<=24;i++){
  //     rs2[i-20] = current.instruction[i];
  //   }
  //   int funct7[7];
  //   for(int i = 25 ;i<=31;i++){
  //     funct7[i-25] = current.instruction[i];
  //   }
  // }
  // else if (current.opcode == 19 || current.opcode == 3 || current.opcode == 103 || current.opcode ==105 || current.opcode == 23){
  //   //opcode I
  //   int rd[5];
  //   for(int i = 7 ;i<=11;i++){
  //     rd[i-7] = current.instruction[i];
  //   }
  //   int funct3[3];
  //   for(int i = 12 ;i<=14;i++){
  //     funct3[i-12] = current.instruction[i];
  //   }
  //   int rs1[5];
  //   for(int i = 15 ;i<=19;i++){
  //     rs1[i-15] = current.instruction[i];
  //   }
  //   int imm[12];
  //   for(int i = 20 ;i<=31;i++){
  //     imm[i-20] = current.instruction[i];
  //   }
  // }
  // else if (current.opcode == 35){
  //   //opcode S
  //   int rd[5];
  //   for(int i = 7 ;i<=11;i++){
  //     rd[i-7] = current.instruction[i];
  //   }
  //   int funct3[3];
  //   for(int i = 12 ;i<=14;i++){
  //     funct3[i-12] = current.instruction[i];
  //   }
  //   int rs1[5];
  //   for(int i = 15 ;i<=19;i++){
  //     rs1[i-15] = current.instruction[i];
  //   }
  //   int rs2[5];
  //   for(int i = 20 ;i<=24;i++){
  //     rs2[i-20] = current.instruction[i];
  //   }
  //   int imm[7];
  //   for(int i = 25 ;i<=31;i++){
  //     imm[i-25] = current.instruction[i];
  //   }
  // }
  // else if(current.opcode == 99){
  //   //opcode B
  //   int rd[5];
  //   for(int i = 7 ;i<=11;i++){
  //     rd[i-7] = current.instruction[i];
  //   }
  //   int funct3[3];
  //   for(int i = 12 ;i<=14;i++){
  //     funct3[i-12] = current.instruction[i];
  //   }
  //   int rs1[5];
  //   for(int i = 15 ;i<=19;i++){
  //     rs1[i-15] = current.instruction[i];
  //   }
  //   int rs2[5];
  //   for(int i = 20 ;i<=24;i++){
  //     rs2[i-20] = current.instruction[i];
  //   }
  //   int imm[7];
  //   for(int i = 25 ;i<=31;i++){
  //     imm[i-25] = current.instruction[i];
  //   }
  // }
  // else if (current.opcode == 55){
  //   //upcode U
  //   int rd[5];
  //   for(int i = 7 ;i<=11;i++){
  //     rd[i-7] = current.instruction[i];
  //   }
  //   int imm[20];
  //   for(int i = 12 ;i<=31;i++){
  //     imm[i-12] = current.instruction[i];
  //   }
  // }
  // else if (current.opcode == 111){
  //   //upcode J
  //   int rd[5];
  //   for(int i = 7 ;i<=11;i++){
  //     rd[i-7] = current.instruction[i];
  //   }
  //   int imm[20];
  //   for(int i = 12 ;i<=31;i++){
  //     imm[i-12] = current.instruction[i];
  //   }
  // }

  

  



  return 0;
}
