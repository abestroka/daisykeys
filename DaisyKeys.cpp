#include "daisysp.h"
#include "daisy_pod.h"

using namespace daisysp;
using namespace daisy;
// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
// Declare a DaisySeed object called hardware

static DaisyPod   pod;
static Oscillator osc;
static AdEnv      ad;

int   wave, mode;
float vibrato, oscFreq;

void Controls();

void NextSamples(float &sig)
{
    float ad_out = ad.Process();
    // vibrato      = lfo.Process();

    // osc.SetFreq(oscFreq + vibrato);
    osc.SetFreq(oscFreq);

    sig = osc.Process();
    // sig = flt.Process(sig);
    sig *= ad_out;
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    Controls();

    for(size_t i = 0; i < size; i += 2)
    {
        float sig;
        NextSamples(sig);

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void){
    // global variables
    float sample_rate;
    oscFreq = 1000.0f;

    //Init everything
    pod.Init();
    pod.SetAudioBlockSize(4);
    sample_rate = pod.AudioSampleRate();
    osc.Init(sample_rate);
    ad.Init(sample_rate);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SAW);
    wave = osc.WAVE_SAW;
    osc.SetFreq(440);
    osc.SetAmp(1);

    //Set envelope parameters
    ad.SetTime(ADENV_SEG_ATTACK, 0.01);
    ad.SetTime(ADENV_SEG_DECAY, .2);
    ad.SetMax(1);
    ad.SetMin(0);
    ad.SetCurve(0.5);

    // start callback
    pod.StartAdc();
    pod.StartAudio(AudioCallback);

    while(1) {}

}


void UpdateButtons()
{
    if(pod.button1.RisingEdge())
    {
        ad.Trigger();
    }

    // if(pod.button2.RisingEdge())
    // {
    //     selfCycle = !selfCycle;
    // }
}

void Controls()
{
    pod.ProcessAnalogControls();
    pod.ProcessDigitalControls();

    // UpdateEncoder();

    // UpdateKnobs();

    // UpdateLeds();

    UpdateButtons();
}