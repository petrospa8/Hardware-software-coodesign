#include "systemc.h"

SC_MODULE(lb_comp) {
    public:
    sc_in<sc_int<32>> salidaMem;
    sc_in<sc_uint<6>> type;
    sc_in<sc_uint<2>> displ;

    sc_out<sc_int<32>> salidaLb;

    void calcular();

    SC_CTOR(lb_comp) {
        cout << "lb_comp: " << name() << endl;

        SC_METHOD(calcular);
        sensitive << salidaMem << type << displ;
    }
};