#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
	int i,j,n,d,r,c,p;srand(time(0));char str[10];
	int m;int **a=NULL,**b = NULL,**g=NULL;int *win=NULL,**hist = NULL,**CDF=NULL;
	int flag = 1;int in,jn,in1,jn1; int Sr;
	int count = 0;
    int k=0;float temp=0.0;int q=0; float Xpsum=0.0;
    FILE *fp[2];  int x = 0;
    int w = 3;

	printf("Enter mask size: ");
	scanf("%d",&w);
	Sr=2*w+1;
	printf("Sr is:  %d\n",Sr);
	int pad = Sr/2;

    fp[0] = fopen("low_tank.pgm","rb");
	fp[1] = fopen("low_tank_GA1.pgm","wb");
	
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
	
	g=(int **)calloc(r+w*2,sizeof(int *));
	for(i=0;i<r+w*2;i++)
	{
		g[i]=(int *)calloc(c+w*2,sizeof(int));
	}
	
	CDF=(int **)calloc(m+1,sizeof(int *));
	for(i=0;i<m+1;i++)
	{
		CDF[i]=(int *)calloc(m+1,sizeof(int));
	}
	
	win=(int*)calloc(Sr*Sr,sizeof(int));
	
	hist=(int**)calloc(m+1,sizeof(int*));
	for(i=0;i<m+1;i++)
	{
		hist[i]=(int *)calloc(m+1,sizeof(int));
	}
	
	for(i=w;i<=r-1+w;i++)
	{
		for(j=w;j<=c-1+w;j++)	//to read the input image
		{
			fscanf(fp[0],"%d",&a[i][j]);
        }
	}
	
	//padding
	
	for(i=0;i<r+w*2;i++)
	{
		for(j=0;j<w;j++)
		{
			a[i][j]=a[i][w];
			g[i][j]=g[i][w];
			a[i][c+w+j]=a[i][c-1+w];
			g[i][c+w+j]=g[i][c-1+w];
		}
	}
	for(j=0;j<c+w*2;j++)
	{
		for(i=0;i<w;i++)
		{
			a[i][j]=a[w][j];
			g[i][j]=g[w][j];
			a[r+w+i][j]=a[r-1+w][j];
			g[r+w+i][j]=g[r-1+w][j];
		}
	}
	
	//average image calculation
	for (i=w; i<=r-1+w; i++) 
	{
        for (j=w; j<=c-1+w; j++)
		{
			x=0;Xpsum=0.0;
			 for(in=-pad;in<=pad;in++)
			 {
			 	for(jn=-pad;jn<=pad;jn++)
				{
			 		win[x]=a[i+in][j+jn];
			 		Xpsum+=win[x];
//			 		printf("%d ",win[x]);	
			 	}
			 }
			 g[i][j]=Xpsum/(float)(Sr*Sr);
		}
	}
	
	int pix = r * c;

//	int hist[m + 1];
	int cdf[m + 1];
	int eh[m + 1][m + 1];
	
	int xi,yi,ix,jx;  //joint histogram
    for (xi = w; xi<=r-1+w; xi++) {
        for (yi = w; yi<=c-1+w; yi++) {
            ix = a[xi][yi];
            jx = g[xi][yi];
            hist[ix][jx]++;
        	}
    	}
	
	int cdfmax = m, cdfmin=INT_MAX;

	for (ix = 0; ix<m+1; ix++) {
		count=0;
        for (jx = 0; jx<m+1; jx++) {
        	count=0;
		    for (xi = 0; xi<ix; xi++) {
		        for (yi = 0; yi<jx; yi++) {
		        	count+=hist[xi][yi];
		        }
		    }
		    CDF[ix][jx]=count;
		    if(CDF[ix][jx]<cdfmin) cdfmin = CDF[ix][jx];
		}
	}
	
    for (xi = 0; xi < m+1; xi++) {
        for (yi = 0; yi < m+1; yi++) {
            eh[xi][yi] = (int)round((((float)m - 1)*(CDF[xi][yi] - cdfmin)) / (pix - 1));
        }
    }
    
    for (i = w; i <=r-1+w; i++)
	{
		for (j = w; j <=c-1+w; j++)
		{
			int orgI=a[i][j];
			b[i-w][j-w] = eh[orgI][orgI];
//			printf("%d ",b[i-w][j-w]);
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
		
    
	fclose(fp[0]);
    fclose(fp[1]);
    free(a);
    free(b);
    free(CDF);
    free(hist);
    free(g);
	return 0;
}

	
