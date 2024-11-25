void run_instructions{
    //lav if statements som læser opcode og funct3 og funct7
    //kør add osv funcs i if statements
}
void add (rd, rs1, rs2 ){
    rd = decToBin(binToDec(rs1) + binToDec(rs2));
}

void sub (rd, rs1,rs2){
    rd = decToBin(binToDec(rs1) - binToDec(rs2));
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
char* decToBin(int n) {
    static char bin[33];
    bin[32] = '\0';
    for (int i = 31; i >= 0; i--) {
        bin[i] = (n % 2) ? '1' : '0';
        n /= 2;
    }
    return bin;
}