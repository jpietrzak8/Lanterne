#include "lanmorseform.h"
#include "ui_lanmorseform.h"

#include "mainwindow.h"
#include <QTimer>

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
}


LanMorseForm::~LanMorseForm()
{
  if (timer) delete timer;

  delete ui;
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
  morseCodeBits.push_back(true);
  morseCodeBits.push_back(true);
  morseCodeBits.push_back(true);
  morseCodeBits.push_back(false);
}


void LanMorseForm::threeUnitGap()
{
  morseCodeBits.push_back(false);
  morseCodeBits.push_back(false);
  morseCodeBits.push_back(false);
}


void LanMorseForm::fourUnitGap()
{
  morseCodeBits.push_back(false);
  morseCodeBits.push_back(false);
  morseCodeBits.push_back(false);
  morseCodeBits.push_back(false);
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