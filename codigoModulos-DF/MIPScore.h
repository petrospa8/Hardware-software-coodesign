#ifndef MIPSCORE_H
#define MIPSCORE_H

#include "systemc.h"
#include "fifo.h"
#include "instructions.h"
#include "mmuMem.h"

#define usarINT
// usarINT con los registros facilita visualizar los valores de los registros
#ifdef usarINT
#define TREG int
#else
#define TREG sc_int<32>
#endif


SC_MODULE (MIPScore) {
public:
	sc_port< read_if_T<sc_int<32>>> memRead;
	sc_port< write_if_T<sc_uint<32>>> addr;
	sc_port< write_if_T<sc_int<32>>> memWrite;
	sc_port< write_if_T<bool>> rW;

	void procesar();
	void setDebug(bool);
	mmuMem *miMem;

	// solo vas a utilizar una
	void codigoL0();
	void codigoL1();
	void codigoFFTgenerica();
	void codigoFiltrar();
	void codigoL0inv();
	void codigoL1inv();

	SC_HAS_PROCESS(MIPScore);

	MIPScore(sc_module_name name_, int idx) : sc_module(name_) {

		cout << "MIPScore: " << name() << endl;

		indice = idx;
		for(int i=0; i<32; ++i)	regs[i] = 0;
		PC = IR = 0;
		CNTS = (int*) &cnt;
		for(int i=0; i<26; ++i) CNTS[i] = 0;	// inicializa los 26 contadores
		debug = false;

		//=============================================
		// elige tu código y conviértelo a ensamblador
		//=============================================

		//SC_THREAD(codigoL0);
		//SC_THREAD(codigoL1);
		//SC_THREAD(codigoFiltrar);
		//SC_THREAD(codigoL0inv);
		//SC_THREAD(codigoL1inv);
		// esta versión sólo se utilizará cuando hayas terminado de generar el código: SC_THREAD(procesar);

	};

private:

	void SW(int reg, int dir);
	void LW(int reg, int dir);

	sc_uint<32> PC;
	sc_int<32> IR;
	TREG regs[32];
	bool debug;
	int indice;

	struct contadores{
	int add, sub, sll, sllv, srl, srlv, sra, srav, mflo, and, or, exor, slt, mul, addi, andi, ori, slti, lui, lw, sw, beq, bne, blez, bgtz, j;
	}; 
	struct contadores cnt;
	int *CNTS;

	#ifdef usarINT
		int RV(int regN) { return regs[regN];}
		unsigned int RVU(int regN) { return (unsigned int)regs[regN];}
	#else
		int RV(int regN) { return regs[regN].to_int(); }
		unsigned int RVU(int regN) { return regs[regN].to_uint(); }
	#endif


};


// macros

#define $0 0
#define $z 0
#define _at 1
#define _v0 2
#define _v1 3
#define _a0 4
#define _a1 5
#define _a2 6
#define _a3 7
#define _t0 8
#define _t1 9
#define _t2 10
#define _t3 11
#define _t4 12
#define _t5 13
#define _t6 14
#define _t7 15
#define _s0 16
#define _s1 17
#define _s2 18
#define _s3 19
#define _s4 20
#define _s5 21
#define _s6 22
#define _s7 23
#define _t8 24
#define _t9 25
#define _k0 26
#define _k1 27
#define _gp 28
#define _sp 29
#define _fp 30
#define _ra 31

#define $zero 0
#define $at 1
#define $v0 2
#define $v1 3
#define $a0 4
#define $a1 5
#define $a2 6
#define $a3 7
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $t5 13
#define $t6 14
#define $t7 15
#define $s0 16
#define $s1 17
#define $s2 18
#define $s3 19
#define $s4 20
#define $s5 21
#define $s6 22
#define $s7 23
#define $t8 24
#define $t9 25
#define $k0 26
#define $k1 27
#define $gp 28
#define $sp 29
#define $fp 30
#define $ra 31

#define Rz regs[0]
#define Rat regs[1]
#define Rv0 regs[2]
#define Rv1 regs[3]
#define Ra0 regs[4]
#define Ra1 regs[5]
#define Ra2 regs[6]
#define Ra3 regs[7]
#define Rt0 regs[8]
#define Rt1 regs[9]
#define Rt2 regs[10]
#define Rt3 regs[11]
#define Rt4 regs[12]
#define Rt5 regs[13]
#define Rt6 regs[14]
#define Rt7 regs[15]
#define Rs0 regs[16]
#define Rs1 regs[17]
#define Rs2 regs[18]
#define Rs3 regs[19]
#define Rs4 regs[20]
#define Rs5 regs[21]
#define Rs6 regs[22]
#define Rs7 regs[23]
#define Rt8 regs[24]
#define Rt9 regs[25]
#define Rk0 regs[26]
#define Rk1 regs[27]
#define Rgp regs[28]
#define Rsp regs[29]
#define Rfp regs[30]
#define Rra regs[31]

#define add(r1, r2, r3) regs[r1]=regs[r2]+regs[r3]
#define sub(r1, r2, r3) regs[r1]=regs[r2]-regs[r3]
#define and(r1, r2, r3) regs[r1]=regs[r2]&regs[r3]
#define or(r1, r2, r3) regs[r1]=regs[r2]|regs[r3]
#define xor(r1, r2, r3) regs[r1]=regs[r2]^regs[r3]
#define slt(r1, r2, r3) regs[r1]=(regs[r2]<regs[r3])?1:0
#define mult r2, r3) LO=regs[r2]+regs[r3]
#define mflo(r1) regs[r1]=LO
#define sll(r1, r2, shamt) regs[r1]=regs[r2]<<shamt
#define srl(r1, r2, shamt) regs[r1]=(regs[r2].to_uint())>>shamt
#define sra(r1, r2, shamt) regs[r1]=regs[r2]>>shamt
#define sllv(r1, r2, r3) regs[r1]=regs[r2]<<regs[r3]
#define srlv(r1, r2, r3) regs[r1]=(regs[r2].to_uint())>>regs[r3]
#define srav(r1, r2, r3) regs[r1]=regs[r2]>>regs[r3]

#define addi(r1, r2, inm) regs[r1]=regs[r2]+((sc_int<16>)inm)
#define andi(r1, r2, inm) regs[r1]=regs[r2]&((sc_int<16>)inm)
#define ori(r1, r2, inm) regs[r1]=regs[r2]|((sc_int<16>)inm)
#define slti(r1, r2, inm) regs[r1]=(regs[r2]<((sc_int<16>)inm))?1:0
#define lui(r1, inm) regs[r1]=inm<<16
#define beq(r1, r2, target) if(regs[r1]==regs[r2])goto target
#define bne(r1, r2, target) if(regs[r1]!=regs[r2])goto target
#define blez(r1, target) if(regs[r1]<=0)goto target
#define bgtz(r1, target) if(regs[r1]>0)goto target
#define j(target) goto target
#endif
