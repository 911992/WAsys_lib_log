/*
 * Wasys_logging.c
 *
 * Basic logging op
 *
 *  Created on: Oct 17, 2019
 *      Author: Arash (911992)
 */

/***/
#include "WAsys_logging.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************/
/* - - - - - - - variables - begin - - - - - - - */
/*************************************************/

WAsys_LOGGING_CONF_T _logging;

/* - - - - - - - variables - end - - - - - - - */


/***************************************************/
/* - - - - - - - static func - begin - - - - - - - */
/***************************************************/

static void _WAsys_default_stdout_logging_cb_func(char *arg_buff,
		size_t arg_len) {
	size_t _a;
	for (_a = 0; _a < arg_len; _a++) {
		putchar(arg_buff[_a]);
		fflush(stdout);
	}
}

static time_t _WAsys_default_system_timestamp_func() {
	time_t _ts;
	time(&_ts);
	return (time_t) _ts;
}

static void _WAsys_print_timestamp() {
	time_t _ts = _logging.system_ts_func();
	struct tm *_tst;
	_tst = localtime(&_ts);
	char msg[WAsys_TIMESTAMP_DATE_FORMAT_LEN];
	strftime(msg, WAsys_TIMESTAMP_DATE_FORMAT_LEN, WAsys_TIMESTAMP_DATE_FORMAT,
			_tst);
	_logging.callback_func(msg,
	WAsys_TIMESTAMP_DATE_FORMAT_LEN - 1 /*-1 for avoiding sending NIL*/);

}

/* - - - - - - - static func - end - - - - - - - */


void WAsys_logging_init(WAsys_LOGGING_CONF_T arg_logging_conf) {
	_logging = arg_logging_conf;
	if (_logging.callback_func == NULL) {
		_logging.callback_func = _WAsys_default_stdout_logging_cb_func;
	}
	if (_logging.including_timestamp) {
		if (_logging.system_ts_func == NULL) {
			_logging.system_ts_func = _WAsys_default_system_timestamp_func;
		}
	}
}

void WAsys_logging_init_default(void) {
	WAsys_LOGGING_CONF_T _conf;
	_conf.callback_func = _WAsys_default_stdout_logging_cb_func;
	_conf.system_ts_func = NULL;
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
char WAsys_msg[_MSG_LEN];

void WAsys_zeromsg(void){
	memset(WAsys_msg, 0, _MSG_LEN);
}

void WAsys_logf(char *fmt, ...) {
	va_list _args;
	va_start(_args, fmt);
#if __cplusplus > 199711L
	vsnprintf(WAsys_msg, sizeof(WAsys_msg), fmt, _args);
#else
	vsprintf(WAsys_msg, fmt, _args);
#endif
	va_end(_args);
	WAsys_log_br(WAsys_msg, 0);
}

#endif

void WAsys_log_br(char *arg_msg, uint8_t arg_br) {
	if (_logging.including_timestamp) {
		_WAsys_print_timestamp();
	}
	_logging.callback_func(arg_msg, strlen(arg_msg));
	if (arg_br) {
		static char _lf[1] = "\n";
		static size_t _lf_len = sizeof(_lf);
		_logging.callback_func(_lf, _lf_len);
	}
}
void WAsys_log(char *arg_msg) {
	WAsys_log_br(arg_msg, 1);
}


#ifdef __cplusplus
}
#endif
