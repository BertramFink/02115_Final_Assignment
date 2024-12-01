li x4, 2048

lui t0, 0x80000     # a positive number has 0 as its most significant bit
addi t0, t0, 0b1111 # a number divisible by 16 has 0000 as its least significant bits
and t0, x4, t0      # mask out only the bits we are interested in
bne t0, zero, else  # check if they are all zero

li x1, 0            # common case
srai x2, x4, 4      # shifting right by 4 is the same as dividing by 16 for positive numbers
j exit

else: 
li x1, 1            # else case
li x2, 0

exit: