#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rectintegration.h"
#include "trapintegration.h"
#include "gaussintegration.h"

#include <QDebug>
#include <memory>


double square(double x)
{
    return x * x;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::unique_ptr<Integral> algo(new GaussIntegration);
    qDebug() << algo->integrate(square, 0, 1, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}
