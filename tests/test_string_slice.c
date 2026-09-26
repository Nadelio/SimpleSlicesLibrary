#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../include/string_slice.h"

static void free_buffer(Buffer* buffer) {
	free(buffer->buffer);
	free(buffer);
}

static void test_memzero(void) {
	char bytes[] = { 1, 2, 3, 4 };
	assert(memzero(bytes + 1, 2) == bytes + 1);
	assert(bytes[0] == 1);
	assert(bytes[1] == 0);
	assert(bytes[2] == 0);
	assert(bytes[3] == 4);
}

static void test_slice_creation(void) {
	char source[] = "abcdef";
	Slice* whole = string_to_slice(source);
	Slice* range = slice_from_range(source, 2, 3);
	Slice* clamped = slice_from_range(source, 4, 99);
	Slice* empty = slice_from_range(source, 99, 2);
	assert(whole != NULL && whole->len == 6 && strcmp(whole->begin, source) == 0);
	assert(range != NULL && range->len == 3 && strcmp(range->begin, "cde") == 0);
	assert(clamped != NULL && clamped->len == 2 && strcmp(clamped->begin, "ef") == 0);
	assert(empty != NULL && empty->len == 0 && strcmp(empty->begin, "") == 0);
	assert(strcmp(source, "abcdef") == 0);

	char* converted = slice_to_string(*range);
	assert(converted != NULL && strcmp(converted, "cde") == 0);
	free(converted);
	free(empty);
	free(clamped);
	free(range);
	free(whole);
}

static void test_slice_operations(void) {
	char first_bytes[] = { 'a', '\0', 'b' };
	char second_bytes[] = { 'a', '\0', 'b' };
	char later_bytes[] = { 'a', '\0', 'c' };
	Slice first = { .len = 3, .begin = first_bytes };
	Slice second = { .len = 3, .begin = second_bytes };
	Slice later = { .len = 3, .begin = later_bytes };
	Slice prefix = { .len = 2, .begin = first_bytes };

	assert(streql(&first, &second));
	assert(!streql(&first, &prefix));
	assert(slice_cmp(&first, &later) == -1);
	assert(slice_cmp(&later, &first) == 1);
	assert(slice_cmp(&first, &second) == 0);
	assert(slice_cmp(&prefix, &first) == -1);
	assert(find_first(&first, 'b') == 2);
	assert(find_first(&first, 'z') == SLICE_NPOS);
}

static void test_buffer_operations(void) {
	Slice* hello = string_to_slice("hello");
	Buffer* buffer = slice_to_buffer(hello, 5);
	assert(buffer != NULL);
	assert(buffer->used_capacity == 5);
	assert(buffer->total_capacity == 5);
	assert(strcmp(buffer->buffer, "hello") == 0);

	Slice* space = string_to_slice(" ");
	assert(concat(buffer, space) == buffer);
	assert(concat_cstr(buffer, "world") == buffer);
	assert(strcmp(buffer->buffer, "hello world") == 0);
	assert(buffer->used_capacity == 11);
	assert(buffer->total_capacity >= 11);
	assert(find_first_in_buffer(buffer, 'w') == 6);
	assert(find_first_in_buffer(buffer, 'z') == SLICE_NPOS);

	Slice* converted = buffer_to_slice(buffer);
	Buffer* equal = slice_to_buffer(converted, converted->len);
	Slice* later_slice = string_to_slice("hello worlds");
	Buffer* later = slice_to_buffer(later_slice, 12);
	assert(converted != NULL && streql(converted, &(Slice){ .len = 11, .begin = "hello world" }));
	assert(buffer_cmp(buffer, equal) == 0);
	assert(buffer_cmp(buffer, later) == -1);

	free_buffer(later);
	free(later_slice);
	free_buffer(equal);
	free(converted);
	free(space);
	free_buffer(buffer);
	free(hello);
}

int main(void) {
	test_memzero();
	test_slice_creation();
	test_slice_operations();
	test_buffer_operations();
	return 0;
}