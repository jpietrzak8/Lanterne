#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#include "lanflashled.h"
#include "lanaboutform.h"
#include "lanmorseform.h"
#include <QTimer>

MainWindow::MainWindow(
  QWidget *parent)
  : QMainWindow(parent),
    led(0),
    aboutForm(0),
    morseForm(0),
    minPause(100),
    maxPause(2000),
    chosenPause(500),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);

  led = new LanFlashLED();

  // set up the spin boxes:
  ui->torchBrightnessSpinBox->setMinimum(led->getMinTorch());
  ui->torchBrightnessSpinBox->setMaximum(led->getMaxTorch());
  ui->torchBrightnessSpinBox->setValue(led->getMaxTorch());
  ui->flashBrightnessSpinBox->setMinimum(led->getMinFlash());
  ui->flashBrightnessSpinBox->setMaximum(led->getMaxFlash());
  ui->flashBrightnessSpinBox->setValue(led->getMinFlash());
  ui->flashDurationSpinBox->setMinimum(led->getMinTime() / 1000);
  ui->flashDurationSpinBox->setMaximum(led->getMaxTime() / 1000);
  ui->flashDurationSpinBox->setValue((led->getMaxTime() / 1000) / 2);
  ui->flashPauseSpinBox->setMinimum(minPause);
  ui->flashPauseSpinBox->setMaximum(maxPause);
  ui->flashPauseSpinBox->setValue(chosenPause);
}

MainWindow::~MainWindow()
{
  if (ledTimer) delete ledTimer;

  if (led) delete led;
  if (morseForm) delete morseForm;
  if (aboutForm) delete aboutForm;

  delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::on_torchBrightnessSpinBox_valueChanged(int arg1)
{
  led->setTorchBrightness(arg1);
}

void MainWindow::on_torchPushButton_clicked()
{
  led->toggleTorch();
}

void MainWindow::on_flashBrightnessSpinBox_valueChanged(int arg1)
{
  led->setFlashBrightness(arg1);
}

void MainWindow::on_flashDurationSpinBox_valueChanged(int arg1)
{
  led->setFlashDuration(arg1 * 1000);
}

void MainWindow::on_flashPauseSpinBox_valueChanged(int arg1)
{
  if (arg1 < minPause)
  {
    chosenPause = minPause;
  }
  else if (arg1 > maxPause)
  {
    chosenPause = maxPause;
  }
  else
  {
    chosenPause = arg1;
  }
}

/*
void MainWindow::on_strobeFlashButton_clicked()
{
  led->strobe();
}
*/

void MainWindow::on_strobeFlashButton_pressed()
{
  if (!ledTimer)
  {
    ledTimer = new QTimer(this);
    connect(ledTimer, SIGNAL(timeout()), this, SLOT(strobe()));

    ledTimer->start(chosenPause + (led->getChosenTime() / 1000));
  }
}


void MainWindow::on_strobeFlashButton_released()
{
  if (ledTimer)
  {
    delete ledTimer;
    ledTimer = 0;
  }
}


void MainWindow::strobe()
{
  led->strobe();
}

void MainWindow::on_actionMorse_Code_triggered()
{
  if (!morseForm)
  {
    morseForm = new LanMorseForm(this);
  }

  morseForm->show();
}

void MainWindow::on_actionAbout_triggered()
{
  if (!aboutForm)
  {
    aboutForm = new LanAboutForm(this);
  }

  aboutForm->show();
}


void MainWindow::turnTorchOn()
{
  led->turnTorchOn();
}


void MainWindow::turnTorchOff()
{
  led->turnTorchOff();
}
