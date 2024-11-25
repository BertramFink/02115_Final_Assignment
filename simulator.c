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


struct Instruction getInstruction(struct Memory* mem, int Line){
  struct Instruction current;
  int num = mem->buffer[0+4*Line]+(mem->buffer[1+4*Line]<<8)+(mem->buffer[2+4*Line]<<16)+(mem->buffer[3+4*Line]<<24);
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

int OpcodeToDec(int bin[]){
  int sum = 0;
  int size = 6;
  int j = 0;
  for(int i = size; i>=0;i--){
    sum += bin[i]*pow(2,size-j);
    j++;
  }
  return sum;
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
  printf("%08x\n",memRead(iMem,0));


  struct Instruction current;
  current =  getInstruction(iMem,2);
  printInstruction(current);
  current.opcode = OpcodeToDec(current.opcodeInstruction);
  printf("\n%d\n",current.opcode);
  
  if (current.opcode == 51){
    //Opcode R
    int rd[5];
    for(int i = 7 ;i<=11;i++){
      rd[i] = current.instruction[i];
    }
    int fucnt3[3];
    for(int i = 12 ;i<=14;i++){
      rd[i] = current.instruction[i];
    }
    int rs1[5];
    for(int i = 15 ;i<=19;i++){
      rd[i] = current.instruction[i];
    }
    int rs2[5];
    for(int i = 20 ;i<=24;i++){
      rd[i] = current.instruction[i];
    }
    int funct7[7];
    for(int i = 25 ;i<=31;i++){
      rd[i] = current.instruction[i];
    }
    printf("rd:%d ass %d",rd[0],current.instruction[31]);
  }
  else if (current.opcode == 19 || current.opcode == 3 || current.opcode == 103 || current.opcode ==105 || current.opcode == 23){
    //opcode I
    int rd[5];
    int fucnt3[3];
    int rs1[5];
    int imm[12];
  }
  else if (current.opcode == 35){
    //opcode S
    int rd[5];
    int fucnt3[3];
    int rs1[5];
    int rs2[5];
    int imm[7];
  }
  else if(current.opcode == 99){
    //opcode B
    int rd[5];
    int fucnt3[3];
    int rs1[5];
    int rs2[5];
    int imm[7];
  }
  else if (current.opcode == 55){
    //upcode U
    int rd[5];
    int imm[20];
  }
  else if (current.opcode == 111){
    //upcode J
    int rd[5];
    int imm[20];
  }



  // switch (current.opcode){
  //   case 51:
  //     printf("opcode - R: ");
  //     break;
  //   case 19 :
  //     printf("opcode - I: ");
  //     break;
  //   case 3 : 
  //     printf("opcode - I: ");
  //     break;
  //   case 103: 
  //     printf("opcode - I: ");
  //     break;
  //   case 105 : 
  //     printf("opcode - I: ");
  //     break;
  //   case 35:
  //     printf("opcode - S: ");
  //     break;
  //   case 99:
  //     printf("opcode - B: ");
  //     break;
  //   case 55:
  //     printf("opcode - U: ");
  //     break;
  //   case 23: 
  //     printf("opcode - I: ");
  //     break;
  //   case 111:
  //     printf("opcode - J: ");
  //     break;
  // }

  return 0;
}
