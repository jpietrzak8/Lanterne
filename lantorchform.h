//
// lantorchform.h
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

#ifndef LANTORCHFORM_H
#define LANTORCHFORM_H

#include <QWidget>

namespace Ui {
class LanTorchForm;
}

class MainWindow;

class LanTorchForm : public QWidget
{
  Q_OBJECT
  
public:
  LanTorchForm(
    MainWindow *mw);

  ~LanTorchForm();
  
private slots:
  void on_torchBrightnessSpinBox_valueChanged(int arg1);
  void on_torchPushButton_clicked();

private:
  MainWindow *mainWindow;

  Ui::LanTorchForm *ui;
};

#endif // LANTORCHFORM_H
