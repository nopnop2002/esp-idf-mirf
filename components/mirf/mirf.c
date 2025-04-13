#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "mirf.h"

#define TAG "NRF24"

// SPI Stuff
#if CONFIG_SPI2_HOST
#define HOST_ID SPI2_HOST
#elif CONFIG_SPI3_HOST
#define HOST_ID SPI3_HOST
#endif

static const int SPI_Frequency = 4000000; // Stable even with a long jumper cable
//static const int SPI_Frequency = 6000000;
//static const int SPI_Frequency = 8000000; // Requires a short jumper cable
//static const int SPI_Frequency = 10000000; // Unstable even with a short jumper cable

//const char rf24_datarates[][8] = {"1Mbps", "2Mbps", "250Kbps"};
char rf24_datarates[][8] = {"1Mbps", "2Mbps", "250Kbps"};
const char rf24_crclength[][10] = {"Disabled", "8 bits", "16 bits"};
//const char rf24_pa_dbm[][8] = {"PA_MIN", "PA_LOW", "PA_HIGH", "PA_MAX"};
char rf24_pa_dbm[][8] = {"PA_MIN", "PA_LOW", "PA_HIGH", "PA_MAX"};

void Nrf24_init(NRF24_t * dev)
{
	esp_err_t ret;

	ESP_LOGI(TAG, "CONFIG_MISO_GPIO=%d", CONFIG_MISO_GPIO);
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d", CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d", CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CE_GPIO=%d", CONFIG_CE_GPIO);
	ESP_LOGI(TAG, "CONFIG_CSN_GPIO=%d", CONFIG_CSN_GPIO);

	//gpio_pad_select_gpio(CONFIG_CE_GPIO);
	gpio_reset_pin(CONFIG_CE_GPIO);
	gpio_set_direction(CONFIG_CE_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(CONFIG_CE_GPIO, 0);

	//gpio_pad_select_gpio(CONFIG_CSN_GPIO);
	gpio_reset_pin(CONFIG_CSN_GPIO);
	gpio_set_direction(CONFIG_CSN_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(CONFIG_CSN_GPIO, 1);

	spi_bus_config_t spi_bus_config = {
		.sclk_io_num = CONFIG_SCLK_GPIO,
		.mosi_io_num = CONFIG_MOSI_GPIO,
		.miso_io_num = CONFIG_MISO_GPIO,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1
	};

	ret = spi_bus_initialize( HOST_ID, &spi_bus_config, SPI_DMA_CH_AUTO );
	ESP_LOGI(TAG, "spi_bus_initialize=%d",ret);
	assert(ret==ESP_OK);

	spi_device_interface_config_t devcfg;
	memset( &devcfg, 0, sizeof( spi_device_interface_config_t ) );
	devcfg.clock_speed_hz = SPI_Frequency;
	// It does not work with hardware CS control.
	//devcfg.spics_io_num = csn_pin;
	// It does work with software CS control.
	devcfg.spics_io_num = -1;
	devcfg.queue_size = 7;
	devcfg.mode = 0;
	devcfg.flags = SPI_DEVICE_NO_DUMMY;

	spi_device_handle_t handle;
	ret = spi_bus_add_device( HOST_ID, &devcfg, &handle);
	ESP_LOGI(TAG, "spi_bus_add_device=%d",ret);
	assert(ret==ESP_OK);

	dev->cePin = CONFIG_CE_GPIO;
	dev->csnPin = CONFIG_CSN_GPIO;
	dev->channel = 1;
	dev->payload = 16;
	dev->_SPIHandle = handle;
}

void Nrf24_deinit(NRF24_t *dev) {
	memset(dev, 0, sizeof(NRF24_t));
	spi_bus_free(HOST_ID);
}

bool spi_write_byte(NRF24_t * dev, uint8_t* Dataout, size_t DataLength )
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

bool spi_read_byte(NRF24_t * dev, uint8_t* Datain, uint8_t* Dataout, size_t DataLength )
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

uint8_t spi_transfer(NRF24_t * dev, uint8_t address) {
	uint8_t datain[1];
	uint8_t dataout[1];
	dataout[0] = address;
	//spi_write_byte(dev, dataout, 1 );
	spi_read_byte(dev, datain, dataout, 1 );
	return datain[0];
}

void spi_csnHi(NRF24_t * dev) {
	gpio_set_level( dev->csnPin, 1 );
}

void spi_csnLow(NRF24_t * dev) {
	gpio_set_level( dev->csnPin, 0 );
}

// Sets the important registers in the MiRF module and powers the module
// in receiving mode
// NB: channel and payload must be set now.
void Nrf24_config(NRF24_t * dev, uint8_t channel, uint8_t payload)
{
	dev->channel = channel;
	dev->payload = payload;
	Nrf24_configRegister(dev, RF_CH, dev->channel); // Set RF channel
	Nrf24_configRegister(dev, RX_PW_P0, dev->payload); // Set length of incoming payload
	Nrf24_configRegister(dev, RX_PW_P1, dev->payload);
	Nrf24_powerUpRx(dev); // Start receiver
	Nrf24_flushRx(dev);
}

// Sets the receiving device address
//void Nrf24_setRADDR(NRF24_t * dev, uint8_t * adr)
esp_err_t Nrf24_setRADDR(NRF24_t * dev, uint8_t * adr)
{
	esp_err_t ret = ESP_OK;
	Nrf24_writeRegister(dev, RX_ADDR_P1, adr, mirf_ADDR_LEN);
	uint8_t buffer[5];
	Nrf24_readRegister(dev, RX_ADDR_P1, buffer, sizeof(buffer));
	for (int i=0;i<5;i++) {
		ESP_LOGD(TAG, "adr[%d]=0x%x buffer[%d]=0x%x", i, adr[i], i, buffer[i]);
		if (adr[i] != buffer[i]) ret = ESP_FAIL;
	}
	return ret;
}

// Sets the transmitting device  address
//void Nrf24_setTADDR(NRF24_t * dev, uint8_t * adr)
esp_err_t Nrf24_setTADDR(NRF24_t * dev, uint8_t * adr)
{
	esp_err_t ret = ESP_OK;
	Nrf24_writeRegister(dev, RX_ADDR_P0, adr, mirf_ADDR_LEN); //RX_ADDR_P0 must be set to the sending addr for auto ack to work.
	Nrf24_writeRegister(dev, TX_ADDR, adr, mirf_ADDR_LEN);
	uint8_t buffer[5];
	Nrf24_readRegister(dev, RX_ADDR_P0, buffer, sizeof(buffer));
	for (int i=0;i<5;i++) {
		ESP_LOGD(TAG, "adr[%d]=0x%x buffer[%d]=0x%x", i, adr[i], i, buffer[i]);
		if (adr[i] != buffer[i]) ret = ESP_FAIL;
	}
	return ret;
}

// Add the receiving device address
void Nrf24_addRADDR(NRF24_t * dev, uint8_t pipe, uint8_t adr)
{
	uint8_t value;
	Nrf24_readRegister(dev, EN_RXADDR, &value, 1);

	if (pipe == 2) {
		Nrf24_configRegister(dev, RX_PW_P2, dev->payload);
		Nrf24_configRegister(dev, RX_ADDR_P2, adr);
		value = value | 0x04;
		Nrf24_configRegister(dev, EN_RXADDR, value);
	} else if (pipe == 3) {
		Nrf24_configRegister(dev, RX_PW_P3, dev->payload);
		Nrf24_configRegister(dev, RX_ADDR_P3, adr);
		value = value | 0x08;
		Nrf24_configRegister(dev, EN_RXADDR, value);
	} else if (pipe == 4) {
		Nrf24_configRegister(dev, RX_PW_P4, dev->payload);
		Nrf24_configRegister(dev, RX_ADDR_P4, adr);
		value = value | 0x10;
		Nrf24_configRegister(dev, EN_RXADDR, value);
	} else if (pipe == 5) {
		Nrf24_configRegister(dev, RX_PW_P5, dev->payload);
		Nrf24_configRegister(dev, RX_ADDR_P5, adr);
		value = value | 0x20;
		Nrf24_configRegister(dev, EN_RXADDR, value);
	}
}

// Checks if data is available for reading
extern bool Nrf24_dataReady(NRF24_t * dev)
{
	// See note in getData() function - just checking RX_DR isn't good enough
	uint8_t status = Nrf24_getStatus(dev);
	//printf("Nrf24_dataReady status=0x%x\n", status);
	if ( status & (1 << RX_DR) ) {
		// Save status
		dev->status = status;
		return 1;
	}
	// We can short circuit on RX_DR, but if it's not set, we still need
	// to check the FIFO for any pending packets
	//return !Nrf24_rxFifoEmpty(dev);
	return 0;
}

// Get pipe number for reading
uint8_t Nrf24_getDataPipe(NRF24_t * dev) {
	//uint8_t status = Nrf24_getStatus(dev);
	//printf("dev->status=0x%x\n",dev->status);
	return ((dev->status & 0x0E) >> 1);
}

extern bool Nrf24_rxFifoEmpty(NRF24_t * dev)
{
	uint8_t fifoStatus;
	Nrf24_readRegister(dev, FIFO_STATUS, &fifoStatus, sizeof(fifoStatus));
	return (fifoStatus & (1 << RX_EMPTY));
}

// Reads payload bytes into data array
extern void Nrf24_getData(NRF24_t * dev, uint8_t * data)
{
	spi_csnLow(dev); // Pull down chip select
	spi_transfer(dev, R_RX_PAYLOAD ); // Send cmd to read rx payload
	spi_read_byte(dev, data, data, dev->payload); // Read payload
	spi_csnHi(dev); // Pull up chip select
	// NVI: per product spec, p 67, note c:
	// "The RX_DR IRQ is asserted by a new packet arrival event. The procedure
	// for handling this interrupt should be: 1) read payload through SPI,
	// 2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more
	// payloads available in RX FIFO, 4) if there are more data in RX FIFO,
	// repeat from step 1)."
	// So if we're going to clear RX_DR here, we need to check the RX FIFO
	// in the dataReady() function
	Nrf24_configRegister(dev, STATUS, (1 << RX_DR)); // Reset status register
}

// Clocks only one byte into the given MiRF register
void Nrf24_configRegister(NRF24_t * dev, uint8_t reg, uint8_t value)
{
	spi_csnLow(dev);
	spi_transfer(dev, W_REGISTER | (REGISTER_MASK & reg));
	spi_transfer(dev, value);
	spi_csnHi(dev);
}

// Reads an array of bytes from the given start position in the MiRF registers
void Nrf24_readRegister(NRF24_t * dev, uint8_t reg, uint8_t * value, uint8_t len)
{
	spi_csnLow(dev);
	spi_transfer(dev, R_REGISTER | (REGISTER_MASK & reg));
	spi_read_byte(dev, value, value, len);
	spi_csnHi(dev);
}

// Writes an array of bytes into inte the MiRF registers
void Nrf24_writeRegister(NRF24_t * dev, uint8_t reg, uint8_t * value, uint8_t len)
{
	spi_csnLow(dev);
	spi_transfer(dev, W_REGISTER | (REGISTER_MASK & reg));
	spi_write_byte(dev, value, len);
	spi_csnHi(dev);
}

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
void Nrf24_send(NRF24_t * dev, uint8_t * value)
{
	uint8_t status;
	status = Nrf24_getStatus(dev);
	while (dev->PTX) // Wait until last paket is send
	{
		status = Nrf24_getStatus(dev);
		if ((status & ((1 << TX_DS)  | (1 << MAX_RT))))
		{
			dev->PTX = 0;
			break;
		}
	}
	Nrf24_ceLow(dev);
	Nrf24_powerUpTx(dev); // Set to transmitter mode , Power up
	spi_csnLow(dev); // Pull down chip select
	spi_transfer(dev, FLUSH_TX ); // Write cmd to flush tx fifo
	spi_csnHi(dev); // Pull up chip select
	spi_csnLow(dev); // Pull down chip select
	spi_transfer(dev, W_TX_PAYLOAD ); // Write cmd to write payload
	spi_write_byte(dev, value, dev->payload); // Write payload
	spi_csnHi(dev); // Pull up chip select
	Nrf24_ceHi(dev); // Start transmission
}


// Sends payload without expecting an ACK from the receiver effectively turning off retransmission of failed payloads.
// See Nrf24l01 "PTX Operation" flowchart in the datasheet.
// NOTE: Make sure to call Nrf24_enableNoAckFeature() before calling this function.
// Is useful when achieving maximum throughput without caring much about losses.
void Nrf24_sendNoAck(NRF24_t * dev, uint8_t * value)
{
	uint8_t status;
	status = Nrf24_getStatus(dev);
	while (dev->PTX) // Wait until last paket is sent
	{
		status = Nrf24_getStatus(dev);
		if ((status & ((1 << TX_DS)  | (1 << MAX_RT))))
		{
			dev->PTX = 0;
			break;
		}
	}
	Nrf24_ceLow(dev);
	Nrf24_powerUpTx(dev); // Set to transmitter mode , Power up
	spi_csnLow(dev); // Pull down chip select
	spi_transfer(dev, FLUSH_TX ); // Write cmd to flush tx fifo
	spi_csnHi(dev); // Pull up chip select
	spi_csnLow(dev); // Pull down chip select
	spi_transfer(dev, W_TX_PAYLOAD_NO_ACK ); // Write cmd to write payload
	spi_write_byte(dev, value, dev->payload); // Write payload
	spi_csnHi(dev); // Pull up chip select
	Nrf24_ceHi(dev); // Start transmission
}

// Test if chip is still sending.
// When sending has finished return chip to listening.
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

// Test if Sending has finished or retry is over.
// When sending has finished return trur.
// When reach maximum number of TX retries return false.
bool Nrf24_isSend(NRF24_t * dev, int timeout) {
	uint8_t status;
	TickType_t startTick = xTaskGetTickCount();
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
				ESP_LOGW(TAG, "Maximum number of TX retries interrupt");
				Nrf24_powerUpRx(dev);
				return false;
			}

			// I believe either TX_DS or MAX_RT will always be notified.
			// Therefore, it is unusual for neither to be notified for a period of time.
			// I don't know exactly how to respond.
			TickType_t diffTick = xTaskGetTickCount() - startTick;
			if ( (diffTick * portTICK_PERIOD_MS) > timeout) {
				ESP_LOGE(TAG, "Status register timeout. status=0x%x", status);
				return false;
			}
			vTaskDelay(1);
		}
	}
	return false;
}

// Enables the W_TX_PAYLOAD command
// NOTE: Make sure to call this before using Nrf24_sendNoAck().
// Can be called anytime after the call to Nrf24_init() and preferably only once.
void Nrf24_enableNoAckFeature(NRF24_t * dev)
{
	uint8_t value;

	Nrf24_readRegister(dev, FEATURE, &value, 1);
	value = value | 1; 
	Nrf24_configRegister(dev, FEATURE, value);
}



uint8_t Nrf24_getStatus(NRF24_t * dev) {
	uint8_t rv;
	Nrf24_readRegister(dev, STATUS, &rv, 1);
	return rv;
}

void Nrf24_powerUpRx(NRF24_t * dev) {
	dev->PTX = 0;
	Nrf24_ceLow(dev);
	Nrf24_configRegister(dev, CONFIG, mirf_CONFIG | ( (1 << PWR_UP) | (1 << PRIM_RX) ) ); //set device as RX mode
	Nrf24_ceHi(dev);
	Nrf24_configRegister(dev, STATUS, (1 << TX_DS) | (1 << MAX_RT)); //Clear seeded interrupt and max tx number interrupt
}

void Nrf24_flushRx(NRF24_t * dev)
{
	spi_csnLow(dev);
	spi_transfer(dev, FLUSH_RX );
	spi_csnHi(dev);
}

void Nrf24_powerUpTx(NRF24_t * dev) {
	dev->PTX = 1;
	Nrf24_configRegister(dev, CONFIG, mirf_CONFIG | ( (1 << PWR_UP) | (0 << PRIM_RX) ) ); //set device as TX mode
	Nrf24_configRegister(dev, STATUS, (1 << TX_DS) | (1 << MAX_RT)); //Clear seeded interrupt and max tx number interrupt
}

void Nrf24_ceHi(NRF24_t * dev) {
	gpio_set_level( dev->cePin, 1 );
}

void Nrf24_ceLow(NRF24_t * dev) {
	gpio_set_level( dev->cePin, 0 );
}

void Nrf24_powerDown(NRF24_t * dev)
{
	Nrf24_ceLow(dev);
	Nrf24_configRegister(dev, CONFIG, mirf_CONFIG );
}

//Set tx power : 0=-18dBm,1=-12dBm,2=-6dBm,3=0dBm
void Nrf24_SetOutputRF_PWR(NRF24_t * dev, uint8_t val)
{
	if (val > 3) return;

	uint8_t value;
	Nrf24_readRegister(dev, RF_SETUP, &value, 1);
	value = value & 0xF9;
	value = value | (val<< RF_PWR);
	//Nrf24_configRegister(dev, RF_SETUP,	(val<< RF_PWR) );
	Nrf24_configRegister(dev, RF_SETUP,	value);
}

//Select between the high speed data rates:0=1Mbps, 1=2Mbps, 2=250Kbps
void Nrf24_SetSpeedDataRates(NRF24_t * dev, uint8_t val)
{
	if (val > 2) return;

	uint8_t value;
	Nrf24_readRegister(dev, RF_SETUP, &value, 1);
	if(val == 2)
	{
		value = value | 0x20;
		value = value & 0xF7;
		//Nrf24_configRegister(dev, RF_SETUP,	(1 << RF_DR_LOW) );
		Nrf24_configRegister(dev, RF_SETUP,	value);
	}
	else
	{
		value = value & 0xD7;
		value = value | (val << RF_DR_HIGH);
		//Nrf24_configRegister(dev, RF_SETUP,	(val << RF_DR_HIGH) );
		Nrf24_configRegister(dev, RF_SETUP,	value);
	}
} 

//Set Auto Retransmit Delay 0=250us, 1=500us, ... 15=4000us
void Nrf24_setRetransmitDelay(NRF24_t * dev, uint8_t val)
{
	uint8_t value;
	Nrf24_readRegister(dev, SETUP_RETR, &value, 1);
	value = value & 0x0F;
	value = value | (val << ARD);
	Nrf24_configRegister(dev, SETUP_RETR, value);
}

void Nrf24_setRetransmitCount(NRF24_t * dev, uint8_t val)
{
	uint8_t value;
	Nrf24_readRegister(dev, SETUP_RETR, &value, 1);
	value = value & 0xF0;
	value = value | val;
	Nrf24_configRegister(dev, SETUP_RETR, value);
}



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
	uint8_t retransmit = Nrf24_getRetransmitDelay(dev);
	int16_t delay = (retransmit+1)*250;
	printf("Retransmit\t = %d us\n", delay);
}

#define _BV(x) (1<<(x))

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
#if 0
		uint8_t* bufptr = buffer + sizeof buffer;
		while (--bufptr >= buffer) {
			printf("%02x", *bufptr);
		}
#endif
		for(int i=0;i<5;i++) {
			printf("%02x", buffer[i]);
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

uint8_t Nrf24_getDataRate(NRF24_t * dev)
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

char * Nrf24_getDataRateString(NRF24_t * dev)
{
	return rf24_datarates[Nrf24_getDataRate(dev)];
}

uint8_t Nrf24_getCRCLength(NRF24_t * dev)
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

char * Nrf24_getPALevelString(NRF24_t * dev)
{
	return rf24_pa_dbm[Nrf24_getPALevel(dev)];
}

uint8_t Nrf24_getRetransmitDelay(NRF24_t * dev)
{
	uint8_t value;
	Nrf24_readRegister(dev, SETUP_RETR, &value, 1);
	return (value >> 4);
}

uint8_t Nrf24_getRetransmitCount(NRF24_t * dev)
{
	uint8_t value;
	Nrf24_readRegister(dev, SETUP_RETR, &value, 1);
	return (value & 0x0F);
}


uint8_t Nrf24_getChannle(NRF24_t * dev)
{
	return dev->channel;
}

uint8_t Nrf24_getPayload(NRF24_t * dev)
{
	return dev->payload;
}
