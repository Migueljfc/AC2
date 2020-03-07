        .equ SFR_BASE_HI, 0xBF88
        .equ TRISE, 0x6100
        .equ TRISB, 0x6040
        .equ PORTE, 0x6110
        .equ LATE, 0x6120
        .equ PORTB,0x6050
        .text
        .globl main
main:   lui $t1,SFR_BASE_HI   # 16 MSbits of SFR area 
        lw $t2,TRISE($t1)     # TRISE address is 0xBF886100
        andi $t2,$t2,0xFFFE   # Modify (bit0 = 0) -> means output
        sw $t2,TRISE($t1)     # WRITE (Write TRISE register)

        lw $t2,TRISB($t1)
        ori $t2,$t2,0x0001
        sw $t2,TRISB($t1) 

while:  lw $t3,PORTB($t1)
        andi $t3,$t3,0x0001

        lw $t4,LATE($t1)
        andi $t4,$t4,0xFFFE
        or $t3,$t3,$t4
        not $t3,$t3
        sw $t3,LATE($t1)
        j while
        jr $ra
        