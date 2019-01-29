#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setHumi(int h);
    int getHumi();
    void setTemp(int t);
    int getTemp();
    void readDHT11();

private slots:
    void onTimer();

private:
    Ui::MainWindow *ui;
    QLabel *m_label;
    QTimer *m_timer;
    int humi;
    int temp;
};

#endif // MAINWINDOW_H
