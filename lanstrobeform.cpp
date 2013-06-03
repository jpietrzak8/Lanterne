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

#include <iostream>

LanStrobeForm::LanStrobeForm(
  MainWindow *mw)
  : QWidget(mw),
    mainWindow(mw),
    strobePeriod(500),
    dutyCycle(10),
    ledTimer(0),
    ui(new Ui::LanStrobeForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  QSettings settings("pietrzak.org", "Lanterne");

  // Set up the brightness combo box:
  setupBrightnessComboBox();

  if (settings.contains("CurrentFlashBrightness"))
  {
    setBrightnessLevel(settings.value("CurrentFlashBrightness").toInt());
  }

  // set up the spin boxes:
  ui->strobePeriodSpinBox->setMinimum(100);
  ui->strobePeriodSpinBox->setMaximum(5000);

  if (settings.contains("CurrentStrobePeriod"))
  {
    int csp = settings.value("CurrentStrobePeriod").toInt();

    if ((csp >= 100) && (csp <= 5000))
    {
      strobePeriod = csp;
    }
  }

  ui->strobePeriodSpinBox->setValue(strobePeriod);

  ui->dutyCycleSpinBox->setMinimum(1);
  ui->dutyCycleSpinBox->setMaximum(25);

  if (settings.contains("CurrentDutyCycle"))
  {
    int cdc = settings.value("CurrentDutyCycle").toInt();

    if ((cdc >= 1) && (cdc <= 25))
    {
      dutyCycle = cdc;
    }
  }

  ui->dutyCycleSpinBox->setValue(dutyCycle);
}


LanStrobeForm::~LanStrobeForm()
{
  QSettings settings("pietrzak.org", "Lanterne");

  settings.setValue(
    "CurrentFlashBrightness",
    ui->flashBrightnessComboBox->currentIndex());

  settings.setValue(
    "CurrentStrobePeriod",
    ui->strobePeriodSpinBox->value());

  settings.setValue(
    "CurrentDutyCycle",
    ui->dutyCycleSpinBox->value());

  if (ledTimer) delete ledTimer;

  delete ui;
}


void LanStrobeForm::on_flashBrightnessComboBox_currentIndexChanged(int index)
{
  mainWindow->setFlashBrightness(index + 12);
}


void LanStrobeForm::on_strobePeriodSpinBox_valueChanged(int arg1)
{
  mainWindow->setFlashDuration(
    ((arg1 * 1000) * ui->dutyCycleSpinBox->value()) / 100);

  strobePeriod = arg1;
}


void LanStrobeForm::on_dutyCycleSpinBox_valueChanged(int arg1)
{
  mainWindow->setFlashDuration(
    ((strobePeriod * 1000) * arg1) / 100);
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

    ledTimer->start(strobePeriod);
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


void LanStrobeForm::setupBrightnessComboBox()
{
  ui->flashBrightnessComboBox->addItem("12 (215 mA)");
  ui->flashBrightnessComboBox->addItem("13 (230 mA)");
  ui->flashBrightnessComboBox->addItem("14 (245 mA)");
  ui->flashBrightnessComboBox->addItem("15 (260 mA)");
  ui->flashBrightnessComboBox->addItem("16 (275 mA)");
  ui->flashBrightnessComboBox->addItem("17 (290 mA)");
  ui->flashBrightnessComboBox->addItem("18 (305 mA)");
  ui->flashBrightnessComboBox->addItem("19 (320 mA)");
}


void LanStrobeForm::setBrightnessLevel(
  int brightness)
{
  int adjustedB = brightness + 12;

  if ( (adjustedB >= mainWindow->getMinFlash())
    && (adjustedB <= mainWindow->getMaxFlash()))
  {
    ui->flashBrightnessComboBox->setCurrentIndex(brightness);
  }
  else
  {
    ui->flashBrightnessComboBox->setCurrentIndex(
      mainWindow->getMinFlash() - 12);
  }
}
