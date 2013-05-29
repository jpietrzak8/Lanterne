//
// lanstrobeform.cpp
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

#include "lanstrobeform.h"
#include "ui_lanstrobeform.h"

#include "mainwindow.h"
#include <QTimer>
#include <QSettings>

LanStrobeForm::LanStrobeForm(
  MainWindow *mw)
  : QWidget(mw),
    mainWindow(mw),
    minPause(50),
    maxPause(2000),
    chosenPause(500),
    ledTimer(0),
    ui(new Ui::LanStrobeForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  // set up the spin boxes:
  QSettings settings("pietrzak.org", "Lanterne");

  ui->flashBrightnessSpinBox->setMinimum(mainWindow->getMinFlash());
  ui->flashBrightnessSpinBox->setMaximum(mainWindow->getMaxFlash());

  int brightness = mainWindow->getMinFlash();

  if (settings.contains("CurrentFlashIntensity"))
  {
    int cfi = settings.value("CurrentFlashIntensity").toInt();

    if ( (cfi >= mainWindow->getMinFlash())
      && (cfi <= mainWindow->getMaxFlash()))
    {
      brightness = cfi;
    }
  }

  ui->flashBrightnessSpinBox->setValue(brightness);

  ui->flashDurationSpinBox->setMinimum(mainWindow->getMinTime() / 1000);
  ui->flashDurationSpinBox->setMaximum(mainWindow->getMaxTime() / 1000);

  int duration = (mainWindow->getMaxTime() / 1000) / 2;

  if (settings.contains("CurrentFlashDuration"))
  {
    int cfd = settings.value("CurrentFlashDuration").toInt();

    if ( (cfd >= (mainWindow->getMinTime() / 1000))
      && (cfd <= (mainWindow->getMaxTime() / 1000)))
    {
      duration = cfd;
    }
  }

  ui->flashDurationSpinBox->setValue(duration);

  ui->flashPauseSpinBox->setMinimum(minPause);
  ui->flashPauseSpinBox->setMaximum(maxPause);

  if (settings.contains("CurrentStrobePause"))
  {
    int csp = settings.value("CurrentStrobePause").toInt();

    if ( (csp >= minPause)
      && (csp <= maxPause))
    {
      chosenPause = csp;
    }
  }

  ui->flashPauseSpinBox->setValue(chosenPause);
}


LanStrobeForm::~LanStrobeForm()
{
  QSettings settings("pietrzak.org", "Lanterne");

  settings.setValue(
    "CurrentFlashIntensity",
    ui->flashBrightnessSpinBox->value());

  settings.setValue(
    "CurrentFlashDuration",
    ui->flashDurationSpinBox->value());

  settings.setValue(
    "CurrentStrobePause",
    ui->flashPauseSpinBox->value());

  if (ledTimer) delete ledTimer;

  delete ui;
}


void LanStrobeForm::on_flashBrightnessSpinBox_valueChanged(int arg1)
{
  mainWindow->setFlashBrightness(arg1);
}

void LanStrobeForm::on_flashDurationSpinBox_valueChanged(int arg1)
{
  mainWindow->setFlashDuration(arg1 * 1000);
}

void LanStrobeForm::on_flashPauseSpinBox_valueChanged(int arg1)
{
  if (arg1 < minPause)
  {
    chosenPause = minPause;
  }
  else if (arg1 > maxPause)
  {
    chosenPause = maxPause;
  }
  else
  {
    chosenPause = arg1;
  }
}


void LanStrobeForm::on_strobeFlashButton_pressed()
{
  if (!ledTimer)
  {
    // First, kick off with a strobe:
    mainWindow->strobe();

    // Now, set up a timer to continue strobing:
    ledTimer = new QTimer(this);
    connect(ledTimer, SIGNAL(timeout()), mainWindow, SLOT(strobe()));

    ledTimer->start(chosenPause + (mainWindow->getChosenTime() / 1000));
  }
}


void LanStrobeForm::on_strobeFlashButton_released()
{
  if (ledTimer)
  {
    delete ledTimer;
    ledTimer = 0;
  }
}


void LanStrobeForm::stopStrobe()
{
  if (ledTimer)
  {
    delete ledTimer;
    ledTimer = 0;
  }
}
