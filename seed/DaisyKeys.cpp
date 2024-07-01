#include "daisysp.h"
#include "daisy_pod.h"

#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;
// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
// Declare a DaisySeed object called hardware

static DaisyPod   pod;
static DaisySeed hw;
static Oscillator osc;
static AdEnv      ad;

int   wave, mode;
float vibrato, oscFreq;

Switch C;
Switch D;
Switch E;
Switch F;
Switch G;
Switch A;
Switch B;
Switch C2;

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

    // button3.Debounce();
    // hw.SetLed(button3.Pressed());

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

    C2.Init(hw.GetPin(7), 1000);
    B.Init(hw.GetPin(8), 1000);
    A.Init(hw.GetPin(9), 1000);
    G.Init(hw.GetPin(10), 1000);
    F.Init(hw.GetPin(11), 1000);
    E.Init(hw.GetPin(12), 1000);
    D.Init(hw.GetPin(22), 1000);
    C.Init(hw.GetPin(16), 1000);


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

    C.Debounce();
    D.Debounce();
    E.Debounce();
    F.Debounce();
    G.Debounce();
    A.Debounce();
    B.Debounce();
    C2.Debounce();

    // if(pod.button1.RisingEdge())
    // {
    //     oscFreq = 440.0f;
    //     ad.Trigger();
    // }

    // if(pod.button2.RisingEdge())
    // {
    //     oscFreq = 220.0f;
    //     ad.Trigger();
    // }
    if(C.RisingEdge())
    {

        // oscFreq = 261.63f;
        oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(D.RisingEdge())
    {

        oscFreq = 293.66f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(E.RisingEdge())
    {

        oscFreq = 329.63f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(F.RisingEdge())
    {

        // oscFreq = 349.23f;
        oscFreq = 369.99f; // F#

        ad.Trigger();
        // hw.SetLed(true);
    }
    if(G.RisingEdge())
    {

        oscFreq = 392.0f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(A.RisingEdge())
    {

        oscFreq = 440.0f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(B.RisingEdge())
    {

        // oscFreq = 493.88f;
            oscFreq = 246.94f; //dropped octave

        ad.Trigger();
        // hw.SetLed(true);
    }
    if(C2.RisingEdge())
    {

        oscFreq = 523.25f;
        ad.Trigger();
        // hw.SetLed(true);
    }

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