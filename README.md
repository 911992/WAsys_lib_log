# WAsys_lib_log
Very simple ANSI-C log lib

Compatible with embedded system, where there is no STDOUT

Sample Usage could be found at `WAsys_logging_usage_demo.c`

## FAQ

### Does it print on `STDOUT`?
If logging were initialized using default routines, so `TRUE`, otherwise it will call the associated IO callback.

### Does it support ANSI standard text coloring?
`FALSE`

### Is it thread-safe?
`FALSE`

### Does it flush the output after each logging?
`FALSE`, even if you initialize the default init way(STDOUT), it won't flush the output

### What if I don't initialize the logging, and log something?
There will be UBs

### Is callback-ed log string(`char*`) `NULL`-terminated?
Yes, but the `arg_len` carries the string length

### Where to change log timestamp format?
Check file `WAsys_logging.h`. Change `WAsys_TIMESTAMP_DATE_FORMAT` and `WAsys_TIMESTAMP_DATE_FORMAT_LEN` macros.
Read comments, otherwise there might be redundant memory usage, or UBs 

## Sample Usage
### X86(*nix/windows)
```c
...
void stdprint(char *arg_buff, size_t arg_len) {
	size_t _a;
	for( _a=0 ; _a < arg_len ; _a++ ){
		putchar(STDOUT,_a);
	}
	/*Or something like(maybe slower)*/
	/*printf("%s",arg_buff);*/
}

int main(int arg_argc,char** arg_args){
	/*Initializing the logging without time stamp, out to STDRR*/
	WAsys_LOGGING_CONF_T _conf;
	/*Need timestamp for each*/
	_conf.including_timestamp = 1;
	/*Using default system time() function*/
	_conf.system_ts_func = NULL;
	/*Function io callback*/
	_conf.callback_func = stdprint;
	/*Should be happens once, but could be re-initialize later*/
	WAsys_logging_init(_conf);
	/*Sample formatted log*/
	WAsys_logf("Happy Programming %x\n", 911);
	/*Sample formatted log*/
	return 0;
}
...
/*
Sample output:
20191018-00:15:34-> Happy Programming 38f
*/
```

### AVR (ATmel 8) using serial
AVR logging(debugging) using serial
From uC to PC(using serial)

```c
...

/*Where this var is handled(incremented) by a timer interrupt, in ms*/
uint32_t systicks;

void putchar_uart0(char c)
{
while ((UCSR0A & DATA_REGISTER_EMPTY)==0);
UDR0=c;
}
...
/*Logging fucntions - begin*/
void wasys_print(char *arg_buff, size_t arg_len) {
	size_t _a;
	for( _a=0 ; _a < arg_len ; _a++ ){
		putchar_uart0(_a);
	}
}

time_t wasys_systemtick(void){
	/*Convert current times in ms to seconds, and return it*/
	return (time_t)(systicks/1000);
}

/*Logging fucntions - end*/

int main(){
...
	/*Initializing the logging without time stamp, out to STDRR*/
	WAsys_LOGGING_CONF_T _conf;
	/*Need timestamp for each*/
	_conf.including_timestamp = 1;
	/*Using wasys_systemtick function*/
	_conf.system_ts_func = wasys_systemtick;
	/*Function io callback*/
	_conf.callback_func = wasys_print;
	/*Should be happens once, but could be re-initialize later*/
	WAsys_logging_init(_conf);
	/*Sample formatted log*/
	WAsys_log("Happy AVR Programming");
	/*Sample formatted log*/
...
}

...
```

## Troubleshooting

### UB/`SEGFAULT` by `WAsys_logf`
By default, the amount for formatting log is 64(see `_MSG_LEN_DEFAULT` and `_MSG_LEN`) in `WAsys_logging.h` file.
If the required string length(see `WAsys_msg`) for asked format is not significant enough, you might have some UB/`SEGFAULT` if compile in ANSI-C, but if you compile it in c11, it will be safe, but results trimmed message.

### Trimmed/truncated log data by `WAsys_logf`
By default, the amount for formatting log is 64(see `_MSG_LEN_DEFAULT` and `_MSG_LEN`) in `WAsys_logging.h` file.

### UB/`SEGFAULT` by `WAsys_log` or `WAsys_log_br`
Make sure you have initialized the logging right(or used the default ones)

### I don't log formatted message
So please change `WAsys_USE_GLOBAL_MSG_VAR` macro to 0 in `WAsys_logging.h`

