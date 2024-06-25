#include <stdio.h>
#include <stdlib.h>

void reduceContrast(int **pixels, int rows, int cols, double factor) {
	int i,j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            pixels[i][j] = (int)(pixels[i][j] * factor);
            pixels[i][j] = (pixels[i][j] < 0) ? 0 : ((pixels[i][j] > 255) ? 255 : pixels[i][j]);
        }
    }
}

int main() {
    FILE *fp1 = fopen("Xray_Lungs.pgm", "r");
    FILE *fp2 = fopen("low_Xray_Lungs.pgm", "w");

    int c, r, m,i,j;

    fscanf(fp1, "P2 %d %d %d", &c, &r, &m);

    int **pixels = (int **)malloc(r * sizeof(int *));
    for (i = 0; i < r; i++) pixels[i] = (int *)malloc(c * sizeof(int));

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            fscanf(fp1, "%d", &pixels[i][j]);
    double cF = 0.5;
    reduceContrast(pixels, r, c, cF);

    fprintf(fp2, "P2\n%d %d\n%d\n", c, r, m);
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            fprintf(fp2, "%d ", pixels[i][j]);
        }
        fprintf(fp2, "\n");
    }

    fclose(fp1);
    fclose(fp2);

    for (i = 0; i < r; i++) free(pixels[i]);
    free(pixels);

    return 0;
}

