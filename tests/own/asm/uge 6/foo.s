	.file	"foo.c"
	.option nopic
	.attribute arch, "rv32i2p1"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	foo
	.type	foo, @function
foo:
	add	a0,a0,a1
	ret
	.size	foo, .-foo
	.ident	"GCC: (13.2.0-11ubuntu1+12) 13.2.0"
