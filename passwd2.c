#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t char_to_num(unsigned char c) {
    if(c >= 'a' && c <= 'f') { return c - 96 + 9; }
    if(c >= '0' && c <= '9') { return c - 48; }
}

void hex_to_bytes(unsigned char* hex, unsigned int hex_len, unsigned char* bytes) {
    for(int i = 0; i < hex_len / 2; i += 1) {
        unsigned char c1 = hex[i * 2];
        unsigned char c2 = hex[(i * 2) + 1];
        printf("%c -> %d\n", c1, char_to_num(c1));

        printf("%c -> %d\n", c2, char_to_num(c2));
        bytes[i] = char_to_num(c1) * 16 + char_to_num(c2);
    }
}

unsigned int try_all(unsigned int cur_length, unsigned int index, unsigned char* passwd, unsigned char* hashed, unsigned char* tocheckbytes, unsigned long long* attempts) {
    if(index >= cur_length) {
        *attempts += 1;
        MD5(passwd, cur_length, hashed);

        if(strncmp(hashed, tocheckbytes, 16) == 0) {
            unsigned char ashexstring[32];
            for(int i = 0; i < 16; i += 1) {
                sprintf(ashexstring + (i * 2), "%02x", hashed[i]);
            }
            printf("Found it! md5(%s) = %s\n", passwd, ashexstring);
            printf("Took %lld attempts\n", *attempts);
            return 1;
        }
        else {
            return -1;
        }
    }
    for(char c = 'a'; c <= 'z'; c += 1) {
        passwd[index] = c;
        unsigned int result = try_all(cur_length, index + 1, passwd, hashed, tocheckbytes, attempts);
        if(result == 1) { return 1; }
    }
    return -1;
}

int main(int argc, char** argv) {
    unsigned char hashed[16];
    unsigned char* tocheck = argv[1];
    unsigned char tocheckbytes[16];
    hex_to_bytes(tocheck, 32, tocheckbytes);
    for(int i = 0; i < 16; i += 1) {
        printf("%d ", tocheckbytes[i]);
    }
    printf("\n");

    unsigned long long attempts = 0;
    unsigned char curpasswd[21]; // we won't use all of it at once
    int result;

    for(int cur_length = 4; cur_length < 20; cur_length += 1) {
        curpasswd[cur_length] = 0;
        printf("Attempting length %d passwords\n", cur_length);
        result = try_all(cur_length, 0, curpasswd, hashed, tocheckbytes, &attempts);
        if(result != -1) { break; }
    }
    if(result == -1) {
        printf("Did not find %s in %lld attempts\n", argv[1], attempts);
    }
}