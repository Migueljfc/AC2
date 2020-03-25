        .equ SFR_BASE_HI, 0xBF88   
        .equ TRISB, 0x6040             
        .equ PORTB, 0x6050             
        .equ LATB, 0x6060              
        .equ TRISE, 0x6100             
        .equ PORTE, 0x6110             
        .equ LATE, 0x6120              
        .equ READ_CORE_TIMER,11
        .equ RESET_CORE_TIMER,12
          
        .data
        .text
        .globl main

main:   subu $sp, $sp, 20               
        sw $ra, 0($sp)
        sw $s0, 4($sp)
        sw $s1, 8($sp)
        sw $s2, 12($sp)
        sw $s3, 16($sp)

        lui $s0, SFR_BASE_HI       
        lw $s1, TRISE($s0)         
        andi $s1, $s1, 0xFFF0      
        sw $s1, TRISE($s0)         

                                     
        lw $s2, TRISB($s0)         
        andi $s2, $s2, 0xFFF0      
        ori  $s2, $s2, 0x000F      
        sw $s2, TRISB($s0)         

        li $s3, 0                  

                                     
while:                               
        li $a0, 1000
        jal delay

        not  $t1, $s3
        andi $t1, $t1, 0x0008      
        srl  $t1, $t1, 3           

        sll  $s3, $s3, 1
        or   $s3, $s3, $t1

        lw $t1, LATE($s0)          
        andi $t1, $t1, 0xFFF0      
        or   $t1, $t1, $s3         
        sw $t1, LATE($s0)          

        j while                    

        lw $ra, 0($sp)             
        lw $s0, 4($sp)
        lw $s1, 8($sp)
        lw $s2, 12($sp)
        lw $s3, 16($sp)
        addiu $sp, $sp, 20

        li $v0, 0
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




        