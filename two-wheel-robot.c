#include "kavr/log.h"
#include "kavr/rf24.h"
#include "kavr/spi.h"
#include "kavr/pwm.h"

uint8_t buffer[2];

int main() {
	log_init(UART_MODE_16_9600);
	log_write("\nStart");

	rf24_pins_init();
	pwm_0_init(PWM_PRESCALER_64);
	pwm_1_init(PWM_PRESCALER_64);
	spi_master_init(SPI_PRESCALER_16);
	log_write("SPI initialized");

	rf24_registers_init("\xab\x28\x38\xbe\xa7", "\xdb\x94\xe4\x10\x28", 3);
	log_write("RF24 registers initialized");

	while (1) {
		rf24_receive(buffer, 3);
		uint8_t engine1 = buffer[0];
		uint8_t engine2 = buffer[1];
		uint8_t directions = buffer[2];
		if (!check_flag(directions, 0)) {
			pwm_pd5_write(engine1);
			pwm_pb1_write(0);
		} else {
			pwm_pd5_write(0);
			pwm_pb1_write(engine1);
		}
		if (!check_flag(directions, 1)) {
			pwm_pd6_write(engine2);
			pwm_pb2_write(0);
		} else {
			pwm_pd6_write(0);
			pwm_pb2_write(engine2);
		}

		log_write_format("RF24 request %d %d %02X", engine1, engine2, directions);
	}
}
