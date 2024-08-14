#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    unsigned char passwd[4];
    unsigned char ashexstring[33];
    ashexstring[32] = 0;
    unsigned char hashed[16];
    unsigned char* tocheck = argv[1];
    unsigned long long attempts = 0;
    for(char c1 = 'a'; c1 <= 'z'; c1 += 1) {
        passwd[0] = c1;
        for(char c2 = 'a'; c2 <= 'z'; c2 += 1) {
            passwd[1] = c2;
            for(char c3 = 'a'; c3 <= 'z'; c3 += 1) {
                passwd[2] = c3;
                for(char c4 = 'a'; c4 <= 'z'; c4 += 1) {
                    passwd[3] = c4;
                    attempts += 1;
                    MD5(passwd, 4, hashed);
                    for(int i = 0; i < 16; i += 1) {
                        sprintf(ashexstring + (i * 2), "%02x", hashed[i]);
                    }
                    if(strncmp("boop", passwd, 4) == 0) {
                        printf("Looking at boop: %s %.32s\n", ashexstring, tocheck);
                        printf("%d\n", strncmp(ashexstring, tocheck, 32));
                    }
                    if(strncmp(ashexstring, tocheck, 32) == 0) {
                        printf("Found it! %.4s = %s\n", passwd, ashexstring);
                        printf("Took %lld attempts\n", attempts);
                        return 0;
                    }
                }
            }
        }
    }
    printf("Did not find %s in %lld attempts\n", argv[1], attempts);
}