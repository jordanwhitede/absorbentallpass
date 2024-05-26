// PluginAbsAllpass.cpp
// Jordan White (jordanwhitede@gmail.com)

#include "SC_PlugIn.hpp"
#include "AbsAllpass.hpp"
#include <cmath>

static InterfaceTable* ft;

namespace AbsAllpass {

AbsAllpass::AbsAllpass() {
    mCalcFunc = make_calc_function<AbsAllpass, &AbsAllpass::next>();
    freq_past = in0(3);
    prevOutputLPIn = 0.0f;
    prevOutputLPOut = 0.0f;
    writephase = 0;
    maxdelay = 1.0;
    bufsize = NEXTPOWEROFTWO((float)sampleRate() * maxdelay);

    delaybuffer = (float *)RTAlloc(mWorld, bufsize * sizeof(float));
    historybuffer = (float *)RTAlloc(mWorld, bufsize * sizeof(float));

    // used to wrap the phase using bit masking
    mask = bufsize - 1;

    // check success of RTAlloc
    if (delaybuffer == NULL || historybuffer == NULL) {

        mCalcFunc = make_calc_function<AbsAllpass, &AbsAllpass::clear>();

        // clear outputs to be safe
        clear(1);

        if (mWorld->mVerbosity -2) {
            Print("Failed to allocate memory for AbsAllpass Ugen.\n");
        }
        return;
    }

    // fill buffers with zeroes
    memset(delaybuffer, 0, bufsize * sizeof(float));
    memset(historybuffer, 0, bufsize * sizeof(float));

    next(1);
}

void AbsAllpass::clear(int inNumSamples) {
    ClearUnitOutputs(this, inNumSamples);
}

    AbsAllpass::~AbsAllpass() {
        RTFree(mWorld, delaybuffer);
        RTFree(mWorld, historybuffer);
}

void AbsAllpass::next(int nSamples) {

    // making slope for freq interpolation in case i ever want this
    // and also to learn how to do it
    SlopeSignal<float> slopedFreq = makeSlope(in0(3), freq_past);

    // ins and outs
    const float* input = in(0);
    float *output = out(0);

    float delay = in0(1);
    // this was const float, i am trying to see if it's ok for it to be just float
    float gain = in0(2);
    // taking this out to try the slope
    const float freq = slopedFreq.consume();
    // float freq = in0(3);


    // cap delay
    if (delay > maxdelay) {
        delay = maxdelay;
        Print("Delay Time capped at 1s");
    }

    // calculate delay phase
    int delayPhaseOffset = delay * (float) sampleRate();

    // calculate low-pass coefficient
    float tau = 1.0f / (2.0f * M_PI * freq);
    float alpha = sampleRate() / (tau + sampleRate());

    /* the question is how to implement the lpf and where.
     * how will end up being a class of its own.
     * where - directly after the delay for BOTH signals. - done
     * this means i have to think about how the history buffer works */
    // the actual comb function
    for (int i = 0; i < nSamples; ++i) {

        historybuffer[writephase] = input[i];

        const float in = input[i];
        const int historyphase = writephase - delayPhaseOffset;
        const int delayphase = writephase - delayPhaseOffset;
        const float delayedInput = historybuffer[historyphase & mask];
        const float delayedOutput = delaybuffer[delayphase & mask];
        const float lowpassInput = alpha * delayedInput + (1.0f - alpha) * prevOutputLPIn;
        const float lowpassOutput = alpha * delayedOutput + (1.0f - alpha) * prevOutputLPOut;
        const float filtered = (-gain * in) + lowpassInput + (gain * lowpassOutput);

                output[i] = zapgremlins(filtered);


                delaybuffer[writephase] = output[i];
                prevOutputLPIn = lowpassInput;
                prevOutputLPOut = lowpassOutput;
                writephase = (writephase + 1) & mask;
                freq_past = slopedFreq.value;
    }
}

} // namespace AbsAllpass

PluginLoad(AbsAllpassUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<AbsAllpass::AbsAllpass>(ft, "AbsAllpass", false);
}
