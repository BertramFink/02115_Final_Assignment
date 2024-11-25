//R-TYPE
void add (rd, rs1, rs2 ){
    rd = rs1 + rs2;
}

void sub (rd, rs1,rs2){
    rd = rs1 - rs2 ;
}

void xor (rd, rs1,rs2){
    rd = rs1 ^ rs2;
}

int or (rd, rs1,rs2){
    rd = rs1 | rs2;
}

void and (rd, rs1,rs2){
    rd = rs1 & rs2;
}

void sll (rd, rs1,rs2){
    rd = rs1 << rs2;
}

void srl (rd, rs1,rs2){
    rd = rs1 >> rs2;
}

void sra (rd, rs1,rs2){

}

void sll (rd, rs1,rs2){
    rd = rs1 << rs2;
}

void slt (rd, rs1,rs2){
    rd = (rs1 < rs2)?1:0;
}

void sltu (rd, rs1,rs2){
    
}
//I-TYPE
void addi (rd, rs1, imm){
    rd = rs1 + imm;
}

void xori (rd, rs1, imm){

}

void ori (rd, rs1, imm){

}

void andi (rd, rs1, imm){

}

void slli (rd, rs1, imm){

}

void srli (rd, rs1, imm){

}

void srai (rd, rs1, imm){

}

void slti (rd, rs1, imm){

}

void sltiu (rd, rs1, imm){

}
//LOAD / STORE

void lb (rd, rs1, imm){

}

void lh (rd, rs1, imm){

}

void lw (rd, rs1, imm){

}

void lbu (rd, rs1, imm){

}

void lhu (rd, rs1, imm){

}
// S-TYPE
void sb (rd, rs1, imm){

}

void sh (rd, rs1, imm){

}

void sw (rd, rs1, imm){

}
//B-TYPE

void beq (rd, rs1, rs2, PC, imm){
if(rs1 == rs2) PC += imm;
}

void bne (rd, rs1, rs2, PC, imm){
if(rs1 != rs2) PC += imm;
}

void blt (rd, rs1, rs2, PC, imm){
if(rs1 < rs2) PC += imm;
}

void bge (rd, rs1, rs2, PC, imm){
if(rs1 >= rs2) PC += imm;
}

void bltu (rd, rs1, rs2, PC, imm){

}

void bgeu (rd, rs1, rs2, PC, imm){

}

//jal shizz
void jal (rd, PC, imm){
rd = PC+4;
PC += imm;
}

void jal (rd, rs1, PC, imm){
rd = PC+4;
PC = rs1 + imm;
}