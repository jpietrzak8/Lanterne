//
// lanpreferencesform.cpp
//
// Copyright 2013 by John Pietrzak
//
// This file contains the main window declaration for Lanterne.
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

#include "lanpreferencesform.h"
#include "ui_lanpreferencesform.h"

#include "mainwindow.h"
#include <QSettings>
#include <QProcess>

//#include <iostream>

LanPreferencesForm::LanPreferencesForm(
  MainWindow *mw)
  : QWidget(mw),
//    cameraDisabled(false),
    mainWindow(mw),
    ui(new Ui::LanPreferencesForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  // There are seven indicator brightness levels available:
  ui->indicatorBrightnessComboBox->addItem("1 (2.5 mA)");
  ui->indicatorBrightnessComboBox->addItem("2 (5 mA)");
  ui->indicatorBrightnessComboBox->addItem("3 (7.5 mA)");
  ui->indicatorBrightnessComboBox->addItem("4 (10 mA)");
  ui->indicatorBrightnessComboBox->addItem("5 (12.5 mA)");
  ui->indicatorBrightnessComboBox->addItem("6 (15 mA)");
  ui->indicatorBrightnessComboBox->addItem("7 (17.5 mA)");

  QSettings settings("pietrzak.org", "Lanterne");

  if (settings.contains("UseTorchButtonAsMorseKey"))
  {
    bool ucbamk = settings.value("UseTorchButtonAsMorseKey").toBool();

    ui->morseKeyCheckBox->setChecked(ucbamk);

    mainWindow->setUseTorchButtonAsMorseKey(ucbamk);
  }

  if (settings.contains("IgnoreCameraCover"))
  {
    bool icc = settings.value("IgnoreCameraCover").toBool();

    ui->disableCoverCheckBox->setChecked(icc);

    mainWindow->setIgnoreCameraCover(icc);
  }

  if (settings.contains("UseIndicatorLEDAsTorch"))
  {
    bool uilat = settings.value("UseIndicatorLEDAsTorch").toBool();

    ui->indicatorLEDCheckBox->setChecked(uilat);

    mainWindow->useIndicatorLED(uilat);
  }

  if (settings.contains("IndicatorBrightnessLevel"))
  {
    int ibl = settings.value("IndicatorBrightnessLevel").toInt();

    ui->indicatorBrightnessComboBox->setCurrentIndex(ibl);
  }

  if (settings.contains("CoverClosesApp"))
  {
    bool cca = settings.value("CoverClosesApp").toBool();

    ui->closeAppCheckBox->setChecked(cca);

    mainWindow->setCoverClosesApp(cca);
  }

/*
  if (settings.contains("DisableCameraApp"))
  {
    bool dca = settings.value("DisableCameraApp").toBool();

    ui->disableCameraCheckBox->setChecked(dca);

    if (dca)
    {
      disableCameraApp();
    }
  }
*/

  if (settings.contains("UseCameraButtonForTorch"))
  {
    bool ucbft = settings.value("UseCameraButtonForTorch").toBool();

    ui->cameraButtonCheckBox->setChecked(ucbft);

    mainWindow->setUseCameraButton(ucbft);
  }
}


LanPreferencesForm::~LanPreferencesForm()
{
  QSettings settings("pietrzak.org", "Lanterne");

  settings.setValue(
    "UseTorchButtonAsMorseKey",
    ui->morseKeyCheckBox->isChecked());

  settings.setValue(
    "IgnoreCameraCover", 
    ui->disableCoverCheckBox->isChecked());

  settings.setValue(
    "UseIndicatorLEDAsTorch",
    ui->indicatorLEDCheckBox->isChecked());

  settings.setValue(
    "IndicatorBrightnessLevel",
    ui->indicatorBrightnessComboBox->currentIndex());

  settings.setValue(
    "CoverClosesApp",
    ui->closeAppCheckBox->isChecked());

/*
  settings.setValue(
    "DisableCameraApp",
    ui->disableCameraCheckBox->isChecked());
*/

  settings.setValue(
    "UseCameraButtonForTorch",
    ui->cameraButtonCheckBox->isChecked());

/*
  if (cameraDisabled)
  {
    // Turn the camera app back on:
    enableCameraApp();
  }
*/

  delete ui;
}


void LanPreferencesForm::on_morseKeyCheckBox_toggled(bool checked)
{
  mainWindow->setUseTorchButtonAsMorseKey(checked);
}


void LanPreferencesForm::on_disableCoverCheckBox_toggled(
  bool checked)
{
  mainWindow->setIgnoreCameraCover(checked);
}


void LanPreferencesForm::on_indicatorLEDCheckBox_toggled(
  bool checked)
{
  mainWindow->useIndicatorLED(checked);
}


void LanPreferencesForm::on_indicatorBrightnessComboBox_currentIndexChanged(
  int index)
{
  mainWindow->setIndicatorBrightnessLevel(index + 1);
}


void LanPreferencesForm::on_closeAppCheckBox_toggled(bool checked)
{
  mainWindow->setCoverClosesApp(checked);
}


/*
void LanPreferencesForm::on_disableCameraCheckBox_toggled(
  bool checked)
{
  if (checked)
  {
    disableCameraApp();
  }
  else if (cameraDisabled)
  {
    enableCameraApp();
  }
}
*/


void LanPreferencesForm::on_cameraButtonCheckBox_toggled(
  bool checked)
{
  mainWindow->setUseCameraButton(checked);
}


/*
void LanPreferencesForm::disableCameraApp()
{
  QProcess process;
  process.execute("/usr/sbin/dsmetool -k /usr/bin/camera-ui");

  cameraDisabled = true;
}
*/


/*
void LanPreferencesForm::enableCameraApp()
{
  QProcess process;
  process.execute("/usr/sbin/dsmetool -t /usr/bin/camera-ui");

  cameraDisabled = false;
}
*/
