#ifndef MAIN_MIRF_H_
#define MAIN_MIRF_H_

#include "driver/spi_master.h"

typedef struct {
    uint8_t PTX;  //In sending mode.
    uint8_t cePin;// CE Pin controls RX / TX, default 8.
    uint8_t csnPin;//CSN Pin Chip Select Not, default 7.
    uint8_t channel;//Channel 0 - 127 or 0 - 84 in the US.
    uint8_t payload;// Payload width in bytes default 16 max 32.
    spi_device_handle_t _SPIHandle;
} NRF24_t;

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define RF_DR_LOW   5
#define PLL_LOCK    4
#define RF_DR_HIGH  3
#define RF_PWR      1
#define LNA_HCURR   0
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

/* Non-P omissions */
#define LNA_HCURR   0

/* P model memory Map */
#define RPD                  0x09
#define W_TX_PAYLOAD_NO_ACK  0xB0

/* P model bit Mnemonics */
#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2

/* Device addrees length:3~5 bytes */
#define mirf_ADDR_LEN    5

/* 
 enable interrupt caused by RX_DR.
 enable interrupt caused by TX_DS.
 enable interrupt caused by MAX_RT.
 enable CRC and CRC data len=1
 mirf_CONFIG = 00001000B
*/
//#define mirf_CONFIG ((1<<EN_CRC) | (0<<CRCO) )

/*
 enable interrupt caused by RX_DR.
 disable interrupt caused by TX_DS.
 enable interrupt caused by MAX_RT.
 enable CRC and CRC data len=1
 mirf_CONFIG == 00101000B
*/
#define mirf_CONFIG ((1<<MASK_TX_DS) | (1<<EN_CRC) | (0<<CRCO) )

/**
 * Power Amplifier level.
 *
 * For use with setPALevel()
 */
typedef enum {
    RF24_PA_MIN = 0,
    RF24_PA_LOW,
    RF24_PA_HIGH,
    RF24_PA_MAX,
    RF24_PA_ERROR
} rf24_pa_dbm_e;

/**
 * Data rate.  How fast data moves through the air.
 *
 * For use with setDataRate()
 */
typedef enum {
    RF24_1MBPS = 0,
    RF24_2MBPS,
    RF24_250KBPS
} rf24_datarate_e;

/**
 * CRC Length.  How big (if any) of a CRC is included.
 *
 * For use with setCRCLength()
 */
typedef enum {
    RF24_CRC_DISABLED = 0,
    RF24_CRC_8,
    RF24_CRC_16
} rf24_crclength_e;


void      Nrf24_init(NRF24_t * dev);
bool      spi_write_byte(NRF24_t * dev, uint8_t* Dataout, size_t DataLength );
bool      spi_read_byte(NRF24_t * dev, uint8_t* Datain, uint8_t* Dataout, size_t DataLength );
uint8_t   spi_transfer(NRF24_t * dev, uint8_t address);
void      spi_csnLow(NRF24_t * dev);
void      spi_csnHi(NRF24_t * dev);
void      Nrf24_config(NRF24_t * dev, uint8_t channel, uint8_t payload);
void      Nrf24_send(NRF24_t * dev, uint8_t *value);
esp_err_t Nrf24_setRADDR(NRF24_t * dev, uint8_t * adr);
esp_err_t Nrf24_setTADDR(NRF24_t * dev, uint8_t * adr);
void      Nrf24_addRADDR(NRF24_t * dev, uint8_t pipe, uint8_t adr);
bool      Nrf24_dataReady(NRF24_t * dev);
uint8_t   Nrf24_getDataPipe(NRF24_t * dev);
bool      Nrf24_isSending(NRF24_t * dev);
bool      Nrf24_isSend(NRF24_t * dev, int timeout);
bool      Nrf24_rxFifoEmpty(NRF24_t * dev);
bool      Nrf24_txFifoEmpty(NRF24_t * dev);
void      Nrf24_getData(NRF24_t * dev, uint8_t * data);
uint8_t   Nrf24_getStatus(NRF24_t * dev);
void      Nrf24_configRegister(NRF24_t * dev, uint8_t reg, uint8_t value);
void      Nrf24_readRegister(NRF24_t * dev, uint8_t reg, uint8_t * value, uint8_t len);
void      Nrf24_writeRegister(NRF24_t * dev, uint8_t reg, uint8_t * value, uint8_t len);
void      Nrf24_powerUpRx(NRF24_t * dev);
void      Nrf24_powerUpTx(NRF24_t * dev);
void      Nrf24_powerDown(NRF24_t * dev);
void      Nrf24_SetOutputRF_PWR(NRF24_t * dev, uint8_t val);
void      Nrf24_SetSpeedDataRates(NRF24_t * dev, uint8_t val);
void      Nrf24_setRetransmitDelay(NRF24_t * dev, uint8_t val);
void      Nrf24_ceHi(NRF24_t * dev);
void      Nrf24_ceLow(NRF24_t * dev);
void      Nrf24_flushRx(NRF24_t * dev);
void      Nrf24_printDetails(NRF24_t * dev);
void      Nrf24_print_status(uint8_t status);
void      Nrf24_print_address_register(NRF24_t * dev, const char* name, uint8_t reg, uint8_t qty);
void      Nrf24_print_byte_register(NRF24_t * dev, const char* name, uint8_t reg, uint8_t qty);
uint8_t   Nrf24_getDataRate(NRF24_t * dev);
uint8_t   Nrf24_getCRCLength(NRF24_t * dev);
uint8_t   Nrf24_getPALevel(NRF24_t * dev);
uint8_t   Nrf24_getRetransmitDelay(NRF24_t * dev);

#endif /* MAIN_MIRF_H_ */

