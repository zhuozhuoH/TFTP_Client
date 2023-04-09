#include "util.h"

// 添加数据
int append_Message(char* buf, rsize_t bufferCount, int* offset, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = vsprintf_s(buf + *offset, bufferCount - *offset, format, args);
    va_end(args);
    *offset += ret + 1;
    return ret;
}

// 向buffer读取数据
int get_Message(char* buf, rsize_t bufferCount, char const* source, int* offset)
{
    int ret = sprintf_s(buf, bufferCount, "%s", source + *offset);
    *offset += ret + 1;
    return ret;
}

// 获得文件大小
int get_filesize(const char* fileName)
{
    struct stat statBuf;
    if (stat(fileName, &statBuf) != 0)
    {
        return -1;
    }
    return statBuf.st_size;
}

// 获得是10进制数长度
int number_length(int num)
{
    if (num == 0)
    {
        return 1;
    }
    int count = 0;
    for (; num != 0; num /= 10)
    {
        count++;
    }
    return count;
}

// 将文件转换为NETASCII编码
int encodeNetascii(const char *fileName) {
    FILE *fp = NULL, *tfp = NULL;
    char tempFileName[1024];
    strcpy_s(tempFileName, 1023, fileName);
    strcat_s(tempFileName, 1023, ".netascii");
    fopen_s(&fp, fileName, "rb");
    fopen_s(&tfp, tempFileName, "wb");
    if (fp == NULL || tfp == NULL)
    {
        if (fp) { fclose(fp); }
        if (fp) { fclose(tfp); }
        return -1;
    }

    int ch = fgetc(fp), lch = NUL;
    while (true)
    {
        if (ch == LF && lch != CR)
        {
            fputc(CR, tfp);
        }
        else if (lch == CR && ch != LF && ch != NUL)
        {
            fputc(NUL, tfp);
        }
        else if (ch == EOF)
        {
            break;
        }
        fputc(ch, tfp);
        lch = ch;
        ch = fgetc(fp);
    }

    fclose(fp);
    fclose(tfp);
    return 0;
}

// 将NETASCII编码解码
int decodeNetascii(const char *fileName, int platform) {
    FILE *fp = NULL, *tfp = NULL;
    char tempFileName[1024];
    strcpy_s(tempFileName, 1023, fileName);
    strcat_s(tempFileName, 1023, ".local");
    fopen_s(&fp, fileName, "rb");
    fopen_s(&tfp, tempFileName, "wb");
    if (fp == NULL || tfp == NULL)
    {
        if (fp)
        {
            fclose(fp);
        }
        if (fp)
        {
            fclose(tfp);
        }
        return -1;
    }

    int ch = 0, nch = 0;
    while (true)
    {
        ch = fgetc(fp);
        if (ch == CR)
        {
            nch = fgetc(fp);
            if ( platform != PLATFORM_WINDOWS && nch == LF)
            {
                ch = LF;
            }
            else if (nch == NUL)
            {
                ch = CR;
            }
            else
            {
                fseek(fp, -1L, SEEK_CUR);
            }
        }
        else if (ch == EOF)
        {
            break;
        }
        fputc(ch, tfp);
    }

    fclose(fp);
    fclose(tfp);
    return 0;
}
