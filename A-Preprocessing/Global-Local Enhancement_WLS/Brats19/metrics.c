#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
	int i,j,r,c,m,l,x,y,z,p,q;	
	char str[25];
	float rmse,psnr,ief,ssim,k=0.0,num=0.0,denom=0.0;
	float mx=0.0,my=0.0,sx=0.0,sy=0.0,cxy=0.0,b1=pow(0.001*255,2),b2=pow(0.03*255,2);
	
	FILE *a = fopen("low_y183_GLIE.pgm","rb");		// enhanced
	FILE *b = fopen("low_y183.pgm","rb");		// noisy
	FILE *orig = fopen("y183.pgm","rb");		// original
	
	fscanf(a,"%s %d %d %d",str,&c,&r,&m);
	fscanf(b,"%s %d %d %d",str,&c,&r,&m);	
	fscanf(orig,"%s %d %d %d",str,&c,&r,&m);
	
	p=ftell(a);			// keeping track to come back 
	q=ftell(orig);
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{	
			fscanf(orig,"%d",&x);
			fscanf(a,"%d",&y);
			fscanf(b,"%d",&z);
			k+=pow(x-y,2);
			num+=pow(x-z,2);
			denom+=pow(x-y,2);
			mx+=x;
			my+=y;
		}
	}
	

	rmse=sqrt(k/(r*c));
	printf("\nRMSE: %0.4f ",rmse);
	
	psnr=20.0*log10(255.0/rmse);
	printf("\nPSNR: %0.4f ",psnr);
	
	ief=num/denom;
	printf("\nIEF: %0.6f ",ief);
	
	
	fseek(a,p,0);
	fseek(orig,q,0);
	
	mx = mx/(r*c);			// mean
	my = my/(r*c);
	
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			fscanf(orig,"%d",&x);
			fscanf(a,"%d",&y);
			sx+=pow(x-mx,2);				// standard deviation
			sy+=pow(y-my,2);				
			cxy+=fabs((x-mx)*(y-my));		// covariance
		}
	}
	sx=sqrt(sx/(r*c));
	sy=sqrt(sy/(r*c));
	cxy=cxy/(r*c);
	
	num=(2*mx*my + b1) + (2*cxy + b2);
	denom=(mx*mx + my*my + b1) + (sx*sx + sy*sy + b2);
	ssim=num/denom;
	
	printf("\nSSIM : %0.4f",ssim);
	
	fclose(a);
	fclose(b);
	fclose(orig);
	
	return 0;
}
