// Quick and dirty test for BLE Midi timestamps in Midi.cpp

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int bit(int value, int index)
{
    return (value & (1 << index)) >> index;
}

void printBits(int value, int n)
{
    for(int i = n - 1; i >= 0; i--)
        printf("%d", bit(value, i));
}

void assert(int expr)
{
    if(!expr) {
        fprintf(stderr, "Test failed\n");
        printf("\n");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    for(int t = 0; t < (1 << 13); t++) {

        // ******** Code to be tested *****************************
        // ******** Midi.cpp : Midi::sendMessage()
        uint8_t headerByte = (1 << 7) | ((t >> 7) & ((1 << 6) - 1));
        uint8_t timestampByte = (1 << 7) | (t & ((1 << 7) - 1));
        // ********************************************************
        printf("%d\t", t);

        printf("timestamp = ");
        printBits(t, 13);
        printf("\t");

        printf("headerByte = ");
        printBits(headerByte, 8);
        printf("\t");

        printf("timestampByte = ");
        printBits(timestampByte, 8);
        printf("\t");

        assert(bit(headerByte, 7) == 1);
        assert(bit(headerByte, 6) == 0);
        assert(bit(timestampByte, 7) == 1);

        for(int i = 7; i <= 12; i++)
            assert(bit(headerByte, i-7) == bit(t, i));

        for(int i = 0; i <= 6; i++)
            assert(bit(timestampByte, i) == bit(t, i));

        printf("ok");
        printf("\n");
    }
    puts("Test passed !");
    return EXIT_SUCCESS;
}