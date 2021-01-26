#include"MIPScore.h"

void MIPScore::SW(int reg, int dir){
	addr	->write( dir );
	rW		->write( WR );
	memWrite->write( regs[reg] );	
}
void MIPScore::LW(int reg, int dir){
	sc_int<32> valor;

	addr	->write( dir );
	rW		->write( RD );
	memRead->read( valor );
	regs[reg] = valor;
}

void MIPScore::codigoFiltrar() {
	addi($t0, $zero, 0x0001);
	addi($t1, $zero, 0x3000);
	SW($t0, regs[$t1] + 0x000c);

	addi($a0, $zero, 0x1000);
	addi($a1, $zero, 0x2000);

	loop:
		anterior:
			LW($t0, regs[$t1]);
			beq($t0, $zero, anterior);

		posterior:
			LW($t0, regs[$t1] + 0x0008);
			beq($t0, $zero, posterior);

		LW($t0, regs[$t1] + 0x0010);
		addi($t1, $zero, 0x0000);
		addi($t8, $zero, 0x0001);
		addi($t9, $zero, 0x0100);

		freqp:
			sll($t2, $t1, 0x0003);
			add($t3, $a0, $t2);
			add($t4, $a1, $t2);

			LW($t5, regs[$t3]);
			LW($t6, regs[$t3] + 0x0004);

			AND($t7, $t0, $t8);
			bne($t7, $zero, skipp);

			sra($t5, $t5, 0x0005);
			sra($t6, $t6, 0x0005);

			skipp:
				SW($t5, regs[$t4]);
				SW($t6, regs[$t4] + 0x0004);
			addi($t1, $t1, 0x0001);
			bne($t8, $t1, freqp);
		sll($t8, $t8, 0x0001);
		bne($t9, $t8, freqp);

		// prepararse para freqs negativas
		srl($t8, $t8, 0x0001);
		addi($a2, $zero, 0x0080);
		addi($t9, $zero, 0x0040);

		freqn:
			sll($t2, $t1, 0x0003);
			add($t3, $a0, $t2);
			add($t4, $a1, $t2);

			LW($t5, regs[$t3]);
			LW($t6, regs[$t3] + 0x0004);

			AND($t7, $t0, $t8);
			bne($t7, $zero, skipn);

			sra($t5, $t5, 0x0005);
			sra($t6, $t6, 0x0005);

			skipn:
				SW($t5, regs[$t4]);
				SW($t6, regs[$t4] + 0x0004);
			addi($t1, $t1, 0x0001);
			sub($a3, $t1, $a2);
			slt($a3, $a3, $t9);
			bne($a3, $zero, freqn);
		srl($t8, $t8, 0x0001);
		srl($t9, $t9, 0x0001);
		add($a2, $a2, $t8);
		bne($t8, $zero, freqn);

		addi($t0, $zero, 0x0001);
		addi($t1, $zero, 0x3000);
		SW($t0, regs[$t1] + 0x0004);
		SW($t0, regs[$t1] + 0x000c);

		addi($t0, $zero, 0x0800);
		XOR($a0, $a0, $t0);
		XOR($a1, $a1, $t0);

	j(loop);
}

void MIPScore::codigoL0(){

	int i, j, k, n, dir1, dir2;
	int calc1, calc2;

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al productor de que est� listo

	Ra0 = 0x1000; // direcci�n datos de entrada
	Ra1 = 0x2000; // direcci�n resultados

	while (true) {

	inicioEspera:
		LW($t0, 0x3000);		// espera al procesador anterior
		if (Rt0 == 0)			// beq $t0, $0, inicioEspera
			goto inicioEspera;

	esperaPosterior:
		LW($t0, 0x3000 + 8);	// espera al procesador posterior
		if (Rt0 == 0)			// beq $t0, $0, esperaPosterior
			goto esperaPosterior;

		for (i = 0; i < 256; i+=2) {

			// para obtener la direcci�n de los datos ha que "invertir" los bits de su direcci�n.
			j = i; k = 0;
			for(n=0; n<8; ++n){	k<<=1;		k |= j & 1;		j>>=1; }		dir1 = k<<1;	// direcci�n 1er dato (real)
			// se respite para la segunda direcci�n
			j = i+1; k=0; 
			for(n=0; n<8; ++n){	k<<=1;		k |= j & 1;		j>>=1; }		dir2 = k<<1;	// direcci�n 2do dato (real)

			// sabemos que para el nivel 0 s�lo hay parte real. Leemos 2 datos (reales) en 2 registros elegidos al azar
			// el �nico motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
			// definidas en esta misma clase. De no ser por ello, se utilizar�an variables normales

			LW($t1, Ra0 + (dir1<<2) );
			LW($t2, Ra0 + (dir2<<2) );

			Rt3 = Rt1 + Rt2;
			Rt4 = Rt1 - Rt2;

			SW($t3, Ra1 + (i << 3));
			SW($zero, Ra1 + (i << 3) + 4);		// la parte img es 0
			SW($t4, Ra1 + (i << 3) + 8);
			SW($zero, Ra1 + (i << 3) + 12);		// la parte img es 0
		}

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que termin�
		SW($t8, 0x3000 + 12);		// avisa al productor de que est� listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}

}

void MIPScore::codigoL1() {
	
	int i, j, tr, ti;
	int calc1, calc2;

	// el �nico motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
	// definidas en esta misma clase. De no ser por ello, se utilizar�an variables normales

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que est� listo

	Ra0 = 0x1000; // direcci�n datos de entrada
	Ra1 = 0x2000; // direcci�n resultados

	while (true) {

	inicioEspera:
		LW($t0, 0x3000);		// espera al procesador anterior
		if (Rt0 == 0)			// beq $t0, $0, inicioEspera
			goto inicioEspera;

	esperaPosterior:
		LW($t0, 0x3000 + 8);	// espera al procesador posterior
		if (Rt0 == 0)			// beq $t0, $0, esperaPosterior
			goto esperaPosterior;

		for (i = 0; i < 512; i += 8) {
			for (j = 0; j < 2; ++j) {
				// leemos 2 datos (reales) en 2 registros
				LW($t1, Ra0 + ((i + j * 2 + 4) << 2));
				LW($t2, Ra0 + ((i + j * 2) << 2));

				if (j == 0) {
					Rt3 = Rt2 - Rt1;	Rt4 = 0;
					Rt5 = Rt2 + Rt1;	Rt6 = 0;
				}
				else {
					Rt3 = Rt2;			Rt4 = Rt1;
					Rt5 = Rt2;			Rt6 = -Rt1;
				}

				SW($t3, Ra1 + ((i + j * 2 + 4) << 2));
				SW($t4, Ra1 + ((i + j * 2 + 5) << 2));

				SW($t5, Ra1 + ((i + j * 2) << 2));
				SW($t6, Ra1 + ((i + j * 2 + 1) << 2));
			}
		}

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que termin�
		SW($t8, 0x3000 + 12);		// avisa al productor de que est� listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}
}



void MIPScore::codigoL0inv() {

	int i, j, k, n, dir1, dir2;
	int calc1, calc2;

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que est� listo

	Ra0 = 0x1000; // direcci�n datos de entrada
	Ra1 = 0x2000; // direcci�n resultados

	while (true) {

	inicioEspera:
		LW($t0, 0x3000);		// espera al procesador anterior
		if (Rt0 == 0)			// beq $t0, $0, inicioEspera
			goto inicioEspera;

	esperaPosterior:
		LW($t0, 0x3000 + 8);	// espera al procesador posterior
		if (Rt0 == 0)			// beq $t0, $0, esperaPosterior
			goto esperaPosterior;

		for (i = 0; i < 256; i += 2) {

			// para obtener la direcci�n de los datos ha que "invertir" los bits de su direcci�n.
			j = i; k = 0;
			for (n = 0; n < 8; ++n) { k <<= 1;		k |= j & 1;		j >>= 1; }		dir1 = k << 1;	// direcci�n 1er dato (real)
			// se respite para la segunda direcci�n
			j = i + 1; k = 0;
			for (n = 0; n < 8; ++n) { k <<= 1;		k |= j & 1;		j >>= 1; }		dir2 = k << 1;	// direcci�n 2do dato (real)

			// Leemos 2 parejas de datos (real e img) en 4 registros elegidos al azar
			// el �nico motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
			// definidas en esta misma clase. De no ser por ello, se utilizar�an variables normales

			LW($t1, Ra0 + (dir1 << 2));		// real
			LW($t2, Ra0 + (dir1 << 2) + 4);	// img
			LW($t3, Ra0 + (dir2 << 2));		// real
			LW($t4, Ra0 + (dir2 << 2) + 4);	// img

			Rt5 = Rt1 + Rt3;
			Rt6 = Rt1 - Rt3;
			Rt7 = Rt2 + Rt4;
			Rt8 = Rt2 - Rt4;

			SW($t5, Ra1 + (i << 3));
			SW($t7, Ra1 + (i << 3) + 4);		// img 
			SW($t6, Ra1 + (i << 3) + 8);
			SW($t8, Ra1 + (i << 3) + 12);		// img
		}

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que termin�
		SW($t8, 0x3000 + 12);		// avisa al productor de que est� listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}

}


void MIPScore::codigoL1inv() {

	int i, j; 

	// el �nico motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
	// definidas en esta misma clase. De no ser por ello, se utilizar�an variables normales

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que est� listo

	Ra0 = 0x1000; // direcci�n datos de entrada
	Ra1 = 0x2000; // direcci�n resultados

	while (true) {

	inicioEspera:
		LW($t0, 0x3000);		// espera al procesador anterior
		if (Rt0 == 0)			// beq $t0, $0, inicioEspera
			goto inicioEspera;

	esperaPosterior:
		LW($t0, 0x3000 + 8);	// espera al procesador posterior
		if (Rt0 == 0)			// beq $t0, $0, esperaPosterior
			goto esperaPosterior;

		for (i = 0; i < 512; i += 8) {
			for (j = 0; j < 2; ++j) {
				// leemos 2 parejas de datos (reales e imaginarios) en 4 registros
				LW($t1, Ra0 + ((i + j * 2 + 4) << 2));
				LW($t2, Ra0 + ((i + j * 2 + 5) << 2));
				LW($t3, Ra0 + ((i + j * 2) << 2));
				LW($t4, Ra0 + ((i + j * 2 + 1) << 2));

				if (j == 0) {
					Rt5 = Rt3 - Rt1;	Rt6 = Rt4 - Rt2;
					Rt7 = Rt3 + Rt1;	Rt8 = Rt4 + Rt2;
				}
				else {
					Rt5 = Rt3 + Rt2;	Rt6 = Rt4 - Rt1;
					Rt7 = Rt3 - Rt2;	Rt8 = Rt4 + Rt1;
				}

				SW($t5, Ra1 + ((i + j * 2 + 4) << 2));
				SW($t6, Ra1 + ((i + j * 2 + 5) << 2));

				SW($t7, Ra1 + ((i + j * 2) << 2));
				SW($t8, Ra1 + ((i + j * 2 + 1) << 2));
			}
		}

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que termin�
		SW($t8, 0x3000 + 12);		// avisa al productor de que est� listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}
}



// este c�digo es gen�rico, y evita escribir una versi�n diferente para cada nivel del 1 al 7
// el par�metro "indice" se utiliza para asignar los coeficientes y los "cruces" de valores
void MIPScore::codigoFFTgenerica() {

	int i, j, tr, ti;
	long long int tmp, *ur, *ui;
	long long int calc1, calc2;
	int LJ, B, Paso;

	// la versi�n final accede a estos arrays utilizando la memoria de datos
	// esta versi�n es una simplificaci�n
	switch(indice) {
	case 2: ur = dur2; ui = dui2; break;
	case 3: ur = dur3; ui = dui3; break;
	case 4: ur = dur4; ui = dui4; break;
	case 5: ur = dur5; ui = dui5; break;
	case 6: ur = dur6; ui = dui6; break;
	case 7: ur = dur7; ui = dui7; break;
	case 11: ur = iur2; ui = iui2; break;
	case 12: ur = iur3; ui = iui3; break;
	case 13: ur = iur4; ui = iui4; break;
	case 14: ur = iur5; ui = iui5; break;
	case 15: ur = iur6; ui = iui6; break;
	case 16: ur = iur7; ui = iui7; break;
	default: cerr << "Error asignando coeficientes en FFT generica. Indice = " << indice << endl;
		exit(-1);
	};

	// calcula, dado el �ndice, como de separados est�n los datos con los que se hacen los c�lculos
	if (indice < 8)		Paso = 4 << indice;
	else				Paso = 4 << (indice - 9);
	LJ = (Paso >> 2);	B = Paso >> 1;

	// el �nico motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
	// definidas en esta misma clase. De no ser por ello, se utilizar�an variables normales

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que est� listo

	Ra0 = 0x1000; // direcci�n datos de entrada
	Ra1 = 0x2000; // direcci�n resultados


	while (true) {

	inicioEspera:
		LW($t0, 0x3000);		// espera al procesador anterior
		if (Rt0 == 0)			// beq $t0, $0, inicioEspera
			goto inicioEspera;

	esperaPosterior:
		LW($t0, 0x3000 + 8);	// espera al procesador posterior
		if (Rt0 == 0)			// beq $t0, $0, esperaPosterior
			goto esperaPosterior;

		for (i = 0; i < 512; i += Paso) {
			for (j = 0; j < LJ; ++j) {
				// leemos 2 parejas de datos (real, img) en 4 registros elegidos al azar
				LW($t0, Ra0 + ((i + j * 2 + B) << 2));
				LW($t1, Ra0 + ((i + j * 2 + B + 1) << 2));
				LW($t2, Ra0 + ((i + j * 2) << 2));
				LW($t3, Ra0 + ((i + j * 2 + 1) << 2));

				calc1 = Rt0 * ur[j];
				calc2 = Rt1 * ui[j];
				calc2 = (calc1 - calc2) >> 12;
				tr = (int)calc2;

				calc1 = Rt0 * ui[j];
				calc2 = Rt1 * ur[j];
				calc2 = (calc1 + calc2) >> 12;
				ti = (int)calc2;

				// estos eran los c�lculos originales tmp = (fftBuffer[i + j * 2 + B] * ur[j] - fftBuffer[i + j * 2 + B + 1] * ui[j]);	tr = tmp >> 12;
				// estos eran los c�lculos originales tmp = (fftBuffer[i + j * 2 + B] * ui[j] + fftBuffer[i + j * 2 + B + 1] * ur[j]);	ti = tmp >> 12;

				Rt0 = Rt2 - tr;
				Rt1 = Rt3 - ti;
				Rt2 += tr;
				Rt3 += ti;
				// se escriben los resultados
				SW($t0, Ra1 + ((i + j * 2 + B) << 2));
				SW($t1, Ra1 + ((i + j * 2 + B + 1) << 2));
				SW($t2, Ra1 + ((i + j * 2) << 2));
				SW($t3, Ra1 + ((i + j * 2 + 1) << 2));
			}
		}

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que termin�
		SW($t8, 0x3000 + 12);		// avisa al productor de que est� listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}

}



void MIPScore::procesar(){

	sc_int<32> LO, tmp;
	sc_uint<6> opCode, functCode;
	int inm, ext;
	unsigned int reg1, reg2, reg3, shamt;
	char* RR[32] = { "$0", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra" };


	while(true){

		addr	->write(PC);
		rW		->write( RD );
		memRead	->read( IR );
		inm = IR(15, 0);	inm &= 0xffff; // por si acaso

		ext = (inm<<16)>>16; 
		opCode = IR(31, 26);
		functCode = IR(5, 0);
		reg1 = IR(25, 21).to_uint();
		reg2 = IR(20, 16).to_uint();
		reg3 = IR(15, 11).to_uint();
		shamt = IR(10, 6).to_uint();
		PC = PC + 4; 

		if(debug)
			printf("%s " ,name());

		switch (opCode.to_uint()) {
		case 0x00: // Tipo R
			if(debug)
				debug = debug;
			switch (functCode) {
			case 0x20:	regs[reg3] = regs[reg1] + regs[reg2];				++cnt.add;	if(debug) printf("%s = %08x = %s + %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// sumar
			case 0x22:	regs[reg3] = regs[reg1] - regs[reg2];				++cnt.sub;	if(debug) printf("%s = %08x = %s - %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// restar
			case 0x00:	regs[reg3] = regs[reg2]	<< shamt;					++cnt.sll;	if(debug) printf("%s = %08x = %s sll %d\n", RR[reg3], RV(reg3), RR[reg2], shamt);		break;	// sll o tambi�n nop
			case 0x04:	regs[reg3] = regs[reg2]	<< regs[reg1];				++cnt.sllv;	if(debug) printf("%s = %08x = %s sllv %s\n", RR[reg3], RV(reg3), RR[reg2], RR[reg1]); break;	// sllv
			case 0x02:	regs[reg3] = RVU(reg2)	>> shamt;					++cnt.srl;	if(debug) printf("%s = %08x = %s srl %d\n", RR[reg3], RV(reg3), RR[reg2], shamt);		break;	// srl
			case 0x06:	regs[reg3] = RVU(reg2)	>> regs[reg1];				++cnt.srlv;	if(debug) printf("%s = %08x = %s srlv %s\n", RR[reg3], RV(reg3), RR[reg2], RR[reg1]); break;	// srlv
			case 0x03:	regs[reg3] = regs[reg2]	>> shamt;					++cnt.sra;	if(debug) printf("%s = %08x = %s sra %d\n", RR[reg3], RV(reg3), RR[reg2], shamt);		break;	// sra
			case 0x07:	regs[reg3] = regs[reg2]	>> regs[reg1];				++cnt.srav;	if(debug) printf("%s = %08x = %s srav %s\n", RR[reg3], RV(reg3), RR[reg2], RR[reg1]); break;	// srav
			case 0x12:	regs[reg3] = LO;									++cnt.mflo;	if(debug) printf("%s = %08x = LO\n", RR[reg3], RV(reg3));								break;	// mflo		termina la multiplicaci�n
			case 0x24:	regs[reg3] = regs[reg1] & regs[reg2];				++cnt.AND;	if(debug) printf("%s = %08x = %s & %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// and
			case 0x25:	regs[reg3] = regs[reg1] | regs[reg2];				++cnt.OR;	if(debug) printf("%s = %08x = %s | %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// or
			case 0x26:	regs[reg3] = regs[reg1] ^ regs[reg2];				++cnt.exor;	if(debug) printf("%s = %08x = %s ^ %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// exor
			case 0x2a:	regs[reg3] = (regs[reg1] < regs[reg2]) ? 1 : 0;		++cnt.slt; 	if(debug) printf("%s = %08x = %s slt %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break; // slt
			case 0x18:	LO = regs[reg1] * regs[reg2];						++cnt.mul;	if(debug) printf("LO = %08x = %s * %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// mult				multiplicacion en 2 partes
			default:	cerr << "Valor de functCode ilegal: " << functCode << endl;
						sc_stop();
			};
			break;
		case 0x08: regs[reg2] = regs[reg1] + ext;							++cnt.addi;		
			if(debug) 		printf("%s = %08x = %s + %04x\n", RR[reg2], RV(reg2), RR[reg1], ext);		break;	// addi
		case 0x0c: regs[reg2] = regs[reg1] & ext;							++cnt.andi;		if(debug) printf("%s = %08x = %s & %04x\n", RR[reg2], RV(reg2), RR[reg1], ext);		break;	// andi
		case 0x0d: regs[reg2] = regs[reg1] | ext;							++cnt.ori;		if(debug) printf("%s = %08x = %s | %04x\n", RR[reg2], RV(reg2), RR[reg1], ext);		break;	// ori
		case 0x0a: regs[reg2] = (regs[reg1] < ext) ? 1 : 0;					++cnt.slti;		if(debug) printf("%s = %08x = %s ^ %04x\n", RR[reg2], RV(reg2), RR[reg1], ext);		break;	// slti
		case 0x0f: regs[reg2] = inm << 16;									++cnt.lui;		if(debug) printf("%s = %08x = lui %04x \n", RR[reg2], RV(reg2), inm);				break;	// lui
		case 0x23: // lw 
			addr	->write( regs[reg1] + ext );		++cnt.lw;
			rW		->write( RD );
			memRead	->read( tmp );	regs[reg2] = tmp;													
			if (debug) 		printf("%s = %08x = lw %04x(%s) \n", RR[reg2], RV(reg2), inm, RR[reg1]);
			break;
		case 0x2b: // sw
			addr	->write( regs[reg1] + ext );		++cnt.sw;
			rW		->write( WR );
			memWrite->write( regs[reg2] );													
			if (debug) 		printf("sw %04x(%s) <- %s = %08x\n", inm, RR[reg1], RR[reg2], RV(reg2) );
			break;
		case 0x04: // beq
			++cnt.beq;
			if( regs[reg1] == regs[reg2]){
				PC += (ext << 2);
				if(debug) 		printf("beq salta\n");
			}else if(debug) 	printf("beq no salta\n");
			break;
		case 0x05: // bne
			++cnt.bne;
			if( regs[reg1] != regs[reg2]){
				PC += (ext << 2);
				if(debug) 	printf("bne salta\n");
			}else if(debug) printf("bne no salta\n");
			break;
		case 0x06: // blez
			++cnt.blez;
			if( regs[reg1] <= 0){
				PC += (ext << 2);
				if(debug)	printf("blez salta\n");
			}else if(debug) printf("blez no salta\n");
			break;
		case 0x07: // bgtz
			++cnt.bgtz;
			if( regs[reg1] > 0){
				PC += (ext << 2);
				if(debug)	printf("bgtz salta\n");
			}else if(debug) printf("bgtz no salta\n");
			break;
		case 0x02: // j
			PC(27, 2) = IR(25, 0);						++cnt.j;
			if(debug)		printf("J salta\n");
			break;
		default:
			cerr << "Valor de opCode ilegal: " << opCode << endl;
			sc_stop();
		};

		regs[0] = 0; // permito que se modifique, pero lo pongo a 0 despu�s. Es m�s f�cil que comprobar todas las escrituras

	}

}



void MIPScore::setDebug(bool onOff){
	debug = onOff;
}

