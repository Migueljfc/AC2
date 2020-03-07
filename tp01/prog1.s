# int main(void)
# {
# 	printStr("AC2 – DETPIC32\n"); // system call
# 	return 0;
# } 
	.equ PRINT_STRING,8
	.data
msg:    .asciiz "AC2 - DETPIC32\n"
	.text
	.globl main
main:	la $a0,msg
	li $v0,PRINT_STRING
	syscall
	li $v0,0
	jr $ra
