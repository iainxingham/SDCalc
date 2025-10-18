#include <QMessageBox>
#include <QtMath>
#include <QClipboard>

#include <boost/math/distributions/students_t.hpp>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->copy_lab->setText("");
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

    ui->copy_lab->setText("");
}


void MainWindow::on_calc_but_clicked()
{
    double mean;
    double val;
    double sd;
    unsigned n;
    bool ok;

    ui->copy_lab->setText("");

    mean = ui->mean_in->text().toDouble(&ok);

    if(not ok) {
        errorBox("Mean missing or invalid");
        ui->mean_res->setText("-");
        ui->n_res->setText("-");
        ui->sd_res->setText("-");

        return;
    }

    ui->mean_res->setText(QString::number(mean));

    n = ui->n_in->text().toUInt(&ok);
    if(not ok) {
        errorBox("Sample size missing or invalid");
        ui->n_res->setText("-");
        ui->sd_res->setText("-");

        return;
    }

    ui->n_res->setText(QString::number(n));

    val = ui->se_in->text().toDouble(&ok);
    if(ok) {
        sd = val * (qSqrt(n));
        ui->sd_res->setText(QString::number(sd));
        return;
    }

    val = ui->variance_in->text().toDouble(&ok);
    if(ok) {
        sd = qSqrt(val);
        ui->sd_res->setText(QString::number(sd));
        return;
    }

    val = ui->upperci_in->text().toDouble(&ok);
    if(ok) {
        double val2;
        double interval;
        val2 = ui->lowerci_in->text().toDouble(&ok);

        if(not ok) {
            errorBox("Found upper confidence interval but lower confidence interval missing or invalid");
            ui->sd_res->setText("-");
            return;
        }

        if(val > val2) interval = val - val2;
        else interval = val2 - val;

        double denom;
        if(ui->ci90->isChecked()) denom = 2 * getT(0.1, n-1);
        else if(ui->ci95->isChecked()) denom = 2 * getT(0.05, n-1);
        else denom = 2 * getT(0.01, n-1);  // 99% CI

        sd = qSqrt(n) * (interval / denom);
        ui->sd_res->setText(QString::number(sd));
        return;
    }

    errorBox("Insufficient information to calculate standard deviation");
}

void MainWindow::errorBox(QString err)
{
    QMessageBox msg(this);

    msg.setWindowTitle("There is an error");
    msg.setText(err);
    msg.setIcon(QMessageBox::Critical);
    msg.exec();
}

// Calculate t for given p and degrees of freedom
double MainWindow::getT(double p, unsigned int df)
{
    boost::math::students_t dist(df);

    return quantile(complement(dist, p / 2));
}


void MainWindow::on_copy_but_clicked()
{
    if((ui->mean_res->text() == "-") or
        (ui->sd_res->text() == "-") or
        (ui->n_res->text() == "-")
        ) {
        errorBox("No results to export");
        return;
    }

    QClipboard *clip = QGuiApplication::clipboard();
    clip->setText(ui->mean_res->text() + "\t" +
                  ui->sd_res->text() + "\t" +
                  ui->n_res->text());

    ui->copy_lab->setText("Copied!");
}

