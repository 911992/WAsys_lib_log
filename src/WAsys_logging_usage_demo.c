/*
 * WAsys_logging_usage_demo.c
 *
 * Logging usage
 *
 *  Created on: Oct 17, 2019
 *      Author: Arash (911992)
 *
 * History:
 *  Revision 0.3 (Jan 24, 2020)
 *     Content disabled(needs macro __WAsys_logging_usage_demo_activated__ be defined)
 *     Byte array print example
 */


/*when above macro is defined, so the sample usage is enabled, is defined by debug build rule*/
#ifdef __WAsys_logging_usage_demo_activated__

#include "WAsys_logging.h"

/*
static void WAsys_stderr_out(char *arg_buff, size_t arg_len) {
	fprintf(stderr, "%s", arg_buff);
}
 */

static void _WAsys_logging_sample_log(void) {

	/*Printing a string with \n at the end*/
	WAsys_log("Hello World (testing WAsys_logging_usage_demo.c)");
	/*Printing a string without \n at the end*/
	WAsys_log_br("\nHello World\n", 0);
	/*Printing a string without \n at the end*/
	WAsys_logf("**\nHello %s\n", "World");

	/*re-Initialize the logging without time stamp, out to STDRR*/
	/*WAsys_LOGGING_CONF_T _conf;
	_conf.including_timestamp = 1;
	_conf.system_ts_func = NULL;
	_conf.callback_func = WAsys_stderr_out;
	WAsys_logging_init(_conf);
	WAsys_logf("\nHappy Programming %x\n", 911);
	 */
}

static void _WAsys_print_out_sample_byte_array(void) {
	log_ts();
	logf("Logging a simple byte array \n[");
	uint8_t sample_array[] = { 'W', 'A', 's', 'y', 's', '-', 0x20, 0x20, 0x01,
			0x24 };
	log_bytea(sample_array, sizeof(sample_array));
	log("]");
}

int main(int arg_len, char **args) {
	/*Initialize the default logging without timestamp, out to STDOUT*/
	WAsys_logging_init_default();

	_WAsys_logging_sample_log();

	_WAsys_print_out_sample_byte_array();
	return 0;
}

#endif
