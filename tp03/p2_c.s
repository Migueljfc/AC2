        .equ SFR_BASE_HI, 0xBF88 # 16 MSbits of SFR area
        .equ TRISE, 0x6100 
        .equ LATE , 0x6120
        .equ TRISB, 0x6040
        .equ PORTB , 0x6050
        .equ RESET_CORE_TIMER,12
        .equ READ_CORE_TIMER,11
        .data
        .text
        .globl main
main:   addi $sp,$sp,-4
        sw $ra,0($sp)

        lui $t1, SFR_BASE_HI
        lw $t2,TRISE($t1)              #READ TRISE
        andi $t2,$t2,0xFFF0            # RE0...RE3 = 0 -> saidas
        sw $t2,TRISE($t1)              # WRITE REGISTER TRISE

        lw $t2,TRISB($t1)               #READ TRISB
        ori $t2,$t2,0x0008              #RB3 = 1 -> Entrada
        sw $t2,TRISB($t1)              #WRITE REGISTER TRIB
        li $t3,1                    

while:  li $a0,500 
        jal delay                       #delay(500)
        lw $t4,PORTB($t1)
        andi $t4,$t4,0x0008
        beq $t4,0,decres
        bne $t4,0,cresc

cresc:  addi $t3,$t3,1
        lw $t2,LATE($t1)                
        andi $t2,$t2,0xFFF0

        add $t2,$t2,$t3
        sw $t3,LATE($t1)

        j while
decres: sub $t3,$t3,1
        lw $t2,LATE($t1)                
        andi $t2,$t2,0xFFF0

        add $t2,$t2,$t3
        sw $t3,LATE($t1)

        j while

        lw $ra,0($sp)
        addi $sp,$sp,4
        
        jr $ra

#################DELAY####################

delay:  move $t0,$a0
for:    ble $t0,$0,endf
        li $v0,RESET_CORE_TIMER
        syscall
while2: li $v0,READ_CORE_TIMER
        syscall
        ble $v0,20000,while2
        addiu $t0,$t0,-1
        j for
endf:   jr $ra
