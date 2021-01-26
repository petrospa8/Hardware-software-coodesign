#include "mcd.h"

void mcd::FSM() {

	sc_int<16> tmp;
	int miraA, miraB;

	miraA = A.read();		// utilizad estas variables para depurar
	miraB = B.read();

	listo.write(false);

	if (reset.read()) {
		estado = ini;
		A.write(0);
		B.write(0);
	}else
		switch (estado) {
		case ini:
			if(valIn.read()){
				A.write(entA.read().to_int());
				B.write(entB.read().to_int());
				estado = restar;
			}
			break;
		case restar:
			if(cero){
				estado = fin;
				break;
			}
			if(signo){
				estado = aMenor;
			}else{
				estado = aMayor;
			}
			break;
		case aMayor:
			A.write(resta);
			estado = restar;
			break;
		case aMenor:
				tmp = A.read();
				A.write(B.read());
				B.write(tmp);
				estado = restar;
			break;
		case fin:
			listo.write(true);
			if(!valIn.read())
				estado = ini;
			break;
		default:
			cerr << "Error, estado inexisente: " << estado << endl;
		}

	resultado.write(A.read().to_uint());
}


void mcd::calcular() {	// no tocar

	resta = A.read() - B.read();
	signo = resta.bit(15);
	cero = (resta == 0);
}
