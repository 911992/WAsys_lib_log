/*
 * Wasys_logging.h
 *
 * Basic logging op
 *
 *  Created on: Oct 17, 2019
 *      Author: Arash (911992)
 */

#ifndef _WASYS_LOGGING_HEADER_
#define _WASYS_LOGGING_HEADER_

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************/
/* - - - - - - - include - begin - - - - - - - */
/***********************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
/* - - - - - - - include - end - - - - - - - */


/**********************************************/
/* - - - - - - - Macros - begin - - - - - - - */
/**********************************************/

/**
 * Define it as 1, in order to enable global WAsys_msg char* static array
 * @see WAsys_msg
 * @see _MSG_LEN
 */
#define WAsys_USE_GLOBAL_MSG_VAR 1

#if (WAsys_USE_GLOBAL_MSG_VAR == 1)

/**
 * @brief Global message default length
 */
#define _MSG_LEN_DEFAULT 64

/**
 * @brief Global message length
 * @see WAsys_logf
 */
#define _MSG_LEN _MSG_LEN_DEFAULT

/**
 * @brief Logs the global WAsys_msg with a new line
 * @see WAsys_msg
 * @see WAsys_log
 */
#define WAsys_log_msg() WAsys_log(WAsys_msg)

#endif

/**
 * @brief Format for strftime function (including coloring)
 * @brief NOTE: make sure the format will result a String with total size of (WAsys_TIMESTAMP_DATE_FORMAT_LEN-1) or less
 * @brief please see ANSI terminal standard coloring
 * \u001b[32m ANSI green
 * \u001b[0m reset
 * @see WAsys_TIMESTAMP_DATE_FORMAT_LEN
 * @see http://www.cplusplus.com/reference/ctime/strftime/
 */
#define WAsys_TIMESTAMP_DATE_FORMAT "%Y%m%d-%H:%M:%S-> "

/**
 * String length for holding the formatted timestamp(plus one extra for \0 at the end)
 * @see WAsys_TIMESTAMP_DATE_FORMAT
 */
#define WAsys_TIMESTAMP_DATE_FORMAT_LEN 18/*time format*/ + 3 /*for -> */


/* - - - - - - - Macros - end - - - - - - - */


/*********************************************/
/* - - - - - - - types - begin - - - - - - - */
/*********************************************/
/**
 * @brief Callback function when logging needs to be happen
 * @param arg_buff the buffer(log string) need to be logged
 * @param arg_len the buffer len/size
 */
typedef void (*WAsys_LOGGING_NATIVE_IO_CALLBACK_FUNC_T)(char *arg_buff, size_t arg_len);

/**
 * @brief Callback function when logging needs current timestamp
 * @brief The function-pointer will be called to get current timestamp/ticks in seconds
 * @return an time_t presents current sys timestamp
 */
typedef time_t (*WAsys_NATIVE_SYSTEM_TIMESTAMP_FUNC_T)(void);

/**
 * @brief Configuration struct for initializing
 * @see WAsys_LOGGING_NATIVE_IO_CALLBACK_FUNC_T
 * @see WAsys_NATIVE_SYSTEM_TIMESTAMP_FUNC_T
 */
typedef struct {
	/**
	 * @brief Logging callback function, set it as NULL for using the default putc function
	 */
	WAsys_LOGGING_NATIVE_IO_CALLBACK_FUNC_T callback_func;
	/**
	 * @brief System timestamp/tick function, set it as null for using default time function
	 */
	WAsys_NATIVE_SYSTEM_TIMESTAMP_FUNC_T system_ts_func;
	/**
	 * @brief TRUE for including the log timestamp
	 * @see WAsys_TIMESTAMP_DATE_FORMAT
	 */
	uint8_t including_timestamp;
} WAsys_LOGGING_CONF_T;
/* - - - - - - - types - end - - - - - - - */


/*****************************************************/
/* - - - - - - - Externed vars - begin - - - - - - - */
/*****************************************************/

#if (WAsys_USE_GLOBAL_MSG_VAR == 1)

/**
 * @brief Global msg var for logging, please change _MSG_LEN to any appreciate value(message size)
 * @see _MSG_LEN
 * @see _MSG_LEN_DEFAULT
 */
extern char WAsys_msg[_MSG_LEN];

#endif
/* - - - - - - - Externed vars - end - - - - - - - */


/*************************************************/
/* - - - - - - - Functions - begin - - - - - - - */
/*************************************************/

#if (WAsys_USE_GLOBAL_MSG_VAR == 1)

/**
 * @brief memset the global WAsys_msg to zeros
 * @see WAsys_msg
 */
void WAsys_zeromsg(void);

/**
 * @brief Use it if you know are what you are doing, make sure WAsys_msg is large enough to host your target formated message
 * @brief Format the given VA_ARGS on global WAsys_msg, then prints it, first on WAsys_msg, then on associate callback io
 * @brief NOTE: make sure _MSG_LEN is big enough. If you find the formatted text incompleted/trimmed(or any SEGFAULT), so this could be not-significant _MSG_LEN
 * @brief Please note it won't add a new line(\n) at the end of the text, place it if you want
 * @see _MSG_LEN
 * @see _MSG_LEN_DEFAULT
 * @see WAsys_log_br
 * @param fmt Format string for formatting message log
 */
void WAsys_logf(char *fmt, ...);

#endif

/**
 * @brief Initialize and sets the given arg_logging_conf as logging configs
 * @param arg_logging_conf contains
 * @see WAsys_LOGGING_CONF_T
 * @see WAsys_logging_init_default
 */
void WAsys_logging_init(WAsys_LOGGING_CONF_T arg_logging_conf);

/**
 * @brief Initialize the logging with default logging confs
 * @brief Printing data on STDOUT
 * @brief NO Timestamp for each log callback
 * @see WAsys_logging_init
 * @see WAsys_logging_init_default_timestamp
 */
void WAsys_logging_init_default(void);

/**
 * @brief Initialize the logging with default logging confs
 * @brief Printing data on STDOUT
 * @brief Timestamp for each log callback
 * @see WAsys_logging_init
 * @see WAsys_logging_init_default
 */
void WAsys_logging_init_default_timestamp(void);

/**
 * @brief Logs the given non-null, null-terminated string on default debug serial pipe
 * It calls @fn WAsys_log_br with arg_br:=true to print an extra \n after message were sent
 * @param arg_msg the null-terminated message should be logged (must be non-null)
 * @see WAsys_log_br
 */
void WAsys_log(char *arg_msg);

/**
 * @brief Logs the given non-null, null-terminated string on default debug serial pipe.
 * Performs another IO write call for new-line(Line-Feed \\n , 0x10) when arg_br is not zero
 * @param arg_msg the null-terminated message (must be non-null)
 * @param arg_br if true(non-zero), then a new line will be appended
 */
void WAsys_log_br(char *arg_msg, uint8_t arg_br);

/* - - - - - - - Functions - end - - - - - - - */

#ifdef __cplusplus
}
#endif

#endif
