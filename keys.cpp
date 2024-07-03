#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;
// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
// Declare a DaisySeed object called hardware

static DaisySeed hw;
static Oscillator osc;
static AdEnv      ad;

int   wave, mode;
float vibrato, oscFreq;

Switch C;
Switch CS;
Switch D;
Switch DS;
Switch E;
Switch F;
Switch FS;
Switch G;
Switch GS;
Switch A;
Switch AS;
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
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);

    sample_rate = hw.AudioSampleRate();
    osc.Init(sample_rate);
    ad.Init(sample_rate);

    C2.Init(hw.GetPin(13), 1000);
    B.Init(hw.GetPin(12), 1000);
    AS.Init(hw.GetPin(11), 1000);
    A.Init(hw.GetPin(10), 1000);
    GS.Init(hw.GetPin(9), 1000);
    G.Init(hw.GetPin(8), 1000);
    FS.Init(hw.GetPin(7), 1000);
    F.Init(hw.GetPin(6), 1000);
    E.Init(hw.GetPin(5), 1000);
    DS.Init(hw.GetPin(4), 1000);
    D.Init(hw.GetPin(3), 1000);
    CS.Init(hw.GetPin(2), 1000);
    C.Init(hw.GetPin(1), 1000);


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


    // Start the adc
    // hw.adc.Start();

    //Start calling the audio callback
    hw.StartAudio(AudioCallback);


    while(1) {}

}


void UpdateButtons()
{

    C.Debounce();
    CS.Debounce();
    D.Debounce();
    DS.Debounce();
    E.Debounce();
    F.Debounce();
    FS.Debounce();
    G.Debounce();
    GS.Debounce();
    A.Debounce();
    AS.Debounce();
    B.Debounce();
    C2.Debounce();

    if(C.RisingEdge())
    {

        oscFreq = 261.63f;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(CS.RisingEdge())
    {

        oscFreq = 277.18f;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(D.RisingEdge())
    {

        oscFreq = 293.66f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(DS.RisingEdge())
    {

        oscFreq = 311.13f;
        // oscFreq = 277.18f; //C#
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

        oscFreq = 349.23f;
        // oscFreq = 369.99f; // F#

        ad.Trigger();
        // hw.SetLed(true);
    }
    if(FS.RisingEdge())
    {

        oscFreq = 369.99f;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(G.RisingEdge())
    {

        oscFreq = 392.0f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(GS.RisingEdge())
    {

        oscFreq = 415.30f;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(A.RisingEdge())
    {

        oscFreq = 440.0f;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(AS.RisingEdge())
    {

        oscFreq = 466.16f;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(B.RisingEdge())
    {

        oscFreq = 493.88f;
            // oscFreq = 246.94f; //dropped octave

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
    // pod.ProcessAnalogControls();
    // pod.ProcessDigitalControls();

    // UpdateEncoder();

    // UpdateKnobs();

    // UpdateLeds();

    UpdateButtons();
}