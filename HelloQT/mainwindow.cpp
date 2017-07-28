#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <wiringPi.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	wiringPiSetup();

	pinMode(0, INPUT);

	if (digitalRead(0) == LOW) {
		m_button = new QPushButton("ON", this);
		btnState = false;
	} else if (digitalRead(0) == HIGH) {
		m_button = new QPushButton("OFF", this);
		btnState = true;
	}

	m_button->setGeometry(QRect(QPoint(100, 50), QSize(200, 50)));

	connect(m_button, SIGNAL(released()), this, SLOT(handleButton()));

	pinMode(0, OUTPUT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton() {
	if (btnState) {
		digitalWrite(0, LOW);
		m_button->setText("ON");
	} else {
		digitalWrite(0, HIGH);
		m_button->setText("OFF");
	}
	btnState = !btnState;
}

