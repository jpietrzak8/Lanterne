# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

# symbian:TARGET.UID3 = 0xE1EA16DE

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
# symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

QT += Maemo5

SOURCES += main.cpp mainwindow.cpp \
    lanflashled.cpp \
    lanexception.cpp \
    lanaboutform.cpp \
    lanmorseform.cpp
HEADERS += mainwindow.h \
    lanflashled.h \
    lanexception.h \
    lanaboutform.h \
    lanmorseform.h
FORMS += mainwindow.ui \
    lanaboutform.ui \
    lanmorseform.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    doc/about.html

RESOURCES += \
    LanterneResources.qrc