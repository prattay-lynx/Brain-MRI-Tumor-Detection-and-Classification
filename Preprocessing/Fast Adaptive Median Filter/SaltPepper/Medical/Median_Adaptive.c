#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

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

int main()
{
    int i,j,n,d,r,c,p;
    srand(time(0));
    char str[10];
    int m;
    int **a=NULL,**b = NULL,**C=NULL;
    int in,jn,in1,jn1;
    int Sr;
    int count = 0;
    float sum=0.0;
    int k=0;
    int q=0;
    FILE *fp[2];
    int w = 3;

    fp[0] = fopen("airplane_60.pgm","r");
    fp[1] = fopen("airplane_60_AMF.pgm","w");

    fscanf(fp[0],"%s %d %d %d",str,&c,&r,&m);
    fprintf(fp[1],"%s %d %d %d\n",str,c,r,m);

    a=(int **)calloc(r,sizeof(int *));
    for(i=0; i<r; i++)
    {
        a[i]=(int *)calloc(c,sizeof(int));
    }

    b=(int **)calloc(r,sizeof(int *));
    for(i=0; i<r; i++)
    {
        b[i]=(int *)calloc(c,sizeof(int));
    }
    
    C=(int **)calloc(r,sizeof(int *));
    for(i=0; i<r; i++)
    {
        C[i]=(int *)calloc(c,sizeof(int));
    }


	for(i=0;i<r;i++)	
	{
		for(j=0;j<c;j++)	//to read the input image
		{
			fscanf(fp[0],"%d",&a[i][j]);
        }
	}


    for (i=0; i<r; i++)
    {
        for (j=0; j<c; j++)
        {
        	

            if(a[i][j]==0 || a[i][j]==255)
            {
            	sum=0.0;
            	count = 0;
                k=i-1;
                while(k>=0)
                {
                    if(a[k][j]==0 || a[k][j]==255)     //up
                    {
                    	
                        k--;
                    }
                    else
                    {
                        sum+=a[k][j];
                        count++;
                        break;
                    }
                }
                k=i+1;
                while(k<r)
                {
                    if(a[k][j]==0 || a[k][j]==255)     //down
                    {
                        k++;
//        				continue;
                    }
                    else
                    {
                        sum+=a[k][j];
                        count++;
                        break;
                    }
                }
                k=j+1;
                while(k<c)
                {
                    if(a[i][k]==0 || a[i][k]==255)     //left
                    {
                        k++;
                    }
                    else
                    {
                        sum+=a[i][k];
                        count++;
                        break;
                    }
                }
                k=j-1;
                while(k>=0)
                {
                    if(a[i][k]==0 || a[i][k]==255)     //right
                    {
                        k--;
//        				continue;
                    }
                    else
                    {
                        sum+=a[i][k];
                        count++;
                        break;
                    }
                }
                k=0;
//                printf("%.2f ",sum);
                if (count>0)
                {
                    b[i][j] = (int)(sum / count);
//                    printf("%d ",b[i][j]);
                }
            }
            else
            {
                b[i][j]=a[i][j];
            }
        }
    }

    for(i=0; i<r; i++)
    {
        for(j=0; j<c; j++)
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


