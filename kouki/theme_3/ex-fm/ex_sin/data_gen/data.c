#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
	FILE* fp;

	char data[360];
	unsigned char dest[360];
	int deg = 0;
	double rad = 0;

	fp = fopen("sin.txt","w");

	for(deg=0;deg<180;deg++)
	{
		rad = ((double)deg) * M_PI / 180.0;
		data[deg] = (char)(sin(rad) * 127.0);
		printf("%d\n",data[deg]);
	}

	int i;
	for(i=0,deg=180;deg<360;deg++)
	{
		data[deg] = data[i++]*(-1);
		printf("%d\n",data[deg]);
	}

	fprintf(fp,"unsigned char sin[] = {");

	for(i=0;i<360;i++)
	{
		dest[i] = data[i] + 127;
		printf("dest[%d]=%d\n",i,dest[i]);
		fprintf(fp,"%d,",dest[i]);
	}

	fprintf(fp,"};");

	return 0;
}
