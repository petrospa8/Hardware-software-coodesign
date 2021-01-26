#ifndef MIPSDF_H
#define MIPSDF_H

#include "systemc.h"

#include "fifo.h"
#include "MIPScore.h"
#include "mmuMem.h"

SC_MODULE (MIPSdf) {
public:
	sc_port< read_if_T<sc_int<32>>> extDataIn;
	sc_port< read_if_T<sc_uint<32>>> extAddrIn;

	sc_port< write_if_T<sc_int<32>>> extDataOut;
	sc_port< write_if_T<sc_uint<32>>> extAddrOut;

	sc_port< read_if_T<sc_int<32>>> inputLeft, inputRight, inputExt;
	sc_port< write_if_T<sc_int<32>>> outpLeft, outpRight;


	void setDebug(bool onOff){ instMIPScore->setDebug(onOff); instMmuMem->setDebug(onOff); };

	SC_HAS_PROCESS(MIPSdf);

	MIPSdf(sc_module_name name_, int idx, char* codeFileName, char* dataFileName) : sc_module(name_) {

		cout << "MIPSdf: " << name() << "  " << codeFileName << " " << dataFileName << endl;

		instMIPScore = new MIPScore("instMIPScore", idx);
		instMmuMem = new mmuMem("instMmuMem", idx, codeFileName, dataFileName);

		indice = idx;
		instMIPScore->miMem = instMmuMem;

		QmemRead	= new fifo_T<sc_int<32>>("QmemRead", 1);
		Qaddr		= new fifo_T<sc_uint<32>>("Qaddr", 1);
		QmemWrite	= new fifo_T<sc_int<32>>("QmemWrite", 1);
		QrW			= new fifo_T<bool>("QrW", 1);

		instMIPScore->	memRead( *QmemRead );
		instMIPScore->	addr( *Qaddr );
		instMIPScore->	memWrite( *QmemWrite );
		instMIPScore->	rW( *QrW );

		instMmuMem->	dataOut( *QmemRead );
		instMmuMem->	addr( *Qaddr );
		instMmuMem->	dataIn( *QmemWrite );
		instMmuMem->	rW( *QrW );
		instMmuMem->	extDataIn( extDataIn );
		instMmuMem->	extAddrIn( extAddrIn );
		instMmuMem->	extDataOut( extDataOut );
		instMmuMem->	extAddrOut( extAddrOut );
		instMmuMem->	inputLeft( inputLeft );
		instMmuMem->	inputRight( inputRight );
		instMmuMem->	outpLeft( outpLeft );
		instMmuMem->	outpRight( outpRight );
		instMmuMem->	inputExt( inputExt );

	};

private:

MIPScore *instMIPScore;
mmuMem *instMmuMem;
int indice;

fifo_T<sc_uint<32>> *Qaddr;
fifo_T<sc_int<32>> *QmemRead, *QmemWrite;
fifo_T<bool> *QrW;

};


#endif