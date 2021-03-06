#pragma once

#include "ThreadQueue.h"
#include "CubicSDRDefs.h"
#include "liquid/liquid.h"

#include <atomic>
#include <mutex>

#include "IOThread.h"

class DemodulatorThread;
class DemodulatorThreadCommand {
public:
    enum DemodulatorThreadCommandEnum {
        DEMOD_THREAD_CMD_NULL,
        DEMOD_THREAD_CMD_DEMOD_PREPROCESS_TERMINATED,
        DEMOD_THREAD_CMD_DEMOD_TERMINATED,
        DEMOD_THREAD_CMD_AUDIO_TERMINATED
    };

    DemodulatorThreadCommand() :
            cmd(DEMOD_THREAD_CMD_NULL), context(NULL), llong_value(0) {

    }

    DemodulatorThreadCommand(DemodulatorThreadCommandEnum cmd) :
            cmd(cmd), context(NULL), llong_value(0) {

    }

    DemodulatorThreadCommandEnum cmd;
    void *context;
    long long llong_value;
};

class DemodulatorThreadControlCommand {
public:
    enum DemodulatorThreadControlCommandEnum {
        DEMOD_THREAD_CMD_CTL_NULL, DEMOD_THREAD_CMD_CTL_SQUELCH_ON, DEMOD_THREAD_CMD_CTL_SQUELCH_OFF, DEMOD_THREAD_CMD_CTL_TYPE
    };

    DemodulatorThreadControlCommand() :
            cmd(DEMOD_THREAD_CMD_CTL_NULL), demodType("") {
    }

    DemodulatorThreadControlCommandEnum cmd;
    std::string demodType;
};

class DemodulatorThreadIQData: public ReferenceCounter {
public:
    long long frequency;
    long long sampleRate;
    std::vector<liquid_float_complex> data;
    std::mutex busy_rw;

    DemodulatorThreadIQData() :
            frequency(0), sampleRate(0) {

    }

    DemodulatorThreadIQData & operator=(const DemodulatorThreadIQData &other) {
        frequency = other.frequency;
        sampleRate = other.sampleRate;
        data.assign(other.data.begin(), other.data.end());
        return *this;
    }

    ~DemodulatorThreadIQData() {

    }
};

class Modem;
class ModemKit;

class DemodulatorThreadPostIQData: public ReferenceCounter {
public:
    std::vector<liquid_float_complex> data;
    long long sampleRate;
    std::string modemName;
    std::string modemType;
    Modem *modem;
    ModemKit *modemKit;

    DemodulatorThreadPostIQData() :
            sampleRate(0), modem(nullptr), modemKit(nullptr) {

    }

    ~DemodulatorThreadPostIQData() {
        std::lock_guard < std::mutex > lock(m_mutex);
    }
};


class DemodulatorThreadAudioData: public ReferenceCounter {
public:
    long long frequency;
    unsigned int sampleRate;
    unsigned char channels;

    std::vector<float> *data;

    DemodulatorThreadAudioData() :
            frequency(0), sampleRate(0), channels(0), data(NULL) {

    }

    DemodulatorThreadAudioData(long long frequency, unsigned int sampleRate, std::vector<float> *data) :
            frequency(frequency), sampleRate(sampleRate), channels(1), data(data) {

    }

    ~DemodulatorThreadAudioData() {

    }
};

typedef ThreadQueue<DemodulatorThreadIQData *> DemodulatorThreadInputQueue;
typedef ThreadQueue<DemodulatorThreadPostIQData *> DemodulatorThreadPostInputQueue;
typedef ThreadQueue<DemodulatorThreadCommand> DemodulatorThreadCommandQueue;
typedef ThreadQueue<DemodulatorThreadControlCommand> DemodulatorThreadControlCommandQueue;
