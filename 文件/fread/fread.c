#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZB_IMAGE_FILE_PATH  "/home/jorin/project/linux_c/zigbee_v6.bin" 
#define ZB_IMAGE_TEMP_FILE_PATH "/home/jorin/project/linux_c/zigbee_v7.bin" 
#define BLOCK_SIZE  128

int main(void)
{
	FILE *ZbImageFile = fopen(ZB_IMAGE_FILE_PATH,"r");
	if(NULL == ZbImageFile){
		printf("open  ZbImageFile error\n");
		return 0;
	}
	fseek(ZbImageFile,0L,SEEK_END);
	long ZbImageFileLen = ftell(ZbImageFile);
	if(ZbImageFile < 0){
		printf("ftell ZbImageFile failed\n");
		fclose(ZbImageFile);
		return -1;
	}
	printf("ZbImageFileLen=%d\n",ZbImageFileLen);
	int block_num = ZbImageFileLen/BLOCK_SIZE;
	int mod = ZbImageFileLen%BLOCK_SIZE;
	printf("%d,%d\n",  block_num,mod);
	unsigned char *p_ZbImageFile = malloc(ZbImageFileLen*sizeof(char));
	if (p_ZbImageFile == NULL){
		printf("malloc for file failed\n");
		fclose(ZbImageFile);
		return -1;
	}
	memset(p_ZbImageFile,0,ZbImageFileLen);

	fseek(ZbImageFile,0L,SEEK_SET);
	int ret = fread(p_ZbImageFile,1,ZbImageFileLen,ZbImageFile);
	if (ret <= 0){
		fclose(ZbImageFile);
		free(p_ZbImageFile);
		printf("ret=%d\n",ret);
	}

	FILE *temp_file = fopen(ZB_IMAGE_TEMP_FILE_PATH,"w+");
	if (temp_file == NULL){
		printf("open or create temp_file failed\n");
		free(p_ZbImageFile);
		fclose(ZbImageFile);
		return -1;
	}
	
	unsigned char *temp_p = p_ZbImageFile;
	for(block_num;  block_num > 0;  block_num--){
		fwrite(temp_p,1,BLOCK_SIZE,temp_file);
		temp_p = temp_p + BLOCK_SIZE;
	}
	fwrite(temp_p,1,mod,temp_file);
	fclose(temp_file);
	free(p_ZbImageFile);
	fclose(ZbImageFile);
	return 0;
}
