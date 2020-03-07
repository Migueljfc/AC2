        .equ putChar,3
        .equ printInt,6
        .equ inkey,1
        .data 
c1:     .asciiz "\r"
c2:     .asciiz "\t"


        .text 
        .globl main
main:   addiu $sp,$sp,-4
        sw $ra,0($sp)
        li $t0,0   #cnt
        li $t1,0   #s
do:     beq $v0,'q',endw
        la $a0,c1
        li $v0,putChar
        syscall
        or $a0,$0,$t0
        li $t5,10
        li $t6,3
        sll $t6,$t6,16
        or $t4,$t5,$t6
        or $a1,$0,$t4
    
        li $v0,printInt
        syscall
        la $a0,c2
        li $v0,putChar
        syscall
        or $a0,$0,$t0
        li $t5,1
        li $t6,8
        sll $t6,$t6,16
        or $t4,$t5,$t6
        or $a1,$0,$t4
        li $v0,printInt
        syscall
        li $a0,5
        jal wait
        li $v0,inkey
        syscall
if:     bne $v0,'+',if2
        li $t1,0
if2:    bne $v0,'-',if3
        li $t1,1
if3:    bne $v0,0,else
        addiu $t0,$t0,1
        and $t0,$t0,0xFF
else:   addiu $t0,$t0,-1
        and $t0,$t0,0xFF
        j do
endw:   lw $ra,0($sp)
        addiu $sp,$sp,4
        li $v0,0
        jr $ra
####################################
wait:   li $t2,0
for:    beq $t2,257500,endfor
        addiu $t2,$t2,1
        j for
endfor: jr $ra
        



        