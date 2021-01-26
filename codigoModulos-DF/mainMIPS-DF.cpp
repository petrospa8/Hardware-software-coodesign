#include "systemc.h"

#include "fifo.h"
#include "MIPSdf.h"
#include "productor.h"
#include "consumidor.h"


char* nombreIdx(char *cad, int idx){
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

SC_MODULE ( fingeEntradaExterna ) { // solo se utiliza con el módulo de filtrado, en las demás hay que ponerla igualmente
public:
sc_port< write_if_T<sc_int<32>>> extInput;

void paraElFiltro(){ 
	extInput->write( 0xff );	// para que el filtro deje pasar todo
	extInput->write( 0xf0 );	// para que el filtro deje pasar solo las frecuencias altas
	extInput->write( 0x0f );	// para que el filtro deje pasar solo las frecuencias bajas
	extInput->write( 0xaa );	// para que el filtro deje pasar una banda de cada 2
}
	SC_CTOR(fingeEntradaExterna) // constructor
	{
		SC_THREAD( paraElFiltro );
	}
}; 


class top : public sc_module
{
	public:

	MIPSdf *mips;
	productor *instProductor;
	consumidor *instConsumidor;
	fingeEntradaExterna *instFingeEntradaExterna;


	fifo_T<sc_int<32>> *QextData[2];
	fifo_T<sc_uint<32>> *QextAddr[2];
	fifo_T<sc_int<32>> *QioLR[2], *QioRL[2];
	fifo_T<sc_int<32>> *QextInput;

	SC_CTOR(top) // constructor
	{
		int i;
		// ahora mismo estos nombres de archivo son superfluos
		char *codigos = ""; // en blanco. Cambiar sólo para ejecutar código máquina
		char *datos = "";

		instFingeEntradaExterna = new fingeEntradaExterna("dummy");

		mips = new MIPSdf("mips", 0 , codigos, datos);
		QextInput = new fifo_T<sc_int<32>>("QextInput", 1);
		

		for(i=0; i<2; ++i){	// una iteración más que el número de procesadores
			QextData[i] = new fifo_T<sc_int<32>>(nombreIdx("QextData", i), 1);
			QextAddr[i] = new fifo_T<sc_uint<32>>(nombreIdx("QextAddr", i), 1);
			QioLR[i] = new fifo_T<sc_int<32>>(nombreIdx("QioLR", i), 1);
			QioRL[i] = new fifo_T<sc_int<32>>(nombreIdx("QioRL", i), 1);			
		}

		mips->extDataIn( *QextData[0] );
		mips->extAddrIn( *QextAddr[0] );
		mips->extDataOut( *QextData[1] );
		mips->extAddrOut( *QextAddr[1] );
		mips->inputLeft( *QioLR[0] );
		mips->outpRight( *QioLR[1] );
		mips->inputRight( *QioRL[1] );
		mips->outpLeft( *QioRL[0] );
		mips->inputExt( *QextInput );

		instFingeEntradaExterna->extInput( *QextInput );

		instProductor = new productor("instProductor", "  PONER AQUI EL FICHERO DE DATOS  ");
		instProductor->	addrOut	( *QextAddr[0] );
		instProductor->	dataOut	( *QextData[0] );
		instProductor->	output	( *QioLR[0] );
		instProductor->	input	( *QioRL[0]  ); 
		
		instConsumidor = new consumidor("instConsumidor", "	PONER AQUI EL FICHERO DE RESULTADOS ");	
		instConsumidor->addrIn	( *QextAddr[1] );
		instConsumidor->dataIn	( *QextData[1] );
		instConsumidor->input	( *QioLR[1] );
		instConsumidor->output	( *QioRL[1] );


	}

};


int sc_main(int nargs, char* vargs[]){

	top principal("top");
	sc_start();

	return 0;

}
