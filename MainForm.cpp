#include "MainForm.h"
#include "ui_MainForm.h"



MainForm::MainForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MainForm)
{
	ui->setupUi(this);	
	m_audio_module.InitRecording(2, 48000, 2);
}

MainForm::~MainForm()
{
	delete ui;
}