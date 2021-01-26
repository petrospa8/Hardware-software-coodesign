#include "lb_comp.h"

void lb_comp::calcular() {
    sc_int<32> tmp = (salidaMem.read() >> (displ.read() << 3)) & 0xff;

    if (tmp & 0x80 && type.read() == 0x20) {
        tmp |= 0xffffff00;
    }

    salidaLb.write(tmp);
}