#include "daisysp.h"
#include "daisy_seed.h"

using namespace daisysp;
using namespace daisy;
// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
// Declare a DaisySeed object called hardware

static DaisySeed hw;
static Oscillator osc1, osc2, osc3;
static AdEnv      ad;
static AdcChannelConfig adcConfig[3];
static AnalogControl pot1, pot2, pot3;



int   mode;
float vibrato, oscFreq, octave;

Switch octave_up; 
Switch octave_down;

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

void NextSamples(float &sig, float &sig1, float &sig2, float &sig3, float &pot_val_1, float &pot_val_2, float &pot_val_3)
{
    float ad_out = ad.Process();
    // vibrato      = lfo.Process();

    // osc.SetFreq(oscFreq + vibrato);
    osc1.SetFreq(oscFreq);
    osc2.SetFreq(oscFreq);
    osc3.SetFreq(oscFreq);

    sig1 = osc1.Process();
    sig2 = osc2.Process();
    sig3 = osc3.Process();

    // sig = flt.Process(sig);
    sig = (sig1 * pot_val_1) + (sig2 * pot_val_2) + (sig3 * pot_val_3);
    sig *= ad_out;
}

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    Controls();

    float pot_val_1 = pot1.Process(); //pot 1 val
    float pot_val_2 = pot2.Process(); //pot 2 val
    float pot_val_3 = pot3.Process(); //pot 3 val


    for(size_t i = 0; i < size; i += 2)
    {
        float sig;
        float sig1;
        float sig2;
        float sig3;
        NextSamples(sig, sig1, sig2, sig3, pot_val_1, pot_val_2, pot_val_3);

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
    octave = 1.0f;

    //Init everything
    hw.Configure();
    hw.Init();
    hw.SetAudioBlockSize(4);

    //pot1
    adcConfig[0].InitSingle(hw.GetPin(28));
    //pot2
    adcConfig[1].InitSingle(hw.GetPin(27));
    //pot3
    adcConfig[2].InitSingle(hw.GetPin(26));

    //init pots
    hw.adc.Init(adcConfig, 3);


    sample_rate = hw.AudioSampleRate();
    osc1.Init(sample_rate);
    osc2.Init(sample_rate);
    osc3.Init(sample_rate);
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

    octave_up.Init(hw.GetPin(29), 1000);
    octave_down.Init(hw.GetPin(30), 1000);

    pot1.Init(hw.adc.GetPtr(1), 1000);
    pot2.Init(hw.adc.GetPtr(2), 1000);
    pot3.Init(hw.adc.GetPtr(3), 1000);


    // Set parameters for oscillators
    osc1.SetWaveform(Oscillator::WAVE_SIN);
    // wave = osc.WAVE_SAW;
    osc1.SetFreq(440);
    osc1.SetAmp(1);

    osc2.SetWaveform(Oscillator::WAVE_SAW);
    // wave = osc.WAVE_SAW;
    osc2.SetFreq(440);
    osc2.SetAmp(1);

    osc3.SetWaveform(Oscillator::WAVE_SQUARE);
    // wave = osc.WAVE_SAW;
    osc3.SetFreq(440);
    osc3.SetAmp(1);

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
    octave_up.Debounce();
    octave_down.Debounce();

     if(octave_up.RisingEdge())
    {
        octave = octave * 2;
        // hw.SetLed(true);
    }
    if(octave_down.RisingEdge())
    {

        octave = octave / 2;

        // hw.SetLed(true);
    }   
    if(C.RisingEdge())
    {

        oscFreq = 261.63f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(CS.RisingEdge())
    {

        oscFreq = 277.18f * octave;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(D.RisingEdge())
    {

        oscFreq = 293.66f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(DS.RisingEdge())
    {

        oscFreq = 311.13f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(E.RisingEdge())
    {

        oscFreq = 329.63f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(F.RisingEdge())
    {

        oscFreq = 349.23f * octave;
        // oscFreq = 369.99f; // F#

        ad.Trigger();
        // hw.SetLed(true);
    }
    if(FS.RisingEdge())
    {

        oscFreq = 369.99f * octave;
        // oscFreq = 277.18f; //C#
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(G.RisingEdge())
    {

        oscFreq = 392.0f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(GS.RisingEdge())
    {

        oscFreq = 415.30f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(A.RisingEdge())
    {

        oscFreq = 440.0f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(AS.RisingEdge())
    {

        oscFreq = 466.16f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(B.RisingEdge())
    {

        oscFreq = 493.88f * octave;
        ad.Trigger();
        // hw.SetLed(true);
    }
    if(C2.RisingEdge())
    {

        oscFreq = 523.25f * octave;
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