#include "lanaudioinputform.h"
#include "ui_lanaudioinputform.h"

LanAudioInputForm::LanAudioInputForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LanAudioInputForm)
{
  ui->setupUi(this);
}

LanAudioInputForm::~LanAudioInputForm()
{
  delete ui;
}
/*

  // Test code:
  audioTestFile.setFileName("testAudio.raw");
  audioTestFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

  QAudioFormat format;
  format.setFrequency(8000);
  format.setChannels(1);
  format.setSampleSize(8);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::UnSignedInt);

  QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
  if (!info.isFormatSupported(format))
  {
    format = info.nearestFormat(format);
  }

  audio = new QAudioInput(format, this);
  QTimer::singleShot(3000, this, SLOT(stopRecording()));
  audio->start(&audioTestFile);
*/
