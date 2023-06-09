#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
using namespace std;

#include <AL/al.h>
#include <AL/alc.h>

#include <cstring>

#include "fft.hpp"
#include "plugins/delay.hpp"
#include "plugins/gain.hpp"
#include "plugins/oscillator.hpp"
#include "sound.hpp"
using namespace sound;
using namespace sound::plugins;

bool error() {
    ALCenum error;
    error = alcGetError(NULL);
    if (error != AL_NO_ERROR)
        cout << "AL Error: " << error << endl;

    return error != AL_NO_ERROR;
}

bool can_enumerate() {
    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    error();
    return enumeration == AL_TRUE;
}

void list_devices() {
    const ALchar * devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    error();
    const ALchar * device = devices;
    const ALchar * next = devices + 1;
    size_t len = 0;

    cout << "Device list:" << endl;
    cout << "-----------" << endl;

    while (device && *device != '\0' && next && *next != '\0') {
        cout << "- " << device << endl;
        len = strlen(device);
        device += len + 1;
        next += len + 2;
    }
}

void init_listener() {
    cout << "Initializing listener" << endl;

    ALfloat listener_pose[] = {0, 0, 1, 0, 1, 0};

    alListener3f(AL_POSITION, 0, 0, 1);
    error();

    alListener3f(AL_VELOCITY, 0, 0, 0);
    error();

    alListenerfv(AL_ORIENTATION, listener_pose);
    error();
}

ALuint create_source() {
    cout << "Create source" << endl;

    ALuint source;

    alGenSources(1, &source);
    error();

    alSourcef(source, AL_PITCH, 1);
    error();

    alSourcef(source, AL_GAIN, 1);
    error();

    alSource3f(source, AL_POSITION, 0, 0, 0);
    error();

    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    error();

    alSourcei(source, AL_LOOPING, AL_FALSE);
    error();

    return source;
}

void load_audio(ALuint buffer) {
    cout << "Loading audio" << endl;

    size_t freq = 44100;
    Context ctx(freq);
    Bus b1(ctx);

    Oscillator osc(ctx, Oscillator::Sin);
    b1.addSource(osc);
    osc.active = true;
    osc.note = 0;
    osc.gain = 0.8;

    Oscillator osc2(ctx, Oscillator::Saw);
    b1.addSource(osc2);
    osc2.active = true;
    osc2.note = 3;
    osc2.gain = 0.5;

    Oscillator osc3(ctx, Oscillator::Square);
    b1.addSource(osc3);
    osc3.active = true;
    osc3.note = 5;
    osc3.gain = 0.3;

    Gain gain(ctx, 0.3);
    b1.addEffect(gain);

    Delay delay(ctx, freq * 0.1, 0.1, 0.3);
    b1.addEffect(delay);

    b1.process();

    auto & buff = b1.getBuffer();

    util::dumpWave("../../testwave.csv", buff.data(), buff.size());

    vector<uint8_t> quantize(buff.size(), 0);
    for (int i = 0; i < buff.size(); i++) {
        quantize[i] = (buff[i] / 2.0 + 0.5) * 128.0;
    }

    std::vector<Complex> b2;
    for (auto v : buff) {
        b2.emplace_back(v);
    }
    CArray data(b2.data(), b2.size());
    fft(data);

    std::cout << "fft" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << data[i] << std::endl;
    }

    util::dumpWave("../../quantize.csv", quantize.data(), quantize.size());

    alBufferData(buffer, AL_FORMAT_MONO8, quantize.data(), quantize.size(), freq);
    error();
}

static void fftDemo() {
    const Complex test[] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
    CArray data(test, 8);

    // forward fft
    fft(data);

    std::cout << "fft" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << data[i] << std::endl;
    }

    // inverse fft
    ifft(data);

    std::cout << std::endl << "ifft" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << data[i] << std::endl;
    }
}

int main() {
    fftDemo();

    ALCdevice * device;

    device = alcOpenDevice(NULL);
    error();
    if (!device) {
        cout << "Could not open device" << endl;
        return 1;
    }

    if (can_enumerate()) {
        list_devices();
    }
    else {
        cout << "ALC cannot enumerate devices" << endl;
    }

    ALCcontext * context;
    context = alcCreateContext(device, NULL);
    error();
    if (!context) {
        cout << "Could not create context" << endl;
        return 1;
    }

    if (!alcMakeContextCurrent(context)) {
        cout << "Failed to make context current" << endl;
        error();
        return 1;
    }
    error();

    init_listener();

    ALuint source = create_source();
    if (!source) {
        cout << "Failed to create source" << endl;
        return 1;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    if (!buffer) {
        cout << "Failed to create buffer" << endl;
        error();
        return 1;
    }
    error();

    load_audio(buffer);

    alSourcei(source, AL_BUFFER, buffer);
    error();

    alSourcePlay(source);
    error();

    ALint source_state;
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    error();
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        error();
    }

    // cleanup context
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}
