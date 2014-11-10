#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rectintegration.h"
#include "trapintegration.h"
#include "gaussintegration.h"
#include "parabintegral.h"

#include <QDebug>
#include <memory>
#include <vector>

double courantFunction(const double x, const int i, const std::vector<double> &xValues)
{
    if(x >= xValues[i - 1] && x < xValues[i])
    {
        return (xValues[i] - x) / (xValues[i] - xValues[i - 1]);
    }
    else if(x > xValues[i] && x <= xValues[i + 1])
    {
        return (x - xValues[i]) / (xValues[i + 1] - xValues[i]);
    }
    else
    {
        return 0.0;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    double h = 4.0 / 100;
    std::vector<double> data;
    for(int i = 0; i < 101; ++i)
    {
        data.push_back(static_cast<double>(i) * h);
    }
    for(int i = 1; i < 100; ++i)
    {
        qDebug() << courantFunction(0.3, i, data);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
