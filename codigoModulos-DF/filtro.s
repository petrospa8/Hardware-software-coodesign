main:
    # Avisa al procesador anterior de que estoy listo
    addi $t0, $zero, 1
    addi $t1, $zero, 0x300c
    sw $t0, 0($t1)

    # Direcciones de entrada y salida
    addi $a0, $zero, 0x1000
    addi $a1, $zero, 0x2000

    loop:
        # Espero al procesador anterior
        anterior:
            addi $t1, $zero, 0x3000
            lw $t0, 0($t1)
            beq $t0, $zero, anterior

        # Espero al procesador siguiente
        posterior:
            addi $t1, $zero, 0x3008
            lw $t0, 0($t1)
            beq $t0, $zero, posterior

        # Leemos la mascara de frecuencias
        addi $t1, $zero, 0x3010
        lw $t0, 0($t1)

        # Iniciamos el puntero y las condiciones de parada
        addi $t1, $zero, 0
        addi $a2, $zero, 1
        addi $a3, $zero, 0x0080
        addi $t8, $zero, 0x0100

        # Eliminar frecuencias positivas
        freqp:
            gfreqp:
                # Preparamos los registros de entrada y salida
                sll $t2, $t1, 3
                add $t3, $a0, $t2
                add $t4, $a1, $t2

                # Carga parte real e imaginaria
                lw $t5, $t3
                lw $t6, 4($t3)

                # Comprobamos el bit de la posicion log2($t1)
                and $t7, $t0, $a2

                # Comprobamos si reducimos
                beq $t7, $zero, skip

                # Reducimos
                srl $t5, $t5, 5
                srl $t6, $t6, 5

                # Guardamos los resultados
                skip:
                    sw $t5, $t4
                    sw $t6, 4($t4)

                # Avanzamos el puntero
                addi $t1, $t1, 1
                bne $a3, $t1, gfreqp

            # Calculamos las nuevas condiciones de parada
            sll $a2, $a2, 1
            sll $a3, $a3, 1

            # Comprobamos si salimos
            bne $a2, $t8, freqp

        # Cambiamos las condiciones de parada
        addi $a2, $zero, 0x0080
        addi $a3, $zero, 0x00c0
        addi $t8, $zero, 0x0040

        # Eliminar frecuencias negativas
        freqn:
            gfreqn:
                # Preparamos los registros de entrada y salida
                sll $t2, $t1, 3
                add $t3, $a0, $t2
                add $t4, $a1, $t2

                # Carga parte real e imaginaria
                lw $t5, $t3
                lw $t6, 4($t3)

                # Comprobamos el bit de la posicion log2($t1)
                and $t7, $t0, $a2

                # Comprobamos si reducimos
                beq $t7, $zero, skip

                # Reducimos
                srl $t5, $t5, 5
                srl $t6, $t6, 5

                # Guardamos los resultados
                skip:
                    sw $t5, $t4
                    sw $t6, 4($t4)

                # Avanzamos el puntero
                addi $t1, $t1, 1
                bne $a3, $t1, gfreqn

            # Calculamos las nuevas condiciones de parada
            srl $a2, $a2, 1
            srl $t8, $t8, 1
            add $a3, $a3, $t8

            # Comprobamos si salimos
            bne $a2, $zero, freqn

        # Preparamos las direcciones de sincronismo
        addi $t0, $zero, 1
        addi $t1, $zero, 0x3004
        addi $t2, $zero, 0x300c

        # Señales de sincronismo
        sw $t0, 0($t1)
        sw $t0, 0($t2)

        # Truco para leer datos del otro bloque
        addi $t0, $zero, 0x0800
        xor $a0, $a0, $t0
        xor $a1, $a1, $t0

        j loop
