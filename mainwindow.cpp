#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clear_but_clicked()
{
    ui->ci95->setChecked(true);

    ui->lowerci_in->clear();
    ui->upperci_in->clear();
    ui->mean_in->clear();
    ui->variance_in->clear();
    ui->n_in->clear();
    ui->se_in->clear();

    ui->n_res->setText("-");
    ui->sd_res->setText("-");
    ui->mean_res->setText("-");
}

