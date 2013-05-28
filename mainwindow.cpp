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
#include "lantorchform.h"
#include "lanstrobeform.h"
#include "lanmorseform.h"
#include <QTimer>

MainWindow::MainWindow(
  QWidget *parent)
  : QMainWindow(parent),
    led(0),
    aboutForm(0),
    torchForm(0),
    strobeForm(0),
    morseForm(0),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);

  led = new LanFlashLED();
}

MainWindow::~MainWindow()
{
  if (led) delete led;
  if (aboutForm) delete aboutForm;
  if (torchForm) delete torchForm;
  if (strobeForm) delete strobeForm;
  if (morseForm) delete morseForm;

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


int MainWindow::getMinTorch()
{
  return led->getMinTorch();
}


int MainWindow::getMaxTorch()
{
  return led->getMaxTorch();
}


void MainWindow::setTorchBrightness(int arg1)
{
  led->setTorchBrightness(arg1);
}


void MainWindow::toggleTorch()
{
  led->toggleTorch();
}


void MainWindow::turnTorchOn()
{
  led->turnTorchOn();
}


void MainWindow::turnTorchOff()
{
  led->turnTorchOff();
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


void MainWindow::strobe()
{
  led->strobe();
}


void MainWindow::on_actionAbout_triggered()
{
  if (!aboutForm)
  {
    aboutForm = new LanAboutForm(this);
  }

  aboutForm->show();
}

void MainWindow::on_torchButton_clicked()
{
  if (!torchForm)
  {
    torchForm = new LanTorchForm(this);
  }

  torchForm->show();
}


void MainWindow::on_strobeButton_clicked()
{
  if (!strobeForm)
  {
    strobeForm = new LanStrobeForm(this);
  }

  strobeForm->show();
}


void MainWindow::on_morseButton_clicked()
{
  if (!morseForm)
  {
    morseForm = new LanMorseForm(this);
  }

  morseForm->show();
}
