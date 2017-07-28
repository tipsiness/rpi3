#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QString>

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXTIMINGS		84
#define DHTPIN			7

int dht22_data[] = {0, 0, 0, 0, 0};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	//bcm2835_init();
	wiringPiSetup();

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_timer->start(1000);

	m_label = new QLabel(QString("Temperature & Humidity"), this);
	m_label->setGeometry(QRect(QPoint(100, 50), QSize(400, 80)));

	QFont font = m_label->font();
	font.setPointSize(20);
	font.setBold(true);
	m_label->setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimer() {
	// Read & Display CDS Cell Value
	int ret = readDHT22Data();
	QString qStr = QString("Temp: ") + QString::number(this->temp) + QString("(C)\nHumi: ") + QString::number(this->humi) + QString("%");
	m_label->setText(qStr);
}

unsigned int MainWindow::readDHT22Data() {
	uint8_t laststate = HIGH;
	uint8_t counter = 9;
	uint8_t i, j = 0;

	dht22_data[0] = dht22_data[1] = dht22_data[2] = dht22_data[3] = dht22_data[4] = 0;

	/* Single-Wire Two-Way Communication */
	pinMode(DHTPIN, OUTPUT);

	/* MCU->DHT, start signal */
	digitalWrite(DHTPIN, HIGH);
	delay(10);

	/* MCU pull down, DHT detect a start signal from MCU */
	digitalWrite(DHTPIN, LOW);
	delay(20);

	/* MCU pull up to receive response signal from DHT */
	digitalWrite(DHTPIN, HIGH);
	delayMicroseconds(30);

	/* MCU read response data consist of temperature and humidity */
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
		float t, h;
		h = (float)dht22_data[0] + (float)dht22_data[1] / 100.0;
		t = (float)dht22_data[2] + (float)dht22_data[3] / 100.0;

		printf("Humidity = %.2f %% Temperature = %.2f *C\n", h, t);

		this->temp = t;
		this->humi = h;

		return 1;
	} else {
		printf("Data not good, skip (read bytes: %d).\n", j);
		return 0;
	}
}





