#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("CodeWizard");
    mainWindow.show();

    return app.exec();
}
