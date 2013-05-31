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

LanPreferencesForm::LanPreferencesForm(
  MainWindow *mw)
  : QWidget(mw),
    mainWindow(mw),
    ui(new Ui::LanPreferencesForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  QSettings settings("pietrzak.org", "Lanterne");

  if (settings.contains("IgnoreCameraCover"))
  {
    bool icc = settings.value("IgnoreCameraCover").toBool();

    ui->disableCoverCheckBox->setChecked(icc);

    mainWindow->setIgnoreCameraCover(icc);
  }
}


LanPreferencesForm::~LanPreferencesForm()
{
  QSettings settings("pietrzak.org", "Lanterne");

  settings.setValue(
    "IgnoreCameraCover", 
    ui->disableCoverCheckBox->isChecked());

  delete ui;
}

void LanPreferencesForm::on_disableCoverCheckBox_toggled(bool checked)
{
  mainWindow->setIgnoreCameraCover(checked);
}
