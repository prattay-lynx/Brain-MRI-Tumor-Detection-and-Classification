#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

int main(){
	//*************************************Declarations**************************************************//
	int i,j,n,d,p,r,c,**seg_img=NULL,**morphoimg=NULL,**binaryimg=NULL;srand(time(0));char str[10];
	int m,**group=NULL,*l=NULL,pos,f=0; float *avg=NULL,*center=NULL;
	
	FILE *fp1 = fopen("m2_cva_enrfcm1_5.txt","r");
	FILE *fp2 = fopen("m2_bin_enrfcm_5.pbm","wb");
	FILE *fp3 = fopen("m2_morpho_enrfcm_5.pbm","wb");

	fscanf(fp1,"%s %d %d %d",str,&c,&r,&p);
	fprintf(fp2,"%s %d %d %d","P1",c,r,p);
	fprintf(fp3,"%s %d %d %d\n","P1",c,r,p);
	
	seg_img=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		seg_img[i]=(int *)calloc(c,sizeof(int));
	}
	
	binaryimg=(int **)calloc(r,sizeof(int *));
	for(i=0;i<r;i++)
	{
		binaryimg[i]=(int *)calloc(c,sizeof(int));
	}
	
	morphoimg=(int **)calloc(r+2,sizeof(int *));
	for(i=0;i<r+2;i++)
	{
		morphoimg[i]=(int *)calloc(c+2,sizeof(int));
	}	
	
	for(i=0;i<r;i++)	
	{
		for(j=0;j<c;j++)	//to read the input image
		{
			fscanf(fp1,"%d",&seg_img[i][j]);
        }
	}
	
	//***********************************Conversion of the segmented image to binary image********************************************//
    float sum1=0.0;
    for(i=0;i<r;i++)	
	{
		for(j=0;j<c;j++)
		{
			sum1=sum1+seg_img[i][j];
		}
	}
	float threshold=0.0;
	threshold=sum1/(r*c);
	printf("Threshold: %.2f ",threshold);            //Calculation of threshold value
	
	for(i=0;i<r;i++)	
	{
		for(j=0;j<c;j++)	
		{
			if(seg_img[i][j] >= ceil(threshold)) binaryimg[i][j]=0;
			else binaryimg[i][j]=1;
		}
	}
	
	for(i=0;i<r;i++)	
	{
		for(j=0;j<c;j++)	//to write the output binary image
		{
			fprintf(fp2," %d",binaryimg[i][j]);
        }
        fprintf(fp2,"\n");
	}
	
	//**********************************************Erosion******************************************************//
	int se[3][3] = {{1,0,0},
	{0,1,0},
	{0,0,1}};
	
	int in,jn;
    
    for(i=1;i<=r;i++)
	{
		for(j=1;j<=c;j++)
		{
			morphoimg[i][j]=binaryimg[i-1][j-1];
		}
	}
	
	for(i=1;i<=r;i++)
	{
		for(j=1;j<=c;j++)
		{
			for(in=-1;in<=1;in++)
			{
				for(jn=-1;jn<=1;jn++)
				{
					if(se[i+in][j+jn])
					{
						if((i+in>=0)&&(i+in< r) && (j+jn>= 0)&&(j+jn< c))
						{
							if(morphoimg[i+in][j+jn]<binaryimg[i-1][j-1]) binaryimg[i-1][j-1]=morphoimg[i+in][j+jn];
						}
					}
				}
			}
		}
	}
	
	//**********************************************Dilation******************************************************//
	for(i=1;i<=r;i++)
	{
		for(j=1;j<=c;j++)
		{
			for(in=-1;in<=1;in++)
			{
				for(jn=-1;jn<=1;jn++)
				{
					if(se[i+in][j+jn])
					{
						if((i+in>=0)&&(i+in< r) && (j+jn>=0)&&(j+jn< c))
						{
							if(morphoimg[i+in][j+jn]>binaryimg[i-1][j-1]) binaryimg[i-1][j-1]=morphoimg[i+in][j+jn];
						}
					}
				}
			}
		}
	}
	//*******************************image after morpho************************//
    for(i=0;i<r;i++)	
	{
		for(j=0;j<c;j++)	//to write the morpho image
		{
			fprintf(fp3," %d",binaryimg[i][j]);
        }
        fprintf(fp3,"\n");
	}
	
	fclose(fp1);fclose(fp2);fclose(fp3);
	free(seg_img);
	free(morphoimg);
	free(binaryimg);
	
	return 0;
	
}
