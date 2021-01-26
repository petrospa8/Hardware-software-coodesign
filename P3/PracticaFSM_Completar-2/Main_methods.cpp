#include "systemc.h"

#include "productorConsumidor.h"
#include "mcd.h"



class top : public sc_module
{
public:
	sc_in < bool > clk, rst;

productorConsumidor *prodCon;
mcd*instMcd;


sc_signal<sc_uint<16>>	datoA, datoB, resultado;
sc_signal<bool>			valido, listo;

	SC_CTOR(top){
	
		int i;

		prodCon = new productorConsumidor("prodCon");
		instMcd = new mcd("instMcd");
		
		prodCon->datoA(datoA);
		prodCon->datoB(datoB);
		prodCon->resultado(resultado);
		prodCon->valOut(valido);
		prodCon->listo(listo);
		prodCon->reset(rst);
		prodCon->clock(clk);

		instMcd->entA(datoA);
		instMcd->entB(datoB);
		instMcd->resultado(resultado);
		instMcd->valIn(valido);
		instMcd->listo(listo);
		instMcd->reset(rst);
		instMcd->clock(clk);

	}

};

int sc_main(int nargs, char* vargs[]){

	sc_clock clk ("clk", 1, SC_NS);
	sc_signal <bool> rst;

	top principal("top");
	principal.clk( clk );
	principal.rst( rst );


	rst.write(true);
	sc_start(2, SC_NS);				

	rst.write(false);
	sc_start();

	return 0;

}
