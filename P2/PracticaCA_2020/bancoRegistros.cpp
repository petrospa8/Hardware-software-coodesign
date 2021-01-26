#include "bancoRegistros.h"



void bancoRegistros::gestion(){

	sc_uint<5> r1, r2, r3; // puedes usar estas variables para los registros que se leen y escriben
	sc_int<32>	dato;
	if (reset.read()) {
		for (int i = 0; i < 32; ++i) REGS[i] = i;	// solo para simular, as� tenemos valores variados
		r1 = r2 = 0; 
		A = B = 0;
	}
	else {
		//aqu� debes poner el c�digo que escribe en un registro (si se escribe)
		//Los datos no se pueden sacar al momento hay que esperar eñ ciclo en el que lo guardan en el registro de output
		
		r1 = regLect1.read();
		r2 = regLect2.read();
		r3 = regEscr.read();
		dato = datoEscr.read();

		if(escrReg){
			REGS[r3] = dato;
		}

	// A y B son los registros que hay justo antes de la ALU
		reg1.write(A);
		reg2.write(B);

		A = REGS[r1];
		B = REGS[r2];
	}

}


