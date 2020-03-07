        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,6 
        .data
        .text
        .globl main
main:   addi $sp,$sp,-4
        sw $ra,0($sp)
        lui $t1,0xBF88
        lw $t2,0x6050($t1)
        li $a0,' '
        li $v0,PUT_CHAR
        syscall
        li $t0,0xFFF0
        sub $t2,$t2,$t0
        li $t0,1000
        addi $t3,$t2,1
        div $t0,$t3
        mflo $t3
        move $a0,$t2
        li $a1,1
        li $v0,PRINT_INT
        syscall
        move $a0,$t3
        jal delay
        lw $ra, 0($sp)          
        addi $sp,$sp,4 
        j main
        li $v0,0         
        jr $ra





#################DELAY####################

delay:  move $t0,$a0
for:    ble $t0,$0,endf
        li $v0,RESET_CORE_TIMER
        syscall
while2:  li $v0,READ_CORE_TIMER
        syscall
        ble $v0,20000,while2
        addiu $t0,$t0,-1
        j for
endf:   jr $ra