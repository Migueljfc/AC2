        .equ SFR_BASE_HI, 0xBF88
        .equ TRISE, 0x6100
        .equ TRISB, 0x6040
        .equ PORTE, 0x6110
        .equ LATE, 0x6120
        .equ PORTB,0x6050
        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
        .text
        .globl main
main:   addiu $sp,$sp,-4
        sw $ra,0($sp)
        li $t3,0                # v = $t0 = 0;
        
        lui $t1,SFR_BASE_HI
        lw $t2,TRISE($t1)       # Read TRISE
        andi $t2,$t2,0xFFFE     # RE0 = 0 -> Saída
        sw $t2,TRISE($t1)       # Write register TRISE

while:  lw $t2,LATE($t1)
        xor $t2,$t2,1
        sw $t2,LATE($t1)

        li $a0,500
        jal delay
        xori $t3,$t3,1
        j while

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



