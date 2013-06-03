//
// main.cpp
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

#include "mainwindow.h"

#include <QtGui/QApplication>

#include <string>
#include "lanflashled.h"

int main(
  int argc,
  char *argv[])
{
  // We want to be able to run Lanterne from either a GUI or from the
  // command line.  So, we'll use a command-line argument of "-nogui"
  // to disable the GUI:

  bool useGUI = true;
  bool toggleTorch = false;
  int index = 1;
  while (index < argc)
  {
    if (std::string(argv[index]) == "-noGUI")
    {
      useGUI = false;
    }
    else if (std::string(argv[index]) == "-toggleTorch")
    {
      toggleTorch = true;
    }

    ++index;
  }

  QApplication app(argc, argv, useGUI);

  if (useGUI)
  {
    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showExpanded();

    return app.exec();
  }
  else
  {
    if (toggleTorch)
    {
      LanFlashLED led;
      led.toggleTorch();
    }

    return 0;
  }
}
