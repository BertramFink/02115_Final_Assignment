  .data
hello:
  .string "Hello, world!\n"

  .text
  li a7, 4
  la a0, hello
  ecall
  li a7, 10
  ecall
