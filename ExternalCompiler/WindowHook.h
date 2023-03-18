#pragma once
#include <Windows.h>
#include "structDefs.h"

/**
 * @brief Sends a signal to the IDE to compile the current project.
 */
bool sendCompileSignal(processInfo* ide);




