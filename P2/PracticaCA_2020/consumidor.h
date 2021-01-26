#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include "systemc.h"
#include <stdio.h>

SC_MODULE (consumidor) {

	sc_in<sc_int<32>>	res, regRes;
	sc_in<bool>			clock, rst, zero;

SC_HAS_PROCESS(consumidor);

void consumir(){
	
	sc_int<32> res1, res2;
	bool esZero;
	int r1, r2, z;

	if (rst.read())
		return;

	res1 = res.read();
	res2 = regRes.read();
	esZero = zero.read();

	fscanf(fichero, "%d %d %d ", &r1, &r2, &z);

	if(r1!=res1.to_int())
		fprintf(stderr, "El resultado sin registrar no coincide @%2d: %6d != %6d\n", contador, r1, res1.to_int());

	if(r2!=res2.to_int())
		fprintf(stderr, "El resultado registrado no coincide @ %2d: %6d !=  %6d\n", contador, r2, res2.to_int());

	if(z != zero)
		fprintf(stderr, "El indicador de cero no coincide @ %2d: %d !=  %d\n", contador, z, zero);

	++contador;

	if(contador == 24){
		fclose(fichero);
		printf("Fin de la simulacion\n");				
		sc_stop();
	}


}
  


consumidor(sc_module_name name_, char *fileName) : sc_module(name_){

	cout<<"consumidor: " << name() << "  " << fileName << endl;
	fichero = fopen(fileName, "rt");
	if( !fichero ){
		cerr << "No se puede abrir el fichero de entrada: " << fileName << endl;
		return;
	}

	contador = 0;

	SC_METHOD(consumir);
        sensitive << clock.pos();	

}


private:
	FILE *fichero;
	int contador;
}; 


#endif