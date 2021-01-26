#ifndef CONSUMIDOR_H
#define CONSUMIDOR_H

#include "systemc.h"
#include "fifo.h"
#include <stdio.h>

SC_MODULE (consumidor) {
public:
sc_port<read_if_T<sc_int<16>>> audio_in; 

SC_HAS_PROCESS(consumidor);

void consumir(){

	sc_int<16> audio;
	int dato; 

	int i;

	for(i=0; i<2048; ++i){
		audio_in->read( audio ); 
		fscanf(fichero, "%d", &dato);
		if(dato != audio.to_int()){
			printf("Error @ %d : %d <> %d\n", i, dato,  audio.to_int());
		}
	}
	printf("Alright");
	fclose(fichero);
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