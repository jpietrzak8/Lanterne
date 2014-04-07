//
// mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "landbus.h"

#include <QTimer>

class LanFlashLED;
class LanAboutForm;
class LanStrobeForm;
class LanMorseForm;
class LanPreferencesForm;
class LanLightSensorForm;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  enum ScreenOrientation {
    ScreenOrientationLockPortrait,
    ScreenOrientationLockLandscape,
    ScreenOrientationAuto
  };

  explicit MainWindow(
    QWidget *parent = 0);

  virtual ~MainWindow();

  // Note that this will only have an effect on Symbian and Fremantle.
  void setOrientation(ScreenOrientation orientation);

  void showExpanded();

//  int getMinTorch();
//  int getMaxTorch();
//  void setTorchBrightness(int arg1);
  void toggleTorch();
  void turnTorchOn();
  void turnTorchOff();

  int getMinFlash();
  int getMaxFlash();
  int getMinTime();
  int getMaxTime();
  int getChosenTime();
  void setFlashBrightness(int arg1);
  void setFlashDuration(int arg1);

  void setUseTorchButtonAsMorseKey(
    bool useTBAMK);

  void setIgnoreCameraCover(
    bool ignore);

  void setUseCameraButton(
    bool useCB);

  void useIndicatorLED(
    bool useILED);

  void setIndicatorBrightnessLevel(
    int brightness);

  void setCoverLocksScreen(
    bool cls);

  void setCoverClosesApp(
    bool cca);

  void setEnableOffTimer(
    bool enabled);

  void setOffTimerDuration(
    int duration);

  void setFlashAfterTimeout(
    bool flag);

  int dotDuration();

public slots:
  void strobe();

  void updateCameraCover(
    bool closed);

  void updateCameraButton(
    bool pressed);

private slots:
  void on_actionStrobe_Flash_triggered();
  void on_actionPreferences_triggered();
  void on_actionAbout_triggered();
  void on_actionLight_Sensor_triggered();

  void on_whiteButton_clicked();
  void on_redButton_clicked();
  void on_torchButton_clicked();
  void on_torchButton_pressed();
  void on_torchButton_released();
//  void on_strobeButton_clicked();
  void on_morseButton_clicked();
  void on_sosButton_clicked();
  void on_torchContinuousButton_clicked();
//  void on_supressLEDButton_clicked();

  void torchTimeout();

private:
  void setupOffTimer();

  LanFlashLED *led;
  LanAboutForm *aboutForm;
  LanStrobeForm *strobeForm;
  LanMorseForm *morseForm;
  LanLightSensorForm *lightSensorForm;
  LanPreferencesForm *preferencesForm;

  bool loopRunning;

  bool useTorchButtonAsMorseKey;

  LanDBus dbus;
  bool cameraCoverClosed;
  bool ignoreCameraCover;

  bool useIndicatorLEDAsTorch;

  bool coverLocksScreen;
  bool coverClosesApp;

  bool useCameraButton;

  bool useOffTimer;
  int offTimerDuration;
  QTimer offTimer;
  bool flashAfterTimeout;

  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
