/*
 * Wasys_logging.c
 *
 * Basic logging op
 *
 *  Created on: Oct 17, 2019 | last edit: Oct 26, 2019 (rev 0.2)
 *      Author: Arash (911992)
 *
 * History:
 *  Revision 0.3 (Jan 24, 2020)
 *     Fixed some missed docs/comments
 *     Changed the signature of _WAsys_print_timestamp function
 *     Logging now support for NULL inputs, see WAsys_NULL_ARGUMENT_LOGGING
 *     Added static function _WAsys_log_br
 *     New function impl as added in header/API file (WAsys_log_bytea_hex, and WA_log_ts)
 *     Function WAsys_logging_init_default now sets the default timestamp func, to allow WAsys_log_ts work
 *
 *  Revision 0.2 (Oct 26, 2019)
 *     Using _WAsys_TIME_FUNC_MAYBE_DEFINED macro for preventing linking error when time() function may not be available
 *  initial version: Oct 17, 2019
 */

/***/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************/
/* - - - - - - - include - begin - - - - - - - */
#include "WAsys_logging.h"
/* - - - - - - - include - - end - - - - - - - */
/***********************************************/



/*************************************************/
/* - - - - - - - variables - begin - - - - - - - */

/*Holds the lib logging configuration*/
WAsys_LOGGING_CONF_T _logging;

/*states if the lib has been initialized or not*/
uint8_t _WAsys_logging_lib_initialized = 0;

/* - - - - - - - variables - - end - - - - - - - */
/*************************************************/


/***************************************************/
/* - - - - - - - static func - begin - - - - - - - */

/**
 * @brief Default print func, prints out the given arg for logging over STDOUT
 * @param arg_buff the message need to be printed
 * @param arg_len size of message(arg_buff) length
 * @see WAsys_logging_init_default
 */
static void _WAsys_default_stdout_logging_cb_func(char *arg_buff,
		size_t arg_len) {
	size_t _a;
	for (_a = 0; _a < arg_len; _a++) {
		putchar(arg_buff[_a]);
		fflush(stdout);
	}
}

/**
 * @brief Default system timestamp func. Calls time() funcs(if lib thinks it might be available) or zero
 * @see WAsys_logging_init_default
 */
static time_t _WAsys_default_system_timestamp_func(void) {

	/*Local var to hold teh timestamp*/
	time_t _ts;

	/*when time() func is possibly available*/
#ifdef _WAsys_TIME_FUNC_MAYBE_DEFINED
	time(&_ts);
#else
	_ts = 0;
#endif
	return (time_t) _ts;
}

/**
 * @brief Format and prints out the current timestamp on associated lib's conf out cb
 * @param timestamp need to be formatted and printed
 * @see WAsys_logging_init_default
 */
static void _WAsys_print_timestamp(time_t arg_ts) {

	/*to hold a datetime not scalar*/
	struct tm *_tst;

	/*filling the datetime struct*/
	_tst = localtime(&arg_ts);

	/*A local var, to hold the printed datetime*/
	char _msg[WAsys_TIMESTAMP_DATE_FORMAT_LEN];

	/*print the result on msg*/
	strftime(_msg, WAsys_TIMESTAMP_DATE_FORMAT_LEN, WAsys_TIMESTAMP_DATE_FORMAT,
			_tst);

	/*calling the associated callback func to printout the timestamp*/
	_logging.callback_func(_msg,
	WAsys_TIMESTAMP_DATE_FORMAT_LEN - 1 /*-1 for avoiding sending NIL*/);

}

/**
 * @brief Performs the real logging. It's called from simple and byte array logging funcs
 * @param arg_msg the null-terminated message (could be NULL)
 * @param arg_br if true(non-zero), then a new line will be appended
 * @param arg_can_include_ts, indicates if the timestamp could be placed regardless of lib config specifying the timestamp
 * @see WAsys_log
 * @see log
 * @see log_br
 * @see WAsys_NULL_ARGUMENT_LOGGING
 * @see WAsys_log_bytea_hex
 * @see log_bytea
 */
static void _WAsys_log_br(char *arg_msg, uint8_t arg_br,
		uint8_t arg_can_include_ts) {

	/*when lib has not initialized yet, then do nothing*/
	if (_WAsys_logging_lib_initialized == 0) {
		return;
	}

	if (_logging.including_timestamp && arg_can_include_ts) {
		WAsys_log_ts();
	}

	char *_log_msg;
	/*using WAsys_NULL_ARGUMENT_LOGGING when given arg is a NULL ptr*/
	if (arg_msg != NULL) {
		_log_msg = arg_msg;
	} else {
		_log_msg = WAsys_NULL_ARGUMENT_LOGGING;
	}

	/*calculating the size of given log msg*/
	size_t _log_msg_len = strlen(_log_msg);

	/*calling for calling cb func for logging*/
	_logging.callback_func(_log_msg, _log_msg_len);
	if (arg_br) {
		static char _lf[1] = { '\n' };
		static size_t _lf_len = sizeof(_lf);
		_logging.callback_func(_lf, _lf_len);
	}
}

/* - - - - - - - static func - - end - - - - - - - */
/***************************************************/

/*************************************************/
/* - - - - - - - Functions - begin - - - - - - - */
void WAsys_logging_init(WAsys_LOGGING_CONF_T arg_logging_conf) {

	/*copy the given arg into local conf var*/
	_logging = arg_logging_conf;

	/*setting default out cb when associated func is NULL*/
	if (_logging.callback_func == NULL) {
		_logging.callback_func = _WAsys_default_stdout_logging_cb_func;
	}

	/*Using default timestamp func, when timestamp is needed using the default date/time func*/
	if (_logging.including_timestamp) {
		if (_logging.system_ts_func == NULL) {
			_logging.system_ts_func = _WAsys_default_system_timestamp_func;
		}
	}

	/*marking the lib initialized*/
	_WAsys_logging_lib_initialized = 1;
}

void WAsys_logging_init_default(void) {
	WAsys_LOGGING_CONF_T _conf;
	_conf.callback_func = _WAsys_default_stdout_logging_cb_func;
	_conf.system_ts_func = _WAsys_default_system_timestamp_func;
	_conf.including_timestamp = 0;
	WAsys_logging_init(_conf);
}

void WAsys_logging_init_default_timestamp(void) {
	WAsys_LOGGING_CONF_T _conf;
	_conf.callback_func = _WAsys_default_stdout_logging_cb_func;
	_conf.system_ts_func = _WAsys_default_system_timestamp_func;
	_conf.including_timestamp = 1;
	WAsys_logging_init(_conf);
}

#if (WAsys_USE_GLOBAL_MSG_VAR == 1)
char WAsys_logging_msg[_MSG_LEN];

void WAsys_zeromsg(void){
	memset(WAsys_logging_msg, 0, _MSG_LEN);
}

void WAsys_logf(char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
#if __cplusplus > 199711L
	vsnprintf(WAsys_logging_msg, sizeof(WAsys_logging_msg), fmt, _args);
#else
	vsprintf(WAsys_logging_msg, fmt, _args);
#endif
	va_end(_args);
	WAsys_log_br(WAsys_logging_msg, 0);
}

#endif

void WAsys_log_br(char *arg_msg, uint8_t arg_br) {

	_WAsys_log_br(arg_msg, arg_br, 1);

}

void WAsys_log_ts(void) {
	/*if it's possible to */
	if (_WAsys_logging_lib_initialized == 0 || _logging.system_ts_func == NULL) {
		return;
	}
	/*get current timestmap*/
	time_t _ts = _logging.system_ts_func();

	/*format and print it out*/
	_WAsys_print_timestamp(_ts);
}

void WAsys_log(char *arg_msg) {
	WAsys_log_br(arg_msg, 1);
}

void WAsys_log_bytea_hex(uint8_t *arg_bytea, size_t arg_bytea_len,
		char *arg_separator) {

	/*when lib has not initialized yet, then do nothing*/
	if (_WAsys_logging_lib_initialized == 0) {
		return;
	}

	/*check if given arg is a NULL ptr*/
	if (arg_bytea == NULL) {
		_WAsys_log_br(WAsys_NULL_BYTEA_ARGUMENT_LOGGING, 0, 0);
		return;
	}
	/*to check if a new loop needs a separator first*/
	uint8_t _need_sep = 0;

	/*to check if placing separator is applicable*/
	uint8_t _can_print_sep;
	if (arg_separator == NULL) {
		_can_print_sep = 0;
	} else {
		_can_print_sep = 1;
	}

	/*temp var to hold one byte*/
	uint8_t _byte_var;

	/*for each byte array*/
	size_t a;
	for (a = 0; a < arg_bytea_len; a++) {
		if (_need_sep == 1 && _can_print_sep == 1) {
			_WAsys_log_br(arg_separator, 0, 0);
		}
		_byte_var = arg_bytea[a];
		/*format/print byte*/
		sprintf(WAsys_logging_msg, WAsys_BYTE_ARRAY_LOG_HEX_FORMAT, _byte_var);
		_WAsys_log_br(WAsys_logging_msg, 0, 0);
		_need_sep = 1;
	}
}

/* - - - - - - - Functions - - end - - - - - - - */
/*************************************************/



#ifdef __cplusplus
}
#endif
