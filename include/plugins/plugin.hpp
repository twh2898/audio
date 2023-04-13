#pragma once

#include <cstddef>
#include <vector>

#include "context.hpp"

namespace sound {
    using std::vector;

    class Plugin {
    public:
        Context & ctx;

        Plugin(Context & ctx) : ctx(ctx) {}

        Plugin(const Plugin & other) = default;
        Plugin(Plugin && other) = default;

        Plugin & operator=(const Plugin & other) = default;
        Plugin & operator=(Plugin && other) = default;

        virtual ~Plugin() {}

        virtual void process(const float * in, float * out, size_t len) = 0;
    };
}
