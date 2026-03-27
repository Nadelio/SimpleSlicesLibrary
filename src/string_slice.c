#include "string_slice.h"

/// @brief Converts a slice back into a standard C string
/// @param slice The source StringSlice
char* slice_to_string(const StringSlice slice) {
    return slice.begin;
}

/// @brief Converts a string to a StringSlice
/// @param string The source string
StringSlice string_to_slice(const char* string) {
    return (StringSlice){ .begin = (char*)string, .len = strlen(string)};
}

/// @brief Creates a StringSlice given a beginning offset and a total length.
/// Will return an empty string (""\0) if offset is greater than the length of the source string.
/// @param string The source string
/// @param offset The offset of the start of the slice
/// @param length The total length of the slice
StringSlice slice_from_range(const char* string, u64 offset, u64 length) {
    if(offset > strlen(string)) return (StringSlice){ .begin = (char*)(string + strlen(string)), .len = 0};
    if(length <= strlen(string + offset)) return (StringSlice){ .begin = (char*)(string + offset), .len = length};
    else {
        u64 offsetted_string_length = strlen(string + offset);
        return (StringSlice) { .begin = (char*)(string + offset), .len = offsetted_string_length};
    }
}