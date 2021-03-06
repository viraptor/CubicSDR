#include "ModemFM.h"

ModemFM::ModemFM() : ModemAnalog() {
    demodFM = freqdem_create(0.5);
}

ModemFM::~ModemFM() {
    freqdem_destroy(demodFM);
}

Modem *ModemFM::factory() {
    return new ModemFM;
}

std::string ModemFM::getName() {
    return "FM";
}

int ModemFM::getDefaultSampleRate() {
    return 200000;
}

void ModemFM::demodulate(ModemKit *kit, ModemIQData *input, AudioThreadInput *audioOut) {
    ModemKitAnalog *fmkit = (ModemKitAnalog *)kit;
    
    initOutputBuffers(fmkit, input);
    
    if (!bufSize) {
        input->decRefCount();
        return;
    }
    
    freqdem_demodulate_block(demodFM, &input->data[0], bufSize, &demodOutputData[0]);

    buildAudioOutput(fmkit, audioOut, false);
}
