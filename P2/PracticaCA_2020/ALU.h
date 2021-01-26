#ifndef ALU_H
#define ALU_H

#include "systemc.h"


SC_MODULE (ALU) {
public:
	sc_in<sc_int<32>>	aluA, aluB;
	sc_in<sc_uint<3>>	aluOp;
	sc_out<bool>		zero;
	sc_out<sc_int<32>>	aluRes, salidaALU;
	sc_in<bool>			reset;
	sc_in_clk			clock;

  void calcular();
  void registro();

  SC_CTOR(ALU) {
	cout<<"ALU: "<<name()<<endl;

	SC_METHOD(calcular);
        sensitive << aluA << aluB << aluOp;
	SC_METHOD(registro);
		sensitive << clock.pos();

  }
private:
	sc_int<32> resultado;
}; 

#endif