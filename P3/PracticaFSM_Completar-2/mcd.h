#ifndef MCD_H
#define MCD_H

#include "systemc.h"


SC_MODULE (mcd) {
public:
	sc_in<sc_uint<16>>	entA, entB;
	sc_in<bool>			valIn;

	sc_out<bool>		listo;
	sc_out<sc_uint<16>>	resultado;
	sc_in<bool>			reset;
	sc_in_clk			clock;

  void calcular();
  void FSM();

  SC_CTOR(mcd) {
	cout<<"mcd: "<<name()<<endl;

	SC_METHOD(calcular);
        sensitive << A << B;
	SC_METHOD(FSM);
		sensitive << clock.pos();

  }
private:
	sc_signal<sc_int<16>> A, B;
	sc_uint<3> estado; 
	sc_int<16> resta;
	bool signo, cero;
	enum st { ini, restar, aMayor, aMenor, fin };
}; 

#endif