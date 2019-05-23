#ifndef LOG_H
#define LOG_H

#include <stdint.h>

#define LOG_MAX_NUM_VARS				(8)
#define LOG_VARIABLE_SAMPLE_DEPTH		(1000)

typedef enum var_type_e {
	INT = 1,
	FLOAT,
	DOUBLE
} var_type_e;

void log_init(void);
void log_callback(void);

void log_start(void);
void log_stop(void);
uint8_t log_is_logging(void);

void log_var_register(int idx, char* name, void *addr, uint32_t samples_per_sec, var_type_e type);
void log_var_empty(int idx);
void log_var_dump_uart(int idx);

#endif // LOG_H
