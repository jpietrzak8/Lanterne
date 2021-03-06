//
// landbus.h
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

#ifndef LANDBUS_H
#define LANDBUS_H

#include <QObject>
#include <QList>
#include <QString>
#include <QMetaType>

class QDBusInterface;

// Some annoying nowhere-documented types for use with DBus:
struct DBusProperty
{
  QString name;
  bool added;
  bool removed;
};
Q_DECLARE_METATYPE(DBusProperty)
Q_DECLARE_METATYPE(QList<DBusProperty>)

class LanDBus: public QObject
{
  Q_OBJECT

public:
  LanDBus();
  ~LanDBus();

  bool currentCameraCoverStatus();

  void lockScreen();

signals:
  void cameraCoverChanged(
    bool coverClosed);

  void cameraButtonChanged(
    bool buttonPressed);

public slots:
  void cameraCoverPropertyModified(
    int count,
    QList<DBusProperty> properties);

  void cameraLaunchPropertyModified(
    int count,
    QList<DBusProperty> properties);

private:
  QDBusInterface *halCameraShutter;
  QDBusInterface *halCameraButtonLaunch;
  QDBusInterface *mce;

  void startMCEInterface();
};

#endif // LANDBUS_H
