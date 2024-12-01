.data
aa: .word 5
bb: .word 7

.text

j main

_add:
# int add(int x, int y) {
#   return x + y;
# }
add a0, a0, a1
jalr x0, x1, 0

main:
lw, a0, aa
lw, a1, bb
jal, x1, _add
