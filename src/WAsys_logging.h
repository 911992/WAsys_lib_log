/*
 * Wasys_logging.h
 *
 * Basic logging op
 *
 *  Created on: Oct 17, 2019 | last edit: Oct 26, 2019 (rev 0.2)
 *      Author: Arash (911992)
 *
 * History:
 *  Revision 0.3 (Jan 24, 2020)
 *     Fixed some missed docs/comments
 *     Added funcs(WAsys_log_bytea_hex) and related macros(log_bytea_sep, and log_bytea) for logging an array as hex
 *     Support for logging NULL ptr args (WAsys_NULL_ARGUMENT_LOGGING , WAsys_NULL_BYTEA_ARGUMENT_LOGGING)
 *     Added func WAsys_log_ts and macro log_ts to printout the timestamp only(if applicable)
 *     Added __MINGW32__ and __MINGW64__ macros check for assuming time func could be available(windows/mingw)
 *
 *  Revision 0.2 (Oct 26, 2019)
 *     Added log, log_br and logf macros for easier logging
 *     Comment/document proofing
 *     Added overview usage(layers)
 *     Added sample usage(blog) for STM32
 *     Added _WAsys_TIME_FUNC_MAYBE_DEFINED macro for preventing linking error (windows and linux only for now)
 *  initial version: Oct 17, 2019
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
 * Checking(guessing) if time() function is available, to prevent linking error
 */
#if defined(_WIN32) || defined(__linux__) || defined(__MINGW32__) || defined(__MINGW64__)
#define _WAsys_TIME_FUNC_MAYBE_DEFINED
#endif

/**
 * @brief Define it as 1, in order to enable global WAsys_logging_msg char* static array
 * @see WAsys_logging_msg
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
 * @brief Logs the global WAsys_logging_msg with a new line
 * @see WAsys_logging_msg
 * @see WAsys_log
 */
#define WAsys_log_msg() WAsys_log(WAsys_logging_msg)

/**
 * @brief Easy access for WAsys_logf
 */
#define logf WAsys_logf

#endif

/**
 * @brief Format for strftime function (including coloring)
 * @brief NOTE: make sure the format will result a String with total size of (WAsys_TIMESTAMP_DATE_FORMAT_LEN-1) or less
 * @see WAsys_TIMESTAMP_DATE_FORMAT_LEN
 * @see http://www.cplusplus.com/reference/ctime/strftime/
 */
#define WAsys_TIMESTAMP_DATE_FORMAT "%Y%m%d-%H:%M:%S-> "

/**
 * @brief String length for holding the formatted timestamp(plus one extra for \0 at the end)
 * @see WAsys_TIMESTAMP_DATE_FORMAT
 */
#define WAsys_TIMESTAMP_DATE_FORMAT_LEN 18/*time format*/ + 3 /*for -> */

/**
 * @brief Easy access for basic logging ops
 */
#define log WAsys_log

#define log_br WAsys_log_br

#define log_ts WAsys_log_ts

/**
 * @brief Printing following val, when given argument for logging is a NULL ptr
 */
#define WAsys_NULL_ARGUMENT_LOGGING "<<NULL>>"

/**
 * @brief Printing following val, when given byte array argument for logging is a NULL ptr
 */
#define WAsys_NULL_BYTEA_ARGUMENT_LOGGING " [ N , U , L , L ] "

/**
 * @brief String format for printing one byte in hex scheme
 * @see WAsys_log_bytea_hex
 */
#define WAsys_BYTE_ARRAY_LOG_HEX_FORMAT "%02x"

/**
 * @brief Default separator for printing an array as hex
 * @see WAsys_log_bytea_hex
 */
#define WAsys_BYTE_ARRAY_LOG_DEFAULT_SEPARATOR " "

/**
 * @brief Easy access for byte array logging ops
 */
#define log_bytea_sep WAsys_log_bytea_hex

/**
 * @brief calls the WAsys_log_bytea_hex for default array separator
 * @see WAsys_BYTE_ARRAY_LOG_DEFAULT_SEPARATOR
 */
#define log_bytea(_arg_bytea,_arg_bytea_len) WAsys_log_bytea_hex(_arg_bytea,_arg_bytea_len,WAsys_BYTE_ARRAY_LOG_DEFAULT_SEPARATOR)

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
typedef time_t (*WAsys_LOGGING_NATIVE_SYSTEM_TIMESTAMP_FUNC_T)(void);

/**
 * @brief Configuration struct for initializing
 * @see WAsys_LOGGING_NATIVE_IO_CALLBACK_FUNC_T
 * @see WAsys_LOGGING_NATIVE_SYSTEM_TIMESTAMP_FUNC_T
 */
typedef struct {
	/**
	 * @brief Logging callback function, set it as NULL for using the default putc function
	 */
	WAsys_LOGGING_NATIVE_IO_CALLBACK_FUNC_T callback_func;
	/**
	 * @brief System timestamp/tick function, set it as null for using default time function
	 */
	WAsys_LOGGING_NATIVE_SYSTEM_TIMESTAMP_FUNC_T system_ts_func;
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
extern char WAsys_logging_msg[_MSG_LEN];

#endif
/* - - - - - - - Externed vars - end - - - - - - - */


/*************************************************/
/* - - - - - - - Functions - begin - - - - - - - */
/*************************************************/

#if (WAsys_USE_GLOBAL_MSG_VAR == 1)

/**
 * @brief memset the global WAsys_logging_msg to zeros
 * @see WAsys_logging_msg
 */
void WAsys_zeromsg(void);

/**
 * @brief Use it if you know are what you are doing, make sure WAsys_logging_msg is large enough to host your target formated message
 * @brief Format the given VA_ARGS on global WAsys_logging_msg, then prints it, first on WAsys_logging_msg, then on associate callback io
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
 * @param arg_logging_conf contains lib initializing setup/conf
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
 * @brief Logs the given null-terminated string(or NULL ptr string) on default debug log pipe(cb func).
 * It calls @fn WAsys_log_br with arg_br:=true to print an extra \n after message were sent
 * Prints WAsys_NULL_ARGUMENT_LOGGING when given argument is a null ptr string
 * @param arg_msg the null-terminated message should be logged (could be NULL)
 * @see WAsys_log_br
 * @see log
 * @see log_br
 * @see WAsys_NULL_ARGUMENT_LOGGING
 * @see WAsys_log_bytea_hex
 * @see log_bytea
 */
void WAsys_log(char *arg_msg);

/**
 * @brief Logs the given null-terminated string(or NULL ptr string) on default debug log pipe(cb func).
 * Performs another IO write call for new-line(Line-Feed \\n , 0x10) when arg_br is not zero
 * Prints WAsys_NULL_ARGUMENT_LOGGING when given argument is a null ptr string
 * @param arg_msg the null-terminated message (could be NULL)
 * @param arg_br if true(non-zero), then a new line will be appended
 * @see WAsys_log
 * @see log
 * @see log_br
 * @see WAsys_NULL_ARGUMENT_LOGGING
 * @see WAsys_log_bytea_hex
 * @see log_bytea
 */
void WAsys_log_br(char *arg_msg, uint8_t arg_br);

/**
 * @brief Gets a time from associated timestamp callback function, then logs it out without a new line. It ignores the op if the given pointer to timestamp func is a NULL, or lib has not initialized.
 * @see WAsys_LOGGING_NATIVE_SYSTEM_TIMESTAMP_FUNC_T
 * @see WAsys_LOGGING_CONF_T
 */
void WAsys_log_ts(void);

/**
 * @brief Logs the given byte-array in hex scheme.
 * @param arg_bytea the byte array need to be formatted and printed(could be NULL)
 * @param arg_bytea_len size of given byte array
 * @param arg_separator a null-terminated string, separator needed between each formated byte(could be NULL)
 * @see WAsys_BYTE_ARRAY_LOG_HEX_FORMAT
 * @see WAsys_NULL_BYTEA_ARGUMENT_LOGGING
 * @see log_bytea_sep
 */
void WAsys_log_bytea_hex(uint8_t *arg_bytea, size_t arg_bytea_len,
		char *arg_separator);

/* - - - - - - - Functions - end - - - - - - - */

#ifdef __cplusplus
}
#endif

#endif
