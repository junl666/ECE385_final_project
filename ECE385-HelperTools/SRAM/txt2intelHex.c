// This file is to convert the sprite.txt to the intel-format hex file sprite.hex,
// which can be used to initialize the memory content of the on-chip-memory in Quartus


#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "../On-Chip Memory/sprite_bytes/sprite.txt"
#define OUTPUT_FILE "sprite.hex"

int main() {
    FILE *inputFile = fopen(INPUT_FILE, "r");
    FILE *outputFile = fopen(OUTPUT_FILE, "w");

    if (!inputFile || !outputFile) {
        printf("Error opening file!\n");
        return 1;
    }

    char line[256];
    int address = 0;
    while (fgets(line, sizeof(line), inputFile)) {
        int byteCount = strlen(line) / 2;
        fprintf(outputFile, ":%02X%04X00", byteCount, address);

        int checksum = byteCount + (address >> 8) + (address & 0xFF);
        for (int i = 0; i < byteCount; ++i) {
            int byte = strtol(line + i * 2, NULL, 16);
            fprintf(outputFile, "%02X", byte);
            checksum += byte;
        }

        fprintf(outputFile, "%02X\n", (-checksum) & 0xFF);
        address += byteCount;
    }

    fprintf(outputFile, ":00000001FF\n");  // EOF record

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
