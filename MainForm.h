#ifndef FRMSWITCHBUTTON_H
#define FRMSWITCHBUTTON_H

#include <QWidget>
#include <thread>
#include <memory>
#include <atomic>

namespace Ui
{
class MainForm;
}

class AVFormatContext;


class MainForm : public QWidget
{
	Q_OBJECT

public:
	explicit MainForm(QWidget *parent = 0);
	~MainForm();
private:
	Ui::MainForm*ui;
};

#endif // frmSwitchButton_H
