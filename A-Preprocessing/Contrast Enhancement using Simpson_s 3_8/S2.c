#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>


int main(){
	int i,j,k,n,d,r,c,p;float h=0.8;srand(time(0));char str[10];
	int m;int **a=NULL,**b = NULL;float **N=NULL;
    FILE *fp[2]; char ch[100];
    
//    printf("Enter the input file name:");
//    scanf("%s",&ch);

    fp[0] = fopen("Brain_2.txt","r");
	fp[1] = fopen("Brain_2_h_0.50.pgm","w");

	fscanf(fp[0],"%s %d %d %d",str,&c,&r,&m);
	fprintf(fp[1],"%s %d %d %d\n",str,c,r,m);
	
	printf("Enter the value of h:");
	scanf("%f",&h);
	
	a=(int **)calloc(r+4,sizeof(int *));
	for(i=0;i<r+4;i++)
	{
		a[i]=(int *)calloc(c+4,sizeof(int));
	}

	b=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		b[i]=(int *)calloc(c,sizeof(int));
	}

	N=(float **)calloc(r,sizeof(float *));
	for(i=0;i<r;i++)
	{
		N[i]=(float *)calloc(c,sizeof(float));
	}
	
	for(i=2;i<=r+1;i++)
	{
		for(j=2;j<=c+1;j++)	//to read the input image
		{
			fscanf(fp[0],"%d",&a[i][j]);
        }
	}

	
	for(i=0;i<r+4;i++)
	{
		for(j=0;j<2;j++)
		{
			a[i][j]=a[i][2];
			a[i][c+2+j]=a[i][c+1];
		}
	}
	for(j=0;j<c+4;j++)
	{
		for(i=0;i<2;i++)
		{
			a[i][j]=a[2][j];
			a[r+2+i][j]=a[r+1][j];
		}
	}

	
	
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			N[i][j]=(float)((3.0*h)*(a[i+1][j+1]+3*a[i+2][j+2]+3*a[i+3][j+3]+a[i+4][j+4]))/8.0;
//			printf("%.2f ",N[i][j]);
			if(N[i][j]<0) b[i][j]=0;
			else if(N[i][j]>255) b[i][j]=255;
			else b[i][j]=N[i][j];
		}
	}
	
	
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			fprintf(fp[1],"%d ",b[i][j]);
//			printf("%d ",b[i][j]);
		}
		fprintf(fp[1],"\n");
	}

	fclose(fp[0]);
	fclose(fp[1]);
	free(a);
	free(b);
	free(N);

	return 0;

}
