#include <stdlib.h>
#include "../include/string_slice.h"

/// @brief Converts a slice back into a standard C string
/// @param slice The source Slice
char* slice_to_string(const Slice slice) {
    return slice.begin;
}

/// @brief Converts a string to a Slice
/// @param string The source string
Slice* string_to_slice(const char* string) {
	Slice* slice = (Slice*)malloc(sizeof(size_t) + (sizeof(char) * strlen(string)));
	memcpy(slice->begin, string, strlen(string) + 1);
	slice->len = strlen(string);
	return slice;
}

/// @brief Creates a Slice given a beginning offset and a total length.
/// Will return an empty string (""\0) if offset is greater than the length of the source string.
/// @param string The source string
/// @param offset The offset of the start of the slice
/// @param length The total length of the slice
Slice* slice_from_range(char* string, u64 offset, u64 length) {
    // this code feels really dirty, pointer arithmetic is weird...

    if(offset > strlen(string)) { // offset is larger than string length
        (string + strlen(string))[0] = '\0'; // need to truncate string so that string_to_slice() works properly
				Slice* return_slice = (Slice*)malloc((strlen(string) * sizeof(char)) + sizeof(size_t));
				Slice temp = (Slice){ .begin = (char*)(string + strlen(string)), .len = 0};
				memcpy(return_slice, &temp, (strlen(string) * sizeof(char)) + sizeof(size_t));
        return return_slice; 
    }
    if(length <= strlen(string + offset)) { // given length is larger than or equal to calculated range
        (string + offset)[length] = '\0'; // need to truncate string so that string_to_slice() works properly
				Slice* return_slice = (Slice*)malloc((strlen(string + offset) * sizeof(char)) + sizeof(size_t));
				Slice temp = (Slice){ .begin = (char*)(string + offset), .len = length};
				memcpy(return_slice, &temp, (strlen(string + offset) * sizeof(char)) + sizeof(size_t));
        return return_slice;
    } else { // given length is smaller than calculated range
        u64 offsetted_string_length = strlen(string + offset);
				Slice* return_slice = (Slice*)malloc(strlen(string + offset) * sizeof(char) + sizeof(size_t));
				Slice temp = (Slice) { .begin = (char*)(string + offset), .len = offsetted_string_length};
				memcpy(return_slice, &temp, (strlen(string + offset) * sizeof(char)) + sizeof(size_t));
        return return_slice; 
    }
}

/// @brief Compares two string slices together and checks if they are equivalent or not
/// @param str_1 The first slice
/// @param str_2 The second slice
bool streql(const Slice* str_1, const Slice* str_2) {
    return strcmp(str_1->begin, str_2->begin) == 0;
}
