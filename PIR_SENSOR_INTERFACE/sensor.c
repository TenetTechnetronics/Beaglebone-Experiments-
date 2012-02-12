#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char set_value[8];
char buffer[100];
int i=0;
FILE *fp;

void mux(char gpmc[8]);
void export(char gpio[8]);
void direction(char gpio[8],char val[8]);
void value(char gpio[8],char val[8]);
void unexport(char gpio[8]);

int main()
{
	
	int pirstate=0;  //Initial state of the PIR sensor
	char st[8];
	mux("2");
	mux("3");

	export("38");  //LED // Export the PINs so that they could be used 
	export("34");  //BUZZER
	export("35");  // PIR Sensor
	
	direction("38","out"); // Configure the PIn mode (output/input)
	direction("35","out");
	direction("34","in");

	while(1)
	{
		if((fp=fopen("/sys/class/gpio/gpio34/value","rb+"))==NULL)
		{
			printf("cannot open file\n");
			exit(1);
		}

		if (fp)
		{
			fgets(st,sizeof(st),fp);
			i=atoi(st);
			// printf("The value is: %d\n",i);  // Provided just in case u need to debug
		}
		else
		{
			printf("cannot get the value");
		}
	
  		fclose(fp);
        

		if(i==1)
		{
        
			if(pirstate==0)
			{
				printf("Motion Detected\n");
				pirstate=1;
				// Glow LED
				value("38","1");
				//Buzzer ON
				value("35","1");
			}
	
		}
        	else
        	{
			if(pirstate==1)
			{
				printf("Motion Ended\n");
				pirstate=0;
				// LED OFF
				value("38","0");
				//Buzzer OFF
				value("35","0");
			
			}
        	}
     }
	
	unexport("38");
	unexport("34");
	unexport("35");
     
	return 0;
}

void mux(char gpmc[8])
{
	i=atoi(gpmc);
	if(sprintf(buffer, "/sys/kernel/debug/omap_mux/gpmc_ad%d", i) < 0)	 
       	{
           	perror("convertion error.\n");
          	exit(1);
      	}

 	if ((fp = fopen(buffer, "r+")) == NULL)
       	{
           	printf("Cannot open direction file.\n");
           	exit(1);
       	}
       	rewind(fp);
       	strcpy(set_value,"37");
       	fwrite(&set_value, sizeof(char), 3, fp);
       	fclose(fp);
       	printf("Mux set to %s\n",gpmc);
	
}
void export(char gpio[8])
{
	if ((fp = fopen("/sys/class/gpio/export", "ab")) == NULL)
	 {
	 	printf("Cannot open export file.\n");
                exit(1);
         }
 	 rewind(fp);
         strcpy(set_value,gpio);
         fwrite(&set_value, sizeof(char), 3, fp);
 	 fclose(fp);
 	 printf("...export file accessed, new pin now accessible\n");
}

void direction(char gpio[8],char val[8])
{
	if ((fp = fopen("/sys/class/gpio/export", "ab")) == NULL)
	 {
	 	printf("Cannot open export file.\n");
                exit(1);
         }
 	rewind(fp);
	i=atoi(gpio);
	fclose(fp);
	if(sprintf(buffer, "/sys/class/gpio/gpio%d/direction", i) < 0)	 
       	{
           	perror("convertion error.\n");
          	exit(1);
      	}

 	if ((fp = fopen(buffer, "r+")) == NULL)
       	{
           	printf("Cannot open direction file.\n");
           	exit(1);
       	}
       	rewind(fp);
       	strcpy(set_value,val);
       	fwrite(&set_value, sizeof(char), 3, fp);
       	fclose(fp);
       	printf("...direction set\n");
 
}
void value(char gpio[8],char val[8])
{
	
	if ((fp = fopen("/sys/class/gpio/export", "ab")) == NULL)
	 {
	 	printf("Cannot open export file.\n");
                exit(1);
         }
 	rewind(fp);
	i=atoi(gpio);
	fclose(fp);
	if(sprintf(buffer, "/sys/class/gpio/gpio%d/value", i) < 0)	 
       	{
           	perror("convertion error.\n");
          	exit(1);
      	}

 	if ((fp = fopen(buffer, "r+")) == NULL)
       	{
           	printf("Cannot open direction file.\n");
           	exit(1);
       	}
       	rewind(fp);
       	strcpy(set_value,val);
       	fwrite(&set_value, sizeof(char), 3, fp);
       	fclose(fp);
       	printf("...value set \n");
 
}
void unexport(char gpio[8])
{
	if ((fp = fopen("/sys/class/gpio/unexport", "ab")) == NULL)
	 {
	 	printf("Cannot open export file.\n");
                exit(1);
         }
 	 rewind(fp);
         strcpy(set_value,gpio);
         fwrite(&set_value, sizeof(char), 3, fp);
 	 fclose(fp);
 	 printf("...unexport file accessed, new pin now unexported\n");
}

