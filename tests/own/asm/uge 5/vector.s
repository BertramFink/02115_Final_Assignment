test:
li t0, 0x100000         # location of vector
li t1, 15
sw t1, 0(t0)
li t1, -5
sw t1, 4(t0)

main:
li t0, 0x100000         # location of vector
addi t1, t0, 200        # end of vector
li a0, 0                # running sum

loop:
lw t2, 0(t0)            # value at ptr

bge t2, zero, skipMult  # skip multiplication if positive
add t2, t2, t2          # multiply t2 by 2
sw t2, 0(t0)
skipMult:
add a0, a0, t2          # add value to sum

addi t0, t0, 4          # ptr = ptr + 4
blt t0, t1, loop        # continue loop if index < length
