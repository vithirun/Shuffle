#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>

int main(int argc, char *argv[])
{
	int opt;
	char *input_file;
	char *output_file;
	if(argc !=5)
	{
		fprintf(stderr,"Usage: shuffle -i inputfile -o outputfile\n"); 
		exit(1);
	}
	if(((strcmp(argv[1],"-i")!=0)|| (strcmp(argv[3],"-o")!=0))&& ((strcmp(argv[1],"-o")!=0)|| (strcmp(argv[3],"-i")!=0)))
	{
		fprintf(stderr,"Usage: shuffle -i inputfile -o outputfile\n");
		exit(1);
	}
	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
		switch (opt) {
			case 'i':
				input_file=optarg;
				break;
			case 'o':
				output_file=optarg;
				break;
		}
	}
	FILE *ptr;
	ptr=fopen(input_file,"r");
	if(!ptr)
	{
		fprintf(stderr,"Error: Cannot open file %s\n",input_file);
		exit(1);
	}
	struct stat filestat;
	char *rbuf;
	int j=0,k=0,m=0;
	int f_d=fileno(ptr);
	fstat(f_d,&filestat);
	rbuf=malloc(filestat.st_size);
	char *try=rbuf;
	int hh=filestat.st_size;
	int first_index=0;
	int last_index=hh-1;
	char temp='\n';
	char c;
	if(ptr)
	{
		long int rvalue=fread(rbuf,1,filestat.st_size,ptr);
		if(rvalue != filestat.st_size)
		{
			fprintf(stderr,"Error : Not fully read\n");
			fclose(ptr);
			exit(1);
		}
	}
	ptr=fopen(output_file,"w");
	while(first_index<last_index)
	{
		for(j=first_index;try[j]!='\n';j++)
		{
			c=try[j];
			fwrite(&c,1,1,ptr);
		}
		fwrite(&temp,1,1,ptr);
		first_index=j+1;
		if(first_index>last_index)
			break;
		if(try[last_index]=='\n')
		{
			last_index--;
		}		
		for(k=last_index;try[k]!='\n';k--);
		for(m=k+1;m<=last_index;m++)
		{
			c=try[m];
			fwrite(&c,1,1,ptr);
		}
		fwrite(&temp,1,1,ptr);
		last_index=k;
	}
	free(rbuf);
	fclose(ptr);
	exit(0);
}
