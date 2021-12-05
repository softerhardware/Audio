/* Teensy support for wm8960 audio codec
 * Copyright (c) 2021, Steve Haynal, steve@softerhardware.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef control_wm8960_h_
#define control_wm8960_h_

#include "AudioControl.h"

class AudioControlWM8960 : public AudioControl
{
public:
    bool enable(void);
    bool disable(void) { return false; }
    bool volume(float n) { return false; } //{ return volumeInteger(n * 80.0f + 47.499f); }
    bool inputLevel(float n) { return false; } // range: 0.0f to 1.0f
    bool inputSelect(int n) { return false; }
    //bool volumeInteger(unsigned int n); // range: 0x2F to 0x7F

    bool leftInputVolume(unsigned int v);
    bool rightInputVolume(unsigned int v);
    bool leftInputMute(unsigned int v);
    bool rightInputMute(unsigned int v);
    bool leftInputZC(unsigned int v);
    bool rightInputZC(unsigned int v);
    bool leftHeadphoneVolume(unsigned int v);
    bool rightHeadphoneVolume(unsigned int v);
    bool leftHeadphoneZC(unsigned int v);
    bool rightHeadphoneZC(unsigned int v);
    bool enableDAC6dBAttenuate(unsigned int v);
    bool muteDAC(unsigned int v);
    bool enableDeemphasis(unsigned int v);
    bool enableADCHPF(unsigned int v);
    bool leftDACVolume(unsigned int v);
    bool rightDACVolume(unsigned int v);
    bool leftADCVolume(unsigned int v);
    bool rightADCVolume(unsigned int v);
    bool leftInputPower(unsigned int v);
    bool rightInputPower(unsigned int v);
    bool leftADCPower(unsigned int v);
    bool rightADCPower(unsigned int v);
    bool micBiasPower(unsigned int v);
    bool leftDACPower(unsigned int v);
    bool rightDACPower(unsigned int v);
    bool leftHeadphonePower(unsigned int v);
    bool rightHeadphonePower(unsigned int v);
    bool leftSpeakerPower(unsigned int v);
    bool rightSpeakerPower(unsigned int v);

protected:
    bool write(uint16_t reg, uint16_t val, uint16_t mask);
    // wm8960 state is write only, this keeps track of state and is initialized with defaults
    uint16_t regmap[56] = {
        0x097, 0x097, 0x000, 0x000, 0x000, 0x008, 0x000, 0x00a, // Registers 0x00-0x07
        0x1c0, 0x000, 0x0ff, 0x0ff, 0x000, 0x000, 0x000, 0x000, // Registers 0x08-0x0f
        0x000, 0x00b, 0x100, 0x032, 0x000, 0x0c3, 0x0c3, 0x1c0, // Registers 0x10-0x17
        0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, // Registers 0x18-0x1f
        0x100, 0x100, 0x050, 0x050, 0x050, 0x050, 0x000, 0x000, // Registers 0x20-0x27
        0x100, 0x100, 0x040, 0x000, 0x000, 0x050, 0x050, 0x000, // Registers 0x28-0x2f
        0x002, 0x037, 0x04d, 0x080, 0x008, 0x031, 0x026, 0x0e9  // Registers 0x30-0x37
    };
};

#endif
