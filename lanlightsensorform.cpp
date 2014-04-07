//
// lanlightsensorform.cpp
//
// Copyright 2014 by John Pietrzak  (jpietrzak8@gmail.com)
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

#include "lanlightsensorform.h"
#include "ui_lanlightsensorform.h"

#include "mainwindow.h"
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QSettings>

LanLightSensorForm::LanLightSensorForm(
  MainWindow *mw)
  : QWidget(mw),
    mainWindow(mw),
    timer(0),
    torchSwitchedOn(false),
    ui(new Ui::LanLightSensorForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  QSettings settings("pietrzak.org", "Lanterne");

  if (settings.contains("AmbientLightTriggerValue"))
  {
    int altv = settings.value("AmbientLightTriggerValue").toInt();

    ui->triggerLevelSpinBox->setValue(altv);
  }
}


LanLightSensorForm::~LanLightSensorForm()
{
  QSettings settings("pietrzak.org", "Lanterne");

  settings.setValue(
    "AmbientLightTriggerValue",
    ui->triggerLevelSpinBox->value());

  if (timer) delete timer;

  delete ui;
}


void LanLightSensorForm::showEvent(
  QShowEvent *event)
{
  QWidget::showEvent(event);

  // Set up the form:
  update();

  // Start the timer running:
  timer = new QTimer(this);
  connect (timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000);
}


void LanLightSensorForm::hideEvent(
  QHideEvent *event)
{
  QWidget::hideEvent(event);

  // if the timer is running, stop it:
  if (timer)
  {
    delete timer;
    timer = 0;
  }

  // Turn the torch off:
  mainWindow->turnTorchOff();
}


void LanLightSensorForm::update()
{
  QFile alslux("/sys/bus/i2c/drivers/tsl2563/2-0029/lux");

  if (!alslux.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    // need to put an error message here
    return;
  }

  QTextStream ts(&alslux);
  int ambientLight;
  ts >> ambientLight;

  if ( (ambientLight > 0) && (ambientLight < 10000))
  {
    ui->lcdNumber->display(ambientLight);
  }

  if (ui->torchTriggerCheckBox->isChecked())
  {
    if (ambientLight <= ui->triggerLevelSpinBox->value())
    {
      if (!torchSwitchedOn)
      {
        mainWindow->turnTorchOn();
        torchSwitchedOn = true;
      }
    }
    else
    {
      if (torchSwitchedOn)
      {
        mainWindow->turnTorchOff();
        torchSwitchedOn = false;
      }
    }
  }
  else
  {
    if (torchSwitchedOn)
    {
      mainWindow->turnTorchOff();
      torchSwitchedOn = false;
    }
  }
}
