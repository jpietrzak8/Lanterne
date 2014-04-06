//
// lanlightsensorform.h
//
// Copyright 2014 by John Pietrzak
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

#ifndef LANLIGHTSENSORFORM_H
#define LANLIGHTSENSORFORM_H

#include <QWidget>

class QTimer;
class MainWindow;

namespace Ui {
class LanLightSensorForm;
}

class LanLightSensorForm : public QWidget
{
  Q_OBJECT
  
public:
  LanLightSensorForm(
    MainWindow *mw);

  ~LanLightSensorForm();

protected:
  virtual void showEvent(
    QShowEvent *event);

  virtual void hideEvent(
    QHideEvent *event);

private slots:
  void update();

private:
  MainWindow *mainWindow;

  QTimer *timer;

  Ui::LanLightSensorForm *ui;
};

#endif // LANLIGHTSENSORFORM_H
