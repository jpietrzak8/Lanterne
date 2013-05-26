#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class LanFlashLED;
class QTimer;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  enum ScreenOrientation {
      ScreenOrientationLockPortrait,
      ScreenOrientationLockLandscape,
      ScreenOrientationAuto
  };

  explicit MainWindow(QWidget *parent = 0);
  virtual ~MainWindow();

  // Note that this will only have an effect on Symbian and Fremantle.
  void setOrientation(ScreenOrientation orientation);

  void showExpanded();

private slots:
  void on_torchBrightnessSpinBox_valueChanged(int arg1);
  void on_torchPushButton_clicked();

  void on_flashBrightnessSpinBox_valueChanged(int arg1);
  void on_flashDurationSpinBox_valueChanged(int arg1);
  void on_flashPauseSpinBox_valueChanged(int arg1);
//  void on_strobeFlashButton_clicked();
  void on_strobeFlashButton_pressed();
  void on_strobeFlashButton_released();

  void strobe();

private:
  LanFlashLED *led;

  int minPause;
  int maxPause;
  int chosenPause;
  QTimer *ledTimer;

  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
