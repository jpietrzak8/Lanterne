#include "lanaboutform.h"
#include "ui_lanaboutform.h"

LanAboutForm::LanAboutForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LanAboutForm)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_Maemo5StackedWindow);
  setWindowFlags(windowFlags() | Qt::Window);
}

LanAboutForm::~LanAboutForm()
{
  delete ui;
}
