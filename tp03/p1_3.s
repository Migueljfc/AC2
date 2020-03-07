        .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
        .equ TRISE, 0x6100 
        .equ LATE , 0x6120
        .equ TRISB, 0x6040
        .equ PORTB , 0x6050
        .text
        .globl main
main:   lui $t1,SFR_BASE_HI
        lw $t2,TRISB($t1)       #Read-> MemAdrr = 0xBF88 + 0x6040
        ori $t2,$t2,0x000F     # RB0...RB3 = 1 -> entradas
        sw $t2,TRISB($t1)       #Write TRISB register 

        lw $t3,TRISE($t1)     # Read -> MemAdrr = 0xBF88 + 0x6100
        andi $t3,$t3,0xFFF0   #RE0...RE3 = 0 -> saidas 
        sw $t3,TRISE($t1)     #Write TRISE register 

while:  lw $t3,PORTB($t1)
        andi $t3,$t3,0x000F
        xor $t3,$t3,1001

        lw $t4,LATE($t1)
        andi $t4,$t4,0xFFF0
        
        or $t4,$t4,$t3
        sw $t4,LATE($t1)
        j while
        jr $ra



