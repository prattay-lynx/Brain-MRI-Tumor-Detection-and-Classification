#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define POPULATION 100
#define GENERATION 30
#define MUTATION 0.001
#define PARAM 12
#define LEN 10

void sortByFitness(double *fitness, int *chromosomes, int n) {
    int i, j, k;
    double temp_fit;
    int temp_ch;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (fitness[j] < fitness[j + 1]) {
                temp_fit = fitness[j];
                fitness[j] = fitness[j + 1];
                fitness[j + 1] = temp_fit;

                for (k = 0; k < PARAM * LEN; k++) {
                    temp_ch = chromosomes[j * PARAM * LEN + k];
                    chromosomes[j * PARAM * LEN + k] = chromosomes[(j + 1) * PARAM * LEN + k];
                    chromosomes[(j + 1) * PARAM * LEN + k] = temp_ch;
                }
            }
        }
    }
}

int main() {
    int i, j, k, l, m, n, r, c, p, q, x, y, count;
    char str[10];

    FILE *fp1 = fopen("low_barbara.pgm", "r");
    FILE *fp2 = fopen("low_barbara_GA.pgm", "w");

    fscanf(fp1, "%s %d %d %d", str, &c, &r, &m);
    fprintf(fp2, "%s %d %d %d", str, c, r, m);

    int **in = (int **)calloc(r, sizeof(int *));
    for (i = 0; i < r; i++) in[i] = (int *)calloc(c, sizeof(int));

    float **out = (float **)calloc(r, sizeof(float *));
    for (i = 0; i < r; i++) out[i] = (float *)calloc(c, sizeof(float));

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++) fscanf(fp1, "%d", &in[i][j]);

    double *hist = (double *)calloc(m + 1, sizeof(double));
    double entropy, contrast, fit, max, min, f1, f2, f3, f4;

    max = 0;
    min = m;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            if (in[i][j] > max)
                max = in[i][j];
            else if (in[i][j] < min)
                min = in[i][j];
        }
    }

    double *popFitness = (double *)calloc(POPULATION, sizeof(double));
    int *popChromosomes = (int *)calloc(POPULATION * PARAM * LEN, sizeof(int));
    double decode, temp, *parameters = (double *)calloc(PARAM, sizeof(double));

    srand(time(NULL));

    for (k = 0; k < POPULATION; k++)
        for (i = 0; i < PARAM * LEN; i++)
            popChromosomes[k * PARAM * LEN + i] = rand() % 2;

    count = 0;
    do {

        for (k = 0; k < POPULATION; k++) {

            decode = 0.0;
            for (i = 1; i <= PARAM * LEN; i++) {
                if (i % LEN == 0) {
                    decode += popChromosomes[k * PARAM * LEN + i - 1] * pow(2.0, -LEN);
                    parameters[i / LEN - 1] = decode;
                    decode = 0.0;
                } else
                    decode += popChromosomes[k * PARAM * LEN + i - 1] * pow(2.0, -(i % LEN));
            }
            for (i = 0; i < PARAM; i++) {
                if (i == 4 || i == 5)
                    parameters[i] = parameters[i] * m;
                else if (i == 6)
                    parameters[i] = parameters[i] * 2 + 1;
            }
            temp = 0.0;
            for (i = 8; i <= 11; i++)
                temp += parameters[i];
            for (i = 8; i <= 11; i++)
                parameters[i] /= temp;
			
//				for(i=0;i<PARAM;i++)
//				printf(" %f",parameters[i]);

            for (i = 0; i < r; i++) {
                for (j = 0; j < c; j++) {
                    f1 = parameters[0] * log(in[i][j]);
                    f2 = 1.0 / (1.0 + pow((max - min) / parameters[5], parameters[6]));

                    if (in[i][j] > parameters[4])
                        f3 = parameters[1] * pow(in[i][j] - parameters[4], 2) + parameters[2] * in[i][j] + parameters[3];
                    else
                        f3 = parameters[2] * in[i][j] + parameters[3];

                    f4 = fabs(max - parameters[5] * pow((max / in[i][j] + parameters[7]) - 1, -parameters[6])) / max;

                    out[i][j] = (parameters[8] * f1 + parameters[9] * f2 + parameters[10] * f3 + parameters[11] * f4) * max;

                    if (out[i][j] < 0) out[i][j] = 0;
                    if (out[i][j] > m) out[i][j] = m;
                }
            }
															//fitness function
            for (i = 0; i < r; i++)
                for (j = 0; j < c; j++)
                    hist[(int)out[i][j]]++;

            for (i = 0; i <= m; i++)
                hist[i] /= (r * c);

            entropy = 0.0;
            for (i = 0; i <= m; i++)
                if (hist[i] != 0.0)										//entropy
                    entropy += -hist[i] * log(hist[i]) / log(2);

            contrast = 0.0;
            for (i = 0; i <= m; i++)
                for (j = 0; j <= m; j++)
                    contrast += hist[i] * hist[j] * pow(i - j, 2);

            fit = entropy * contrast;
            popFitness[k] = fit;
        }

        sortByFitness(popFitness, popChromosomes, POPULATION);		// sorting the fitness values

        for (k = 0; k < POPULATION / 2; k += 2) {
            if ((double)rand() / RAND_MAX <= MUTATION) {					//mutation
                y = rand() % LEN;				

                for (j = 0; j < y; j++)
                    popChromosomes[(k + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[k * PARAM * LEN + (i - 1) * LEN + j];
                popChromosomes[(k + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + y] = 1 - popChromosomes[k * PARAM * LEN + (i - 1) * LEN + y];
                for (j = y + 1; j < LEN; j++)
                    popChromosomes[(k + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[k * PARAM * LEN + (i - 1) * LEN + j];

                for (j = 0; j < y; j++)
                    popChromosomes[(k + 1 + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[(k + 1) * PARAM * LEN + (i - 1) * LEN + j];
                popChromosomes[(k + 1 + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + y] = 1 - popChromosomes[(k + 1) * PARAM * LEN + (i - 1) * LEN + y];
                for (j = y + 1; j < LEN; j++)
                    popChromosomes[(k + 1 + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[(k + 1) * PARAM * LEN + (i - 1) * LEN + j];
            } else 
			{										// crossover
                x = rand() % LEN;

                for (j = 0; j < x; j++)
                    popChromosomes[(k + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[k * PARAM * LEN + (i - 1) * LEN + j];
                for (j = 0; j < x; j++)
                    popChromosomes[(k + 1 + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[(k + 1) * PARAM * LEN + (i - 1) * LEN + j];
                for (j = x; j < LEN; j++)
                    popChromosomes[(k + 1 + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[k * PARAM * LEN + (i - 1) * LEN + j];
                for (j = x; j < LEN; j++)
                    popChromosomes[(k + POPULATION / 2) * PARAM * LEN + (i - 1) * LEN + j] = popChromosomes[(k + 1) * PARAM * LEN + (i - 1) * LEN + j];
            }
        }

        printf("\n %f ", popFitness[0]);
        count++;

    } while (count < GENERATION);

    decode = 0.0;
    for (i = 1; i <= PARAM * LEN; i++) {
        if (i % LEN == 0) {
            decode += popChromosomes[0 * PARAM * LEN + i - 1] * pow(2.0, -LEN);
            parameters[i / LEN - 1] = decode;
            decode = 0.0;
        } else
            decode += popChromosomes[0 * PARAM * LEN + i - 1] * pow(2.0, -(i % LEN));
    }
    for (i = 0; i < PARAM; i++) {
        if (i == 4 || i == 5)						//par5 and par6
            parameters[i] = parameters[i] * m;
        else if (i == 6)							//par7
            parameters[i] = parameters[i] * 2 + 1;
    }
    temp = 0.0;
    for (i = 8; i <= 11; i++)						//par9+par10+par11+par12=1
        temp += parameters[i];
    for (i = 8; i <= 11; i++)
        parameters[i] /= temp;

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            f1 = parameters[0] * log(in[i][j]);										// decoding parameters
            f2 = 1.0 / (1.0 + pow((max - min) / parameters[5], parameters[6]));

            if (in[i][j] > parameters[4])
                f3 = parameters[1] * pow(in[i][j] - parameters[4], 2) + parameters[2] * in[i][j] + parameters[3];
            else
                f3 = parameters[2] * in[i][j] + parameters[3];

            f4 = fabs(max - parameters[5] * pow((max / in[i][j] + parameters[7]) - 1, -parameters[6])) / max;

            out[i][j] = (parameters[8] * f1 + parameters[9] * f2 + parameters[10] * f3 + parameters[11] * f4) * max;

            if (out[i][j] < 0) out[i][j] = 0;
            if (out[i][j] > m) out[i][j] = m;
        }
    }

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            fprintf(fp2, " %d", (int)out[i][j]);

    fclose(fp1);
    fclose(fp2);
    for (i = 0; i < r; i++) {
        free(in[i]);
        free(out[i]);
    }
    free(in);
    free(out);
    free(popFitness);
    free(popChromosomes);
    free(hist);

    return 0;
}

