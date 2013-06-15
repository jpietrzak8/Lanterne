//
// lanflashled.h
//
// Copyright 2013 by John Pietrzak  (jpietrzak8@gmail.com)
//
// This file is part of Lanterne.
//
// Lanterne is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Lanterne is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Lanterne; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef LANFLASHLED_H
#define LANFLASHLED_H

//
// Support for engaging the N900's camera flash LEDs; this is used in
// conjunction with the IR camera shutter controls, to allow the N900 to
// be used as a "remote flash".
//

class LanFlashLED
{
public:
  LanFlashLED();

  ~LanFlashLED();

  // Torch controls:
  void toggleTorch();
  void turnTorchOn();
  void turnTorchOff();

  // Flash strobe controls:
  int getMinFlash();
  int getMaxFlash();
  int getMinTime();
  int getMaxTime();
  int getChosenTime();

  void setFlashBrightness(
    int brightness);

  void setFlashDuration(
    int duration);

  void strobe();

  // Indicator controls:
  void toggleIndicator();
  void turnIndicatorOn();
  void turnIndicatorOff();
  void setIndicatorBrightnessLevel(
    int brightness);

  bool ledsCurrentlyLit();
  void swapLEDs();

private:
  void openFlashDevice();

  void switchIndicator(
    int brightness);

  int fileDescriptor;

  int minTorch;
  int maxTorch;
  bool torchOn;

  int minFlash;
  int maxFlash;
  int chosenFlash;

  int minTime;
  int maxTime;
  int chosenTime;

  int minIndicator;
  int maxIndicator;
  int chosenIndicator;
  bool indicatorOn;
};

#endif // LANFLASHLED_H
