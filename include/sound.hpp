#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "context.hpp"
#include "bus.hpp"
#include "plugins/plugin.hpp"

namespace sound {

    // namespace wave {
    //     float sint(int n, float t) {
    //         // float spc = t / note(n);
    //         return std::sin((t * note(n)) * 2 * M_PI);
    //     }

    //     float sin(int i, int n, int samp) {
    //         float spc = samp / note(n);
    //         return std::sin((i / spc) * 2 * M_PI);
    //     }

    //     float sawt(int n, float t) {
    //         float ival;
    //         return std::modf(t * note(n), &ival);
    //     }

    //     float saw(int i, int n, int samp) {
    //         float spc = samp / note(n);
    //         float ival;
    //         return std::modf(i / spc, &ival);
    //     }

    //     float trit(int n, float t) {
    //         return std::abs(sawt(n, t) - 0.5) * 2;
    //     }

    //     float tri(int i, int n, int samp) {
    //         return std::abs(saw(i, n, samp) - 0.5) * 2;
    //     }

    //     float sqrt(int n, float t, float duty = 0.5) {
    //         return sawt(n, t) > duty;
    //     }

    //     float sqr(int i, int n, int samp, float duty = 0.5) {
    //         return saw(i, n, samp) > duty;
    //     }
    // }

    // using std::shared_ptr;
    // using std::vector;
    // using std::move;

    // TODO: Incomplete, never finished timing, decay or release
    // class Envelope {
    //     float attack;
    //     float decay;
    //     float sustain;
    //     float release;

    //     mutable bool m_active;
    //     mutable float m_gain;

    // public:
    //     Envelope(float attack, float decay, float sustain, float release)
    //         : attack(attack),
    //           decay(decay),
    //           sustain(sustain),
    //           release(release),
    //           m_active(false),
    //           m_gain(0.0) {}

    //     Envelope(const Envelope & other)
    //         : attack(other.attack),
    //           decay(other.decay),
    //           sustain(other.sustain),
    //           release(other.release),
    //           m_active(false),
    //           m_gain(0.0) {}

    //     Envelope(Envelope && other)
    //         : attack(other.attack),
    //           decay(other.decay),
    //           sustain(other.sustain),
    //           release(other.release),
    //           m_active(false),
    //           m_gain(0.0) {}

    //     void start() const {
    //         m_active = true;
    //     }

    //     void end() const {
    //         m_active = false;
    //     }

    //     float gain(float t) const {
    //         if (t <= attack) {
    //             m_gain = lerp(0, 1, t / attack);
    //         }
    //         else if (t <= attack + decay) {
    //             m_gain = lerp(1, sustain, (t - attack) / decay);
    //         }

    //         return m_gain;
    //     }
    // };
}
