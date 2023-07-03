#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void size(char *sizeArc, int size) {
    sizeArc[0] = (char) (size / (256 * 256 * 256));
    sizeArc[1] = (char) ((size % (256 * 256 * 256)) / (256 * 256));
    sizeArc[2] = (char) (((size % (256 * 256 * 256)) % (256 * 256)) / 256);
    sizeArc[3] = (char) (((size % (256 * 256 * 256)) % (256 * 256)) % 256);
}

void createArc(FILE *arc, char *arcName, int argc, char *argv[]) {
    FILE *newFile;
    for (int i = 5; i < argc; i++) {
        newFile = fopen(argv[i], "rb");
        if (newFile == NULL) {
            printf("ERROR: Wrong file!");
            return;
        }
        fclose(newFile);
    }
    arc = fopen(arcName, "wb");
    char sizeArc[4];
    size(sizeArc, argc - 5);
    fwrite(sizeArc, 1, 4, arc);
    for (int i = 5; i < argc; i++) {
        newFile = fopen(argv[i], "rb");
        if (newFile == NULL) {
            printf("ERROR: Wrong file!");
            return;
        }
        fseek(newFile, 0, SEEK_END);
        unsigned long long sizeF = ftell(newFile);
        fprintf(arc, "%llu", sizeF);
        fwrite(" ", 1, 1, arc);
        char sizeFileName[4];
        size(sizeFileName, strlen(argv[i]));
        fwrite(sizeFileName, 1, 4, arc);
        fwrite(argv[i], 1, strlen(argv[i]), arc);
        char *buffer = calloc(sizeF, sizeof(char));
        fseek(newFile, 0, SEEK_SET);
        fread(buffer, 1, sizeF, newFile);
        fwrite(buffer, 1, sizeF, arc);
        free(buffer);
        fclose(newFile);
        printf("%s added successfully\n", argv[i]);
    }
}

void listArc(FILE *arc) {
    if (arc == NULL) {
        printf("ERROR: Wrong archive!");
        return;
    }
    char sizeArc[4];
    fread(sizeArc, 1, 4, arc);
    int numOfFiles = sizeArc[0] * 256 * 256 * 256 + sizeArc[1] * 256 * 256 + sizeArc[2] + sizeArc[3];
    printf("Archive contains %d files\n", numOfFiles);
    printf("-------------------------------------\n");
    for (int i = 0; i < numOfFiles; i++) {
        unsigned long long sizeF;
        fscanf(arc, "%llu", &sizeF);
        fseek(arc, 1, SEEK_CUR);
        char sizeFileName[4];
        fread(sizeFileName, 1, 4, arc);
        int sizeFN =
                sizeFileName[0] * 256 * 256 * 256 + sizeFileName[1] * 256 * 256 + sizeFileName[2] + sizeFileName[3];
        char file[sizeFN];
        fread(file, 1, sizeFN, arc);
        printf("File %d: %s\n", i + 1, file);
        fseek(arc, sizeF, SEEK_CUR);
    }
}

void extractArc(FILE *arc) {
    if (arc == NULL) {
        printf("ERROR: Wrong archive!");
        return;
    }
    char sizeArc[4];
    fread(sizeArc, 1, 4, arc);
    int numOfFiles = sizeArc[0] * 256 * 256 * 256 + sizeArc[1] * 256 * 256 + sizeArc[2] + sizeArc[3];
    for (int i = 0; i < numOfFiles; i++) {
        unsigned long long sizeF;
        fscanf(arc, "%llu", &sizeF);
        fseek(arc, 1, SEEK_CUR);
        char sizeFileName[4];
        fread(sizeFileName, 1, 4, arc);
        int sizeFN =
                sizeFileName[0] * 256 * 256 * 256 + sizeFileName[1] * 256 * 25 + sizeFileName[2] + sizeFileName[3];
        char file[sizeFN];
        fread(file, 1, sizeFN, arc);
        char dir[sizeFN + 4];
        sprintf(dir, "arc/%s", file);
        FILE *tmp = fopen(dir, "wb");
        char *buffer = calloc(sizeF, sizeof(char));
        fread(buffer, 1, sizeF, arc);
        fwrite(buffer, 1, sizeF, tmp);
        free(buffer);
        fclose(tmp);
    }
}

int main(int argc, char *argv[]) {
    char *arcName;
    FILE *arc;
    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "--file")) {
            arcName = argv[i + 1];
        } else if (!strcmp(argv[i], "--create")) {
            createArc(arc, arcName, argc, argv);
            i = argc;
        } else if (!strcmp(argv[i], "--extract")) {
            arc = fopen(arcName, "rb");
            extractArc(arc);
        } else if (!strcmp(argv[i], "--list")) {
            arc = fopen(arcName, "rb");
            listArc(arc);
        }
    }
    fclose(arc);
    return 0;
}
