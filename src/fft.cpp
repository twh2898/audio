#include "fft.hpp"

// https://rosettacode.org/wiki/Fast_Fourier_transform#C++
namespace sound {
    using std::polar;
    using std::slice;

    void fftSimple(CArray & arr) {
        const auto N = arr.size();
        if (N == 0)
            return;

        CArray even = arr[slice(0, N / 2, 2)];
        CArray odd = arr[slice(1, N / 2, 2)];

        fftSimple(even);
        fftSimple(odd);

        for (size_t k = 0; k < N / 2; k++) {
            Complex t = polar(1.0, -2 * M_PI * k / N);
            arr[k] = even[k] + t;
            arr[k + N / 2] = even[k] - t;
        }
    }

    void fft(CArray & arr) {
        size_t N = arr.size(), k = N, n;
        double thetaT = M_PI / N;
        Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
        while (k > 1) {
            n = k;
            k >>= 1;
            phiT = phiT * phiT;
            T = 1.0;
            for (size_t l = 0; l < k; l++) {
                for (size_t a = l; a < N; a += n) {
                    size_t b = a + k;
                    Complex t = arr[a] - arr[b];
                    arr[a] += arr[b];
                    arr[b] = t * T;
                }
                T *= phiT;
            }
        }

        size_t m = log2(N);
        for (size_t a = 0; a < N; a++) {
            size_t b = a;

            // Reverse bits
            b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
            b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
            b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
            b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
            b = ((b >> 16) | (b << 16)) >> (32 - m);

            if (b > a) {
                Complex t = arr[a];
                arr[a] = arr[b];
                arr[b] = t;
            }
        }
    }

    void ifft(CArray & arr) {
        arr = arr.apply(std::conj);
        fft(arr);
        arr = arr.apply(std::conj);
        arr /= arr.size();
    }
}
