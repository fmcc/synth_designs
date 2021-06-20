#include <Audio.h>
#include <Wire.h>
#include <MIDI.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       Osc1;      //xy=326,169
AudioSynthWaveform       SubOsc; //xy=328,273
AudioSynthWaveform       Osc2; //xy=328,313
AudioSynthWaveform       SineSubOsc;      //xy=343,233
AudioEffectMultiply      multiply1;      //xy=526,232
AudioMixer4              Mixer2;         //xy=701,264
AudioMixer4              Mixer1;         //xy=702,194
AudioMixer4              mixer3;         //xy=866,243
AudioEffectEnvelope      AmpEnvelope;      //xy=1072,225
AudioOutputMQS           mqs1;           //xy=1261,239
AudioConnection          patchCord1(Osc1, 0, Mixer1, 0);
AudioConnection          patchCord2(Osc1, 0, multiply1, 0);
AudioConnection          patchCord3(SubOsc, 0, Mixer1, 2);
AudioConnection          patchCord4(SubOsc, 0, Mixer2, 2);
AudioConnection          patchCord5(Osc2, 0, Mixer2, 0);
AudioConnection          patchCord6(Osc2, 0, multiply1, 1);
AudioConnection          patchCord7(SineSubOsc, 0, Mixer1, 1);
AudioConnection          patchCord8(SineSubOsc, 0, Mixer2, 1);
AudioConnection          patchCord9(multiply1, 0, Mixer1, 3);
AudioConnection          patchCord10(multiply1, 0, Mixer2, 3);
AudioConnection          patchCord11(Mixer2, 0, mixer3, 1);
AudioConnection          patchCord12(Mixer1, 0, mixer3, 0);
AudioConnection          patchCord13(mixer3, AmpEnvelope);
AudioConnection          patchCord14(AmpEnvelope, 0, mqs1, 0);
// GUItool: end automatically generated code


const float note_frequencies[128] = {8.176, 8.662, 9.177, 9.723, 10.301, 10.913, 11.562, 12.25, 12.978, 13.75, 14.568, 15.434, 16.352, 17.324, 18.354, 19.445, 20.602, 21.827, 23.125, 24.5, 25.957, 27.5, 29.135, 30.868, 32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55, 58.27, 61.735, 65.406, 69.296, 73.416, 77.782, 82.407, 87.307, 92.499, 97.999, 103.826, 110, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883, 523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767, 1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533, 2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 3322.438, 3520, 3729.31, 3951.066, 4186.009, 4434.922, 4698.636, 4978.032, 5274.041, 5587.652, 5919.911, 6271.927, 6644.875, 7040, 7458.62, 7902.133, 8372.018, 8869.844, 9397.273, 9956.063, 10548.08, 11175.3, 11839.82, 12543.85};

const int octaves[7] = {12, 24, 36, 48, 60, 72, 84};
const int intervals[7] = {-7, -5, -3, 0, 3, 5, 7};
const float volumes[6] = {0, 0.1, 0.2, 0.3, 0.4, 0.5};
const int waveforms[6] = {
  WAVEFORM_SINE,
  WAVEFORM_SAWTOOTH,
  WAVEFORM_SAWTOOTH_REVERSE,
  WAVEFORM_SQUARE,
  WAVEFORM_TRIANGLE,
  WAVEFORM_PULSE,
};

const int sleng_teng[14] = {0, 1, 1, 1, 0, 1, 1, 1, 0, 1,1,1,5,4};
const int sleng_teng_note_len[14] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1,1,2,2};

byte OSC1_RANGE_PIN = 14; 
byte OSC1_WAVEFORM_PIN = 15; 
byte OSC2_RANGE_PIN = 16; 
byte OSC2_WAVEFORM_PIN = 17; 
byte OSC2_INTERVAL_PIN = 18; 
byte ATTACK_PIN = 19; 
byte DECAY_PIN = 20; 
byte SUSTAIN_PIN = 21; 
byte RELEASE_PIN = 22; 


int led = 13;
byte NOTE_1_PIN = 7; 
byte NOTE_2_PIN = 8; 

byte SUBOSC_PIN = 3; 
byte RINGMOD_PIN = 4; 
byte OTHER_PIN = 5; 

bool note_on = false;
int note_offset = 0; 

int osc1_range_value; 
int osc1_waveform_value; 
int osc2_range_value; 
int osc2_waveform_value; 
int osc2_interval_value; 
int attack_value; 
int decay_value; 
float sustain_value; 
int release_value; 

float subosc_gain = 0.5; 
// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  pinMode(NOTE_1_PIN, INPUT_PULLUP);
  pinMode(NOTE_2_PIN, INPUT_PULLUP);
  pinMode(SUBOSC_PIN, INPUT_PULLUP);
  pinMode(RINGMOD_PIN, INPUT_PULLUP);


  AudioNoInterrupts();
  // audio library init
  AudioMemory(15);
  Osc1.begin(0.8, 440, WAVEFORM_SAWTOOTH);  
  Osc2.begin(0.8, 441, WAVEFORM_SAWTOOTH);
  SubOsc.begin(0.8, 439, WAVEFORM_SAWTOOTH);
  SineSubOsc.begin(0.8, 439, WAVEFORM_SINE);
  AudioInterrupts();
}

// the loop routine runs over and over again forever:
void loop() {
osc1_range_value = octaves[map(analogRead(OSC1_RANGE_PIN), 0, 1023, 0, 6)];
osc1_waveform_value = waveforms[map(analogRead(OSC1_WAVEFORM_PIN), 0, 1023, 0, 5)];
osc2_range_value = octaves[map(analogRead(OSC2_RANGE_PIN), 0, 1023, 0, 6)];
osc2_waveform_value = waveforms[map(analogRead(OSC2_WAVEFORM_PIN), 0, 1023, 0, 5)];
osc2_interval_value = intervals[map(analogRead(OSC2_INTERVAL_PIN), 0, 1023, 0, 6)];
attack_value = map(analogRead(ATTACK_PIN), 0, 1023, 0, 1000);
decay_value = map(analogRead(DECAY_PIN), 0, 1023, 0, 5000);
sustain_value = map(analogRead(SUSTAIN_PIN), 0, 1023, 0.0, 1.0);
release_value = map(analogRead(RELEASE_PIN), 0, 1023, 0, 10000);

  if (digitalRead(NOTE_1_PIN) == LOW) {
    note_offset = 0;
    note_on = true; 
  } 
  if (digitalRead(NOTE_2_PIN) == LOW) {
    note_offset += 1;
    note_on = true; 
  }

  if (digitalRead(SUBOSC_PIN) == LOW) {
    Mixer1.gain(1, 0.5);
    Mixer2.gain(1, 0.5);
    Mixer1.gain(2, 0.5);
    Mixer2.gain(2, 0.5);
  } else {
    Mixer1.gain(1, 0);
    Mixer2.gain(1, 0);
    Mixer1.gain(2, 0);
    Mixer2.gain(2, 0);
  } 
  if (digitalRead(RINGMOD_PIN) == LOW) {
    Mixer1.gain(3, 0.5);
    Mixer2.gain(3, 0.5);
    Mixer1.gain(0, 0);
    Mixer2.gain(0, 0);
  } else {
    Mixer1.gain(0, 0.5);
    Mixer2.gain(0, 0.5);
    Mixer1.gain(3, 0);
    Mixer2.gain(3, 0);
  }
  
  for (int i=0; i<14; i++) {
    note_offset = sleng_teng[i];
    Osc1.begin(0.5, note_frequencies[osc1_range_value+note_offset], osc1_waveform_value);
    SubOsc.begin(0.5, note_frequencies[osc1_range_value+note_offset-24], osc1_waveform_value);
    SineSubOsc.begin(0.5, note_frequencies[osc1_range_value+note_offset-24], WAVEFORM_SINE);
    Osc2.begin(0.5, note_frequencies[osc2_range_value+note_offset+osc2_interval_value], osc2_waveform_value);
    AmpEnvelope.attack(attack_value);
    AmpEnvelope.decay(decay_value);
    AmpEnvelope.sustain(sustain_value);
    AmpEnvelope.release(release_value); 


    AmpEnvelope.noteOn();
    delay(100*sleng_teng_note_len[i]);
    AmpEnvelope.noteOff();
    delay(200);
  } 
  note_on = false; 
}
