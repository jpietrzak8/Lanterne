#ifndef LANABOUTFORM_H
#define LANABOUTFORM_H

#include <QWidget>

namespace Ui {
class LanAboutForm;
}

class LanAboutForm : public QWidget
{
  Q_OBJECT
  
public:
  explicit LanAboutForm(QWidget *parent = 0);
  ~LanAboutForm();
  
private:
  Ui::LanAboutForm *ui;
};

#endif // LANABOUTFORM_H
