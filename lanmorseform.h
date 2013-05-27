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
