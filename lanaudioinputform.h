#ifndef LANAUDIOINPUTFORM_H
#define LANAUDIOINPUTFORM_H

#include <QWidget>

namespace Ui {
class LanAudioInputForm;
}

class LanAudioInputForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LanAudioInputForm(QWidget *parent = 0);
  ~LanAudioInputForm();
  
private:
  Ui::LanAudioInputForm *ui;
};

#endif // LANAUDIOINPUTFORM_H
