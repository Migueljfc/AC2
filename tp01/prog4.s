#int main(void)
# {
 #int value;
 #  while(1)
 #  {
 #      printStr("\nIntroduza um numero (sinal e módulo): ");
 #      value = readInt10();
 #      printStr("\nValor lido, em base 2: ");
 #      printInt(value, 2);
 #      printStr("\nValor lido, em base 16: ");
 #      printInt(value, 16);
 #      printStr("\nValor lido, em base 10 (unsigned): ");
 #      printInt(value, 10);
 #      printStr("\nValor lido, em base 10 (signed): ");
 #      printInt10(value);
 #  }
 #  return 0;
 #}

        .equ printStr,8
        .equ printInt,6
        .equ printInt10,7
        .equ readInt10,5
        .data 
msg1:   .asciiz "\nIntroduza um numero (sinal e módulo): "
msg2:   .asciiz "\nValor lido, em base 2: "
msg3:   .asciiz "\nValor lido, em base 16: "
msg4:   .asciiz "\nValor lido, em base 10 (unsigned): "
msg5:   .asciiz "\nValor lido, em base 10 (signed): "
        .text
        .globl main
main:   bne $0,0,endw
        la $a0,msg1
        li $v0,printStr
        syscall
        li $v0,readInt10
        syscall
        move $t0,$v0
        la $a0,msg2
        li $v0,printStr
        syscall
        move $a0,$t0
        li $a1,2
        li $v0,printInt
        syscall
        la $a0,msg3
        li $v0,printStr
        syscall
        move $a0,$t0
        li $a1,16
        li $v0,printInt
        syscall
        la $a0,msg4
        li $v0,printStr
        syscall
        move $a0,$t0
        li $a1,10
        li $v0,printInt
        syscall
        la $a0,msg5
        li $v0,printStr
        syscall
        move $a0,$t0
        li $v0,printInt10
        syscall
        j main
endw:   jr $ra
