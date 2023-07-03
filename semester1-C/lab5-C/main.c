#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int gameWidth, gameHeight;

typedef struct {
    unsigned char *header;
    int height;
    int width;
    int size;
    int **image;
    unsigned char *colourInfo;
} bmp;

bmp readBMP(FILE *file) {
    bmp newField;
    unsigned char *header = (unsigned char *) calloc(54, 1);
    fread(header, 1, 54, file);
    newField.header = header;
    newField.width = header[21] * 256 * 256 * 256 +
            header[20] * 256 * 256 + header[19] * 256 + header[18];
    newField.height = header[25] * 256 * 256 * 256 +
            header[24] * 256 * 256 + header[23] * 256 + header[22];
    newField.size = header[5] * 256 * 256 * 256 +
            header[4] * 256 * 256 + header[3] * 256 + header[2];
    unsigned char *colourInfo = (unsigned char *) calloc(newField.size - 54, 1);
    fseek(file, 54, SEEK_SET);
    fread(colourInfo, 1, newField.size - 54, file);
    newField.colourInfo = colourInfo;
    int **image = (int **) malloc(newField.height * sizeof(int *));
    for (int i = 0; i < newField.height; i++) {
        image[i] = (int *) malloc(newField.width * sizeof(int));
    }
    int ind = 0;
    for (int i = newField.height - 1; i >= 0; i--) {
        for (int j = 0; j < newField.width; j++) {
            if (colourInfo[ind] == 255 && colourInfo[ind + 1] == 255 && colourInfo[ind + 2] == 255) {
                image[i][j] = 0;
            } else {
                image[i][j] = 1;
            }
            ind += 3;
        }
        ind += newField.width % 4;
    }
    newField.image = image;
    return newField;
}

void createBMP(char *file, bmp field) {
    FILE *generation = fopen(file, "w");
    fwrite(field.header, 1, 54, generation);
    int ind = 0;
    for (int i = field.height - 1; i >= 0; i--) {
        for (int j = 0; j < field.width; j++) {
            for (int rgb = 0; rgb < 3; rgb++) {
                if (field.image[i][j] == 0) {
                    field.colourInfo[ind] = 255;
                } else {
                    field.colourInfo[ind] = 0;
                }
                ind++;
            }
        }
        while (ind % 4 != 0) {
            field.colourInfo[ind] = 0;
            ind++;
        }
    }
    fseek(generation, 54, SEEK_SET);
    fwrite(field.colourInfo, 1, field.size - 54, generation);
    fclose(generation);
}

int neighbours(bmp cells, int h, int w) {
    int counter = 0;
    if (w != 0) {
        counter += cells.image[h][w - 1];
    }
    if (h != 0) {
        counter += cells.image[h - 1][w];
    }
    if (w != 0 && h != 0) {
        counter += cells.image[h - 1][w - 1];
    }
    if (w != gameWidth - 1) {
        counter += cells.image[h][w + 1];
    }
    if (h != gameHeight - 1) {
        counter += cells.image[h + 1][w];
    }
    if (w != gameWidth - 1 && h != gameHeight - 1) {
        counter += cells.image[h + 1][w + 1];
    }
    if (w != gameWidth - 1 && h != 0) {
        counter += cells.image[h - 1][w + 1];
    }
    if (w != 0 && h != gameHeight - 1) {
        counter += cells.image[h + 1][w - 1];
    }
    return counter;
}

void gameLife(int max_iter, int dump_freq, char *outputWay, bmp game) {
    for (int iter = 0; iter < max_iter; iter++) {
        int **tmp = (int **) malloc(game.height * sizeof(int *));
        for (int i = 0; i < game.height; i++) {
            tmp[i] = (int *) malloc(game.width * sizeof(int));
        }
        char *genFile = (char *) malloc((max_iter + 5 + strlen(outputWay)) * sizeof(char));
        sprintf(genFile, "%s/%d.bmp", outputWay, iter + 1);
        for (int i = 0; i < game.height; i++) {
            for (int j = 0; j < game.width; j++) {
                int nbrs = neighbours(game, i, j);
                if (game.image[i][j] == 0) {
                    if (nbrs == 3) {
                        tmp[i][j] = 1;
                    } else {
                        tmp[i][j] = 0;
                    }
                } else {
                    if (nbrs > 3 || nbrs < 2) {
                        tmp[i][j] = 0;
                    } else {
                        tmp[i][j] = 1;
                    }
                }
            }
        }
        int same = 0;
        for (int i = 0; i < game.height; i++) {
            for (int j = 0; j < game.width; j++) {
                if (tmp[i][j] == game.image[i][j]) {
                    same += 1;
                } else {
                    game.image[i][j] = tmp[i][j];
                }
            }
        }
        if (same == game.height * game.width) {
            return;
        }
        if (!(iter % dump_freq)) {
            createBMP(genFile, game);
        }
        free(tmp);
        free(genFile);
    }
}

int main(int argc, char *argv[]) {
    int max_iter = 2;
    int dump_freq = 1;
    char *file, *outputWay;
    FILE *bmpFile;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input")) {
            file = argv[i + 1];
            bmpFile = fopen(file, "r");
            if (bmpFile == NULL) {
                printf("Wrong file.\n");
                return 0;
            }
        } else if (!strcmp(argv[i], "--output")) {
            outputWay = argv[i + 1];
        } else if (!strcmp(argv[i], "--max_iter")) {
            sscanf(argv[i + 1], "%d", &max_iter);
        } else if (!strcmp(argv[i], "--dump_freq")) {
            sscanf(argv[i + 1], "%d", &dump_freq);
        }
    }

    bmp game = readBMP(bmpFile);
    gameHeight = game.height;
    gameWidth = game.width;
    gameLife(max_iter, dump_freq, outputWay, game);

    return 0;
}
