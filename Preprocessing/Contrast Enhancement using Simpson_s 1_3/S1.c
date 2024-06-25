#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>


int main(){
	int i,j,k,n,d,r,c,p;float h=0.8;srand(time(0));char str[10];
	int m;int **a=NULL,**b = NULL;float **N=NULL;
    FILE *fp[2];

    fp[0] = fopen("brain.txt","r");
	fp[1] = fopen("brain_SIMP.pgm","wb");

	fscanf(fp[0],"%s %d %d %d",str,&c,&r,&m);
	fprintf(fp[1],"%s %d %d %d\n",str,c,r,m);
	
	printf("Enter the value of h:");
	scanf("%f",&h);
	
	a=(int **)calloc(r+2,sizeof(int *));
	for(i=0;i<r+2;i++)
	{
		a[i]=(int *)calloc(c+2,sizeof(int));
	}

	b=(int **)calloc(r+2,sizeof(int *));
	for(i=0;i<r+2;i++)
	{
		b[i]=(int *)calloc(c+2,sizeof(int));
	}

	N=(float **)calloc(r+2,sizeof(float *));
	for(i=0;i<r+2;i++)
	{
		N[i]=(float *)calloc(c+2,sizeof(float));
	}
	
	for(i=1;i<=r;i++)
	{
		for(j=1;j<=c;j++)	//to read the input image
		{
			fscanf(fp[0],"%d",&a[i][j]);
        }
	}

	for(i=0;i<r+2;i++)
    {
		a[i][0]=a[i][1];
     	a[i][c+1]=a[i][c];
	}
	for(j=0;j<c+2;j++)
	{
		a[0][j] = a[1][j];
		a[r+1][j]= a[r][j];
	}
	
	
	for(i=1;i<=r;i++)
	{
		for(j=1;j<=c;j++)
		{
			N[i][j]=(float)((h/3.0)*(a[i-1][j-1]+4*a[i][j]+a[i+1][j+1]));
			if(N[i][j]<0) b[i][j]=0;
			else if(N[i][j]>255) b[i][j]=255;
			else b[i][j]=N[i][j];
		}
	}
	
	
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
//			printf("%d ",b[i][j]);
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
