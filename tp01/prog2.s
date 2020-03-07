#int main(void)
#{
# 	char c;
# 	do
# 	{
# 		c = getChar();
# 		if( c != '\n' )
# 		putChar( c );
# 	} while( c != '\n' );
# 	return 0;
# } 
	.equ getChar,2
	.equ putChar,3
	.text
	.globl main

main:	
do:	li $v0,getChar
	syscall
if: 	beq $v0,'\n',endif
	move $a0,$v0
	#addi $a0,$a0,-32
	la $v0,putChar
	syscall
endif:	bne $v0,'\n',do
	li $v0,0
	jr $ra
	
	
	
	
	
	
	
