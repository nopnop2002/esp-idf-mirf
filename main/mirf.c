#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "mirf.h"

#define TAG "NRF24"

#if CONFIG_IDF_TARGET_ESP32
#define LCD_HOST    HSPI_HOST
#elif CONFIG_IDF_TARGET_ESP32S2
#define LCD_HOST    SPI2_HOST
#elif defined CONFIG_IDF_TARGET_ESP32C3
#define LCD_HOST    SPI2_HOST
#endif


static const int SPI_Frequency = 4000000;
//static const int SPI_Frequency = 8000000; // don't work

const char rf24_datarates[][8] = {"1MBPS", "2MBPS", "250KBPS"};
const char rf24_crclength[][10] = {"Disabled", "8 bits", "16 bits"};
const char rf24_pa_dbm[][8] = {"PA_MIN", "PA_LOW", "PA_HIGH", "PA_MAX"};

void spi_master_init(NRF24_t * dev, int8_t ce_pin, int8_t csn_pin, int miso_pin, int mosi_pin, int sclk_pin)
{
	esp_err_t ret;

	ESP_LOGI(TAG, "ce_pin=%d", ce_pin);
	ESP_LOGI(TAG, "csn_pin=%d", csn_pin);
	ESP_LOGI(TAG, "miso_pin=%d", miso_pin);
	ESP_LOGI(TAG, "mosi_pin=%d", mosi_pin);
	ESP_LOGI(TAG, "sclk_pin=%d", sclk_pin);

	//gpio_pad_select_gpio( ce_pin );
	gpio_reset_pin( ce_pin );
	gpio_set_direction( ce_pin, GPIO_MODE_OUTPUT );
	gpio_set_level( ce_pin, 0 );

	//gpio_pad_select_gpio( csn_pin );
	gpio_reset_pin( csn_pin );
	gpio_set_direction( csn_pin, GPIO_MODE_OUTPUT );
	gpio_set_level( csn_pin, 1 );

	spi_bus_config_t spi_bus_config = {
		.sclk_io_num = sclk_pin,
		.mosi_io_num = mosi_pin,
		.miso_io_num = miso_pin,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1
	};

	ret = spi_bus_initialize( LCD_HOST, &spi_bus_config, SPI_DMA_CH_AUTO );
	ESP_LOGI(TAG, "spi_bus_initialize=%d",ret);
	assert(ret==ESP_OK);

	spi_device_interface_config_t devcfg;
	memset( &devcfg, 0, sizeof( spi_device_interface_config_t ) );
	devcfg.clock_speed_hz = SPI_Frequency;
	devcfg.queue_size = 7;
	devcfg.mode = 0;
	devcfg.flags = SPI_DEVICE_NO_DUMMY;

#if 0
	spi_device_interface_config_t devcfg={
		.clock_speed_hz = SPI_Frequency,
		.queue_size = 7,
		.mode = 0,
		.flags = SPI_DEVICE_NO_DUMMY,
	};
#endif

	spi_device_handle_t handle;
	ret = spi_bus_add_device( LCD_HOST, &devcfg, &handle);
	ESP_LOGI(TAG, "spi_bus_add_device=%d",ret);
	assert(ret==ESP_OK);
	dev->cePin = ce_pin;
	dev->csnPin = csn_pin;
	dev->channel = 1;
	dev->payload = 16;
	dev->_SPIHandle = handle;
}


bool spi_master_write_byte(NRF24_t * dev, uint8_t* Dataout, size_t DataLength )
{
	spi_transaction_t SPITransaction;

	if ( DataLength > 0 ) {
		memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
		SPITransaction.length = DataLength * 8;
		SPITransaction.tx_buffer = Dataout;
		SPITransaction.rx_buffer = NULL;
		spi_device_transmit( dev->_SPIHandle, &SPITransaction );
	}

	return true;
}

bool spi_master_read_byte(NRF24_t * dev, uint8_t* Datain, uint8_t* Dataout, size_t DataLength )
{
	spi_transaction_t SPITransaction;

	if ( DataLength > 0 ) {
		memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
		SPITransaction.length = DataLength * 8;
		SPITransaction.tx_buffer = Dataout;
		SPITransaction.rx_buffer = Datain;
		spi_device_transmit( dev->_SPIHandle, &SPITransaction );
	}

	return true;
}

void Nrf24_transfer(NRF24_t * dev, uint8_t address) {
	uint8_t dataout[2];
	dataout[0] = address;
	spi_master_write_byte(dev, dataout, 1 );
}

void Nrf24_transferSync(NRF24_t * dev, uint8_t *dataout, uint8_t *datain, uint8_t len) {
	spi_master_read_byte(dev, datain, dataout, len );
}

void Nrf24_transmitSync(NRF24_t * dev, uint8_t *dataout, uint8_t len) {
	spi_master_write_byte(dev, dataout, len);
}


void Nrf24_config(NRF24_t * dev, uint8_t channel, uint8_t payload)
// Sets the important registers in the MiRF module and powers the module
// in receiving mode
// NB: channel and payload must be set now.
{
	dev->channel = channel;
	dev->payload = payload;
	Nrf24_configRegister(dev, RF_CH, dev->channel);		// Set RF channel
	Nrf24_configRegister(dev, RX_PW_P0, dev->payload);	// Set length of incoming payload
	Nrf24_configRegister(dev, RX_PW_P1, dev->payload);
	Nrf24_powerUpRx(dev);						 // Start receiver
	Nrf24_flushRx(dev);
}

void Nrf24_setRADDR(NRF24_t * dev, uint8_t * adr)  // Sets the receiving device address
{
	Nrf24_ceLow(dev);
	Nrf24_writeRegister(dev, RX_ADDR_P1, adr, mirf_ADDR_LEN);
	Nrf24_ceHi(dev);
}

void Nrf24_setTADDR(NRF24_t * dev, uint8_t * adr)			// Sets the transmitting device  address
{
	Nrf24_writeRegister(dev, RX_ADDR_P0, adr, mirf_ADDR_LEN);//RX_ADDR_P0 must be set to the sending addr for auto ack to work.
	Nrf24_writeRegister(dev, TX_ADDR, adr, mirf_ADDR_LEN);
}

extern bool Nrf24_dataReady(NRF24_t * dev)			  // Checks if data is available for reading
{
	uint8_t status = Nrf24_getStatus(dev);			  // See note in getData() function - just checking RX_DR isn't good enough
	if ( status & (1 << RX_DR) ) return 1;	 // We can short circuit on RX_DR, but if it's not set, we still need
	return !Nrf24_rxFifoEmpty(dev);					  // to check the FIFO for any pending packets
}

extern bool Nrf24_rxFifoEmpty(NRF24_t * dev)
{
	uint8_t fifoStatus;
	Nrf24_readRegister(dev, FIFO_STATUS, &fifoStatus, sizeof(fifoStatus));
	return (fifoStatus & (1 << RX_EMPTY));
}

extern void Nrf24_getData(NRF24_t * dev, uint8_t * data)// Reads payload bytes into data array
{
	Nrf24_csnLow(dev);								 // Pull down chip select
	Nrf24_transfer(dev, R_RX_PAYLOAD );			 // Send cmd to read rx payload
	Nrf24_transferSync(dev, data, data, dev->payload);		// Read payload
	Nrf24_csnHi(dev);								 // Pull up chip select
	// NVI: per product spec, p 67, note c:
	//	"The RX_DR IRQ is asserted by a new packet arrival event. The procedure
	//	for handling this interrupt should be: 1) read payload through SPI,
	//	2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more
	//	payloads available in RX FIFO, 4) if there are more data in RX FIFO,
	//	repeat from step 1)."
	// So if we're going to clear RX_DR here, we need to check the RX FIFO
	// in the dataReady() function
	Nrf24_configRegister(dev, STATUS, (1 << RX_DR)); // Reset status register
}

void Nrf24_configRegister(NRF24_t * dev, uint8_t reg, uint8_t value)  // Clocks only one byte into the given MiRF register
{
	Nrf24_csnLow(dev);
	Nrf24_transfer(dev, W_REGISTER | (REGISTER_MASK & reg));
	Nrf24_transfer(dev, value);
	Nrf24_csnHi(dev);
}

void Nrf24_readRegister(NRF24_t * dev, uint8_t reg, uint8_t * value, uint8_t len)// Reads an array of bytes from the given start position in the MiRF registers.
{
	Nrf24_csnLow(dev);
	Nrf24_transfer(dev, R_REGISTER | (REGISTER_MASK & reg));
	Nrf24_transferSync(dev, value, value, len);
	Nrf24_csnHi(dev);
}

void Nrf24_writeRegister(NRF24_t * dev, uint8_t reg, uint8_t * value, uint8_t len)// Writes an array of bytes into inte the MiRF registers.
{
	Nrf24_csnLow(dev);
	Nrf24_transfer(dev, W_REGISTER | (REGISTER_MASK & reg));
	Nrf24_transmitSync(dev, value, len);
	Nrf24_csnHi(dev);
}

void Nrf24_send(NRF24_t * dev, uint8_t * value)  // Sends a data package to the default address. Be sure to send the correct
									// amount of bytes as configured as payload on the receiver.
{
	uint8_t status;
	status = Nrf24_getStatus(dev);
	while (dev->PTX)				// Wait until last paket is send
	{
		status = Nrf24_getStatus(dev);
		if ((status & ((1 << TX_DS)  | (1 << MAX_RT))))
		{
			dev->PTX = 0;
			break;
		}
	}
	Nrf24_ceLow(dev);
	Nrf24_powerUpTx(dev);		// Set to transmitter mode , Power up
	Nrf24_csnLow(dev);					  // Pull down chip select
	Nrf24_transfer(dev, FLUSH_TX );		// Write cmd to flush tx fifo
	Nrf24_csnHi(dev);					 // Pull up chip select
	Nrf24_csnLow(dev);					  // Pull down chip select
	Nrf24_transfer(dev, W_TX_PAYLOAD ); // Write cmd to write payload
	Nrf24_transmitSync(dev, value, dev->payload);  // Write payload
	Nrf24_csnHi(dev);					 // Pull up chip select
	Nrf24_ceHi(dev);					 // Start transmission
}

/*
   isSending.
   Test if chip is still sending.
   When sending has finished return chip to listening.
*/
bool Nrf24_isSending(NRF24_t * dev) {
	uint8_t status;
	if (dev->PTX)
	{
		status = Nrf24_getStatus(dev);
		if ((status & ((1 << TX_DS)  | (1 << MAX_RT)))) {// if sending successful (TX_DS) or max retries exceded (MAX_RT).
			Nrf24_powerUpRx(dev);
			return false;
		}
		return true;
	}
	return false;
}

/**
   isSend.

   Test if Sending has finished or retry is over.
   When sending has finished return trur.
   When reach maximum number of TX retries return false.

*/

bool Nrf24_isSend(NRF24_t * dev) {
  uint8_t status;
  if (dev->PTX) {
    while(1) {
      status = Nrf24_getStatus(dev);
      /*
        if sending successful (TX_DS) or max retries exceded (MAX_RT).
      */

      if (status & (1 << TX_DS)) { // Data Sent TX FIFO interrup
        Nrf24_powerUpRx(dev);
        return true;
      }

      if (status & (1 << MAX_RT)) { // Maximum number of TX retries interrupt
        Nrf24_powerUpRx(dev);
        return false;
      }
      vTaskDelay(1);
    }
  }
  return false;
}


uint8_t Nrf24_getStatus(NRF24_t * dev) {
	uint8_t rv;
	Nrf24_readRegister(dev, STATUS, &rv, 1);
	return rv;
}

void Nrf24_powerUpRx(NRF24_t * dev) {
	dev->PTX = 0;
	Nrf24_ceLow(dev);
	Nrf24_configRegister(dev, CONFIG, mirf_CONFIG | ( (1 << PWR_UP) | (1 << PRIM_RX) ) ); //set device as TX mode
	Nrf24_ceHi(dev);
	Nrf24_configRegister(dev, STATUS, (1 << TX_DS) | (1 << MAX_RT));	  //Clear seeded interrupt and max tx number interrupt
}

void Nrf24_flushRx(NRF24_t * dev)
{
	Nrf24_csnLow(dev);
	Nrf24_transfer(dev, FLUSH_RX );
	Nrf24_csnHi(dev);
}

void Nrf24_powerUpTx(NRF24_t * dev) {
	dev->PTX = 1;
	Nrf24_configRegister(dev, CONFIG, mirf_CONFIG | ( (1 << PWR_UP) | (0 << PRIM_RX) ) );
}

void Nrf24_ceHi(NRF24_t * dev) {
	gpio_set_level( dev->cePin, 1 );
}

void Nrf24_ceLow(NRF24_t * dev) {
	gpio_set_level( dev->cePin, 0 );
}

void Nrf24_csnHi(NRF24_t * dev) {
	gpio_set_level( dev->csnPin, 1 );
}

void Nrf24_csnLow(NRF24_t * dev) {
	gpio_set_level( dev->csnPin, 0 );
}

void Nrf24_powerDown(NRF24_t * dev)
{
	Nrf24_ceLow(dev);
	Nrf24_configRegister(dev, CONFIG, mirf_CONFIG );
}

void Nrf24_SetOutputRF_PWR(NRF24_t * dev, uint8_t val) //Set tx power : 0=-18dBm,1=-12dBm,2=-6dBm,3=0dBm,
{
   Nrf24_configRegister(dev, RF_SETUP,	(val<< RF_PWR) );
}

void Nrf24_SetSpeedDataRates(NRF24_t * dev, uint8_t val) //Select between the high speed data rates:0=1Mbps, 1=2Mbps, 2=250Kbps
{
   if(val>1)
   {
	   Nrf24_configRegister(dev, RF_SETUP,	(1 << RF_DR_LOW) );
   }
   else
   {
	   Nrf24_configRegister(dev, RF_SETUP,	(val << RF_DR_HIGH) );
   }
} 

#define _BV(x) (1<<(x))

void Nrf24_printDetails(NRF24_t * dev)
{

	printf("================ SPI Configuration ================\n" );
	printf("CSN Pin  \t = GPIO%d\n",dev->csnPin);
	printf("CE Pin	\t = GPIO%d\n", dev->cePin);
	printf("Clock Speed\t = %d\n", SPI_Frequency);
	printf("================ NRF Configuration ================\n");

	Nrf24_print_status(Nrf24_getStatus(dev));

	Nrf24_print_address_register(dev, "RX_ADDR_P0-1", RX_ADDR_P0, 2);
	Nrf24_print_byte_register(dev, "RX_ADDR_P2-5", RX_ADDR_P2, 4);
	Nrf24_print_address_register(dev, "TX_ADDR\t", TX_ADDR, 1);

	Nrf24_print_byte_register(dev, "RX_PW_P0-6", RX_PW_P0, 6);
	Nrf24_print_byte_register(dev, "EN_AA\t", EN_AA, 1);
	Nrf24_print_byte_register(dev, "EN_RXADDR", EN_RXADDR, 1);
	Nrf24_print_byte_register(dev, "RF_CH\t", RF_CH, 1);
	Nrf24_print_byte_register(dev, "RF_SETUP", RF_SETUP, 1);
	Nrf24_print_byte_register(dev, "CONFIG\t", CONFIG, 1);
	Nrf24_print_byte_register(dev, "DYNPD/FEATURE", DYNPD, 2);
	//printf("getDataRate()=%d\n",Nrf24_getDataRate(dev));
	printf("Data Rate\t = %s\n",rf24_datarates[Nrf24_getDataRate(dev)]);
#if 0
	printf_P(PSTR("Model\t\t = "
	PRIPSTR
	"\r\n"),pgm_read_ptr(&rf24_model_e_str_P[isPVariant()]));
#endif
	//printf("getCRCLength()=%d\n",Nrf24_getCRCLength(dev));
	printf("CRC Length\t = %s\n", rf24_crclength[Nrf24_getCRCLength(dev)]);
	//printf("getPALevel()=%d\n",Nrf24_getPALevel(dev));
	printf("PA Power\t = %s\n", rf24_pa_dbm[Nrf24_getPALevel(dev)]);

}

void Nrf24_print_status(uint8_t status)
{
	printf("STATUS\t\t = 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\r\n", status, (status & _BV(RX_DR)) ? 1 : 0,
			(status & _BV(TX_DS)) ? 1 : 0, (status & _BV(MAX_RT)) ? 1 : 0, ((status >> RX_P_NO) & 0x07), (status & _BV(TX_FULL)) ? 1 : 0);
}

void Nrf24_print_address_register(NRF24_t * dev, const char* name, uint8_t reg, uint8_t qty)
{
	printf("%s\t =",name);
	while (qty--) {
		//uint8_t buffer[addr_width];
		uint8_t buffer[5];
		Nrf24_readRegister(dev, reg++, buffer, sizeof(buffer));

		printf(" 0x");
		uint8_t* bufptr = buffer + sizeof buffer;
		while (--bufptr >= buffer) {
			printf("%02x", *bufptr);
		}
	}
	printf("\r\n");
}

void Nrf24_print_byte_register(NRF24_t * dev, const char* name, uint8_t reg, uint8_t qty)
{
	printf("%s\t =", name);
	while (qty--) {
		uint8_t buffer[1];
		Nrf24_readRegister(dev, reg++, buffer, 1);
		printf(" 0x%02x", buffer[0]);
	}
	printf("\r\n");
}

rf24_datarate_e Nrf24_getDataRate(NRF24_t * dev)
{
	rf24_datarate_e result;
	uint8_t dr;
	Nrf24_readRegister(dev, RF_SETUP, &dr, sizeof(dr));
	//printf("RF_SETUP=%x\n",dr);
	dr = dr & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

	// switch uses RAM (evil!)
	// Order matters in our case below
	if (dr == _BV(RF_DR_LOW)) {
		// '10' = 250KBPS
		result = RF24_250KBPS;
	} else if (dr == _BV(RF_DR_HIGH)) {
		// '01' = 2MBPS
		result = RF24_2MBPS;
	} else {
		// '00' = 1MBPS
		result = RF24_1MBPS;
	}
	return result;
}

rf24_crclength_e Nrf24_getCRCLength(NRF24_t * dev)
{
	rf24_crclength_e result = RF24_CRC_DISABLED;

	uint8_t config;
	Nrf24_readRegister(dev, CONFIG, &config, sizeof(config));
	//printf("CONFIG=%x\n",config);
	config = config & (_BV(CRCO) | _BV(EN_CRC));
	uint8_t AA;
	Nrf24_readRegister(dev, EN_AA, &AA, sizeof(AA));

	if (config & _BV(EN_CRC) || AA) {
		if (config & _BV(CRCO)) {
			result = RF24_CRC_16;
		} else {
			result = RF24_CRC_8;
		}
	}

	return result;
}

uint8_t Nrf24_getPALevel(NRF24_t * dev)
{
	uint8_t level;
	Nrf24_readRegister(dev, RF_SETUP, &level, sizeof(level));
	//printf("RF_SETUP=%x\n",level);
	level = (level & (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH))) >> 1;
	return (level);
}

