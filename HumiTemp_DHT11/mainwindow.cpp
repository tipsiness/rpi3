#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <wiringPi.h>
#include <QTextStream>
#include <sys/types.h>
#include <stdint.h>

#define MAXTIMINGS		84
#define DHTPIN			7

int dht22_data[] = {0, 0, 0, 0, 0};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wiringPiSetup();
    setHumi(0);
    setTemp(0);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    m_timer->start(2000);

    m_label = new QLabel("Humidity & Temperature", this);
    m_label->setGeometry(QRect(QPoint(50, 50), QSize(350, 200)));

    QFont font = m_label->font();
    font.setPointSize(20);
    font.setBold(true);
    m_label->setFont(font);
    //m_label->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setHumi(int h) { this->humi = h; }
int MainWindow::getHumi() { return this->humi; }
void MainWindow::setTemp(int t) { this->temp = t; }
int MainWindow::getTemp() { return this->temp; }

void MainWindow::onTimer() {
    readDHT11();

	QString qStr = QString("Humidity: ") + QString::number(getHumi()) \
				   + QString("\nTemperature: ") + QString::number(getTemp());

	m_label->setText(qStr);
}

void MainWindow::readDHT11() {
	uint8_t laststate = HIGH;
	uint8_t counter = 9;
	uint8_t i, j = 0;

	dht22_data[0] = dht22_data[1] = dht22_data[2] = dht22_data[3] = dht22_data[4] = 0;

	pinMode(DHTPIN, OUTPUT);

	digitalWrite(DHTPIN, HIGH);
	delay(10);

	digitalWrite(DHTPIN, LOW);
	delay(20);

	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(30);

	pinMode(DHTPIN, INPUT);

	for (i = 0; i < MAXTIMINGS; i++) {
		counter = 0;
		while (digitalRead(DHTPIN) == laststate) {
			counter++;
			delayMicroseconds(1);

			if (counter == 75)
				break;
		}
		laststate = digitalRead(DHTPIN);

		if ((i >= 4) && (i % 2 == 0)) {
			dht22_data[j/8] <<= 1;		// read bit '0'
			if (counter > 30)
				dht22_data[j/8] |= 1;	// read bit '1'
			j++;
		}
	}

	if ((j >= 40) && (dht22_data[4] == ((dht22_data[0] + dht22_data[1] + dht22_data[2] + dht22_data[3]) & 0xFF))) {
		int t, h;
		h = dht22_data[0];
        t = dht22_data[2];

		setTemp(t);
		setHumi(h);

		return;
	} 
}
