#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("-----------------%d------------------\n", sizeof(short int));
    // 定义文件指针和文件名
    FILE *inputFile, *outputFile;
    char inputFilename[] = "bg.txt";
    char outputFilename[] = "bg.ram";

    // 打开输入文件用于读取
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        perror("无法打开输入文件");
        return 1;
    }

    // 打开输出文件用于写入（以二进制模式打开）
    outputFile = fopen(outputFilename, "wb");
    if (outputFile == NULL) {
        perror("无法打开输出文件");
        fclose(inputFile);
        return 1;
    }

    // 定义一个缓冲区用于读取每一行
    char line[100];
    // 用于存储转换后的整数
    short int number;

    // 逐行读取文件
    while (fgets(line, sizeof(line), inputFile)) {
        // 使用strtol将字符串转换为整数
        number = strtol(line, NULL, 10);

        // 将整数写入二进制文件
        if (fwrite(&number, sizeof(short int), 1, outputFile) != 1) {
            perror("写入输出文件时出错");
            fclose(inputFile);
            fclose(outputFile);
            return 1;
        }
    }

    // 关闭文件
    fclose(inputFile);
    fclose(outputFile);

    printf("数据已成功写入二进制文件。\n");

    return 0;
}
