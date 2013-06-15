//
// lanflashled.cpp
//
// Copyright 2013 by John Pietrzak (jpietrzak8@gmail.com)
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

//
// Note: I've finally discovered some data on the N900's Flash LEDs.
// There's a Maemo.org page:
//
// http://wiki.maemo.org/N900_Hardware_Flash_Torch
//
// And from there, you can get to the data sheet for the ADP1653 device.
//
// If I'm reading it right, the V4L2 intensity settings imply that the
// torch is running at 50 mA (setting 1), while the flash runs from
// 215 mA (setting 12) to 320 mA (setting 19).
//

#include "lanflashled.h"

#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <fcntl.h>

// Error handling stuff:
#include "lanexception.h"
#include <errno.h>
#include <sstream>

// The Flash LEDs are tied into video device 0, along with the camera itself:
#define PATH_TO_FLASH_DEVICE "/dev/video0"

#include <iostream>

LanFlashLED::LanFlashLED()
  : fileDescriptor(-1),
    minTorch(0),
    maxTorch(1),
    torchOn(false),
    minFlash(12),
    maxFlash(19),
    chosenFlash(12),
    minTime(3000),
    maxTime(10000),
    chosenTime(0),
    minIndicator(0),
    maxIndicator(7),
    chosenIndicator(7),
    indicatorOn(false)
{
  openFlashDevice();
}


LanFlashLED::~LanFlashLED()
{
  if (torchOn) toggleTorch();

  if (fileDescriptor >= 0)
  {
    if (close(fileDescriptor) == -1)
    {
      // Failed to close the Flash LED:
      std::stringstream ss;
      ss << "Failed to close flash LED device.\n";
      ss << "Error is " << strerror(errno) << "\n";
      throw LanException(ss.str());
    }
  }
}


void LanFlashLED::toggleTorch()
{
  struct v4l2_control ctrl;

  // Sanity check:
  if (fileDescriptor == -1)
  {
    // Throw an error here?
    return;
  }

  ctrl.id = V4L2_CID_TORCH_INTENSITY;

  if (torchOn)
  {
    // Turn torch off:
    ctrl.value = minTorch;
    torchOn = false;
  }
  else
  {
    // Turn torch on:
    ctrl.value = maxTorch;
    torchOn = true;
  }

  if (ioctl(fileDescriptor, VIDIOC_S_CTRL, &ctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to set torch intensity to " << ctrl.value << "\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }
}


void LanFlashLED::turnTorchOn()
{
  if (!torchOn) toggleTorch();
}


void LanFlashLED::turnTorchOff()
{
  if (torchOn) toggleTorch();
}


int LanFlashLED::getMinFlash()
{
  return minFlash;
}


int LanFlashLED::getMaxFlash()
{
  return maxFlash;
}


int LanFlashLED::getMinTime()
{
  return minTime;
}


int LanFlashLED::getMaxTime()
{
  return maxTime;
}


int LanFlashLED::getChosenTime()
{
  return chosenTime;
}


void LanFlashLED::setFlashBrightness(
  int brightness)
{
  if (brightness < minFlash)
  {
    chosenFlash = minFlash;
  }
  else if (brightness > maxFlash)
  {
    chosenFlash = maxFlash;
  }
  else
  {
    chosenFlash = brightness;
  }
}


void LanFlashLED::setFlashDuration(
  int duration)
{
  if (duration < minTime)
  {
    chosenTime = minTime;
  }
  else if (duration > maxTime)
  {
    chosenTime = maxTime;
  }
  else
  {
    chosenTime = duration;
  }
}


void LanFlashLED::strobe()
{
  if (torchOn) toggleTorch();

  struct v4l2_control ctrl;

  // Sanity check:
  if (fileDescriptor == -1)
  {
    // Throw an error here?
    return;
  }

  ctrl.id = V4L2_CID_FLASH_INTENSITY;
  ctrl.value = chosenFlash;

  if (ioctl(fileDescriptor, VIDIOC_S_CTRL, &ctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to set flash intensity to " << chosenFlash << "\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  ctrl.id = V4L2_CID_FLASH_TIMEOUT;
  // For now, let's be a bit conservative and cut the max time in half:
  ctrl.value = chosenTime;

  if (ioctl(fileDescriptor, VIDIOC_S_CTRL, &ctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to set flash timeout to " << chosenTime << "\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  ctrl.id = V4L2_CID_FLASH_STROBE;

  if (ioctl(fileDescriptor, VIDIOC_S_CTRL, &ctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to strobe flash.\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }
}


void LanFlashLED::toggleIndicator()
{
  if (indicatorOn)
  {
    switchIndicator(minIndicator);
    indicatorOn = false;
  }
  else
  {
    switchIndicator(chosenIndicator);
    indicatorOn = true;
  }
}


void LanFlashLED::turnIndicatorOn()
{
  switchIndicator(chosenIndicator);
  indicatorOn = true;
}


void LanFlashLED::turnIndicatorOff()
{
  switchIndicator(minIndicator);
  indicatorOn = false;
}


void LanFlashLED::setIndicatorBrightnessLevel(
  int brightness)
{
  if (brightness < minIndicator)
  {
    chosenIndicator = minIndicator;
  }
  else if (brightness > maxIndicator)
  {
    chosenIndicator = maxIndicator;
  }
  else
  {
    chosenIndicator = brightness;
  }
}


bool LanFlashLED::ledsCurrentlyLit()
{
  return (torchOn || indicatorOn);
}


void LanFlashLED::swapLEDs()
{
  if (torchOn)
  {
    turnTorchOff();
    turnIndicatorOn();
  }
  else if (indicatorOn)
  {
    turnIndicatorOff();
    turnTorchOn();
  }
}


void LanFlashLED::openFlashDevice()
{
  // Not sure why "O_RDWR", but it seems to be necessary:
  fileDescriptor = open(PATH_TO_FLASH_DEVICE, O_RDWR | O_NONBLOCK, 0);
//  fileDescriptor = open(PATH_TO_FLASH_DEVICE, O_RDWR, 0);

  if (fileDescriptor == -1)
  {
    std::stringstream ss;
    ss << "Failed to connect to " << PATH_TO_FLASH_DEVICE << "\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  // Find out the intensity values for the LED:

  struct v4l2_queryctrl qctrl;

  // Retrieve intensity values for strobe usage:
  qctrl.id = V4L2_CID_FLASH_INTENSITY;

  if (ioctl(fileDescriptor, VIDIOC_QUERYCTRL, &qctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to retrieve flash LED intensity values.\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  minFlash = qctrl.minimum;
  maxFlash = qctrl.maximum;
  chosenFlash = qctrl.minimum;

  // Retrieve timeout values for strobe usage:
  qctrl.id = V4L2_CID_FLASH_TIMEOUT;

  if (ioctl(fileDescriptor, VIDIOC_QUERYCTRL, &qctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to retrieve flash timeout values.\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  minTime = qctrl.minimum;
  maxTime = qctrl.maximum;
  chosenTime = qctrl.maximum / 2;

  // Retrieve intensity values for sustained usage:
  qctrl.id = V4L2_CID_TORCH_INTENSITY;

  if (ioctl(fileDescriptor, VIDIOC_QUERYCTRL, &qctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to retrieve torch LED intensity values.\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  minTorch = qctrl.minimum;
  maxTorch = qctrl.maximum;

  // Does this pick up the indicator LED?
  qctrl.id = V4L2_CID_INDICATOR_INTENSITY;

  if (ioctl(fileDescriptor, VIDIOC_QUERYCTRL, &qctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to retrieve indicator LED intensity values.\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }

  minIndicator = qctrl.minimum;
  maxIndicator = qctrl.maximum;
  chosenIndicator = qctrl.maximum;
}


void LanFlashLED::switchIndicator(
  int brightness)
{
  struct v4l2_control ctrl;

  // Sanity check:
  if (fileDescriptor == -1)
  {
    return;
  }

  ctrl.id = V4L2_CID_INDICATOR_INTENSITY;
  ctrl.value = brightness;

  if (ioctl(fileDescriptor, VIDIOC_S_CTRL, &ctrl) == -1)
  {
    std::stringstream ss;
    ss << "Failed to set indicator intensity to " << ctrl.value << "\n";
    ss << "Error is " << strerror(errno) << "\n";
    throw LanException(ss.str());
  }
}
