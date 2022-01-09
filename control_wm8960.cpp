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

#include <Arduino.h>
#include "control_wm8960.h"
#include "Wire.h"

// See https://blog.titanwolf.in/a?ID=00500-80a77412-7973-49b3-b2e9-bc1beb847257


#define WM8960_I2C_ADDR 0x1A


bool AudioControlWM8960::enable(void)
{
    Wire.begin();
    delay(5);

    // Reset
    if (!write(0x0f, 0, 0b01)) {
        return false; // no WM8960 chip responding
    }

    delay(5);

    // Enable VMID and VREF
    write(0x19, 0x1c0, 0b111000000);

    // Enable DAC and Headphones
    write(0x1a, 0x1e0, 0b111100000);

    // Enable mixer
    write(0x2f, 0x0c,0x0c);

    // Setup clocking
    write(0x04, 0x00, 0b111111111);

    // Unmute
    muteDAC(0);

    // 16-bit data and i2s interface
    write(0x07, 0x02, 0b000001111); // I2S, 16 bit, MCLK slave

    leftHeadphoneVolume(0xff);

    rightHeadphoneVolume(0xff);

    leftDACVolume(0xff);

    rightDACVolume(0xff);

    // Connect Left DAC to left output mixer
    write(0x22,0x100,0x100);
    delay(5);

    // Connect Right DAC to right output mixer
    write(0x25,0x100,0x100);


    delay(100); // how long to power up?


    return true;
}

bool AudioControlWM8960::volume(float n) {
    unsigned int i;
    i = 47 + (int) (80.0*n+0.5);
    leftHeadphoneVolume(i);
    rightHeadphoneVolume(i);
}


// mask set bits are the bits in val which should be written
bool AudioControlWM8960::write(uint16_t reg, uint16_t val, uint16_t mask)
{

    uint16_t newval;

    newval = (regmap[reg] & ~mask) | (val & mask);
    regmap[reg] = newval;

    Wire.beginTransmission(WM8960_I2C_ADDR);

    Wire.write((reg << 1) | ((newval >> 8) & 1));
    Wire.write(newval & 0xFF);

    if (Wire.endTransmission() == 0) return true;
    return false;

}


// 6 bits, 111111 is +30dB down to 000000 -17.25dB, 0.75dB steps
bool AudioControlWM8960::leftInputVolume(unsigned int v)
{
    return write(0x00, 0b100000000 | (v & 0b0111111), 0b100111111);
}

bool AudioControlWM8960::rightInputVolume(unsigned int v)
{
    return write(0x01, 0b100000000 | (v & 0b0111111), 0b100111111);
}

// Write 1 to mute
bool AudioControlWM8960::leftInputMute(unsigned int v)
{
    return write(0x00, (v & 0b01) << 7, 0b110000000);
}

bool AudioControlWM8960::rightInputMute(unsigned int v)
{
    return write(0x01, (v & 0b01) << 7, 0b110000000);
}

// Write 1 to enable change on zero crossing only
bool AudioControlWM8960::leftInputZC(unsigned int v)
{
    return write(0x00, (v & 0b01) << 6, 0b101000000);
}

bool AudioControlWM8960::rightInputZC(unsigned int v)
{
    return write(0x01, (v & 0b01) << 6, 0b101000000);
}

// 7 bits, 1111111 is +6dB down to 0000000 -73dB, 1dB steps
bool AudioControlWM8960::leftHeadphoneVolume(unsigned int v)
{
    return write(0x02, 0b100000000 | (v & 0b01111111), 0b101111111);
}

bool AudioControlWM8960::rightHeadphoneVolume(unsigned int v)
{
    return write(0x03, 0b100000000 | (v & 0b01111111), 0b101111111);
}

// Write 1 to enable change on zero crossing only
bool AudioControlWM8960::leftHeadphoneZC(unsigned int v)
{
    return write(0x02, (v & 0b01) << 7, 0b110000000);
}

bool AudioControlWM8960::rightHeadphoneZC(unsigned int v)
{
    return write(0x03, (v & 0b01) << 7, 0b110000000);
}

// Write 1 to enable
bool AudioControlWM8960::enableDAC6dBAttenuate(unsigned int v)
{
    return write(0x05, (v & 0xb01) << 7, 0b010000000);
}

// Write 1 to mute
bool AudioControlWM8960::muteDAC(unsigned int v)
{
    return write(0x05, (v & 0xb01) << 3, 0b000001000);
}

// Write 1 to enable
bool AudioControlWM8960::enableDeemphasis(unsigned int v)
{
    if (v) {
        return write(0x05, 0b011, 0b011);
    } else {
        return write(0x05, 0b000, 0b011);
    }
}

// Write 1 to enable
bool AudioControlWM8960::enableADCHPF(unsigned int v)
{
    return write(0x05, v & 0b01, 0b01);
}


// 8 bits, 11111111 is 0dB down to 00000001 -127dB, 0.5dB steps, 00000000 is mute
bool AudioControlWM8960::leftDACVolume(unsigned int v)
{
    return write(0x0a, 0b100000000 | (v & 0b011111111), 0b111111111);
}

bool AudioControlWM8960::rightDACVolume(unsigned int v)
{
    return write(0x0b, 0b100000000 | (v & 0b011111111), 0b111111111);
}


// 8 bits, 11111111 is +30dB down to 00000001 -97dB, 0.5dB steps, 00000000 is mute
bool AudioControlWM8960::leftADCVolume(unsigned int v)
{
    return write(0x15, 0b100000000 | (v & 0b011111111), 0b111111111);
}

bool AudioControlWM8960::rightADCVolume(unsigned int v)
{
    return write(0x16, 0b100000000 | (v & 0b011111111), 0b111111111);
}

bool AudioControlWM8960::leftInputPower(unsigned int v)
{
    return write(0x19, (v & 0b01) << 5, 0b01 << 5);
}

bool AudioControlWM8960::rightInputPower(unsigned int v)
{
    return write(0x19, (v & 0b01) << 4, 0b01 << 4);
}

bool AudioControlWM8960::leftADCPower(unsigned int v)
{
    return write(0x19, (v & 0b01) << 3, 0b01 << 3);
}

bool AudioControlWM8960::rightADCPower(unsigned int v)
{
    return write(0x19, (v & 0b01) << 2, 0b01 << 2);
}

bool AudioControlWM8960::micBiasPower(unsigned int v)
{
    return write(0x19, (v & 0b01) << 1, 0b01 << 1);
}

bool AudioControlWM8960::leftDACPower(unsigned int v)
{
    return write(0x1a, (v & 0b01) << 8, 0b01 << 8);
}

bool AudioControlWM8960::rightDACPower(unsigned int v)
{
    return write(0x1a, (v & 0b01) << 7, 0b01 << 7);
}

bool AudioControlWM8960::leftHeadphonePower(unsigned int v)
{
    return write(0x1a, (v & 0b01) << 6, 0b01 << 6);
}

bool AudioControlWM8960::rightHeadphonePower(unsigned int v)
{
    return write(0x1a, (v & 0b01) << 5, 0b01 << 5);
}

bool AudioControlWM8960::leftSpeakerPower(unsigned int v)
{
    return write(0x1a, (v & 0b01) << 4, 0b01 << 4);
}

bool AudioControlWM8960::rightSpeakerPower(unsigned int v)
{
    return write(0x1a, (v & 0b01) << 3, 0b01 << 3);
}
