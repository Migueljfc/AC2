#int main(void)
#{
#	char c;
#	do
#	{
#		while( (c = inkey()) == 0 );
#		if( c != '\n' )
#			putChar( c );
#	} while( c != '\n' );
#	 return 0;
# } 

		.equ putChar,3
		.equ inkey,1
		.data
		.text
		.globl main
main:
do:		li $v0,inkey
		syscall
while:	bne $v0,0,if
if:		beq $v0,'\n',endW
		move $a0,$v0
		li $v0,putChar
		syscall
endW:	bne $a0,'\n',do
		li $v0,0
		jr $ra
	
	
	
	
