#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t _binary_puzzle_2025_06_input_start __asm("_binary_puzzle_2025_06_input_start");
extern uint32_t _binary_puzzle_2025_06_input_end __asm("_binary_puzzle_2025_06_input_end");
extern uint32_t _binary_puzzle_2025_06_input_size __asm("_binary_puzzle_2025_06_input_size");

#define puzzle_2025_06_input_start ((const char *)&_binary_puzzle_2025_06_input_start)
#define puzzle_2025_06_input_end ((const char *)&_binary_puzzle_2025_06_input_end)
#define puzzle_2025_06_input_size (puzzle_2025_06_input_end - puzzle_2025_06_input_start)

#ifdef __cplusplus
}
#endif
