#ifndef _J_UTILS_H_
#define _J_UTILS_H_

#include <string.h>
#include <assert.h>

#define J_ASM_STRING_MAX_LEN 4096

static __inline__ const char *j_string_to_asm_string(const char *string)
{
	static char buffer[J_ASM_STRING_MAX_LEN];
	char *bufpos = buffer;
	int i, sn, len = strlen(string), buflen = J_ASM_STRING_MAX_LEN;
	
	assert('\"' == string[0] && '\"' == string[len-1]);
	
	for (i = 1; i < len-1;) {
		if (string[i] == 92) {
			assert (i < len-2);

			switch (string[i+1]) {
				case 'n':  sn = snprintf(bufpos, buflen, "0x%x , ", '\n'); break;
				case 't':  sn = snprintf(bufpos, buflen, "0x%x , ", '\t'); break;
				case 'r':  sn = snprintf(bufpos, buflen, "0x%x , ", '\r'); break;
				case '\\': sn = snprintf(bufpos, buflen, "0x%x , ", '\\'); break;
				default: assert(0);
			}
			
			assert(sn < buflen && sn > 0);
			buflen -= sn;
			bufpos += sn;
			
			i += 2;
		} else {
			sn = snprintf(bufpos, buflen, "0x%x , ", string[i]);
			assert(sn < buflen && sn > 0);
			buflen -= sn;
			bufpos += sn;
			i += 1;
		} 
	}

	sn = snprintf(bufpos, buflen, "0x0"); 
	assert(sn < buflen && sn > 0);

	return buffer;
}

#endif

