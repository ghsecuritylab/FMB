/**
 Finna Make Bank
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_drv_power.h"
#include "nrf_serial.h"
#include "app_timer.h"
#include "app_util.h"
#include "nrf_log_default_backends.h"

int i = 0;
#define SPI0_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI0_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

#define TEST_STRING "Nordic"
static uint8_t       m_tx_buf[8];           /**< TX buffer. */
static uint8_t       m_rx_buf[8];    /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */
bool a = false;
    int adxlpower = 8;

         char in;
         int sampletime = 50;
         char erase;
         
uint8_t data[7510][6];
int count = 0;
   uint8_t newline[2];

/**
 * @brief SPI user event handler.
 * @param event
 */

/*
 Serial config
 */

#define OP_QUEUES_SIZE          3
#define APP_TIMER_PRESCALER     NRF_SERIAL_APP_TIMER_PRESCALER

static void sleep_handler(void)
{
    __WFE();
    __SEV();
    __WFE();
}

NRF_SERIAL_DRV_UART_CONFIG_DEF(m_uart0_drv_config,
                      26, 12,
                      RTS_PIN_NUMBER, CTS_PIN_NUMBER,
                      NRF_UART_HWFC_DISABLED, NRF_UART_PARITY_EXCLUDED,
                      NRF_UART_BAUDRATE_9600,
                      UART_DEFAULT_CONFIG_IRQ_PRIORITY);



#define SERIAL_FIFO_TX_SIZE 32
#define SERIAL_FIFO_RX_SIZE 32

NRF_SERIAL_QUEUES_DEF(serial_queues, SERIAL_FIFO_TX_SIZE, SERIAL_FIFO_RX_SIZE);


#define SERIAL_BUFF_TX_SIZE 1
#define SERIAL_BUFF_RX_SIZE 1

NRF_SERIAL_BUFFERS_DEF(serial_buffs, SERIAL_BUFF_TX_SIZE, SERIAL_BUFF_RX_SIZE);

NRF_SERIAL_CONFIG_DEF(serial_config, NRF_SERIAL_MODE_IRQ,
                      &serial_queues, &serial_buffs, NULL, sleep_handler);


NRF_SERIAL_UART_DEF(serial_uart, 0);


void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
  //  NRF_LOG_INFO("Transfer completed.");
    if (m_rx_buf[0] != 0)
    {
    //    NRF_LOG_INFO(" Received:");
     //   NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));
    }
}


int main(void)
{
    
     nrf_gpio_cfg_output(adxlpower);
     nrf_gpio_pin_set(adxlpower);
     nrf_delay_ms(500);
     nrf_gpio_cfg_input(6,NRF_GPIO_PIN_PULLDOWN);

    ret_code_t ret;

    ret = nrf_drv_clock_init();
    APP_ERROR_CHECK(ret);
    ret = nrf_drv_power_init(NULL);
    APP_ERROR_CHECK(ret);

    nrf_drv_clock_lfclk_request(NULL);
    ret = app_timer_init();
    APP_ERROR_CHECK(ret);
    
    
    ret = nrf_serial_init(&serial_uart, &m_uart0_drv_config, &serial_config);
    APP_ERROR_CHECK(ret);
    
   
        
 

nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
spi_config.ss_pin   = SPI_SS_PIN;
spi_config.miso_pin = SPI_MISO_PIN;
spi_config.mosi_pin = SPI_MOSI_PIN;
spi_config.sck_pin  = SPI_SCK_PIN;
APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));


uint8_t powerctl[3];
powerctl[0] = 0x0A;
powerctl[1] = 0x2D;
powerctl[2] = 0x02; 

nrf_drv_spi_transfer(&spi, powerctl, 3, m_rx_buf, 3);
nrf_delay_ms(50);


    while (!spi_xfer_done)
    {
       // __WFE();
    }

while(true){
  newline[0] = 0x0D;
    newline[1] = 0x0A;
    
    a = nrf_gpio_pin_read(6);

      
       while(a){
          // a = false;
     //  ret =   nrf_serial_read(&serial_uart, &in, sizeof(in), NULL, 0);
   

    
       //     if(in == 49){
             //   i = 0;
            for(int i = 0; i < 7810; i++){
                (void)nrf_serial_write(&serial_uart, &data[i][0], (sizeof(data[0])), NULL, 0);
                (void)nrf_serial_write(&serial_uart, &newline[0], (sizeof(newline)), NULL, 0);
                nrf_delay_ms(10);
               // nrf_serial_read(&serial_uart, &in, sizeof(in), NULL, 0);
                    a = nrf_gpio_pin_read(6);

                if(a == false){
                    break;
                    }
                }
           // }

    //   (void)nrf_serial_flush(&serial_uart, 0);

       }

while (count<7809)
{
    
    nrf_drv_spi_transfer(&spi, powerctl, 3, m_rx_buf, 3);
nrf_delay_ms(10);
    memset(m_rx_buf, 0, m_length);
    memset(m_tx_buf, 0, m_length);
    m_tx_buf[0] = 0x0b;
    m_tx_buf[1] = 0x0E;
    m_tx_buf[2] = 0x00;
    m_tx_buf[3] = 0x00;
    m_tx_buf[4] = 0x00;
    m_tx_buf[5] = 0x00;
    m_tx_buf[6] = 0x00;
    m_tx_buf[7] = 0x00;
    

    
    spi_xfer_done = false;
    

    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, m_tx_buf, 8, m_rx_buf, 8));
    nrf_delay_ms(190);



    while (!spi_xfer_done)
    {
        __WFE();
    }
 
  
    data[count][0] = m_rx_buf[2];
    data[count][1] = m_rx_buf[3];
    data[count][2] = m_rx_buf[4];
    data[count][3] = m_rx_buf[5];
    data[count][4] = m_rx_buf[6];
    data[count][5] = m_rx_buf[7];

  
    

    a = nrf_gpio_pin_read(6);

      
       while(a){
          // a = false;
     //  ret =   nrf_serial_read(&serial_uart, &in, sizeof(in), NULL, 0);
   

    newline[0] = 0x0D;
    newline[1] = 0x0A;
    
       //     if(in == 49){
             //   i = 0;
            for(int i = 0; i < 7810; i++){
                (void)nrf_serial_write(&serial_uart, &data[i][0], (sizeof(data[0])), NULL, 0);
                (void)nrf_serial_write(&serial_uart, &newline[0], (sizeof(newline)), NULL, 0);
                nrf_delay_ms(10);
               // nrf_serial_read(&serial_uart, &in, sizeof(in), NULL, 0);
                    a = nrf_gpio_pin_read(6);

                if(a == false){
                    break;
                    }
                }
           // }

    //   (void)nrf_serial_flush(&serial_uart, 0);

       }
    

       count = count + 1;
}
}
}

