//
// lanmorseform.h
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

#ifndef LANMORSEFORM_H
#define LANMORSEFORM_H

#include <QWidget>

#include <list>

class QTimer;

typedef std::list<bool> LanBoolList;

namespace Ui {
class LanMorseForm;
}

class MainWindow;

class LanMorseForm : public QWidget
{
  Q_OBJECT
  
public:
  LanMorseForm(
    MainWindow *mw);

  ~LanMorseForm();
  
private slots:
  void on_morseButton_clicked();
  void dot();
  void dash();
  void threeUnitGap();
  void fourUnitGap();
  void runMorseCode();

private:
  MainWindow *mainWindow;

  LanBoolList morseCodeBits;
  LanBoolList::const_iterator morseCodePosition;

  QTimer *timer;

  Ui::LanMorseForm *ui;
};

#endif // LANMORSEFORM_H
