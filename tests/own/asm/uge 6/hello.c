asm("li sp, 0x100000"); // SP set to 1 MB
asm("jal main");        // call main
asm("li a7, 10");       // prepare ecall exit
asm("ecall");           // now your simulator should stop
void prints(volatile char* ptr);

void main() {
  char* str = "Hello world";
  prints(str);
}

void prints(volatile char* ptr){ // ptr is passed through register a0
  asm("li a7, 4"); //You must decide the value of x
  asm("ecall");
}