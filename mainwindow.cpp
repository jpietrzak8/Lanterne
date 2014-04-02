//
// mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#include "lanflashled.h"
#include "lanaboutform.h"
#include "lanstrobeform.h"
#include "lanmorseform.h"
#include "lanpreferencesform.h"
#include <QTimer>
#include <QDesktopWidget>
#include <QSettings>
#include <QMaemo5InformationBox>

//#include <iostream>

MainWindow::MainWindow(
  QWidget *parent)
  : QMainWindow(parent),
    led(0),
    aboutForm(0),
    strobeForm(0),
    morseForm(0),
    preferencesForm(0),
    loopRunning(false),
    useTorchButtonAsMorseKey(false),
    cameraCoverClosed(true),
    ignoreCameraCover(false),
    useIndicatorLEDAsTorch(false),
    coverLocksScreen(false),
    coverClosesApp(false),
    useCameraButton(false),
    useOffTimer(false),
    flashAfterTimeout(false),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);

  led = new LanFlashLED();

  // Set up the timer:
  connect(
    &offTimer,
    SIGNAL(timeout()),
    this,
    SLOT(torchTimeout()));

  // Set up the user preferences (along with the window):
  preferencesForm = new LanPreferencesForm(this);

  // Set up DBus camera cover monitoring:
  connect(
    &dbus,
    SIGNAL(cameraCoverChanged(bool)),
    this,
    SLOT(updateCameraCover(bool)));

  // Initialize the current camera cover status:
  cameraCoverClosed = dbus.currentCameraCoverStatus();

  // Set up DBus camera button monitoring:
  connect(
    &dbus,
    SIGNAL(cameraButtonChanged(bool)),
    this,
    SLOT(updateCameraButton(bool)));

  // Attempt to fix a problem with buttons not working during rotation:
  strobeForm = new LanStrobeForm(this);
  connect(
    QApplication::desktop(),
    SIGNAL(resized(int)),
    strobeForm,
    SLOT(stopStrobe()));

  // If the user has chosen auto startup, turn on the LEDs now:
  if (preferencesForm->startupAutomatically())
  {
    switch (preferencesForm->getStartupMode())
    {
    case SteadyOn_Mode:
      if (ignoreCameraCover || !cameraCoverClosed) turnTorchOn();
      break;

    case Pulsed_Mode:
      on_torchContinuousButton_clicked();
      break;

    case SOS_Mode:
      on_sosButton_clicked();
      break;

    default:
      // Do nothing here.
      break;
    }
  }

  QSettings settings("pietrzak.org", "Lanterne");

  if (!settings.contains("BrightLightWarningSeen"))
  {
    QMaemo5InformationBox::information(
      0,
      "WARNING: The N900 Flash LEDs are very bright -- avoid looking directly at them!",
      0);
  }

  if (settings.contains("UseIndicatorLEDAsTorch"))
  {
    bool uilat = settings.value("UseIndicatorLEDAsTorch").toBool();

    ui->whiteButton->setChecked(!uilat);
    ui->redButton->setChecked(uilat);

    useIndicatorLED(uilat);
  }
  else
  {
    ui->whiteButton->setChecked(true);
  }
}


MainWindow::~MainWindow()
{
  QSettings settings("pietrzak.org", "Lanterne");
  if (!settings.contains("BrightLightWarningSeen"))
  {
    settings.setValue("BrightLightWarningSeen", true);
  }

  settings.setValue("UseIndicatorLEDAsTorch", ui->redButton->isChecked());

  if (aboutForm) delete aboutForm;
  if (strobeForm) delete strobeForm;
  if (morseForm) delete morseForm;
  if (preferencesForm) delete preferencesForm;

  if (led) delete led;

  delete ui;
}


void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}


void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}


void MainWindow::toggleTorch()
{
  // Do nothing if camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (useIndicatorLEDAsTorch)
  {
    led->toggleIndicator();
  }
  else
  {
    led->toggleTorch();
  }

  if (led->ledsCurrentlyLit())
  {
    setupOffTimer();
  }
  else
  {
    offTimer.stop();
  }
}


void MainWindow::turnTorchOn()
{
  // Do nothing if camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (useIndicatorLEDAsTorch)
  {
    led->turnIndicatorOn();
  }
  else
  {
    led->turnTorchOn();
  }

  setupOffTimer();
}


void MainWindow::turnTorchOff()
{
  led->turnIndicatorOff();
  led->turnTorchOff();
  offTimer.stop();
}


int MainWindow::getMinFlash()
{
  return led->getMinFlash();
}


int MainWindow::getMaxFlash()
{
  return led->getMaxFlash();
}


int MainWindow::getMinTime()
{
  return led->getMinTime();
}


int MainWindow::getMaxTime()
{
  return led->getMaxTime();
}


int MainWindow::getChosenTime()
{
  return led->getChosenTime();
}


void MainWindow::setFlashBrightness(int arg1)
{
  led->setFlashBrightness(arg1);
}


void MainWindow::setFlashDuration(int arg1)
{
  led->setFlashDuration(arg1);
}


void MainWindow::setUseTorchButtonAsMorseKey(
  bool useTBAMK)
{
  useTorchButtonAsMorseKey = useTBAMK;
}


void MainWindow::setIgnoreCameraCover(
  bool ignore)
{
  ignoreCameraCover = ignore;
}


void MainWindow::setUseCameraButton(
  bool useCB)
{
  useCameraButton = useCB;
}


void MainWindow::useIndicatorLED(
  bool useILED)
{
  if (useIndicatorLEDAsTorch != useILED)
  {
    led->swapLEDs();
  }

  useIndicatorLEDAsTorch = useILED;

  // Turn off the torch in either case:
//  turnTorchOff();
}


void MainWindow::setIndicatorBrightnessLevel(
  int brightness)
{
//std::cout << "setting indicator brightness level to " << brightness << std::endl;
  led->setIndicatorBrightnessLevel(brightness);
}


void MainWindow::setCoverLocksScreen(
  bool cls)
{
  coverLocksScreen = cls;
}


void MainWindow::setCoverClosesApp(
  bool cca)
{
  coverClosesApp = cca;
}


void MainWindow::setEnableOffTimer(
  bool enabled)
{
  useOffTimer = enabled;

  if (useOffTimer)
  {
    if (loopRunning || led->ledsCurrentlyLit())
    {
      setupOffTimer();
    }
  }
  else
  {
    offTimer.stop();
  }
}


void MainWindow::setOffTimerDuration(
  int duration)
{
  offTimerDuration = duration;
}


void MainWindow::setFlashAfterTimeout(
  bool flag)
{
  flashAfterTimeout = flag;
}


int MainWindow::dotDuration()
{
  if (preferencesForm)
  {
    return preferencesForm->getDotDuration();
  }
  else
  {
    return 100; // a safe default value
  }
}


void MainWindow::strobe()
{
  // Do nothing if camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  led->strobe();
}


void MainWindow::updateCameraCover(
  bool closed)
{
  cameraCoverClosed = closed;

  // If the cover is closed, make sure all LED operations are shut down:
  if (cameraCoverClosed && !ignoreCameraCover)
  {
    if (morseForm)
    {
      morseForm->stopTimer();
    }

    turnTorchOff();

    // If "coverLocksScreen" set, lock the screen:
    if (coverLocksScreen)
    {
      dbus.lockScreen();
    }

    // If "coverClosesApp" has been set, exit Lanterne immediately:
    if (coverClosesApp)
    {
      QApplication::quit();
    }
  }
}


void MainWindow::updateCameraButton(
  bool pressed)
{
  if (useCameraButton)
  {
    if (useTorchButtonAsMorseKey)
    {
      if (pressed)
      {
        toggleTorch();
      }
    }
    else
    {
      if (pressed)
      {
        turnTorchOn();
      }
      else
      {
        turnTorchOff();
      }
    }
  }
}


void MainWindow::on_actionPreferences_triggered()
{
  preferencesForm->show();
}


void MainWindow::on_actionAbout_triggered()
{
  if (!aboutForm)
  {
    aboutForm = new LanAboutForm(this);
  }

  aboutForm->show();
}


void MainWindow::on_actionStrobe_Flash_triggered()
{
  if (morseForm)
  {
    morseForm->stopTimer();
    loopRunning = false;
  }

  if (!strobeForm)
  {
    strobeForm = new LanStrobeForm(this);
  }

  strobeForm->show();
}


void MainWindow::on_whiteButton_clicked()
{
  useIndicatorLED(false);
}


void MainWindow::on_redButton_clicked()
{
  useIndicatorLED(true);
}


void MainWindow::on_torchButton_clicked()
{
  // Skip this method if we are in Morse Key mode:
  if (useTorchButtonAsMorseKey) return;

  // Also, do nothing if the camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (morseForm)
  {
    morseForm->stopTimer();
    loopRunning = false;
  }

  toggleTorch();
}


void MainWindow::on_torchButton_pressed()
{
  // Skip this method if we are not in Morse Key mode:
  if (!useTorchButtonAsMorseKey) return;

  // Also, do nothing if the camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (morseForm)
  {
    morseForm->stopTimer();
    loopRunning = false;
  }

  turnTorchOn();
}


void MainWindow::on_torchButton_released()
{
  // Skip this method if we are not in Morse Key mode:
  if (!useTorchButtonAsMorseKey) return;

  // Also, do nothing if the camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (morseForm)
  {
    morseForm->stopTimer();
    loopRunning = false;
  }

  turnTorchOff();
}


/*
void MainWindow::on_strobeButton_clicked()
{
  if (morseForm)
  {
    morseForm->stopTimer();
    loopRunning = false;
  }

  if (!strobeForm)
  {
    strobeForm = new LanStrobeForm(this);
  }

  strobeForm->show();
}
*/


void MainWindow::on_morseButton_clicked()
{
  if (!morseForm)
  {
    morseForm = new LanMorseForm(this);
  }
  else
  {
    morseForm->stopTimer();
    loopRunning = false;
  }

  morseForm->show();
}


void MainWindow::on_sosButton_clicked()
{
  // Do nothing if the camera cover is closed:
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (!morseForm)
  {
    morseForm = new LanMorseForm(this);
  }

  if (!loopRunning)
  {
    morseForm->startSOS();
    loopRunning = true;
    setupOffTimer();
  }
  else
  {
    morseForm->stopTimer();
    loopRunning = false;
    offTimer.stop();
  }
}


void MainWindow::on_torchContinuousButton_clicked()
{
  if (cameraCoverClosed && !ignoreCameraCover) return;

  if (!morseForm)
  {
    morseForm = new LanMorseForm(this);
  }

  if (!loopRunning)
  {
    morseForm->startE();
    loopRunning = true;
    setupOffTimer();
  }
  else
  {
    morseForm->stopTimer();
    loopRunning = false;
    offTimer.stop();
  }
}

/*
void MainWindow::on_supressLEDButton_clicked()
{
  // No need to check camera cover, as this turns nothing on:
  if (!morseForm)
  {
    morseForm = new LanMorseForm(this);
  }

  if (!loopRunning)
  {
    morseForm->startSupressLEDs();
    loopRunning = true;
  }
  else
  {
    morseForm->stopTimer();
    loopRunning = false;
  }
}
*/


void MainWindow::torchTimeout()
{
  if (loopRunning)
  {
    morseForm->stopTimer();
    loopRunning = false;
  }
  else
  {
    turnTorchOff();
  }

  offTimer.stop();

  if (flashAfterTimeout)
  {
    // Start flashing the low-power LED, to help find the phone now that the
    // torch is off.
//    ui->whiteButton->setChecked(false);
//    ui->redButton->setChecked(true);
    useIndicatorLED(true);
    if (!morseForm)
    {
      morseForm = new LanMorseForm(this);
    }
    morseForm->startE();
    loopRunning = true;
  }

  QMaemo5InformationBox::information(
    0, "Lanterne torch timed out", 0);

  if (flashAfterTimeout)
  {
    morseForm->stopTimer();
    loopRunning = false;

    if (ui->whiteButton->isChecked())
    {
      useIndicatorLED(false);
    }
  }
}


void MainWindow::setupOffTimer()
{
  if (useOffTimer)
  {
    // multiply minutes by 60 seconds by 1000 milliseconds:
    unsigned int milliseconds = offTimerDuration * 60000;

    // Start off the timer immediately
    offTimer.stop();
    offTimer.start(milliseconds);
  }
}
