#include "systemc.h"

#include "fifo.h"
#include "MIPSdf.h"
#include "productor.h"
#include "consumidor.h"

#define Nproc 17

char* nombreIdx(char *cad, int idx){
	static char nombreYnumero[128];
	sprintf(nombreYnumero, "%s_%d", cad, idx);
	return nombreYnumero;
}

SC_MODULE ( fingeEntradaExterna ) {
public:
sc_port< write_if_T<sc_int<32>>> extInput[ Nproc ];

void paraElFiltro(){ 
	extInput[8]->write( 0xff );	// para que el filtro deje pasar todo
	extInput[8]->write( 0xf0 );	// para que el filtro deje pasar solo las frecuencias altas
	extInput[8]->write( 0x0f );	// para que el filtro deje pasar solo las frecuencias bajas
	extInput[8]->write( 0xaa );	// para que el filtro deje pasar una banda de cada 2
}
	SC_CTOR(fingeEntradaExterna) // constructor
	{
		SC_THREAD( paraElFiltro );
	}
}; 


class top : public sc_module
{
	public:

	MIPSdf *mips[Nproc];
	productor *instProductor;
	consumidor *instConsumidor;
	fingeEntradaExterna *instFingeEntradaExterna;


	fifo_T<sc_int<32>> *QextData[Nproc+1];
	fifo_T<sc_uint<32>> *QextAddr[Nproc+1];
	fifo_T<sc_int<32>> *QioLR[Nproc+1], *QioRL[Nproc+1];
	fifo_T<sc_int<32>> *QextInput[Nproc];

	SC_CTOR(top) // constructor
	{
		int i;
		// ahora mismo estos nombres de archivo están vacios. Posteriormente se pondrán los archivos de código máquina y de datos internos
		char *codigos[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
		char *datos[] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

		instFingeEntradaExterna = new fingeEntradaExterna("dummy");

		for (i = 0; i < Nproc; ++i){
			mips[i] = new MIPSdf(nombreIdx("mips", i), i , codigos[i], datos[i]);
			QextInput[i] = new fifo_T<sc_int<32>>(nombreIdx("QextInput", i), 1);
		}

		for(i=0; i<=Nproc; ++i){	// una iteración más que el número de procesadores
			QextData[i] = new fifo_T<sc_int<32>>(nombreIdx("QextData", i), 1);
			QextAddr[i] = new fifo_T<sc_uint<32>>(nombreIdx("QextAddr", i), 1);
			QioLR[i] = new fifo_T<sc_int<32>>(nombreIdx("QioLR", i), 1);
			QioRL[i] = new fifo_T<sc_int<32>>(nombreIdx("QioRL", i), 1);			
		}

		for(i=0; i<Nproc; ++i){
			mips[i]->extDataIn( *QextData[i] );
			mips[i]->extAddrIn( *QextAddr[i] );
			mips[i]->extDataOut( *QextData[i+1] );
			mips[i]->extAddrOut( *QextAddr[i+1] );
			mips[i]->inputLeft( *QioLR[i] );
			mips[i]->outpRight( *QioLR[i+1] );
			mips[i]->inputRight( *QioRL[i+1] );
			mips[i]->outpLeft( *QioRL[i] );
			mips[i]->inputExt( *QextInput[i] );
			instFingeEntradaExterna->extInput[i]( *QextInput[i] );
		}


	//	mips[1]->setDebug(true);		// si se desea


		instProductor = new productor("instProductor", "inputL0.txt");
		instProductor->	addrOut	( *QextAddr[0] );
		instProductor->	dataOut	( *QextData[0] );
		instProductor->	output	( *QioLR[0] );
		instProductor->	input	( *QioRL[0]  ); 
		
		instConsumidor = new consumidor("instConsumidor", "outputDirectaInversaFiltrada.txt");	// fichero de resultados finales
		instConsumidor->addrIn	( *QextAddr[Nproc] );
		instConsumidor->dataIn	( *QextData[Nproc] );
		instConsumidor->input	( *QioLR[Nproc] );
		instConsumidor->output	( *QioRL[Nproc] );


	}

};


int sc_main(int nargs, char* vargs[]){

	top principal("top");
	sc_start();

	return 0;

}
