//
// lantorchform.cpp
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

#include "lantorchform.h"
#include "ui_lantorchform.h"

#include "mainwindow.h"

LanTorchForm::LanTorchForm(
  MainWindow *mw)
  : QWidget(mw),
    mainWindow(mw),
    ui(new Ui::LanTorchForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  // set up the spin box:
  ui->torchBrightnessSpinBox->setMinimum(mainWindow->getMinTorch());
  ui->torchBrightnessSpinBox->setMaximum(mainWindow->getMaxTorch());
  ui->torchBrightnessSpinBox->setValue(mainWindow->getMaxTorch());
}

LanTorchForm::~LanTorchForm()
{
  delete ui;
}

void LanTorchForm::on_torchBrightnessSpinBox_valueChanged(int arg1)
{
  mainWindow->setTorchBrightness(arg1);
}

void LanTorchForm::on_torchPushButton_clicked()
{
  mainWindow->toggleTorch();
}
