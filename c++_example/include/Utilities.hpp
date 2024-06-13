/**
 * Here we have all the std libraries and all the utils that we need for the
 * realization of the practice
 */
#include "VariadicTable.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>

#pragma once

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

using namespace std::chrono_literals;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
