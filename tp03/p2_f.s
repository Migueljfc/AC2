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
        ori $t2,$t2,0x0004              #RB2 = 1 -> Entrada
        sw $t2,TRISB($t1)              #WRITE REGISTER TRIB
        li $s0,0                       #contador = 0;

while:  lw $t0,PORTB($t1)
        andi $t0,$t0,0x0004

        beq $t0,$0,right                 #if(RB2 == 1){

        andi $t2,$0,0x0008
        bne $t2,$s0,