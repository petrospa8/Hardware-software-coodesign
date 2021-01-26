#ifndef MEZCLAR_H
#define MEZCLAR_H

#include "systemc.h"
#include "fifo.h"


SC_MODULE (mezclar) {
public:
sc_port<read_if_T<sc_int<16>>>  audio_in1, audio_in2;
sc_port<write_if_T<sc_int<16>>>  audio_out;


// qu� falta aqu� ?
  void procesar();

  SC_CTOR(mezclar) {
	cout<<"mezclar: "<<name()<<endl;
    
       // qu� falta aqu� ?
      SC_THREAD(procesar);
  }

}; 

#endif