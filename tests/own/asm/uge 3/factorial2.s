.data
n: .word 10

.text
main:
la t0, n
lw a0, 0(t0)
jal ra, factorial
   
addi a1, a0, 0
addi a0, x0, 1
ecall # print pesult

addi a0, x0, 10
ecall # exit

factorial:
# int fact (int n)
# { 
#   if (n < 1) return 1;
#   else return n * fact(n - 1);
# }
stack:
addi sp, sp, -8
sw ra, 0(sp)
sw a0, 4(sp)
edge_case:
li t0, 1
bgeu a0, t0, standard_case
li a0, 1
j return
standard_case:
addi a0, a0, -1
jal ra, factorial
lw t0, 4(sp)
mul a0, a0, t0
return:
lw ra, 0(sp)
addi sp, sp, 8
jalr x0, ra, 0
