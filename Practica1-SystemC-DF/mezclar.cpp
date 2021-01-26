#include"mezclar.h"

void mezclar::procesar(){

	int i;
	sc_uint<9> factor1, factor2;	
	sc_int<16> din1, din2;
	sc_int<26> mult1, mult2, suma;

	//... ser� necesario descartar algunos datos 
	for(i=0; i<256; i++){
		this->audio_in1->read(din1);
	}

	factor1 = 0;	factor2 = 511; 

	while(true){	
			
		// lee los datos de entrada
		this->audio_in1->read(din1);
		this->audio_in2->read(din2);
		
		// calcula y emite el resultado
		mult1 = din1 * factor1;
		mult2 = din2 * factor2;
		suma = mult1 + mult2;

		suma = suma(25, 9);

		this->audio_out->write(suma);
		// actualiza los factores de ponderaci�n
		factor1++;
		factor2--;
	}
}
