        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,6 
        .data
        .text
        .globl main
main:   addi $sp,$sp,-4
        sw $ra,0($sp)
        li $a0,2000
        jal delay
        lui $t1,0xBF88
        lw $t2,0x6050($t1)
        li $a0,' '
        li $v0,PUT_CHAR
        syscall
        li $t0,0xFFF0
        sub $t2,$t2,$t0
        move $a0,$t2
        li $a1,2
        li $v0,PRINT_INT
        syscall
        j main
        lw $ra,0($sp)
        addi $sp,$sp,4
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