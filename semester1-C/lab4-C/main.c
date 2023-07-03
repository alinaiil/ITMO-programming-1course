#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t getFileSize(char *file) {
    size_t size;
    FILE *song = fopen(file, "r");
    fseek(song, 0, SEEK_END);
    size = ftell(song);
    fclose(song);
    return size;
}

void showFile(char *bytes) {
    int i = 10;
    while (bytes[i]) {
        char name[5];
        for (int j = 0; j < 4; j++) {
            name[j] = bytes[i + j];
        }
        name[4] = '\0';
        printf("%s: ", name);
        i += 4;
        int size = bytes[i] * 256 * 256 * 256 + bytes[i + 1] * 256 * 256 + bytes[i + 2] + bytes[i + 3];
        i += 6;
        for (int j = 0; j < size; j++) {
            printf("%c", bytes[i]);
            i++;
        }
        printf("\n");
    }
}

int getFrame(char *bytes, char *frame) {
    int i = 10;
    while (bytes[i]) {
        char curFrame[5];
        for (int j = 0; j < 4; j++) {
            curFrame[j] = bytes[i + j];
        }
        curFrame[4] = '\0';
        i += 4;
        int size = bytes[i] * 256 * 256 * 256 + bytes[i + 1] * 256 * 256 + bytes[i + 2] * 256 + bytes[i + 3];
        i += 6;
        if (!strcmp(curFrame, frame)) {
            printf("%s: ", frame);
            for (int j = 0; j < size; j++) {
                printf("%c", bytes[i]);
                i++;
            }
            return 1;
        } else {
            i += size;
        }
    }
    printf("No frame %s found", frame);
    return 0;
}

void rewriteSize(char *bytes, int newSize, int index) {
    bytes[index] = (char) (newSize / (256 * 256 * 256));
    bytes[index + 1] = (char) ((newSize % (256 * 256 * 256)) / (256 * 256));
    bytes[index + 2] = (char) (((newSize % (256 * 256 * 256)) % (256 * 256)) / 256);
    bytes[index + 3] = (char) (((newSize % (256 * 256 * 256)) % (256 * 256)) % 256);
}

void lastCopying(FILE *tmp, FILE *song, char *file) {
    char *bufFinal = calloc(getFileSize("test.mp3"), sizeof(char));
    tmp = fopen("test.mp3", "rb");
    song = fopen(file, "wb");
    fseek(tmp, 0, SEEK_SET);
    fread(bufFinal, 1, getFileSize("test.mp3"), tmp);
    fwrite(bufFinal, 1, getFileSize("test.mp3"), song);
    free(bufFinal);
    fclose(song);
    fclose(tmp);
    remove("test.mp3");
}

void setFrame(char *file, char *bytes, char *frame, char *value) {
    printf("Previous value: ");
    int size_value = strlen(value) + 1;
    if (!getFrame(bytes, frame)) {
        FILE *tmp = fopen("test.mp3", "ab");
        FILE *song = fopen(file, "rb");

        char* bufHeader = calloc(10, sizeof(char));
        fread(bufHeader, 1, 10, song);
        fwrite(bufHeader, 1, 10, tmp);
        free(bufHeader);

        char sizeNewFrame[4];
        rewriteSize(sizeNewFrame, size_value, 0);
        fwrite(frame, 1, 4, tmp);
        fwrite(sizeNewFrame, 1, 4, tmp);
        fwrite("\0\0", 1, 2, tmp);

        char* bufValue = calloc(size_value, sizeof(char));
        bufValue[0] = (char) 0;
        for (int k = 1; k < size_value; k++) {
            bufValue[k] = value[k - 1];
        }
        fwrite(bufValue, 1, size_value, tmp);
        free(bufValue);

        fseek(song, 10, SEEK_SET);
        char* bufEnd = calloc(getFileSize(file) - 10, sizeof(char));
        fread(bufEnd, 1, getFileSize(file) - 10, song);
        fwrite(bufEnd, 1, getFileSize(file) - 10, tmp);
        free(bufEnd);
        fclose(song);
        fclose(tmp);

        lastCopying(tmp, song, file);
        printf("\nNew value: %s", value);
        return;
    }
    printf("\nNew value: %s", value);
    for (int i = 0; i < getFileSize(file); i++) {
        char curFrame[5];
        for (int j = 0; j < 4; j++) {
            curFrame[j] = bytes[i + j];
        }
        curFrame[4] = '\0';
        if (!strcmp(curFrame, frame)) {
            FILE *tmp = fopen("test.mp3", "ab");
            FILE *song = fopen(file, "rb");
            char* buffer = calloc(i + 4, sizeof(char));
            fread(buffer, 1, i + 4, song);
            fwrite(buffer, 1, i + 4, tmp);
            free(buffer);

            int size = bytes[i + 4] * 256 * 256 * 256 + bytes[i + 5] * 256 * 256 + bytes[i + 6] + bytes[i + 7];
            rewriteSize(bytes, size_value, i + 4);
            char* bufSize = calloc(6, sizeof(char));
            for (int k = 0; k < 6; k++) {
                bufSize[k] = bytes[i + 4 + k];
            }
            fwrite(bufSize, 1, 6, tmp);
            i += 10;
            free(bufSize);

            char* bufValue = calloc(size_value, sizeof(char));
            bufValue[0] = (char) 0;
            for (int k = 1; k < size_value; k++) {
                bufValue[k] = value[k - 1];
            }
            fwrite(bufValue, 1, size_value, tmp);
            free(bufValue);
            i += size;

            fseek(song, i, SEEK_SET);
            char* bufEnd = calloc(getFileSize(file) - i, sizeof(char));
            fread(bufEnd, 1, getFileSize(file) - i, song);
            fwrite(bufEnd, 1, getFileSize(file) - i, tmp);
            free(bufEnd);
            fclose(song);
            fclose(tmp);

            lastCopying(tmp, song, file);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    char *file, *frame, *value;
    FILE *song;
    char *bytes;
    char set = 0, get = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][2] == 'f') {
            file = strpbrk(argv[i], "=") + 1;
            song = fopen(file, "r");
            bytes = (char *) malloc(sizeof(char) * getFileSize(file));
            fread(bytes, 1, getFileSize(file), song);
        }
        if (argv[i][2] == 'g') {
            get = 1;
            frame = strpbrk(argv[i], "=") + 1;
        }
        if (argv[i][2] == 's' && strcmp(argv[i], "--show") != 0) {
            set = 1;
            frame = strpbrk(argv[i], "=") + 1;
        }
        if (argv[i][2] == 'v') {
            int valueSize = -8;
            for (int j = i; j < argc; j++) {
                valueSize++;
                valueSize += strlen(argv[j]);
            }
            char* valueBuf = calloc(valueSize, sizeof(char));
            int counter = 0;
            int firstI = i;
            while (i < argc) {
                for(int j = 0; j < strlen(argv[i]); j++) {
                    if (i == firstI && j < 8) {
                        continue;
                    }
                    valueBuf[counter++] = argv[i][j];
                }
                if (i == argc - 1) {
                    valueBuf[counter] = '\0';
                    break;
                } else {
                    valueBuf[counter++] = ' ';
                    i++;
                }
            }
            value = valueBuf;
        }
        if (!strcmp(argv[i], "--show")) {
            showFile(bytes);
        }
    }
    if (get) getFrame(bytes, frame);
    if (set) setFrame(file, bytes, frame, value);
    fclose(song);
    free(bytes);
    return 0;
}
