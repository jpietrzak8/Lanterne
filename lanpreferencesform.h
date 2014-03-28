//
// lanpreferencesform.h
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

#ifndef LANPREFERENCESFORM_H
#define LANPREFERENCESFORM_H

#include <QWidget>

enum StartupMode
{
  SteadyOn_Mode,
  Pulsed_Mode,
  SOS_Mode
};

namespace Ui {
class LanPreferencesForm;
}

class MainWindow;

class LanPreferencesForm : public QWidget
{
  Q_OBJECT
  
public:
  LanPreferencesForm(
    MainWindow *mw);

  ~LanPreferencesForm();

  bool startupAutomatically();

  StartupMode getStartupMode();

private slots:
  void on_morseKeyCheckBox_toggled(bool checked);
  void on_disableCoverCheckBox_toggled(bool checked);
//  void on_indicatorLEDCheckBox_toggled(bool checked);
  void on_indicatorBrightnessComboBox_currentIndexChanged(int index);
  void on_closeAppCheckBox_toggled(bool checked);
  void on_cameraButtonCheckBox_toggled(bool checked);
  void on_startupCheckBox_toggled(bool checked);
  void on_startupComboBox_currentIndexChanged(int index);
  void on_offTimerCheckBox_toggled(bool checked);
  void on_offTimerSpinBox_valueChanged(int arg1);
  void on_flashAfterTimeoutCheckBox_toggled(bool checked);
//  void on_disableCameraCheckBox_toggled(bool checked);

private:
/*
  void disableCameraApp();
  void enableCameraApp();

  bool cameraDisabled;
*/

  MainWindow *mainWindow;

  bool autoStartup;
  StartupMode mode;

  Ui::LanPreferencesForm *ui;
};

#endif // LANPREFERENCESFORM_H
