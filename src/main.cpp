#include "BPViewer.h"
#include <QtWidgets/QApplication>

#include <QtSql>

int main(int argc, char *argv[])
{
	QString connectionString = "Driver={SQL Server};"
		"Server=tcp:tommygod3.database.windows.net,1433;"
		"Database=Test;"
		"Uid=james;"
		"Pwd=SjowIsDumpster10.;"
		"Encrypt=yes;"
		"TrustServerCertificate=no;"
		"Connection Timeout=30;";

	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	db.setDatabaseName(connectionString);
	bool ok = false;
	if (db.open())
	{
		ok = true;
	}
	else
	{
		QString error = db.lastError().text();
		ok = false;
	}
	QSqlQuery query;
	query.exec("SELECT * FROM Allowance");
	while (query.next()) 
	{
		std::string flute = query.value(0).toString().toStdString();
		std::string allowance = query.value(1).toString().toStdString();
	}

	QApplication application(argc, argv);
	BPViewer mainWindow;
	mainWindow.show();
	return application.exec();
}
