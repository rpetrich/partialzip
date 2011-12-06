#include <stdio.h>
#include "common.h"
#include "partial/partial.h"

char endianness = IS_LITTLE_ENDIAN;

void callback(ZipInfo* info, CDFile* file, size_t progress) {
	int percentDone = progress * 100/file->compressedSize;
	printf("Getting: %d%%\n", percentDone);
}

size_t data_callback(ZipInfo* info, CDFile* file, unsigned char *buffer, size_t size, void *userInfo)
{
	char **pos = userInfo;
	memcpy(*pos, buffer, size);
	*pos += size;
}

int main(int argc, char* argv[]) {
	ZipInfo* info = PartialZipInit("file://test.zip");
	if(!info)
	{
		printf("Cannot find /tmp/test.zip\n");
		return 0;
	}

	PartialZipSetProgressCallback(info, callback);

	CDFile* file = PartialZipFindFile(info, "test.txt");
	if(!file)
	{
		printf("Cannot find test.txt in /tmp/test.zip\n");
		return 0;
	}

	int dataLen = file->size;
	unsigned char *data = malloc(dataLen+1);
	unsigned char *pos = data;
	PartialZipGetFile(info, file, data_callback, &pos);
	*pos = '\0';

	PartialZipRelease(info);

	printf("%s\n", data);

	free(data);

	return 0;
}

