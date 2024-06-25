#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double calculate_a(double L, double H, double l, double h, double e, double b) {
    return (H - L) / ((h - l) * (h + l - 2 * b));
}

double calculate_b(double L, double H, double l, double h, double e, double E) {
    return (pow(h, 2) * (E - L) - e*(H - L) + pow(l, 2) * (H - E)) / (2 * (h*(E - L) - e*(H - L) + l*(H - E)));
}

double calculate_c(double L, double l, double b, double a) {
    return L - a * pow(l - b, 2);
}

int main() {
    int i, j, r, c, m;
    char str[10]; int **A=NULL,**B=NULL,**X=NULL;
    FILE *fp1 = fopen("Xray_Lungs.pgm", "r");
    FILE *fp2 = fopen("Xray_Lungs_bcet.pgm", "w");

    fscanf(fp1, "%s %d %d %d", str, &c, &r, &m);
    fprintf(fp2, "%s %d %d %d\n", str, c, r, m);
	
	A=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		A[i]=(int *)calloc(c,sizeof(int));
	}

	B=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		B[i]=(int *)calloc(c,sizeof(int));
	}
	
	X=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		X[i]=(int *)calloc(c,sizeof(int));
	}
	
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)	//to read the input image
		{
			fscanf(fp1,"%d",&A[i][j]);
        }
	}
	
	int pix = r * c;

	int hist[m + 1];
	for (i = 0; i < m + 1; i++)
	{
		hist[i] = 0;
	}
	int cdf[m + 1];
	int eh[m + 1];
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			hist[A[i][j]]++;
		}
	}

	for (i = 0; i < m + 1; i++)
	{
//		printf("%d ", hist[i]);
	}

	int cdfmax = m, cdfmin;
	int count = 0;
	for (i = 0; i < m + 1; i++)
	{

		count += hist[i];
		cdf[i] = count;
	}
	
	for (i = 0; i < m + 1; i++)
	{
		eh[i] = (m * (cdf[i])) / (pix);
//		printf("%d ", eh[i]);
	}

	double E;
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			X[i][j] = eh[A[i][j]];
			E+=X[i][j];
		}
	}
	E=E/pix;
	printf("%.2lf ",E);
	
    double l = INFINITY, h = -INFINITY, e = 0;

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            fscanf(fp1, "%d", &A[i][j]);
            if (A[i][j] < l) {
                l = A[i][j];
            }
            if (A[i][j] > h) {
                h = A[i][j];
            }
            e += A[i][j];
        }
    }

    e /= (r * c);
    printf("%.2lf ",e);

    double L = 0.0, H = 255.0; 

    double b = calculate_b(L, H, l, h, e, E);
    double a_val = calculate_a(L, H, l, h, e, b);
    double c_val = calculate_c(L, l, b, a_val);
//    a_val=fabs(a_val);
    printf("b: %lf\n",b);
    printf("a: %lf\n",a_val);
    printf("c: %lf\n",c_val); 

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            double x = A[i][j];
            double y = a_val * pow(x - b, 2) + c_val;
            B[i][j] = (int)y;
            if(B[i][j]>255) B[i][j]=255;
//            printf("%d ",B[i][j]);
        }
    }

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            fprintf(fp2, " %d", B[i][j]);
        }
        fprintf(fp2, "\n");
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}

