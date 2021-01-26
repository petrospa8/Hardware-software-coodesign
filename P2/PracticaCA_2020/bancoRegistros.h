#ifndef BANCOREGISTROS_H
#define BANCOREGISTROS_H

#include "systemc.h"

SC_MODULE (bancoRegistros) {

	sc_in<bool>			escrReg;
	sc_in<sc_int<32>>	datoEscr;
	sc_in<sc_uint<5>>	regLect1, regLect2, regEscr;
	sc_out<sc_int<32>>	reg1, reg2;
	sc_in<bool>			reset;
	sc_in_clk			clock;

  void gestion();

  SC_CTOR(bancoRegistros) {
	cout<<"bancoRegistros: "<<name()<<endl;

	SC_METHOD(gestion);   
        sensitive << clock.pos();
  }

private:
	sc_int<32> REGS[32], A, B;
}; 


#endif
