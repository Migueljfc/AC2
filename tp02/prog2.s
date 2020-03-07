        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .equ PUT_CHAR,3
        .equ PRINT_INT,6
        .data
        .text 
        .globl main
main:   li $t0,0
        addi $sp,$sp,-4
        sw $ra,0($sp)
while:  li $a0,1000
        jal delay
        li $v0,RESET_CORE_TIMER
        syscall
        li $a0, ' '
        li $v0,PUT_CHAR
        syscall
        addi $s0,$s0,1
        move $a0,$s0
        li $a1,10
        li $v0,PRINT_INT
        syscall
        j while 
endw:   lw $ra,0($sp)
        addi $sp,$sp,4
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


