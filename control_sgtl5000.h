#ifndef control_sgtl5000_h_
#define control_sgtl5000_h_

#include "AudioControl.h"

class AudioControlSGTL5000 : public AudioControl
{
public:
	bool enable(void);
	bool disable(void) { return false; }
	bool volume(float n) { return volumeInteger(n * 1.29 + 0.499); }
	bool inputLevel(float n) {return false;}
	bool muteHeadphone(void) { return write(0x0024, ana_ctrl | (1<<4)); }
	bool unmuteHeadphone(void) { return write(0x0024, ana_ctrl & ~(1<<4)); }
	bool muteLineout(void) { return write(0x0024, ana_ctrl | (1<<8)); }
	bool unmuteLineout(void) { return write(0x0024, ana_ctrl & ~(1<<8)); }
	bool inputSelect(int n) {
		if (n == AUDIO_INPUT_LINEIN) {
			return write(0x0024, ana_ctrl | (1<<2));
		} else if (n == AUDIO_INPUT_MIC) {
			//return write(0x002A, 0x0172) && write(0x0024, ana_ctrl & ~(1<<2));
			return write(0x002A, 0x0173) && write(0x0024, ana_ctrl & ~(1<<2)); // +40dB
		} else {
			return false;
		}
	}
	//bool inputLinein(void) { return write(0x0024, ana_ctrl | (1<<2)); }
	//bool inputMic(void) { return write(0x002A, 0x0172) && write(0x0024, ana_ctrl & ~(1<<2)); }

	bool volume(float left, float right);
	unsigned short micGain(unsigned int n) { return modify(0x002A, n&3, 3); }
	unsigned short lo_lvl(uint8_t n);
	unsigned short lo_lvl(uint8_t left, uint8_t right);
	unsigned short dac_vol(float n);
	unsigned short dac_vol(float left, float right);
	unsigned short dap_mix_enable(uint8_t n);
	unsigned short dap_enable(uint8_t n);
	unsigned short dap_enable(void);
	unsigned short dap_peqs(uint8_t n);
	unsigned short dap_audio_eq(uint8_t n);
	unsigned short dap_audio_eq_band(uint8_t bandNum, float n);
	void dap_audio_eq_geq(float bass, float mid_bass, float midrange, float mid_treble, float treble);
	void dap_audio_eq_tone(float bass, float treble);
	void load_peq(uint8_t filterNum, int *filterParameters);
	
	
protected:
	bool muted;
	bool volumeInteger(unsigned int n); // range: 0x00 to 0x80
	uint16_t ana_ctrl;

	unsigned char calcVol(float n, unsigned char range);

	unsigned int read(unsigned int reg);
	bool write(unsigned int reg, unsigned int val);
	unsigned int modify(unsigned int reg, unsigned int val, unsigned int iMask);
};

//For Filter Type: 0 = LPF, 1 = HPF, 2 = BPF, 3 = NOTCH, 4 = PeakingEQ, 5 = LowShelf, 6 = HighShelf
  #define FILTER_LOPASS 0
  #define FILTER_HIPASS 1
  #define FILTER_BANDPASS 2
  #define FILTER_NOTCH 3
  #define FILTER_PARAEQ 4
  #define FILTER_LOSHELF 5
  #define FILTER_HISHELF 6

void calcBiquad(uint8_t filtertype, float fC, float dB_Gain, float Q, uint32_t quantization_unit, uint32_t fS, int *coef);

#endif
