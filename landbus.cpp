//
// landbus.cpp
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

#include "landbus.h"

#include <QDBusInterface>
#include <QDBusMetaType>

#include <iostream>

// Some odd operators required for getting the DBus parameters right:
const QDBusArgument & operator<<(
  QDBusArgument &arg,
  const DBusProperty &change)
{
  arg.beginStructure();
  arg << change.name << change.added << change.removed;
  arg.endStructure();
  return arg;
}


const QDBusArgument & operator>>(
  const QDBusArgument &arg,
  DBusProperty &change)
{
  arg.beginStructure();
  arg >> change.name >> change.added >> change.removed;
  arg.endStructure();
  return arg;
}


LanDBus::LanDBus()
  : halCameraShutter(0)
{
  // Set up the QDBusInterface:

  halCameraShutter = new QDBusInterface(
    "org.freedesktop.Hal",
    "/org/freedesktop/Hal/devices/platform_cam_shutter",
    "org.freedesktop.Hal.Device",
    QDBusConnection::systemBus());

  // Connect any camera cover updates to our slot:

  qDBusRegisterMetaType<DBusProperty>();
  qDBusRegisterMetaType<QList<DBusProperty> >();

  QDBusConnection::systemBus().connect(
    "",
    "/org/freedesktop/Hal/devices/platform_cam_shutter",
    "org.freedesktop.Hal.Device",
    "PropertyModified",
    this,
    SLOT(cameraCoverPropertyModified(int, QList<DBusProperty>)));
}


LanDBus::~LanDBus()
{
  if (halCameraShutter) delete halCameraShutter;
}


void LanDBus::checkCameraCoverStatus()
{
  QDBusMessage message =
    halCameraShutter->call("GetProperty", "button.state.value");

  // Should check for errors here!

  emit cameraCoverChanged(message.arguments().at(0).toBool());
}


void LanDBus::cameraCoverPropertyModified(
  int count,
  QList<DBusProperty> properties)
{
  Q_UNUSED(count);
  Q_UNUSED(properties);
  checkCameraCoverStatus();
}
