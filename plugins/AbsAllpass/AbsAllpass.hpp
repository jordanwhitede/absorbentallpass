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

    // Member variables
    // sample rate? delay time, coefficient, ffreq
};

} // namespace AbsAllpass
