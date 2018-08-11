#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

struct userdet
{
	char uname[28];
	char fname[28];
	int startoff;
	int numberofchar;
};

void parseFile(char *uname, char *fname);
void addFile(char *uname, char *fname, char *buff);
int findBlobLocation(FILE *fm);
void getFile(char *uname, char *fname);
void downloadFile(char *uname, char *fname);
void deleteFile(char *uname, char *fname);

void main()
{
	char uname[20], fname[20];
	printf("Enter Username: ");
	scanf("%s", &uname);
	printf("Enter Filename: ");
	scanf("%s", &fname);
	//getFile(uname, fname);
	//parseFile(uname, fname);
	//downloadFile(uname, fname);
	deleteFile(uname, fname);
	_getch();
}

void parseFile(char *uname, char *fname)
{
	FILE *fp;
    char buff[10240],ch;
	int i=0,len;
	fp = fopen(fname, "rb");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	while (ftell(fp) != len){
		fread(&ch, sizeof(char), 1, fp);
		buff[i] = ch;
		i++;
	}
	buff[i] = '\0';
	fclose(fp);
	printf("Image File: %s", buff);
	addFile(uname, fname, buff);
}

void addFile(char *uname, char *fname, char *buff)
{
	FILE *fm,*fw;
	int buff_len,startoff=0,i;
	struct userdet u;
	buff_len = strlen(buff);
	fm = fopen("metaUser.bin", "rb+");
	fw = fopen("blob.bin", "rb+");
	startoff = findBlobLocation(fm);
	fseek(fw, startoff, SEEK_SET);
    fwrite(buff,buff_len*sizeof(char), 1, fw);
	fseek(fm, 0, SEEK_END);
	u.numberofchar = buff_len;
	u.startoff = startoff;
	for (i = 0; uname[i];i++)
        u.uname[i] = uname[i];
	for (i = 0; fname[i]; i++)
		u.fname[i] = fname[i];
	fwrite(&u, sizeof(struct userdet), 1, fm);
	fclose(fm);
	fclose(fw);
}

int findBlobLocation(FILE *fm)
{
	struct userdet u;
	fseek(fm, 0, SEEK_END);
	if (ftell(fm) == 0)
	{
		return 0;
	}
	fseek(fm,-64, SEEK_END);
	fread(&u, sizeof(struct userdet), 1, fm);
	return (u.startoff + u.numberofchar);
}

void getFile(char *uname, char *fname)
{
	FILE *fm,*fw;
	struct userdet u;
	char buff[1024];
	int i,j,ucount,fcount,found = 0;
	fm = fopen("metaUser.bin", "rb+");
	fw = fopen("blob.bin", "rb+");
	while (!feof(fm))
	{
		fread(&u, sizeof(struct userdet), 1, fm);
		for (i = 0,ucount = 0; uname[i]; i++)
		{
			if (uname[i] == u.uname[i])
				ucount++;
		}
		for (j = 0,fcount = 0; uname[j]; j++)
		{
			if (fname[j] == u.fname[j])
				fcount++;
		}
		if (ucount == i && fcount == j)
		{
			found = 1;
			break;
		}
	}
	if (found)
	{
		fseek(fw, u.startoff, SEEK_SET);
		fread(&buff, sizeof(char)*u.numberofchar, 1, fw);
		printf("\n\nFile : %s", buff);
	}
	else{
		printf("\n\nNot a valid Username or filename. Please try again!!\n\n");
	}
	fclose(fm);
	fclose(fw);
}

void deleteFile(char *uname, char *fname)
{
	FILE *fm, *fw;
	struct userdet u;
	char buff[1024];
	int i, j, ucount, fcount, found = 0;
	fm = fopen("metaUser.bin", "rb+");
	fw = fopen("blob.bin", "rb+");
	while (!feof(fm))
	{
		fread(&u, sizeof(struct userdet), 1, fm);
		for (i = 0, ucount = 0; uname[i]; i++)
		{
			if (uname[i] == u.uname[i])
				ucount++;
		}
		for (j = 0, fcount = 0; uname[j]; j++)
		{
			if (fname[j] == u.fname[j])
				fcount++;
		}
		if (ucount == i && fcount == j)
		{
			found = 1;
			break;
		}
	}
	if (found)
	{
		fseek(fw, u.startoff, SEEK_SET);
		fread(&buff, sizeof(char)*u.numberofchar, 1, fw);
		memset(&buff, 0, u.numberofchar);
		fseek(fw, u.startoff, SEEK_SET);
		fwrite(&buff, sizeof(char)*u.numberofchar, 1, fw);
		fseek(fm, -64, SEEK_CUR);
		memset(&u, 0, 64);
		fwrite(&u, sizeof(char) * 64, 1, fm);
	}
	else{
		printf("\n\nNot a valid Username or filename. Please try again!!\n\n");
	}
	fclose(fm);
	fclose(fw);
}

void downloadFile(char *uname, char *fname)
{
	FILE *fm, *fw,*fd;
	struct userdet u;
	char buff[1024], fn[40] = "abc_downloaded.txt";;
	int i, j, ucount, fcount, found = 0;
	fm = fopen("metaUser.bin", "rb+");
	fw = fopen("blob.bin", "rb+");
	while (!feof(fm))
	{
		fread(&u, sizeof(struct userdet), 1, fm);
		for (i = 0, ucount = 0; uname[i]; i++)
		{
			if (uname[i] == u.uname[i])
				ucount++;
		}
		for (j = 0, fcount = 0; uname[j]; j++)
		{
			if (fname[j] == u.fname[j])
				fcount++;
		}
		if (ucount == i && fcount == j)
		{
			found = 1;
			break;
		}
	}
	if (found)
	{
		fseek(fw, u.startoff, SEEK_SET);
		fread(&buff, sizeof(char)*u.numberofchar, 1, fw);
		buff[u.numberofchar] = '\0';
		printf("\n\nFile : %s", buff);
		fd = fopen(fn, "w");
		fprintf(fd, "%s", buff);
		fclose(fd);
	}
	else{
		printf("\n\nNot a valid Username or filename. Please try again!!\n\n");
	}
	fclose(fm);
	fclose(fw);
}