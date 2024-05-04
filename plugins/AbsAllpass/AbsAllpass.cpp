// PluginAbsAllpass.cpp
// Jordan White (jordanwhitede@gmail.com)

#include "SC_PlugIn.hpp"
#include "AbsAllpass.hpp"

static InterfaceTable* ft;

namespace AbsAllpass {

AbsAllpass::AbsAllpass() {
    mCalcFunc = make_calc_function<AbsAllpass, &AbsAllpass::next>();
    next(1);
}

void AbsAllpass::next(int nSamples) {

    // Audio rate input
    const float* input = in(0);

    // Control rate parameter: gain.
    const float gain = in0(1);

    // Output buffer
    float* outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

} // namespace AbsAllpass

PluginLoad(AbsAllpassUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<AbsAllpass::AbsAllpass>(ft, "AbsAllpass", false);
}
