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
	void setBKP(bool);
	mmuMem *miMem;

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

		switch(indice){
		case 0: SC_THREAD(codigoL0);	break;
		case 1: SC_THREAD(codigoL1);	break;
		case 2: 
		case 3: 
		case 4: 
		case 5: 
		case 6: 
		case 7: SC_THREAD(codigoFFTgenerica);	break;
		case 8: SC_THREAD(codigoFiltrar);	break;
		case 9:	SC_THREAD(codigoL0inv);	break;
		case 10: SC_THREAD(codigoL1inv);	break;
		case 11: 
		case 12: 
		case 13:
		case 14: 
		case 15: 
		case 16: SC_THREAD(codigoFFTgenerica);	break;
		default:SC_THREAD(procesar); // si no coincide el código procesa el código cargado desde fichero
		};
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

// constantes para calcular la FFT


static long long int dur2[] = { 4096, 2896, 0, -2896 };
static long long int dui2[] = { 0, -2896, -4096, -2896 };
static long long int iur2[] = { 4096, 2896, 0, -2896 };
static long long int iui2[] = { 0, 2896, 4095, 2895 };

static long long int  dur3[] = { 4096, 3784, 2896, 1567, 0, -1567, -2896, -3784 };
static long long int  dui3[] = { 0, -1567, -2896, -3784, -4096, -3784, -2897, -1569 };
static long long int  iur3[] = { 4096, 3784, 2896, 1567, 0, -1566, -2894, -3781 };
static long long int  iui3[] = { 0, 1567, 2895, 3782, 4093, 3781, 2893, 1565 };

static long long int  dur4[] = { 4096, 4017, 3783, 3404, 2894, 2273, 1564, 795, -4, -803, -1571, -2279, -2899, -3408, -3786, -4019 };
static long long int  dui4[] = { 0, -799, -1568, -2276, -2897, -3406, -3784, -4017, -4095, -4016, -3782, -3403, -2893, -2272, -1564, -796 };
static long long int  iur4[] = { 4096, 4017, 3783, 3404, 2894, 2273, 1565, 797, -1, -799, -1566, -2273, -2892, -3400, -3777, -4009 };
static long long int  iui4[] = { 0, 799, 1567, 2274, 2894, 3402, 3779, 4011, 4089, 4009, 3775, 3396, 2887, 2267, 1560, 793 };

static long long int  dur5[] = { 4096, 4076, 4016, 3918, 3782, 3610, 3403, 3163, 2893, 2595, 2272, 1927, 1564, 1186, 796, 399, -2, -403, -800, -1189, -1567, -1930, -2274, -2597, -2895, -3165, -3404, -3611, -3783, -3919, -4017, -4077 };
static long long int  dui5[] = { 0, -401, -799, -1189, -1567, -1930, -2274, -2597, -2894, -3164, -3403, -3609, -3781, -3916, -4013, -4072, -4092, -4072, -4013, -3916, -3781, -3610, -3404, -3165, -2896, -2599, -2277, -1933, -1571, -1193, -804, -407 };
static long long int  iur5[] = { 4096, 4076, 4016, 3918, 3782, 3610, 3403, 3164, 2894, 2597, 2275, 1931, 1568, 1190, 801, 404, 4, -396, -792, -1181, -1558, -1920, -2263, -2585, -2882, -3151, -3389, -3595, -3766, -3901, -3998, -4057 };
static long long int  iui5[] = { 0, 401, 798, 1187, 1564, 1926, 2270, 2592, 2889, 3158, 3396, 3602, 3773, 3908, 4005, 4063, 4082, 4062, 4003, 3905, 3770, 3599, 3393, 3154, 2885, 2588, 2266, 1923, 1561, 1184, 796, 400 };

static long long int  dur6[] = { 4096, 4091, 4076, 4051, 4016, 3972, 3918, 3855, 3783, 3702, 3612, 3513, 3406, 3291, 3168, 3037, 2899, 2754, 2602, 2444, 2280, 2111, 1937, 1758, 1575, 1388, 1198, 1005, 809, 612, 413, 213, 13, -188, -388, -587, -785, -981, -1175, -1366, -1554, -1738, -1918, -2093, -2263, -2428, -2587, -2740, -2886, -3025, -3157, -3282, -3399, -3508, -3608, -3700, -3783, -3857, -3922, -3978, -4024, -4061, -4088, -4105 };
static long long int  dui6[] = { 0, -200, -400, -599, -797, -993, -1186, -1376, -1563, -1746, -1925, -2100, -2269, -2433, -2591, -2743, -2888, -3027, -3158, -3282, -3398, -3506, -3605, -3696, -3778, -3851, -3915, -3969, -4014, -4049, -4074, -4090, -4096, -4092, -4078, -4055, -4022, -3979, -3927, -3865, -3794, -3714, -3625, -3527, -3421, -3307, -3185, -3055, -2918, -2774, -2623, -2466, -2303, -2135, -1962, -1784, -1602, -1416, -1226, -1033, -838, -641, -442, -242 };
static long long int  iur6[] = { 4096, 4091, 4076, 4051, 4016, 3972, 3918, 3855, 3783, 3702, 3612, 3514, 3407, 3292, 3169, 3039, 2901, 2757, 2606, 2449, 2286, 2118, 1945, 1767, 1585, 1399, 1210, 1018, 824, 628, 430, 231, 32, -167, -366, -564, -761, -956, -1148, -1338, -1525, -1708, -1887, -2061, -2230, -2394, -2552, -2704, -2850, -2989, -3121, -3245, -3361, -3469, -3569, -3660, -3743, -3817, -3882, -3937, -3983, -4019, -4046, -4063 };
static long long int  iui6[] = { 0, 200, 399, 597, 794, 989, 1181, 1370, 1556, 1738, 1916, 2090, 2259, 2422, 2579, 2730, 2875, 3013, 3143, 3266, 3381, 3488, 3587, 3677, 3758, 3830, 3893, 3947, 3991, 4026, 4051, 4067, 4073, 4069, 4055, 4032, 3999, 3956, 3904, 3843, 3772, 3692, 3604, 3507, 3402, 3288, 3167, 3038, 2902, 2759, 2609, 2453, 2291, 2124, 1952, 1775, 1594, 1409, 1220, 1028, 834, 638, 440, 241 };

static long long int  dur7[] = { 4096, 4094, 4089, 4082, 4072, 4060, 4045, 4028, 4008, 3986, 3962, 3935, 3906, 3875, 3841, 3805, 3767, 3727, 3684, 3639, 3592, 3543, 3492, 3439, 3384, 3327, 3268, 3207, 3144, 3079, 3012, 2943, 2873, 2801, 2727, 2652, 2575, 2497, 2417, 2336, 2253, 2169, 2084, 1997, 1909, 1820, 1730, 1639, 1547, 1454, 1360, 1266, 1171, 1075, 979, 882, 784, 686, 588, 489, 390, 291, 192, 93, -6, -105, -204, -303, -402, -501, -599, -697, -795, -892, -988, -1084, -1179, -1273, -1367, -1460, -1552, -1643, -1733, -1822, -1910, -1997, -2082, -2166, -2249, -2331, -2411, -2490, -2567, -2643, -2717, -2789, -2860, -2929, -2996, -3061, -3125, -3187, -3247, -3305, -3361, -3415, -3467, -3517, -3565, -3610, -3653, -3694, -3733, -3770, -3804, -3836, -3866, -3894, -3919, -3942, -3963, -3981, -3997, -4011, -4022, -4031, -4037, -4041 };
static long long int  dui7[] = { 0, -100, -200, -300, -400, -500, -599, -698, -796, -894, -991, -1088, -1184, -1279, -1373, -1467, -1560, -1652, -1743, -1833, -1921, -2008, -2094, -2179, -2262, -2344, -2425, -2504, -2582, -2658, -2732, -2805, -2876, -2945, -3012, -3078, -3142, -3204, -3264, -3322, -3378, -3432, -3484, -3534, -3582, -3627, -3670, -3711, -3750, -3786, -3820, -3852, -3882, -3909, -3934, -3956, -3976, -3994, -4009, -4022, -4032, -4040, -4046, -4049, -4050, -4048, -4044, -4038, -4029, -4018, -4004, -3988, -3970, -3949, -3926, -3900, -3872, -3842, -3810, -3775, -3738, -3699, -3658, -3614, -3568, -3520, -3470, -3418, -3364, -3308, -3250, -3190, -3128, -3064, -2998, -2931, -2862, -2791, -2719, -2645, -2569, -2492, -2413, -2333, -2252, -2169, -2085, -2000, -1914, -1827, -1738, -1648, -1558, -1467, -1375, -1282, -1188, -1094, -999, -903, -807, -710, -613, -516, -418, -320, -222, -124 };
static long long int  iur7[] = { 4096, 4094, 4089, 4082, 4072, 4060, 4045, 4028, 4009, 3987, 3963, 3937, 3908, 3877, 3844, 3808, 3770, 3730, 3688, 3644, 3597, 3548, 3497, 3444, 3389, 3332, 3273, 3212, 3149, 3084, 3018, 2950, 2880, 2809, 2736, 2661, 2585, 2507, 2428, 2347, 2265, 2182, 2097, 2011, 1924, 1836, 1747, 1657, 1566, 1474, 1381, 1288, 1194, 1099, 1004, 908, 812, 715, 618, 520, 422, 324, 226, 128, 30, -68, -166, -264, -362, -460, -557, -654, -750, -846, -941, -1036, -1130, -1223, -1316, -1408, -1499, -1589, -1678, -1766, -1853, -1939, -2024, -2107, -2189, -2270, -2349, -2427, -2503, -2578, -2651, -2723, -2793, -2861, -2927, -2992, -3055, -3116, -3175, -3232, -3287, -3340, -3391, -3440, -3487, -3532, -3575, -3616, -3654, -3690, -3724, -3756, -3785, -3812, -3837, -3860, -3880, -3898, -3914, -3927, -3938, -3947, -3953, -3957 };
static long long int  iui7[] = { 0, 100, 199, 298, 397, 496, 594, 692, 790, 887, 983, 1079, 1174, 1268, 1362, 1455, 1547, 1638, 1728, 1817, 1905, 1991, 2076, 2160, 2243, 2324, 2404, 2482, 2559, 2634, 2708, 2780, 2850, 2918, 2985, 3050, 3113, 3174, 3233, 3290, 3345, 3398, 3449, 3498, 3545, 3590, 3633, 3673, 3711, 3747, 3781, 3812, 3841, 3868, 3892, 3914, 3934, 3951, 3966, 3979, 3989, 3997, 4002, 4005, 4006, 4004, 4000, 3993, 3984, 3973, 3959, 3943, 3925, 3904, 3881, 3856, 3828, 3798, 3766, 3732, 3695, 3656, 3615, 3572, 3527, 3480, 3430, 3378, 3324, 3268, 3210, 3151, 3090, 3027, 2962, 2895, 2827, 2757, 2685, 2612, 2537, 2461, 2383, 2304, 2223, 2141, 2058, 1974, 1889, 1802, 1714, 1625, 1535, 1445, 1354, 1262, 1169, 1076, 982, 887, 792, 696, 600, 504, 407, 310, 213, 116 };

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
