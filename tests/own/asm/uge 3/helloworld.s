.data
hello:
.asciiz "Hello, world!"

.text
li a0, 11
la t0, hello

loop:
lbu a1, 0(t0)
beq a1, x0, exit
ecall
addi t0, t0, 1
j loop

exit: