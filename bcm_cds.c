#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <bcm2835.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000  // 1MHz


int read_mcp3208_adc(unsigned char adcChannel)
{
  unsigned char tx_buff[3];
  unsigned char rx_buff[3];
  int adcValue = 0;

  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_begin();

  tx_buff[0] = 0x06 | ((adcChannel & 0x07) >> 7);
  tx_buff[1] = ((adcChannel & 0x07) << 6);
  tx_buff[2] = 0x00;

  bcm2835_spi_transfernb(tx_buff, rx_buff, 3);

  rx_buff[1] = 0x0F & rx_buff[1];
  adcValue = ( rx_buff[1] << 8) | rx_buff[2];

  bcm2835_spi_end();

  return adcValue;
}


int main (void)
{
  int adcChannel = 0;
  int adcValue   = 0;

  bcm2835_init();

  while(1)
  {
    adcValue = read_mcp3208_adc(adcChannel);
    printf("CdS Value = %u, %1.2f(V)\n", adcValue, ((3.3/4096)*adcValue));
	usleep(500000);
  }

  return 0;
}
