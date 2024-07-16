#include <QApplication>
#include <iostream>

#include "CustomPlot.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
    MainWindow() : QMainWindow(), customPlot(new CustomPlot) {
      setCentralWidget(customPlot);
    }
private:
    CustomPlot customPlot;
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return QApplication::exec();
}
