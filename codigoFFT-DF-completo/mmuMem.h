#ifndef MMUMEM_H
#define MMUMEM_H

#include "systemc.h"
#include "fifo.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define RD 0
#define WR 1

SC_MODULE (mmuMem) {
public:

	sc_port< read_if_T<sc_int<32>>> dataIn;
	sc_port< read_if_T<sc_uint<32>>> addr;
	sc_port< write_if_T<sc_int<32>>> dataOut;
	sc_port< read_if_T<bool>> rW;

	sc_port< read_if_T<sc_int<32>>> extDataIn;
	sc_port< read_if_T<sc_uint<32>>> extAddrIn;

	sc_port< write_if_T<sc_int<32>>> extDataOut;
	sc_port< write_if_T<sc_uint<32>>> extAddrOut;

	sc_port< read_if_T<sc_int<32>>> inputLeft, inputRight, inputExt;
	sc_port< write_if_T<sc_int<32>>> outpLeft, outpRight;

	void internalAccess();
	void externalAccess();
	void setDebug(bool);
	void contenidoModulo(int modulo); 
	

	SC_HAS_PROCESS(mmuMem);

	mmuMem(sc_module_name name_, int idx, char* codeFileName, char* dataFileName) : sc_module(name_) {

		char tmp[64], *cad ;
		unsigned int ent;
		cout << "mmuMem: " << name() << "  " << codeFileName << " " << dataFileName << endl;
		indice = idx;
		debug = false;

		if (codeFileName[0]) {  // comprueba si hay código máquina en un fichero
			contenido = fopen(codeFileName, "rt");
			if (!contenido) {
				fprintf(stderr, "Error abriendo %s\n", codeFileName);
				exit(-1);
			}
			for (int i = 0; i < 512; ++i) {
				fscanf(contenido, "%s", tmp);			cad = tmp;
				while(cad[0]==' ') ++cad;
				if( (cad[0]=='0') && ((cad[1]=='x') || (cad[1]=='X')) )
					sscanf(cad, "%x", &ent);
				else
					sscanf(cad, "%d", &ent);
				mem[0][i] = ent;		
			}
			fclose(contenido);
		}

		if (dataFileName[0]) {  // comprueba si hay datos internos en un fichero
			contenido = fopen(dataFileName, "rt");
			if (!contenido) {
				fprintf(stderr, "Error abriendo %s\n", dataFileName);
				exit(-1);
			}
			for (int i = 0; i < 512; ++i) {
				fscanf(contenido, "%s", tmp);			cad = tmp;
				while (cad[0] == ' ') ++cad;
				if ((cad[0] == '0') && ((cad[1] == 'x') || (cad[1] == 'X')))
					sscanf(cad, "%x", &ent);
				else
					sscanf(cad, "%d", &ent);
				mem[1][i] = ent;
			}
			fclose(contenido);
		}

		SC_THREAD(internalAccess);
		SC_THREAD(externalAccess);
	}

private:
	sc_int<32> mem[4][512];	// Memoria organizada en 4 "bancos"
	FILE *contenido;
	int indice;
	bool debug; 	

};


#endif