// PluginAbsAllpass.hpp
// Jordan White (jordanwhitede@gmail.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace AbsAllpass {

class AbsAllpass : public SCUnit {
public:
    AbsAllpass();
    ~AbsAllpass();

private:
    // Calc function
    void next(int nSamples);
    void clear(int inNumSamples);

    // Member variables
  float* historybuffer;
  float* delaybuffer;
  int bufsize;
  int mask;
  int writephase;
  float maxdelay;
  float prevOutputLPIn;
  float prevOutputLPOut;
  float freq_past;



};

} // namespace AbsAllpass
