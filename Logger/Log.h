#pragma once

#include "logger.h"

static logger< console_log_policy > log_inst("execution.log");


#define LOG log_inst.print< severity_type::debug >
#define LOG_ERR log_inst.print< severity_type::error >
#define LOG_WARN log_inst.print< severity_type::warning >
