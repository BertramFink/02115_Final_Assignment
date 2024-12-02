	.text
# print_int
	li a0, 42
	li a7, 1
	ecall

# print_float
	li a0, 0x40490FDB
	li a7, 2
	ecall

# print_char
	li a0, 'A'
	li a7, 11
	ecall

# print_hex
	li a0, 0xDEADBEEF
	li a7, 34
	ecall

# print_bin
	li a0, 1234
	li a7, 35
	ecall

# print_unsigned
	li a0, 4294967295
	li a7, 36
	ecall

# status_code
	li a0, 0
	li a7, 93
	ecall
