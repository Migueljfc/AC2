        .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
        .equ TRISE, 0x6100 
        .equ LATE , 0x6120
        .equ TRISB, 0x6040
        .equ PORTB , 0x6050
        .equ RESET_CORE_TIMER,12
        .equ READ_CORE_TIMER,11
        .text
        .globl main
main:   addi $sp,$sp,-4
        sw $ra,0($ra)

        lui $t1, SFR_BASE_HI
        lw $t2,TRISE($t1)              #READ TRISE
        andi $t2,$t2,0xFFF0            # RE0...RE3 = 0 -> saidas
        sw $t2,TRISE($t1)              # WRITE REGISTER TRISE

        lw $t2,TRISB($t1)               #READ TRISB
        ori $t2,$t2,0x0008              #RB3 = 1 -> Entrada
        sw $t2,TRISB($t1)              #WRITE REGISTER TRIB
        li $t3,1 

while:  
