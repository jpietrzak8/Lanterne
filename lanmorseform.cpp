//
// lanmorseform.cpp
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

#include "lanmorseform.h"
#include "ui_lanmorseform.h"

#include "mainwindow.h"
#include <QTimer>
#include <QSettings>

LanMorseForm::LanMorseForm(
  MainWindow *mw)
  : QWidget(mw),
    mainWindow(mw),
    timer(0),
    ui(new Ui::LanMorseForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);

  setupSOSCode();

  QSettings settings("pietrzak.org", "Lanterne");

  ui->dotDurationSpinBox->setMinimum(10);
  ui->dotDurationSpinBox->setMaximum(1000);
  ui->dotDurationSpinBox->setValue(100);

  if (settings.contains("CurrentDotDuration"))
  {
    int cdd = settings.value("CurrentDotDuration").toInt();

    if ((cdd >= 10) && (cdd <= 1000))
    {
      ui->dotDurationSpinBox->setValue(cdd);
    }
  }
}


LanMorseForm::~LanMorseForm()
{
  QSettings settings("pietrzak.org", "Lanterne");

  settings.setValue(
    "CurrentDotDuration",
    ui->dotDurationSpinBox->value());

  if (timer) delete timer;

  delete ui;
}


void LanMorseForm::startSOS()
{
  if (timer)
  {
    delete timer;
    timer = 0;
  }

  sosCodePosition = sosCodeBits.begin();
  timer = new QTimer(this);
  connect (timer, SIGNAL(timeout()), this, SLOT(runSOSCode()));
  timer->start(ui->dotDurationSpinBox->value());
}


void LanMorseForm::stopSOS()
{
  if (timer)
  {
    delete timer;
    timer = 0;
  }

  mainWindow->turnTorchOff();
}


void LanMorseForm::on_morseButton_clicked()
{
  morseCodeBits.clear();

  if (timer)
  {
    delete timer;
    timer = 0;
  }

  QString morseText = ui->morseLineEdit->text();

  int index = 0;
  int stringSize = morseText.size();

  while (index < stringSize)
  {
    switch (morseText.at(index).toAscii())
    {
    case 'a':
    case 'A':
      dot(); dash(); break;

    case 'b':
    case 'B':
      dash(); dot(); dot(); dot(); break;

    case 'c':
    case 'C':
      dash(); dot(); dash(); dot(); break;

    case 'd':
    case 'D':
      dash(); dot(); dot(); break;

    case 'e':
    case 'E':
      dot(); break;

    case 'f':
    case 'F':
      dot(); dot(); dash(); dot(); break;

    case 'g':
    case 'G':
      dash(); dash(); dot(); break;

    case 'h':
    case 'H':
      dot(); dot(); dot(); dot(); break;

    case 'i':
    case 'I':
      dot(); dot(); break;

    case 'j':
    case 'J':
      dot(); dash(); dash(); dash(); break;

    case 'k':
    case 'K':
      dash(); dot(); dash(); break;

    case 'l':
    case 'L':
      dot(); dash(); dot(); dot(); break;

    case 'm':
    case 'M':
      dash(); dash(); break;

    case 'n':
    case 'N':
      dash(); dot(); break;

    case 'o':
    case 'O':
      dash(); dash(); dash(); break;

    case 'p':
    case 'P':
      dot(); dash(); dash(); dot(); break;

    case 'q':
    case 'Q':
      dash(); dash(); dot(); dash(); break;

    case 'r':
    case 'R':
      dot(); dash(); dot(); break;

    case 's':
    case 'S':
      dot(); dot(); dot(); break;

    case 't':
    case 'T':
      dash(); break;

    case 'u':
    case 'U':
      dot(); dot(); dash(); break;

    case 'v':
    case 'V':
      dot(); dot(); dot(); dash(); break;

    case 'w':
    case 'W':
      dot(); dash(); dash(); break;

    case 'x':
    case 'X':
      dash(); dot(); dot(); dash(); break;

    case 'y':
    case 'Y':
      dash(); dot(); dash(); dash(); break;

    case 'z':
    case 'Z':
      dash(); dash(); dot(); dot(); break;

    case '0':
      dash(); dash(); dash(); dash(); dash(); break;

    case '1':
      dot(); dash(); dash(); dash(); dash(); break;

    case '2':
      dot(); dot(); dash(); dash(); dash(); break;

    case '3':
      dot(); dot(); dot(); dash(); dash(); break;

    case '4':
      dot(); dot(); dot(); dot(); dash(); break;

    case '5':
      dot(); dot(); dot(); dot(); dot(); break;

    case '6':
      dash(); dot(); dot(); dot(); dot(); break;

    case '7':
      dash(); dash(); dot(); dot(); dot(); break;

    case '8':
      dash(); dash(); dash(); dot(); dot(); break;

    case '9':
      dash(); dash(); dash(); dash(); dot(); break;

    case '.':
      dot(); dash(); dot(); dash(); dot(); dash(); break;

    case ',':
      dash(); dash(); dot(); dot(); dash(); dash(); break;

    case '?':
      dot(); dot(); dash(); dash(); dot(); dot(); break;

    case '\'':
      dot(); dash(); dash(); dash(); dash(); dot(); break;

    case '!':
      dash(); dot(); dash(); dot(); dash(); dash(); break;

    case '/':
      dash(); dot(); dot(); dash(); dot(); break;

    case '(':
      dash(); dot(); dash(); dash(); dot(); break;

    case ')':
      dash(); dot(); dash(); dash(); dot(); dash(); break;

    case '&':
      dot(); dash(); dot(); dot(); dot(); break;

    case ':':
      dash(); dash(); dash(); dot(); dot(); dot(); break;

    case ';':
      dash(); dot(); dash(); dot(); dash(); dot(); break;

    case '=':
      dash(); dot(); dot(); dot(); dash(); break;

    case '+':
      dot(); dash(); dot(); dash(); dot(); break;

    case '-':
      dash(); dot(); dot(); dot(); dot(); dash(); break;

    case '_':
      dot(); dot(); dash(); dash(); dot(); dash(); break;

    case '"':
      dot(); dash(); dot(); dot(); dash(); dot(); break;

    case '$':
      dot(); dot(); dot(); dash(); dot(); dot(); dash(); break;

    case '@':
      dot(); dash(); dash(); dot(); dash(); dot(); break;

    case ' ':
      // End of a word, so need to add 4 units to the 3-unit character gap:
      fourUnitGap(); break;

    default:
      break;
    }

    // At the end of every character is a 3 unit gap:
    threeUnitGap();

    ++index;
  }

  // Now, execute the morseCodeBits:
  morseCodePosition = morseCodeBits.begin();
  timer = new QTimer(this);
  connect (timer, SIGNAL(timeout()), this, SLOT(runMorseCode()));
  timer->start(ui->dotDurationSpinBox->value());
}


void LanMorseForm::dot()
{
  morseCodeBits.push_back(true);
  morseCodeBits.push_back(false);
}


void LanMorseForm::dash()
{
  pushBits(morseCodeBits, true, 3);
  morseCodeBits.push_back(false);
}


void LanMorseForm::threeUnitGap()
{
  pushBits(morseCodeBits, false, 3);
}


void LanMorseForm::fourUnitGap()
{
  pushBits(morseCodeBits, false, 4);
}


void LanMorseForm::runMorseCode()
{
  if (morseCodePosition == morseCodeBits.end())
  {
    if (timer)
    {
      delete timer;
      timer = 0;
    }

    return;
  }

  if (*morseCodePosition)
  {
    mainWindow->turnTorchOn();
  }
  else
  {
    mainWindow->turnTorchOff();
  }

  ++morseCodePosition;
}


void LanMorseForm::runSOSCode()
{
  if (sosCodePosition == sosCodeBits.end())
  {
    sosCodePosition = sosCodeBits.begin();
  }

  if (*sosCodePosition)
  {
    mainWindow->turnTorchOn();
  }
  else
  {
    mainWindow->turnTorchOff();
  }

  ++sosCodePosition;
}


void LanMorseForm::setupSOSCode()
{
  // We'll do the standard SOS, followed by a standard 7-dot word space.

  // S: Three dots, plus char space:
  sosCodeBits.push_back(true);
  sosCodeBits.push_back(false);
  sosCodeBits.push_back(true);
  sosCodeBits.push_back(false);
  sosCodeBits.push_back(true);
  pushBits(sosCodeBits, false, 4);

  // O: Three dashes, plus char space:
  pushBits(sosCodeBits, true, 3);
  sosCodeBits.push_back(false);
  pushBits(sosCodeBits, true, 3);
  sosCodeBits.push_back(false);
  pushBits(sosCodeBits, true, 3);
  pushBits(sosCodeBits, false, 4);

  // S: Three dots, plus word space:
  sosCodeBits.push_back(true);
  sosCodeBits.push_back(false);
  sosCodeBits.push_back(true);
  sosCodeBits.push_back(false);
  sosCodeBits.push_back(true);
  pushBits(sosCodeBits, false, 8);
}


void LanMorseForm::pushBits(
  LanBoolList &bits,
  bool value,
  unsigned int quantity)
{
  unsigned int index = 0;

  while (index < quantity)
  {
    bits.push_back(value);

    ++index;
  }
}
