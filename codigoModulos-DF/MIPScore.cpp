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

	int i, j, tr, ti;

	// el único motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
	// definidas en esta misma clase. De no ser por ello, se utilizarían variables normales

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que está listo

	Ra0 = 0x1000; // dirección datos de entrada
	Ra1 = 0x2000; // dirección resultados

	while (true) {

	inicioEspera:
		LW($t0, 0x3000);		// espera al procesador anterior
		if (Rt0 == 0)			// beq $t0, $0, inicioEspera
			goto inicioEspera;

	esperaPosterior:
		LW($t0, 0x3000 + 8);	// espera al procesador posterior
		if (Rt0 == 0)			// beq $t0, $0, esperaPosterior
			goto esperaPosterior;

		LW($t0, 0x3000 + 16);		// lee la "máscara" para saber qué frecuencias pasan y cuales no

		// esta es la frecuencia 0
		i = 0;
		LW($t1, Ra0 + (i << 3));		// carga la parte real
		LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

		if( !(Rt0 & 0x01) )	{	// si el bit '0' de la máscara es 0, reduzco el peso de esta frecuencia en un factor 32
			Rt1 >>= 5;
			Rt2 >>= 5;
		}
		SW($t1, Ra1 + (i << 3));		// almacena la parte real
		SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		// frecuencia 1
		i++;
		LW($t1, Ra0 + (i << 3));		// carga la parte real
		LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

		if( !(Rt0 & 0x02) )	{	// si el bit '1' de la máscara es 0, reduzco el peso de esta frecuencia en un factor 32
			Rt1 >>= 5;
			Rt2 >>= 5;
		}
		SW($t1, Ra1 + (i << 3));		// almacena la parte real
		SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		// frecuencias 2 y 3
		for(i=2; i<4; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x04) )	{	// si el bit '2' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 4 a 7
		for(i=4; i<8; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x08) )	{	// si el bit '3' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 8 a 15
		for(i=8; i<16; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x10) )	{	// si el bit '4' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 16 a 31
		for(i=16; i<32; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x20) )	{	// si el bit '5' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 32 a 63
		for(i=32; i<64; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x40) )	{	// si el bit '6' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 64 a 127		(incluida parte "reflejada", por eso sigue con 64 más)
		for(i=64; i<192; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x80) )	{	// si el bit '7' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 32 a 63 (parte reflejada)
		for(i=192; i<224; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x40) )	{	// si el bit '6' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 16 a 31 (parte reflejada)
		for(i=224; i<240; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x20) )	{	// si el bit '5' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 8 a 15 (parte reflejada)
		for(i=240; i<248; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x10) )	{	// si el bit '4' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 4 a 7 (parte reflejada)
		for(i=248; i<252; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x08) )	{	// si el bit '3' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencias 2 y 3 (parte reflejada)
		for(i=252; i<254; ++i){
			LW($t1, Ra0 + (i << 3));		// carga la parte real
			LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

			if( !(Rt0 & 0x04) )	{	// si el bit '2' de la máscara es 0, reduzco el peso de estas frecuencias en un factor 32
				Rt1 >>= 5;
				Rt2 >>= 5;
			}
			SW($t1, Ra1 + (i << 3));		// almacena la parte real
			SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		}
		// frecuencia 1 (parte reflejada)
		i=254;
		LW($t1, Ra0 + (i << 3));		// carga la parte real
		LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

		if( !(Rt0 & 0x02) )	{	// si el bit '1' de la máscara es 0, reduzco el peso de esta frecuencia en un factor 32
			Rt1 >>= 5;
			Rt2 >>= 5;
		}
		SW($t1, Ra1 + (i << 3));		// almacena la parte real
		SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img
		// frecuencia 0  (parte reflejada)
		i = 255;
		LW($t1, Ra0 + (i << 3));		// carga la parte real
		LW($t2, Ra0 + (i << 3) + 4);	// carga la parte img

		if( !(Rt0 & 0x01) )	{	// si el bit '0' de la máscara es 0, reduzco el peso de esta frecuencia en un factor 32
			Rt1 >>= 5;
			Rt2 >>= 5;
		}
		SW($t1, Ra1 + (i << 3));		// almacena la parte real
		SW($t2, Ra1 + (i << 3) + 4);	// almacena la parte img

		// ahora se envían las señales de sincronismo y se vuelve a empezar

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que terminó
		SW($t8, 0x3000 + 12);		// avisa al productor de que está listo
		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}
}

void MIPScore::codigoL0(){

	int i, j, k, n, dir1, dir2;
	int calc1, calc2;

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al productor de que está listo

	Ra0 = 0x1000; // dirección datos de entrada
	Ra1 = 0x2000; // dirección resultados

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

			// para obtener la dirección de los datos ha que "invertir" los bits de su dirección.
			j = i; k = 0;
			for(n=0; n<8; ++n){	k<<=1;		k |= j & 1;		j>>=1; }		dir1 = k<<1;	// dirección 1er dato (real)
			// se respite para la segunda dirección
			j = i+1; k=0; 
			for(n=0; n<8; ++n){	k<<=1;		k |= j & 1;		j>>=1; }		dir2 = k<<1;	// dirección 2do dato (real)

			// sabemos que para el nivel 0 sólo hay parte real. Leemos 2 datos (reales) en 2 registros elegidos al azar
			// el único motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
			// definidas en esta misma clase. De no ser por ello, se utilizarían variables normales

			LW($t1, Ra0 + (dir1<<2) );
			LW($t2, Ra0 + (dir2<<2) );

			Rt3 = Rt1 + Rt2;
			Rt4 = Rt1 - Rt2;

			SW($t3, Ra1 + (i << 3));
			SW($zero, Ra1 + (i << 3) + 4);		// la parte img es 0
			SW($t4, Ra1 + (i << 3) + 8);
			SW($zero, Ra1 + (i << 3) + 12);		// la parte img es 0
		}

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que terminó
		SW($t8, 0x3000 + 12);		// avisa al productor de que está listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}

}

void MIPScore::codigoL1() {
	
	int i, j, tr, ti;
	int calc1, calc2;

	// el único motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
	// definidas en esta misma clase. De no ser por ello, se utilizarían variables normales

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que está listo

	Ra0 = 0x1000; // dirección datos de entrada
	Ra1 = 0x2000; // dirección resultados

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

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que terminó
		SW($t8, 0x3000 + 12);		// avisa al productor de que está listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}
}



void MIPScore::codigoL0inv() {

	int i, j, k, n, dir1, dir2;
	int calc1, calc2;

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que está listo

	Ra0 = 0x1000; // dirección datos de entrada
	Ra1 = 0x2000; // dirección resultados

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

			// para obtener la dirección de los datos ha que "invertir" los bits de su dirección.
			j = i; k = 0;
			for (n = 0; n < 8; ++n) { k <<= 1;		k |= j & 1;		j >>= 1; }		dir1 = k << 1;	// dirección 1er dato (real)
			// se respite para la segunda dirección
			j = i + 1; k = 0;
			for (n = 0; n < 8; ++n) { k <<= 1;		k |= j & 1;		j >>= 1; }		dir2 = k << 1;	// dirección 2do dato (real)

			// Leemos 2 parejas de datos (real e img) en 4 registros elegidos al azar
			// el único motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
			// definidas en esta misma clase. De no ser por ello, se utilizarían variables normales

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

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que terminó
		SW($t8, 0x3000 + 12);		// avisa al productor de que está listo

		Ra0 ^= 0x800;				// truco para leer datos del otro bloque	( doble buffer o buffer ping-pong)
		Ra1 ^= 0x800;				// truco para escribri resultados el otro bloque
	}

}


void MIPScore::codigoL1inv() {

	int i, j; 

	// el único motivo por el que se van a utilizar registros (Rt0, Rt1,...) es para poder utilizar las funciones LW y SW
	// definidas en esta misma clase. De no ser por ello, se utilizarían variables normales

	Rt8 = 1;				// elijo un registro cualquiera para pasar un valor no nulo
	SW($t8, 0x3000 + 12);	// avisa al procesador anterior de que está listo

	Ra0 = 0x1000; // dirección datos de entrada
	Ra1 = 0x2000; // dirección resultados

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

		SW($t8, 0x3000 + 4);		// avisa al consumidor de que terminó
		SW($t8, 0x3000 + 12);		// avisa al productor de que está listo

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
			case 0x00:	regs[reg3] = regs[reg2]	<< shamt;					++cnt.sll;	if(debug) printf("%s = %08x = %s sll %d\n", RR[reg3], RV(reg3), RR[reg2], shamt);		break;	// sll o también nop
			case 0x04:	regs[reg3] = regs[reg2]	<< regs[reg1];				++cnt.sllv;	if(debug) printf("%s = %08x = %s sllv %s\n", RR[reg3], RV(reg3), RR[reg2], RR[reg1]); break;	// sllv
			case 0x02:	regs[reg3] = RVU(reg2)	>> shamt;					++cnt.srl;	if(debug) printf("%s = %08x = %s srl %d\n", RR[reg3], RV(reg3), RR[reg2], shamt);		break;	// srl
			case 0x06:	regs[reg3] = RVU(reg2)	>> regs[reg1];				++cnt.srlv;	if(debug) printf("%s = %08x = %s srlv %s\n", RR[reg3], RV(reg3), RR[reg2], RR[reg1]); break;	// srlv
			case 0x03:	regs[reg3] = regs[reg2]	>> shamt;					++cnt.sra;	if(debug) printf("%s = %08x = %s sra %d\n", RR[reg3], RV(reg3), RR[reg2], shamt);		break;	// sra
			case 0x07:	regs[reg3] = regs[reg2]	>> regs[reg1];				++cnt.srav;	if(debug) printf("%s = %08x = %s srav %s\n", RR[reg3], RV(reg3), RR[reg2], RR[reg1]); break;	// srav
			case 0x12:	regs[reg3] = LO;									++cnt.mflo;	if(debug) printf("%s = %08x = LO\n", RR[reg3], RV(reg3));								break;	// mflo		termina la multiplicación
			case 0x24:	regs[reg3] = regs[reg1] & regs[reg2];				++cnt.and;	if(debug) printf("%s = %08x = %s & %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// and
			case 0x25:	regs[reg3] = regs[reg1] | regs[reg2];				++cnt.or;	if(debug) printf("%s = %08x = %s | %s\n", RR[reg3], RV(reg3), RR[reg1], RR[reg2]);	break;	// or
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

		regs[0] = 0; // permito que se modifique, pero lo pongo a 0 después. Es más fácil que comprobar todas las escrituras

	}

}



void MIPScore::setDebug(bool onOff){
	debug = onOff;
}

