#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new TaskModel;
    ui->tableView->setModel(m_model);
    m_dataReader.reset( new DataReader("data.txt") );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_dataReader->getAllTasksAsync([=](TasksVector vect)
    {
        m_model->resetModel( vect );
    });
}
