#ifndef STRING_SLICE_H
#define STRING_SLICE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// various integer typedefs
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
    size_t len;
    char* begin;
} Slice;

// @brief A string that can be used to concatenate to
typede struct {
	size_t used_capacity;
	size_t total_capacity;
	char* buffer;
} Buffer;

/// @brief Zeros a portion of memory starting a destination and ending at destination + count
/// @param destination The pointer to begin writing zeros at
/// @param count The amount of zeros to write
void* memzero(void* destination, size_t count);

// Function declarations
/// @brief Converts a slice back into a standard C string
/// @param slice The source Slice
char* slice_to_string(const Slice slice);

/// @brief Converts a string to a Slice
/// @param string The source string
Slice* string_to_slice(const char* string);

/// @brief Creates a Slice given a beginning offset and a total length
/// @param string The source string
/// @param offset The offset of the start of the slice
/// @param length The total length of the slice
Slice* slice_from_range(char* string, size_t offset, size_t length);

/// @brief Compares two slices together and checks if they are equivalent or not
/// @param str_1 The first slice
/// @param str_2 The second slice
bool streql(const Slice* str_1, const Slice* str_2);

/// @brief Add the contents of a slice to the end of a buffer and return that buffer 
/// @param buffer The buffer to concatenate slice to
/// @param slice The slice to concatenate at the end of buffer
Buffer* concat(Buffer* buffer, Slice* slice);

/// @brief Add a C string to the end of a buffer and return that buffer
/// @param buffer The buffer to concatenate the C string to
/// @param cstr The C string to concatenate at the end of buffer
Buffer* concat_cstr(Buffer* buffer, char* cstr);

/// @brief Find the first occurrence of a specific character in a Slice and return the index of that character
/// @param slice The slice to search for the character in
/// @param search_char The character to search for in the slice
size_t find_first(Slice* slice, char search_char);

/// @brief Find the first occurrence of a specific character in a Buffer and return the index of that character
/// @param buffer The buffer to search for the character in
/// @param search_char The character to search for in the buffer
size_t find_first_in_buffer(Buffer* buffer, char search_char);

/// @brief Compare to slices together and return a 1 if s1 > s2, return 0 if s1 == s2, and return -1 if s1 < s2
/// @param s1 The first slice
/// @param s2 The second slice
i8 slice_cmp(Slice* s1, Slice* s2);

/// @brief Compare to buffers together and return a 1 if b1 > b2, return 0 if b1 == b2, and return -1 if b1 < b2
/// @param b1 The first buffer
/// @param b2 The second buffer
i8 buffer_cmp(Buffer* b1, Buffer* b2);

/// @brief Convert the given buffer into a Slice struct and return the pointer to that Slice
/// @param buffer The buffer to convert into a slice 
Slice* buffer_to_slice(Buffer* buffer);

/// @brief Convert the given Slice into a Buffer and return the pointer to that Buffer
/// @param slice The slice to convert into a buffer
/// @param capacity The total capacity to allocate for the buffer
Buffer* slice_to_buffer(Slice* slice, size_t capacity);

#endif // STRING_SLICE_H
