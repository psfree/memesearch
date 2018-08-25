#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOP 0x1F2003D5
#define NOP2 0xD503201F

typedef unsigned char u8;
typedef unsigned int u32;

u32 * memsearch(const u8 *startPos, u32 searchSize, const void *pattern, u32 patternSize) {
    if(!searchSize) return 0;
    for (u8 *pos = (u8 *)startPos; pos <= startPos + searchSize - patternSize; pos++) {
        if (memcmp(pos, pattern, patternSize) == 0) return (u32 *)pos;
    }
    return 0;
}

int main() {
	FILE * fp;
	fp = fopen("FS410d.kip", "rb");
	fseek(fp, 0, SEEK_END);
	unsigned int sz = ftell(fp);
	rewind(fp);
	unsigned char * bytes = malloc(sz);
	fread(bytes, 1, sz, fp);
	bytes+=0x100;
	printf("%x %x\n", sz, *(u32 *)(bytes));
	char pattern[8] = {0xE5, 0x07, 0x00, 0x32, 0xE0, 0x03, 0x16, 0xAA};
	u32 * addr = memsearch(bytes, sz-0x100, pattern, sizeof(pattern));
	printf("%x %x\n", addr, addr[1]);
	
	u32 max_dist = 0x10;
	u32 orig=0;
	for(int i=0; i<max_dist; i++) {
		u32 op = addr[i];
		printf("addr[%d]: %X\n", i, op);
		if(op & 0xFC000000==0x94000000) { //is a BL op
			orig=op;
			op = NOP2;
			printf("gay\n");
			break;
		}
	}
	return 0;
}
