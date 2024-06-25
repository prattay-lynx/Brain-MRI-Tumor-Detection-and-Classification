#include <stdio.h>
#include <stdlib.h>

int main() {
    int r, c, m, i, j;
    char str[10]; float h;
    
    printf("Enter h:"); scanf("%f", &h);

    FILE *fp1 = fopen("low_1_090.pgm", "rb");
    FILE *fp2 = fopen("low_1_090_GLIE.pgm", "w");

    fscanf(fp1, "%s %d %d %d", str, &c, &r, &m);
    fprintf(fp2, "%s %d %d %d", str, c, r, m);

    int **a = (int **)calloc(r + 2, sizeof(int *));
    for (i = 0; i < r + 2; i++)
        a[i] = (int *)calloc(c + 2, sizeof(int));

    float **b = (float **)calloc(r, sizeof(float *));
    for (i = 0; i < r; i++)
        b[i] = (float *)calloc(c, sizeof(float));

    for (i = 1; i <= r; i++)
        for (j = 1; j <= c; j++)
            fscanf(fp1, "%d", &a[i][j]);

    for (i = 0; i < r + 2; i++) {
        a[i][0] = a[i][1];
        a[i][c + 1] = a[i][c];
    }

    for (j = 0; j < c + 2; j++) {
        a[0][j] = a[1][j];
        a[r + 1][j] = a[r][j];
    }

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++) {
            b[i][j] = (h / 2.0) * (a[i][j] + 2 * a[i + 1][j + 1] + a[i + 2][j + 2]);
            b[i][j] = b[i][j] > m ? 255 : b[i][j];
            b[i][j] = b[i][j] < 0 ? 0 : b[i][j];
            fprintf(fp2, " %d", (int)b[i][j]);
        }
    free(a);
    free(b);

    fclose(fp1);
    fclose(fp2);
    return 0;
}

