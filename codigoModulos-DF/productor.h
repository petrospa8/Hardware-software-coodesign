#ifndef PRODUCTOR_H
#define PRODUCTOR_H

#include "systemc.h"
#include "fifo.h"
#include <stdio.h>

SC_MODULE ( productor ) {
public:
sc_port< write_if_T<sc_uint<32>>> addrOut;
sc_port< write_if_T<sc_int<32>>> dataOut, output; 
sc_port< read_if_T<sc_int<32>>> input; 

SC_HAS_PROCESS(productor);

void producir(){
	int i, j, dir, dato; 
	sc_int<32> dummy;

	for(j=0; j<4; ++j){			  
		for(i=0; i<512; ++i){	
			fscanf(fichero, "%d %d", &dir, &dato);	
			addrOut->write(dir); dataOut->write(dato); 
		}			
		output->write( 1 );			// mensaje de que el productor está listo para enviar más
		printf("productor listo\n");
		input->read( dummy );		// no sigue si el siguiente procesador no está listo
		printf("respuesta al productor \n");
	}
	fclose(fichero);
}

productor(sc_module_name name_, char *fileName) : sc_module(name_){
	cout<<"productor: " << name() << "  " << fileName << endl;
	fichero = fopen(fileName, "rt");
	if(!fichero){
		fprintf(stderr, "No es posible abrir el fichero %s\n", fileName);
		exit(-1);
	}
    SC_THREAD(producir);
}
private:
FILE *fichero;

}; 

#endif