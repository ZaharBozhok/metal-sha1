/*
SHA1 tests by Philip Woolford <woolford.philip@gmail.com>
100% Public Domain
 */

#include "sha1.h"
#include "stdio.h"

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

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("String required\n");
    return 0;
  }

  char *str = argv[1];
  char result[21];
  char hexresult[41];

  Sha1Digest computed = Sha1_get(str, my_strlen(str));
  Sha1Digest_toStr(&computed, hexresult);
  /*
  printf(hexresult);
  size_t offset;
  for (offset = 0; offset < 20; offset++)
  {
    sprintf((hexresult + (2 * offset)), "%02X", result[offset] & 0xff);
  }
  */

  printf(hexresult);
  printf("\n");

  return 0;
}
