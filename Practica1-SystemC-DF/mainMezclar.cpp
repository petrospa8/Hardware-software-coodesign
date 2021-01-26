#include "systemc.h"

#include "fifo.h"
#include "mezclar.h"
#include "consumidor.h"
#include "productor.h"




class top : public sc_module
{
	public:

	fifo_T<sc_int<16>> *Qdin1, *Qdin2, *Qout;

	productor *datosEntrada;
	consumidor *salidaResultados;
	mezclar *instMezclar; 

	SC_CTOR(top) // the module constructor
	{

		datosEntrada = new productor("datosEntrada");
		salidaResultados  = new consumidor("salidaResultados", "resultados.txt");
		instMezclar = new mezclar("mezclar"); 

		Qdin1 = new fifo_T<sc_int<16>>("Qdin1", 1); 
		Qdin2 = new fifo_T<sc_int<16>>("Qdin2", 2);
		Qout = new fifo_T<sc_int<16>>("Qout", 3);
		// instancia las colas restantes

		datosEntrada->din1( *Qdin1 );
		datosEntrada->din2( *Qdin2 );
		instMezclar->audio_in1( *Qdin1 );
		instMezclar->audio_in2( *Qdin2 );
		instMezclar->audio_out( *Qout );
		salidaResultados->audio_in( *Qout ); 

		// conecta el m�dulo mezclar con el productor y el consumidor
		

	}

};


int sc_main(int nargs, char* vargs[]){

	top principal("top");
	sc_start();

	return 0;

}
