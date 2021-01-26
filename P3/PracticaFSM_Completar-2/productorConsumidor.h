#ifndef PRODUCTORCONSUMIDOR_H
#define PRODUCTORCONSUMIDOR_H

#include "systemc.h"
#include <stdio.h>


SC_MODULE (productorConsumidor) {

	sc_out<sc_uint<16>>	datoA, datoB;
	sc_out<bool>		valOut;
	sc_in<bool>			listo;
	sc_in<sc_uint<16>>	resultado;
	sc_in<bool>			reset;
	sc_in_clk			clock;



SC_HAS_PROCESS(productorConsumidor);

void producir() {

	int res;

	if (reset.read()) {
		contador = 0;
		estado = 0;
		valOut.write(false);
		return;
	}

	switch (estado) {
	case 0: // enviar un dato
		datoA.write(listaA[contador]);
		datoB.write(listaB[contador]);
		valOut.write(true);
		estado = 1;
		break;
	case 1:
		if (listo.read()) {
			valOut.write(false);
			estado = 2;
			res = resultado.read();
			if(res != listaRes[contador])
				fprintf(stderr, "Error en dato n. %d, el MCD de %d y %d es %d, y no %d\n", contador, listaA[contador], listaB[contador], listaRes[contador], res);
			//else			fprintf(stderr, "\t\tdato %d, el MCD de %d y %d es %d\n", contador, listaA[contador], listaB[contador], res);
			++contador; 
			if (contador == 100) {
				fprintf(stderr, "Se han procesado todos los datos\n");
				sc_stop();
			}
		}
		else
			valOut.write(true);
		break;
	default:
		valOut.write(false);
		if (!listo.read())
			estado = 0;
	}
}


productorConsumidor(sc_module_name name_) : sc_module(name_){

	cout<<"productor: " << name() << endl;

	contador = estado = 0; 

	SC_METHOD(producir);
        sensitive << clock.pos();
    
}

private:	
	int contador, estado; 
	unsigned int listaA[100] = {180, 80, 154, 340, 323, 77, 45, 104, 276, 351, 35, 273, 192, 420, 594, 464, 160, 75, 207, 8, 320, 81, 665, 520, 663, 54, 392, 504, 18, 341, 40, 222, 320, 140, 252, 486, 330, 336, 50, 768, 10, 48, 144, 225, 301, 76, 644, 18, 533, 918, 18, 16, 594, 697, 817, 15, 275, 169, 494, 93, 338, 33, 378, 248, 540, 93, 143, 432, 90, 66, 494, 297, 442, 108, 153, 931, 779, 574, 88, 351, 704, 90, 782, 24, 208, 160, 132, 990, 36, 45, 110, 224, 731, 576, 136, 98, 360, 190, 230, 112};
	unsigned int listaB[100] = { 66, 75, 616, 136, 893, 22, 240, 264, 12, 663, 336, 133, 360, 30, 528, 400, 140, 285, 360, 200, 420, 21, 361, 250, 612, 48, 128, 42, 432, 473, 190, 306, 10, 273, 114, 288, 240, 112, 4, 784, 255, 39, 368, 125, 133, 164, 266, 180, 234, 714, 18, 26, 324, 544, 703, 141, 44, 78, 273, 45, 364, 539, 936, 48, 690, 81, 104, 180, 16, 39, 836, 55, 663, 99, 90, 171, 988, 672, 212, 65, 512, 129, 680, 148, 130, 256, 75, 18, 48, 85, 180, 440, 748, 480, 84, 32, 136, 70, 135, 77 };
	unsigned int listaRes[100]  = { 6, 5, 154, 68, 19, 11, 15, 8, 12, 39, 7, 7, 24, 30, 66, 16, 20, 15, 9, 8, 20, 3, 19, 10, 51, 6, 8, 42, 18, 11, 10, 6, 10, 7, 6, 18, 30, 112, 2, 16, 5, 3, 16, 25, 7, 4, 14, 18, 13, 102, 18, 2, 54, 17, 19, 3, 11, 13, 13, 3, 26, 11, 18, 8, 30, 3, 13, 36, 2, 3, 38, 11, 221, 9, 9, 19, 19, 14, 4, 13, 64, 3, 34, 4, 26, 32, 3, 18, 12, 5, 10, 8, 17, 96, 4, 2, 8, 10, 5, 7 };
};


#endif