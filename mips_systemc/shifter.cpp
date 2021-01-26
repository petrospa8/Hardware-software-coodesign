#include"shifter.h"

void shifter::shift(){
	
	sh16 = 0xdeaddead;

}


void shifter::registro(){

	if( rst.read() ){
		shifted.write(0);
	}else{
		shifted.write(sh16);
	}
}
