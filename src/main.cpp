#include "CustomPlot.hpp"
#include <QApplication>
#include <QVBoxLayout>
#include <qcustomplot.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  auto *widget = new QWidget;
  auto *layout = new QVBoxLayout;
  widget->setLayout(layout);

  auto *customPlot = new CustomPlot;
  layout->addWidget(customPlot);

  widget->show();
  return QApplication::exec();
}
