main:
    addi $t0, $zero, 0x0001
    addi $t1, $zero, 0x3000
    sw $t0 , 0xc($t1)

    addi $a0, $zero, 0x1000
    addi $a1, $zero, 0x2000

    loop:
        anterior: 
            lw $t0, 0($t1)
            beq $t0, $zero, anterior
        posterior:
            lw $t0, 0x8($t1)
            beq $t0, $zero, posterior

        lw $t0, 0x10($t1)
		addi $t1, $zero, 0x0000
		addi $t8, $zero, 0x0001
		addi $t9, $zero, 0x0100

		freqp:
			sll $t2, $t1, 0x0003
			add $t3, $a0, $t2
			add $t4, $a1, $t2

			lw $t5, 0($t3)
			lw $t6, 0x4($t3)

			and $t7, $t0, $t8
			bne $t7, $zero, skipp

			sra $t5, $t5, 0x0005
			sra $t6, $t6, 0x0005

			skipp:
				sw $t5, 0($t4)
				sw $t6, 0x4($t4)

			addi $t1, $t1, 0x0001
			bne $t8, $t1, freqp
		sll $t8, $t8, 0x0001
		bne $t9, $t8, freqp

		#prepararse para freqs negativas
		srl $t8, $t8, 0x0001
		addi $a2, $zero, 0x0080
		addi $t9, $zero, 0x0040

		freqn:
			sll $t2, $t1, 0x0003
			add $t3, $a0, $t2
			add $t4, $a1, $t2
            

			lw $t5, 0($t3)
			lw $t6, 0x4($t3)

			and $t7, $t0, $t8
			bne $t7, $zero, skipn

			sra $t5, $t5, 0x0005
			sra $t6, $t6, 0x0005

			skipn:
				sw $t5, 0($t4)
				sw $t6, 0x4($t4)
			addi $t1, $t1, 0x0001
			sub $a3, $t1, $a2
			slt $a3, $a3, $t9
			bne $a3, $zero, freqn
		srl $t8, $t8, 0x0001
		srl $t9, $t9, 0x0001
		add $a2, $a2, $t8 
		bne $t8, $zero, freqn

		addi $t0, $zero, 0x0001
		addi $t1, $zero, 0x3000
		sw $t0, 0x4($t1)
		sw $t0, 0xc($t1)

		addi $t0, $zero, 0x0800
		xor $a0, $a0, $t0
		xor $a1, $a1, $t0

	j loop 