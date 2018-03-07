#include "BPViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	BPViewer mainWindow;
	mainWindow.show();
	return application.exec();
}
