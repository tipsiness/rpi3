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
	unsigned int readDHT22Data();

private slots:
	void onTimer();

private:
    Ui::MainWindow *ui;
	QLabel *m_label;
	QTimer *m_timer;
	float temp;
	float humi;
};

#endif // MAINWINDOW_H
