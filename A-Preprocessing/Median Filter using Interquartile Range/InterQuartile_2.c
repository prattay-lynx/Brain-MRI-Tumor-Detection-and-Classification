#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void sort(int ar[],int n)
{
	int temp,j,i;
	for(i=1;i<n;i++)
	{
		temp=ar[i];
		j=i-1;
		while(j>=0 && ar[j]>temp)
		{
			ar[j+1]=ar[j];
			j--;
		}
		ar[j+1]=temp;
	}
}

int IndexMedian(int Li,int Ui){
	return (Li+Ui)/2;
}

double calculate_psnr(int** originalImage, int** filteredImage, int width, int height) {
    double mse = 0.0;
	int i,j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int diff = originalImage[i][j] - filteredImage[i][j];
            mse += diff * diff;
        }
    }
    mse /= (width * height);
    double psnr = 20 * log10(255.0) - 10 * log10(mse);

    return psnr;
}

int main(){
	int i,j,n,d,r,c,p;srand(time(0));char str[10];
	int m,f=0;int fm; int **a=NULL,**b = NULL;int *win=NULL;
	int flag = 1;int in,jn,in1,jn1; int Sr;
	int count = 0; float sum=0.0;
    int k=0;float temp=0.0;int q=0; float totalPixels=0.0;
    FILE *fp[2];  int x = 0;
    int w = 3;
    int *nW=NULL;

	printf("Enter mask size:");
	scanf("%d",&w);
	Sr=2*w+1;
	printf("Sr is:  %d\n",Sr);
	int pad = Sr/2;

    fp[0] = fopen("Brain_c_3.txt","r");
	fp[1] = fopen("Brain_c_3_IQR.pgm","wb");
	
	fscanf(fp[0],"%s %d %d %d",str,&c,&r,&m);
	fprintf(fp[1],"%s %d %d %d\n",str,c,r,m);
	
	a=(int **)calloc(r+w*2,sizeof(int *));
	for(i=0;i<r+w*2;i++)
	{
		a[i]=(int *)calloc(c+w*2,sizeof(int));
	}

	b=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		b[i]=(int *)calloc(c,sizeof(int));
	}
	
	nW=(int*)calloc(Sr*Sr,sizeof(int));
	win=(int*)calloc(Sr*Sr,sizeof(int));
	
	
	for(i=w;i<=r-1+w;i++)
	{
		for(j=w;j<=c-1+w;j++)	//to read the input image
		{
			fscanf(fp[0],"%d",&a[i][j]);
        }
	}
	
	for(i=0;i<r+w*2;i++)
	{
		for(j=0;j<w;j++)
		{
			a[i][j]=a[i][w];
			a[i][c+w+j]=a[i][c-1+w];
		}
	}
	for(j=0;j<c+w*2;j++)
	{
		for(i=0;i<w;i++)
		{
			a[i][j]=a[w][j];
			a[r+w+i][j]=a[r-1+w][j];
		}
	}


	for (i=0; i<r; i++) {
        for (j=0; j<c; j++){
		
			 x=0;k=0;
			
			 for(in=-pad;in<=pad;in++){
			 	for(jn=-pad;jn<=pad;jn++){
			 		win[x]=a[i+in+w][j+jn+w];
//			 		printf("%d ",win[x]);
			 		x++;
			 	}
			 }

			sort(win,Sr*Sr);

			for(q=0;q<Sr*Sr;q++)
			{
				if(win[q]==0 || win[q]==255)
				   continue;
				else
					{
						nW[k]=win[q];
//						printf("%d ",nW[k]);
						k++;
					}
			}
//			printf("\n");
			totalPixels=k;
			int Li = 0;
			int Ui = k-1;
			
			int index = IndexMedian(Li,Ui);

			int IQR_lower = (int)totalPixels/4.0;
			int IQR_upper = (int)((float)(3*totalPixels))/4.0;
			float deno = IQR_upper-IQR_lower+1;
			
			for(q=IQR_lower;q<=IQR_upper;q++)
				sum+=nW[q];
			
			b[i][j]=(int)(sum/deno);
//			printf("%d ",b[i][j]);
			sum=0.0;
			for(q=0;q<Sr*Sr;q++)
			{
				nW[q]=0;
			}
		  }
		}
		
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)	//to write the output image
		{
			fprintf(fp[1]," %d",b[i][j]);
        }
        fprintf(fp[1],"\n");
	}
	
	double psnr = calculate_psnr(a, b, r, c);
    printf("Peak Signal-to-Noise Ratio (PSNR): %.2f\n", psnr);
	
	fclose(fp[0]);
    fclose(fp[1]);
    free(a);
    free(b);
	return 0;
}
