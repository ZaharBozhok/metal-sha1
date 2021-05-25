//
//  util.h
//  MetalComputeBasic
//
//  Created by Zakhar Bozhok on 23.05.2021.
//  Copyright © 2021 Apple. All rights reserved.
//

#ifndef util_h
#define util_h
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

void my_set(char arr[][keySize], int rowsNum, const char *str, int bias)
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

#endif /* util_h */
