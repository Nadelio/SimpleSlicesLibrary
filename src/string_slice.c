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
StringSlice slice_from_range(char* string, u64 offset, u64 length) {
    // this code feels really dirty, pointer arithmetic is weird...

    if(offset > strlen(string)) {
        (string + strlen(string))[0] = '\0'; // need to truncate string so that string_to_slice() works properly
        return (StringSlice){ .begin = (char*)(string + strlen(string)), .len = 0};
    }
    if(length <= strlen(string + offset)) {
        (string + offset)[length] = '\0'; // need to truncate string so that string_to_slice() works properly
        return (StringSlice){ .begin = (char*)(string + offset), .len = length};
    } else {
        u64 offsetted_string_length = strlen(string + offset);
        return (StringSlice) { .begin = (char*)(string + offset), .len = offsetted_string_length};
    }
}

/// @brief Compares two string slices together and checks if they are equivalent or not
/// @param str_1 The first slice
/// @param str_2 The second slice
bool streql(const StringSlice* str_1, const StringSlice* str_2) {
    return strcmp(str_1->begin, str_2->begin) == 0;
}
