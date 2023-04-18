#pragma once

#include <complex>
#include <valarray>

namespace sound {
    using Complex = std::complex<double>;
    using CArray = std::valarray<Complex>;

    void fftSimple(CArray & arr);

    void fft(CArray & arr);

    void ifft(CArray & arr);
}
