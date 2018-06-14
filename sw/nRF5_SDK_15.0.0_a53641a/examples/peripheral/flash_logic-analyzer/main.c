/**
 * FMB FLASH Logic Analyzer
 * Revision 1
 * 06/12/2018
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "nrf.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_power.h"
#include "nrf_serial.h"
#include "app_timer.h"
#include "app_error.h"
#include "app_util.h"

#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SPI0_INSTANCE  0 /**< SPI 1 instance index. */
static const nrf_drv_spi_t spi_flash= NRF_DRV_SPI_INSTANCE(SPI0_INSTANCE);  /**< SPI 1 instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

static uint8_t       m_tx_buf[2];           /**< TX buffer. */
static uint8_t       m_rx_buf[3];    /**< RX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */

static uint8_t flash_data[3];


ret_code_t ret;


/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
}

#define OP_QUEUES_SIZE          3
#define APP_TIMER_PRESCALER     NRF_SERIAL_APP_TIMER_PRESCALER

static void sleep_handler(void)
{
    __WFE();
    __SEV();
    __WFE();
}

NRF_SERIAL_DRV_UART_CONFIG_DEF(m_uart0_drv_config,
                      26, 27,
                      RTS_PIN_NUMBER, CTS_PIN_NUMBER,
                      NRF_UART_HWFC_DISABLED, NRF_UART_PARITY_EXCLUDED,
                      NRF_UART_BAUDRATE_115200,
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


void serial_initialize();
void serial_uninitialize();

int main(void)
{
   /* int HOLD = 22;
    nrf_gpio_cfg_output(HOLD);
    nrf_gpio_pin_set(HOLD);
    */
    ret = nrf_drv_clock_init();
    APP_ERROR_CHECK(ret);
    ret = nrf_drv_power_init(NULL);
    APP_ERROR_CHECK(ret);

    nrf_drv_clock_lfclk_request(NULL);
    ret = app_timer_init();
    APP_ERROR_CHECK(ret);
    
    serial_initialize();
    
    nrf_drv_spi_config_t spi_config_flash = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config_flash.ss_pin   = SPI_SS_PIN;
    spi_config_flash.miso_pin = SPI_MISO_PIN;
    spi_config_flash.mosi_pin = SPI_MOSI_PIN;
    spi_config_flash.sck_pin  = SPI_SCK_PIN;
    
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi_flash, &spi_config_flash, spi_event_handler, NULL));
  
    uint8_t newline[2];
    newline[0] = 0x0D;
    newline[1] = 0x0A;
    
    while (true)
    {
        memset(m_rx_buf, 0, m_length);
        memset(m_tx_buf, 0, m_length);
        m_tx_buf[0] = 0x9F;
        m_tx_buf[0] = 0x00;
        

        spi_xfer_done = false;
        APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_flash, m_tx_buf, 2, m_rx_buf, 3));
        nrf_delay_ms(1000);

        while (!spi_xfer_done)
        {
            __WFE();
        }     
        
        for(int i = 0; i < 3; i++){
            flash_data[i] = m_rx_buf[i];
        }
        
        (void)nrf_serial_write(&serial_uart, &flash_data[0], (sizeof(flash_data)), NULL, 0);
        (void)nrf_serial_write(&serial_uart, &newline[0], (sizeof(newline)), NULL, 0);
        (void)nrf_serial_flush(&serial_uart, 0);
        
        
         
    }
}

void serial_initialize(){
    ret = nrf_serial_init(&serial_uart, &m_uart0_drv_config, &serial_config);
    APP_ERROR_CHECK(ret);
    nrf_delay_ms(2000);
    APP_ERROR_CHECK(ret);
}