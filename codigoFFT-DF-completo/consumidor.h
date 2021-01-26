#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include "systemc.h"
#include "fifo.h"
#include <stdio.h>

SC_MODULE (consumidor) {
public:
sc_port< read_if_T<sc_uint<32>>> addrIn;
sc_port< read_if_T<sc_int<32>>> dataIn, input; 
sc_port< write_if_T<sc_int<32>>> output; 

SC_HAS_PROCESS(consumidor);

  void consumir(){
	  int i, j, dir, dat; 
	  sc_int<32> dato;
	  sc_uint<32> addr; 

	  for(j=0; j<4; ++j){
		  printf("consumidor listo\n");
		  output->write( 1 );				// el consumidor está listo
		for(i=0; i<512; ++i){  
			addrIn->read(addr);	dataIn->read(dato);
			fscanf(fichero, "%d %d ", &dir, &dat);
			if( (addr != dir) || (dato != dat) )		// poner un Breakpoint en la línea siguiente
				printf("Error bloque %d, indice %d %s:  [%d] <- %d   no coincide con [%d] <- %d\n", j, i>>1, (i&1)?"img":"real", addr.to_uint(), dato.to_int(), dir, dat);
		}
		input->read(dato);					// espera a que haya nuevos datos antes de decir que está listo para recibir más
		printf("respuesta al consumidor \n");
	  }
	  printf("FINAL. Ha habido errores????\n");
	  sc_stop();
  }

consumidor(sc_module_name name_, char *fileName) : sc_module(name_){
	cout<<"consumidor: " << name() << "  " << fileName << endl;
			
	fichero = fopen(fileName, "rt");
	if(!fichero){
		fprintf(stderr, "No es posible abrir el fichero %s\n", fileName);
		exit(-1);
	}
    SC_THREAD(consumir);
}
private:
FILE *fichero;
}; 

#endif