/*
 * WAsys_logging_usage_demo.c
 *
 * Logging usage
 *
 *  Created on: Oct 17, 2019
 *      Author: Arash (911992)
 */

#include "WAsys_logging.h"

void WAsys_stderr_out(char *arg_buff, size_t arg_len) {
	fprintf(stderr, "%s", arg_buff);
}

int main(int arg_len, char **args) {
	/*Initialize the default logging with time stamp, out to STDOUT*/
	WAsys_logging_init_default_timestamp();
	/*Printing a string with \n at the end*/
	WAsys_log("Hello World (testing WAsys_logging_usage_demo.c)");
	/*Printing a string without \n at the end*/
	WAsys_log_br("\nHello World\n", 0);
	/*Printing a string without \n at the end*/
	WAsys_logf("**\nHello %s\n", "World");

	/*re-Initialize the default logging without time stamp, out to STDOUT*/
	WAsys_log("\nHappy logging\n");

	/*re-Initialize the logging without time stamp, out to STDRR*/
	WAsys_LOGGING_CONF_T _conf;
	_conf.including_timestamp = 1;
	_conf.system_ts_func = NULL;
	_conf.callback_func = WAsys_stderr_out;
	WAsys_logging_init(_conf);
	WAsys_logf("\nHappy Programming %x\n", 911);
	return 0;
}
