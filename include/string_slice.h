#ifndef STRING_SLICE_H
#define STRING_SLICE_H

#include <stdint.h>
#include <string.h>

// Type definitions
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

/// @brief A C string with an enforced length
typedef struct {
    uint64_t len;
    char* begin;
} StringSlice;

// Function declarations
/// @brief Converts a slice back into a standard C string
/// @param slice The source StringSlice
char* slice_to_string(const StringSlice slice);

/// @brief Converts a string to a StringSlice
/// @param string The source string
StringSlice string_to_slice(const char* string);

/// @brief Creates a StringSlice given a beginning offset and a total length
/// @param string The source string
/// @param offset The offset of the start of the slice
/// @param length The total length of the slice
StringSlice slice_from_range(const char* string, u64 offset, u64 length);

#endif // STRING_SLICE_H
