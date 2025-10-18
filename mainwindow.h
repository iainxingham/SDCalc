#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clear_but_clicked();

    void on_calc_but_clicked();

    void on_copy_but_clicked();

private:
    Ui::MainWindow *ui;

    void errorBox(QString err);
    double getT(double p, unsigned df);
};
#endif // MAINWINDOW_H
