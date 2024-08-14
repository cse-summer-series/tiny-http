#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int min_length = 4, max_length = 10;
    char buf[min_length];
    unsigned char hash[16];
    MD5(argv[1], strlen(argv[1]), hash);
    for(int i = 0; i < 16; i += 1) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    printf("%ld %ld\n", sizeof(hash), sizeof(long long));
}