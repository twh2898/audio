#include <cmath>
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

#include <AL/al.h>
#include <AL/alc.h>

#include <cstring>

#include "sound.hpp"
using namespace sound;

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

    ofstream of("../single.csv");

    size_t freq = 48000;
    size_t size = freq * 1;
    float amp = 0.8;
    char data[size];
    for (int i = 0; i < size; i++) {
        float t = (float)i / freq;
        float s = wave::sqrt(0, t) + wave::trit(3, t) + wave::sint(7, t);
        // float s = wave::sqr(i, 0, freq) + wave::tri(i, 3, freq)
        //           + wave::sin(i, 7, freq);
        // data[i] = 128 + (128 * amp * s / 3.0);
        // data[i] = 128 + (128 * amp * dcomp(s, 16));
        data[i] = 128 + (128 * amp * scomp(s));
        of << (s / 3.0) << ',' << dcomp(s, 3) << ',' << scomp(s) << endl;
        // if (i < freq / 2) {
        //     data[i] = 128 + (64 * wave::sqr(i, 0, freq, 0.5));
        // }
        // else {
        //     float s = wave::sin(i, 0, freq) + wave::sin(i, 3, freq)
        //               + wave::sin(i, 7, freq);
        //     data[i] = 128 + (64 * s / 3.0);
        //     // data[i] = 128 + sin(i / M_PI) * (64 - (64 * i / freq));
        // }
    }

    of.close();

    alBufferData(buffer, AL_FORMAT_MONO8, data, size, freq);
    error();
}

int main() {
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
