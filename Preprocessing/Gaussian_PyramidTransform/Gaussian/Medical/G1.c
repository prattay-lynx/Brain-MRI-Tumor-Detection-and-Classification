#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int i, j, r, c, m, p, q, l, x, y;
    char str[10];

    FILE *fp1 = fopen("1_090_0.05_0.01.pgm", "rb");
    FILE *fp2 = fopen("1_090_0.05_0.01_gauspy.pgm", "w");

    fscanf(fp1, "%s %d %d %d", str, &c, &r, &m);
    fprintf(fp2, "%s %d %d %d", str, c, r, m);

    int **a = (int **)calloc(r, sizeof(int *));
    for (i = 0; i < r; i++)
    {
        a[i] = (int *)calloc(c, sizeof(int));
    }
    int **b = (int **)calloc(r, sizeof(int *));
    for (i = 0; i < r; i++)
    {
        b[i] = (int *)calloc(c, sizeof(int));
    }

    double *hist = (double *)calloc(m + 1, sizeof(double));
    double *modh = (double *)calloc(m + 1, sizeof(double));

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            fscanf(fp1, "%d", &a[i][j]);
            hist[a[i][j]]++;
        }
    }

    for (l = 0; l <= m; l++)
        hist[l] /= (r * c);

    double pi = 2 * acos(0.0), sum, mean, var, temp, const_a;

    mean = 0.0;
    for (l = 0; l <= m; l++)
        mean += hist[l] * l;

    var = 0.0;
    for (l = 0; l <= m; l++)
        var += hist[l] * pow(l - mean, 2);

    const_a = sqrt(3 * var) / pi;

    for (l = 0; l <= m; l++)
    {
        temp = l;
        modh[l] = temp - (const_a * (1.0 - exp(-temp / const_a)) / (1.0 + exp(-temp / const_a)));
        if (modh[l] < 0.0)
            modh[l] = 0.0;
    }

    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            temp = 0.0;
            for (p = -1; p <= 1; p++)
            {
                for (q = -1; q <= 1; q++)
                {
                    x = i + p;
                    y = j + q;
                    x = (x < 0) ? 0 : x;
                    x = (x > r - 1) ? r - 1 : x;
                    y = (y < 0) ? 0 : y;
                    y = (y > c - 1) ? c - 1 : y;
                    temp += modh[a[x][y]];
                }
            }
            temp /= 9.0;
            fprintf(fp2, " %d", (int)temp);
        }
    }

    for (i = 0; i < r; i++)
        free(a[i]);
    for (i = 0; i < r; i++)
        free(b[i]);
    free(a);
    free(b);
    free(hist);
    free(modh);
    fclose(fp1);
    fclose(fp2);
    return 0;
}
