#ifdef APP_DEMO

#include "usr/demo/cmd/cmd_cc.h"
#include "usr/demo/task_cc.h"
#include "drv/analog.h"
#include "drv/pwm.h"
#include "sys/commands.h"
#include "sys/debug.h"
#include "sys/defines.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// To initialize this module for CC 0:
//
// Configure I/O
// > cc 0 pwm a 2
// > cc 0 pwm b 1
// > cc 0 pwm c 0
// > cc 0 adc a 16 -2.5 0
// > cc 0 adc b 15 -2.5 0
// > cc 0 adc c 13 -2.5 0
//
// Configure tuning
// > cc 0 vdc 10
// > cc 0 tune 1 0.0015 0.0015 628
//
// Start current controller
// > cc 0 init
//
// Set operating point
// > cc 0 set 1 1 10
//
// Stop current controller
// > cc 0 deinit

float value;

static command_entry_t cmd_entry;

#define NUM_HELP_ENTRIES (8)
static command_help_t cmd_help[NUM_HELP_ENTRIES] = {
    {"setup [ashad|yusuke]", "Set-up specific bench configuration"},
    {"<cc_idx> init", "Initialize current controller"},
    {"<cc_idx> deinit", "Deinitialize current controller"},
    {"<cc_idx> vdc <volts>", "Set DC link voltage"},
    {"<cc_idx> pwm <a|b|c> <pwm_chnl>", "Configure PWM outputs per phase"},
    {"<cc_idx> adc <a|b|c> <adc_chnl> <adc_gain> <adc_offset>", "Configure ADC input per phase"},
    {"<cc_idx> tune <Rs> <Ld> <Lq> <bw>", "Tune with Rs [ohms], Ldq [H], and bandwidth [rad/s]"},
    {"<cc_idx> set <Id*> <Iq*> <omega_e>", "Set operating point for Idq* [Adq] at omega_e [rad/s]"},
};

void cmd_cc_register(void)
{
    commands_cmd_init(&cmd_entry,
            "cc", "Current control (D/Q) related commands",
            cmd_help, NUM_HELP_ENTRIES,
            cmd_cc
    );

    // Register the command with the system
    commands_cmd_register(&cmd_entry);
}

static inline bool STREQ(char *in1, char *in2)
{
    return (strcmp(in1, in2) == 0) ? true : false;
}

int cmd_cc(int argc, char **argv)
{
    if (argc == 3 && STREQ("init", argv[2])) {
        // Enable current controller
        int cc_idx = atoi(argv[1]);
        task_cc_init(cc_idx);
        return SUCCESS;
    }

    if (argc == 3 && STREQ("deinit", argv[2])) {
        // Disable current controller
        int cc_idx = atoi(argv[1]);
        task_cc_deinit(cc_idx);
        return SUCCESS;
    }

    if (argc == 3 && STREQ("setup", argv[1]) && STREQ("ashad", argv[2])) {
		// Setup for Ashad bench
		task_cc_vdc_set(0, 10.0);

		task_cc_adc(0, 0, 16, -2.5, 0.0); // A
		task_cc_adc(0, 1, 15, -2.5, 0.0); // B
		task_cc_adc(0, 2, 13, -2.5, 0.0); // C

		task_cc_pwm(0, 0, 2); // A
		task_cc_pwm(0, 1, 1); // B
		task_cc_pwm(0, 2, 0); // C

		task_cc_tune(0, 1.0, 0.0015, 0.0015, 628.0);

		return SUCCESS;
	}

    if (argc == 3 && STREQ("setup", argv[1]) && STREQ("yusuke", argv[2])) {
        // Setup for Ashad bench
        task_cc_vdc_set(0, 10.0);
        task_cc_vdc_set(1, 10.0);

        task_cc_adc(0, 0, 3, 1.434, -0.01); // TA
        task_cc_adc(0, 1, 2, 1.434, -0.08); // TB
        task_cc_adc(0, 2, 1, 1.434, -0.00); // TC
        task_cc_adc(1, 0, 6, 2.199, -0.01); // SA
        task_cc_adc(1, 1, 5, 2.199, -0.04); // SB
        task_cc_adc(1, 2, 4, 2.199, -0.07); // SC

        task_cc_pwm(0, 0, 5); // TA
        task_cc_pwm(0, 1, 4); // TB
        task_cc_pwm(0, 2, 3); // TC
        task_cc_pwm(1, 0, 1); // SA
        task_cc_pwm(1, 1, 2); // SB
        task_cc_pwm(1, 2, 0); // SC

        task_cc_tune(0, 0.6, 0.0025, 0.0025, 6283.0); //Torque
        task_cc_tune(1, 2.4, 0.010, 0.010, 6283.0); //Suspension

        return SUCCESS;
    }

    if (argc == 4 && STREQ("vdc", argv[2])) {
        // Set DC link voltage
        int cc_idx = atoi(argv[1]);

        double vdc = strtod(argv[3], NULL);

        // Sanitize inputs
        if (vdc <= 0.0 || vdc > 1000.0) return INVALID_ARGUMENTS;

        task_cc_vdc_set(cc_idx, vdc);
        return SUCCESS;
    }

    if (argc == 5 && STREQ("pwm", argv[2])) {
        int cc_idx = atoi(argv[1]);

        // Extract phase user is configuring
        uint8_t phase;
        if (STREQ("a", argv[3])) {
            phase = 0;
        } else if (STREQ("b", argv[3])) {
            phase = 1;
        } else if (STREQ("c", argv[3])) {
            phase = 2;
        } else {
            return INVALID_ARGUMENTS;
        }

        uint8_t pwm_chnl = atoi(argv[4]);
        if (pwm_chnl >= 24) {
            return INVALID_ARGUMENTS;
        }

        task_cc_pwm(cc_idx, phase, pwm_chnl);

        return SUCCESS;
    }

    if (argc == 7 && STREQ("adc", argv[2])) {
        int cc_idx = atoi(argv[1]);

        // Extract phase user is configuring
        uint8_t phase;
        if (STREQ("a", argv[3])) {
            phase = 0;
        } else if (STREQ("b", argv[3])) {
            phase = 1;
        } else if (STREQ("c", argv[3])) {
            phase = 2;
        } else {
            return INVALID_ARGUMENTS;
        }

        uint8_t adc_chnl = atoi(argv[4]);
        if (adc_chnl <= 0 || adc_chnl >= ANALOG_NUM_CHANNELS) {
            return INVALID_ARGUMENTS;
        }

        double adc_gain = strtod(argv[5], NULL);
        double adc_offset = strtod(argv[6], NULL);

        if (adc_gain <= -1e6 || adc_gain >= 1e6) return INVALID_ARGUMENTS;
        if (adc_offset <= -1e6 || adc_offset >= 1e6) return INVALID_ARGUMENTS;

        task_cc_adc(cc_idx, phase, adc_chnl, adc_gain, adc_offset);

        return SUCCESS;
    }

    if (argc == 7 && STREQ("tune", argv[2])) {
        int cc_idx = atoi(argv[1]);

        // Tune current controller
        double Rs, Ld, Lq, bw;

        // Extract arguments
        Rs = strtod(argv[3], NULL);
        Ld = strtod(argv[4], NULL);
        Lq = strtod(argv[5], NULL);
        bw = strtod(argv[6], NULL);

        // Sanitize inputs
        if (Rs <= 0.0 || Rs > 100.0) return INVALID_ARGUMENTS;
        if (Ld <= 0.0 || Ld > 100.0) return INVALID_ARGUMENTS;
        if (Lq <= 0.0 || Lq > 100.0) return INVALID_ARGUMENTS;
        if (bw <= 0.0 || bw > 100000.0) return INVALID_ARGUMENTS;

        task_cc_tune(cc_idx, Rs, Ld, Lq, bw);
        return SUCCESS;
    }

    if (argc == 6 && STREQ("set", argv[2])) {
        int cc_idx = atoi(argv[1]);

        // Tune current controller
        double Id_star, Iq_star, omega_e;

        // Extract arguments
        Id_star = strtod(argv[3], NULL);
        Iq_star = strtod(argv[4], NULL);
        omega_e = strtod(argv[5], NULL);

        // Sanitize inputs
        if (Id_star <= -100.0 || Id_star >= 100.0) return INVALID_ARGUMENTS;
        if (Iq_star <= -100.0 || Iq_star >= 100.0) return INVALID_ARGUMENTS;
        if (omega_e <= -1e6 || omega_e >= 1e6) return INVALID_ARGUMENTS;

        task_cc_set(cc_idx, Id_star, Iq_star, omega_e);
        return SUCCESS;
    }

    return INVALID_ARGUMENTS;
}

#endif // APP_DEMO
