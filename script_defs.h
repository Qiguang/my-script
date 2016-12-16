#ifndef SCRIPT_DEFS_H_U2KQ461E
#define SCRIPT_DEFS_H_U2KQ461E
#include <stdint.h>

#pragma pack(push)
#pragma pack(1)
typedef struct _script_header {
	uint8_t text_len;
	uint8_t const_len;
	uint8_t string_len;
	uint8_t data_len;
	uint8_t buf[1];
} script_header;

#pragma pack(pop)

#endif /* end of include guard: SCRIPT_DEFS_H_U2KQ461E */
