#include "mmuMem.h"


void mmuMem::internalAccess(){

	sc_uint<32> direccion;
	sc_uint<3> modulo;
	sc_uint<9> dir;
	sc_uint<10> dirOut;
	sc_int<32> tmp;
	bool readWrite; 


	while(true){
		addr->read( direccion );
		direccion &= 0x3fff;		// limito a 16K. También podría generar un mensaje de error si se pasara...
		rW->read( readWrite );

		if(direccion & 0x3){
			cerr << "MMU " << name() << ". Error, la dirección de memoria no es múltiplo de 4: " << direccion << endl;
			sc_stop();
		}
		dir = direccion(10, 2);		// direcciones dentro del módulo y a nivel de palabra, no de byte
		modulo = direccion(13, 11);

		if( (modulo == 0) && readWrite){
			cerr << "MMU " << name() << ". Error, intentado escribir sobre el código" << endl;
			sc_stop();
		}

		if( ((modulo == 2) || (modulo == 3)) && readWrite){
			cerr << "MMU " << name() << ". Error, intentado escribir sobre memoria compartida de lectura" <<  endl;
			sc_stop();
		}

		if( ((modulo == 4) || (modulo == 5)) && (!readWrite)){
			cerr << "MMU " << name() << ". Error, intentado leer la memoria compartida de escritura" <<  endl;
			sc_stop();
		}

		if(modulo<4){		// operaciones normales
			if(readWrite){
				dataIn->read( tmp );
				mem[modulo][dir] = tmp;
			}else
				dataOut->write( mem[modulo][dir] );
		}else{
			if(modulo<6){	// escritura en memoria compartida (si fuese lectura hubiese pitado arriba
				dataIn->read( tmp );				
				extDataOut->write( tmp );
				extAddrOut->write( direccion );	// envío la dirección completa de 32 bits
			}else{			// E/S
				if( (modulo>6) || (dir>=5) ){
					cerr << "MMU " << name() << ". Error, acceso E/S fuera de rango: " << direccion <<  endl;
					sc_stop();
				}
				if( readWrite && ( (dir == 0) || (dir == 2) || (dir == 4))){
					cerr << "MMU " << name() << ". Error, escritura E/S en puerto de lectura: " << dir <<  endl;
					sc_stop();
				}
				if( (!readWrite) && ( (dir == 1) || (dir == 3)) ){
					cerr << "MMU " << name() << ". Error, lectura E/S fuera en puerto de escritura: " << dir <<  endl;
					sc_stop();
				}
//				printf("============  %2d  dir: %d  \n", indice, dir.to_int());
				switch(dir){		// estas son las direcciones de entrada y salida
				case 0:		inputLeft->read( tmp );	dataOut->write( tmp );	break;
				case 1:		dataIn->read( tmp );	outpRight->write( tmp );		break;
				case 2:		inputRight->read( tmp);	dataOut->write( tmp );	break;
				case 3:		dataIn->read( tmp );	outpLeft->write( tmp );	break;	
				default:	inputExt->read( tmp );	dataOut->write( tmp );	break;
				};
			}
		}
	}
}

void mmuMem::externalAccess(){	// permite que otro procesador escriba en los módulos 2 y 3

	sc_uint<32> direccion;
	bool modulo;
	sc_uint<9> dir;
	sc_int<32> tmp;

	while(true){
		extAddrIn->read( direccion );
		extDataIn->read( tmp );
		dir = direccion(10,2);
		modulo = direccion.bit(11);
		mem[2+modulo][dir] = tmp;
	}
}

void mmuMem::setDebug(bool onOff){ debug = onOff; }


void mmuMem::contenidoModulo(int modulo) {

	cerr << "BD mem " << name() << " modulo " << modulo << endl;
	for (int i = 0; i < 512; i += 2)
		cerr << hex << i << ": " << mem[modulo][i] << "   " << mem[modulo][i + 1] << endl;
}

