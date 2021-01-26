#ifndef PRODUCTOR_H
#define PRODUCTOR_H

#include "systemc.h"
#include "fifo.h"
#include <stdio.h>
#include <math.h>

SC_MODULE (productor) {
public:
sc_port<write_if_T<sc_int<16>>>  din1, din2;	

SC_HAS_PROCESS(productor);

void producir(){

	int i;
	double darg, dval;

	for(i=0; i<256; ++i)
		din1->write( 0xffff ^ i);

	for(i=0; i<2048; ++i){
		darg = (double) i;
		dval = 4500.0*sin(darg*0.02+0.2)*(1000.0+darg)/(1500.0+darg);
		din1->write( (short)dval );
		dval = 2000.0*cos(darg*0.012+0.3)*(2500.0-darg)/(2700.0-darg);
		din2->write( (short)dval );
	}
}


productor(sc_module_name name_) : sc_module(name_){

	cout<<"productor: " << name() << "  " << endl;
	
    SC_THREAD(producir);
}


}; 


#endif