#include <stdlib.h>
#include "../include/string_slice.h"

static Slice* allocate_slice(const char* string, size_t length) {
  if(length > SLICE_NPOS - sizeof(Slice) - 1) {
    return NULL;
  }

  Slice* slice = malloc(sizeof(Slice) + length + 1);
  if(slice == NULL) {
    return NULL;
  }

  slice->len = length;
  slice->begin = (char*)(slice + 1);
  if(length > 0) {
    memcpy(slice->begin, string, length);
  }
  slice->begin[length] = '\0';
  return slice;
}

static i8 compare_bytes(const char* first, size_t first_length, const char* second, size_t second_length) {
  size_t common_length = first_length < second_length ? first_length : second_length;
  int result = common_length == 0 ? 0 : memcmp(first, second, common_length);

  if(result < 0) {
    return -1;
  }
  if(result > 0) {
    return 1;
  }
  if(first_length < second_length) {
    return -1;
  }
  if(first_length > second_length) {
    return 1;
  }
  return 0;
}

void* memzero(void* destination, size_t count) {
  return memset(destination, 0, count);
}

char* slice_to_string(const Slice slice) {
  if(slice.begin == NULL && slice.len != 0) {
    return NULL;
  }
  if(slice.len == SLICE_NPOS) {
    return NULL;
  }

  char* string = malloc(slice.len + 1);
  if(string == NULL) {
    return NULL;
  }

  if(slice.len > 0) {
    memcpy(string, slice.begin, slice.len);
  }
  string[slice.len] = '\0';
  return string;
}

Slice* string_to_slice(const char* string) {
  if(string == NULL) {
    return NULL;
  }
  return allocate_slice(string, strlen(string));
}

Slice* slice_from_range(char* string, size_t offset, size_t length) {
  if(string == NULL) {
    return NULL;
  }

  size_t string_length = strlen(string);
  if(offset > string_length) {
    return allocate_slice("", 0);
  }

  size_t available = string_length - offset;
  size_t slice_length = length < available ? length : available;
  return allocate_slice(string + offset, slice_length);
}

bool streql(const Slice* str_1, const Slice* str_2) {
  if(str_1 == NULL || str_2 == NULL) {
    return false;
  }
  if(str_1->len != str_2->len) {
    return false;
  }
  return str_1->len == 0 || memcmp(str_1->begin, str_2->begin, str_1->len) == 0;
}

Buffer* concat(Buffer* buffer, Slice* slice) {
  if(buffer == NULL || slice == NULL || (slice->begin == NULL && slice->len != 0)) {
    return NULL;
  }
  if(buffer->used_capacity > buffer->total_capacity
    || (buffer->buffer == NULL && buffer->total_capacity != 0)) {
    return NULL;
  }
  if(slice->len > SLICE_NPOS - buffer->used_capacity) {
    return NULL;
  }

  size_t required_capacity = buffer->used_capacity + slice->len;
  if(buffer->buffer == NULL || required_capacity > buffer->total_capacity) {
    size_t new_capacity = buffer->total_capacity == 0 ? 1 : buffer->total_capacity;
    while(new_capacity < required_capacity) {
      if(new_capacity > SLICE_NPOS / 2) {
        new_capacity = required_capacity;
        break;
      }
      new_capacity *= 2;
    }

    if(new_capacity == SLICE_NPOS) {
      return NULL;
    }
    char* resized_buffer = realloc(buffer->buffer, new_capacity + 1);
    if(resized_buffer == NULL) {
      return NULL;
    }
    buffer->buffer = resized_buffer;
    buffer->total_capacity = new_capacity;
  }

  if(slice->len > 0) {
    memcpy(buffer->buffer + buffer->used_capacity, slice->begin, slice->len);
  }
  buffer->used_capacity = required_capacity;
  buffer->buffer[buffer->used_capacity] = '\0';
  return buffer;
}

Buffer* concat_cstr(Buffer* buffer, char* cstr) {
  if(cstr == NULL) {
    return NULL;
  }

  Slice slice = { .len = strlen(cstr), .begin = cstr };
  return concat(buffer, &slice);
}

size_t find_first(Slice* slice, char search_char) {
  if(slice == NULL || (slice->begin == NULL && slice->len != 0)) {
    return SLICE_NPOS;
  }

  for(size_t index = 0; index < slice->len; ++index) {
    if(slice->begin[index] == search_char) {
      return index;
    }
  }
  return SLICE_NPOS;
}

size_t find_first_in_buffer(Buffer* buffer, char search_char) {
  if(buffer == NULL || (buffer->buffer == NULL && buffer->used_capacity != 0)) {
    return SLICE_NPOS;
  }

  for(size_t index = 0; index < buffer->used_capacity; ++index) {
    if(buffer->buffer[index] == search_char) {
      return index;
    }
  }
  return SLICE_NPOS;
}

i8 slice_cmp(Slice* s1, Slice* s2) {
  if(s1 == NULL || s2 == NULL
    || (s1->begin == NULL && s1->len != 0)
    || (s2->begin == NULL && s2->len != 0)) {
    return 0;
  }
  return compare_bytes(s1->begin, s1->len, s2->begin, s2->len);
}

i8 buffer_cmp(Buffer* b1, Buffer* b2) {
  if(b1 == NULL || b2 == NULL
    || (b1->buffer == NULL && b1->used_capacity != 0)
    || (b2->buffer == NULL && b2->used_capacity != 0)) {
    return 0;
  }
  return compare_bytes(b1->buffer, b1->used_capacity, b2->buffer, b2->used_capacity);
}

Slice* buffer_to_slice(Buffer* buffer) {
  if(buffer == NULL || (buffer->buffer == NULL && buffer->used_capacity != 0)) {
    return NULL;
  }
  return allocate_slice(buffer->buffer, buffer->used_capacity);
}

Buffer* slice_to_buffer(Slice* slice, size_t capacity) {
  if(slice == NULL || (slice->begin == NULL && slice->len != 0)) {
    return NULL;
  }

  size_t total_capacity = capacity > slice->len ? capacity : slice->len;
  if(total_capacity == SLICE_NPOS) {
    return NULL;
  }

  Buffer* buffer = malloc(sizeof(Buffer));
  if(buffer == NULL) {
    return NULL;
  }

  buffer->buffer = malloc(total_capacity + 1);
  if(buffer->buffer == NULL) {
    free(buffer);
    return NULL;
  }

  buffer->used_capacity = slice->len;
  buffer->total_capacity = total_capacity;
  if(slice->len > 0) {
    memcpy(buffer->buffer, slice->begin, slice->len);
  }
  buffer->buffer[slice->len] = '\0';
  return buffer;
}
