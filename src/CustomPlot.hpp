#pragma once

#include <qcustomplot.h>
#include <QWidget>

class CustomPlot : public QCustomPlot {
  CustomPlot(QWidget* parent = nullptr) : QCustomPlot(parent) {

  }
};
