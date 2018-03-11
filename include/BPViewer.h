#ifndef BPVIEWER_H
#define BPVIEWER_H

#include <QtWidgets>
#include "ui_BPViewer.h"
#include "order.h"

class BPViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit BPViewer(QWidget *parent = Q_NULLPTR);

private:
	Ui::BPViewerClass ui;

	BP::Order* order;

	void showMessage(std::string text);

public slots:
	void setFlute();
	void setPW();
	void setPQ();
	void setStyle();
	void setLength();
	void setWidth();
	void setHeight();
	void setQuantity();
	void setPPB();
	void setPOT();
	void calculateValues();
	void resetValues();
};

#endif