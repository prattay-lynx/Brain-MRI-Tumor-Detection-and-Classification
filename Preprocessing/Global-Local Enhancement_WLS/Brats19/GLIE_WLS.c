#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<time.h>
#include<float.h>

#define B 8 
#define beta 4.0


int main(){
    int i, j, r, c, m, count = 0;
    char str[10];
    int **a = NULL; float **G = NULL; int **b = NULL;
    int *hist = NULL, *cdf = NULL;float *gwcdf = NULL;
    float *pdf = NULL, *gwpdf = NULL;
    float Pmax = FLT_MIN, Pmin = FLT_MAX, temp = 0.0;
//    float beta = 0.0;
    FILE *fp[2];

    fp[0] = fopen("low_y55.pgm", "rb");
    fp[1] = fopen("low_y55_GLIE.pgm", "wb");

    fscanf(fp[0], "%s %d %d %d", str, &c, &r, &m);
    fprintf(fp[1], "%s %d %d %d\n", str, c, r, m);

    a = (int **)calloc(r, sizeof(int *));
    for (i = 0; i < r; i++) {
        a[i] = (int *)calloc(c, sizeof(int));
    }
    b = (int **)calloc(r, sizeof(int *));
    for (i = 0; i < r; i++) {
        b[i] = (int *)calloc(c, sizeof(int));
    }

    G = (float **)calloc(r, sizeof(float *));
    for (i = 0; i < r; i++) {
        G[i] = (float *)calloc(c, sizeof(float ));
    }

    hist = (int *)calloc(m + 1, sizeof(int));
    cdf = (int *)calloc(m + 1, sizeof(int));
    gwcdf = (float *)calloc(m + 1, sizeof(float));
    pdf = (float *)calloc(m + 1, sizeof(float));
    gwpdf = (float *)calloc(m + 1, sizeof(float));

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            fscanf(fp[0], "%d", &a[i][j]);
        }
    }

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            hist[a[i][j]]++; // histogram

        }
    }

    for (i = 0; i < m + 1; i++) {
        count += hist[i];
        cdf[i] = count;
        pdf[i] = (float)hist[i] / (float)(r * c); // PDF

        if (pdf[i] > Pmax) Pmax = pdf[i];
//        printf(" %.2f ",Pmax);
        if (pdf[i] < Pmin) Pmin = pdf[i];
//        printf(" %.2f ",Pmin);
    }

    for (i = 0; i < m + 1; i++) {
        gwpdf[i] = Pmax * pow(((pdf[i] - Pmin) / (Pmax - Pmin)), cdf[i]);
        temp += gwpdf[i];
    }

    float temp1 = 0.0;
    float gwcdf_sum = 0.0;

    for (j = 0; j < m + 1; j++) {
        for (i = 0; i < j + 1; i++)
        	temp1 += ((float)gwpdf[i] / temp);
        gwcdf[j] = (temp1);
//        temp1 = 0.0;
        gwcdf_sum += gwcdf[j];
    }
//    gwcdf[j] = (temp1);
    printf("%.2f ",gwcdf_sum);

    float gamma = fabs(1.0 - gwcdf_sum); // gamma parameter
    printf("gamma: %f ",gamma);

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            G[i][j] = (float)(255.0 * (pow((a[i][j] / 255.0), gamma))); // enhanced base image
//            printf("%.2f ",G[i][j]);
        }
    }

    float mean = 0.0, std_orig=0.0, std_enhb=0.0;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            mean += a[i][j];
        }
    }
    mean /= (r * c);

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            std_orig += pow((a[i][j] - mean), 2);   //standard deviation of original image
        }
    }
    
    std_orig = sqrt(std_orig / (r * c));
    printf(" %.2f ",std_orig);
    mean =0.0;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            mean +=G[i][j];
        }
    }
    mean /= (r * c);
    
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            std_enhb += pow((G[i][j] - mean), 2);   //standard deviation of enhanced base image
        }
    }
    std_enhb = sqrt(std_enhb / (r * c));
	printf(" %.2f ",std_enhb);
 
//    beta = 1.0 + sqrt(fabs(std_orig-std_enhb)/(pow(2,B)-1));  
    printf(" %.2f", beta);

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            b[i][j] = beta*a[i][j]+(int)G[i][j];
            if(b[i][j]>255) b[i][j]=255;
            fprintf(fp[1], "%d ", (int)(b[i][j]));
        }
        fprintf(fp[1], "\n");
    }
    
	for (i = 0; i < r; i++) {
        free(a[i]);
        free(G[i]);
        free(b[i]);
    }

    free(a);
    free(G);
    free(b);
    free(hist);
    free(cdf);
    free(gwcdf);
    free(pdf);
    free(gwpdf);

    fclose(fp[0]);
    fclose(fp[1]);

    return 0;
}

