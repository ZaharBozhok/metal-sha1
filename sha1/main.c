#include <stdio.h>
#include <string.h>
#include "sha1.h"

const char maxStr[] = "4294967295";
const char partialKey[] = "53cr3t-";
const uint8_t keySize = 17;

uint32_t my_pow(const uint32_t num, const uint8_t pow)
{
    uint32_t res = 1;
    for (size_t i = 0; i < pow; i++)
    {
        res *= num;
    }
    return res;
}

void numToStr(uint32_t num, char *buff)
{
    if (num == 0)
    {
        buff[0] = '0';
        return;
    }
    if (num > 1000000000)
    {
        size_t j = 9;
        do
        {
            buff[j] = num % 10 + '0';
            num /= 10;
        } while (j--);
        return;
    }
    for (size_t i = 1; i < 11; i++)
    {
        if (num / my_pow(10, i) == 0)
        {
            size_t j = i - 1;
            do
            {
                buff[j] = num % 10 + '0';
                num /= 10;
            } while (j--);
            return;
        }
    }
}

void set(char arr[][keySize], int rowsNum, const char *str, int bias)
{
    for (int i = 0; i < rowsNum; i++)
    {
        char *pStr = &arr[i][1];
        strcpy(pStr, str);
        char *pNum = &arr[i][1] + strlen(str);
        numToStr(i + bias, pNum);
        arr[i][0] = strlen(pStr);
    }
}

void Sha1Digest_toStr(const Sha1Digest *digest, char *dst)
{
    int i;
    for (i = 0; i < 20; i++)
    {
        int c0 = ((uint8_t *)digest->digest)[i] >> 4;
        int c1 = ((uint8_t *)digest->digest)[i] & 0xf;

        c0 = c0 <= 9 ? '0' + c0 : 'A' + c0 - 0xa;
        c1 = c1 <= 9 ? '0' + c1 : 'A' + c1 - 0xa;

        *dst++ = (char)c0;
        *dst++ = (char)c1;
    }
    *dst = '\0';
}

void CalcSha1(char arr[][keySize], int rowsNum, struct _Sha1Digest *hashes)
{
    for (int i = 0; i < rowsNum; i++)
    {
        hashes[i] = Sha1_get(&arr[i][1], (size_t)arr[i][0]);
    }
}

void ShowShas(const size_t size, struct _Sha1Digest *shas, char buff[][keySize])
{
    char hexresult[41] = {0};
    for (int i = 0; i < size; i++)
    {
        Sha1Digest_toStr(&shas[i], hexresult);
        printf("strlen(str)=%d, sha1sum(\"%s\")=\"%s\"\n", (uint8_t)buff[i][0], &buff[i][1], hexresult);
    }
}

int main()
{
    uint32_t rowsNum = 10;

    uint32_t buffSize = (keySize + sizeof(uint8_t) + sizeof(char)) * rowsNum;
    struct _Sha1Digest hashes[rowsNum];
    char buff[buffSize];
    //for (int i = 0; i < 2; i++)
    //{
        memset(buff, 0, buffSize);
        set((char(*)[keySize])buff, rowsNum, partialKey, 0 * rowsNum);

        CalcSha1((char(*)[keySize])buff, rowsNum, hashes);
        ShowShas(rowsNum, hashes, buff);
    //}

    return 0;
}