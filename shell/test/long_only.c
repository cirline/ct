#include <stdio.h>  
#include <stdlib.h>  
#include <getopt.h> //getopt_long()头文件位置  

int main(int argc, char** argv)  
{  
	const char *optstring="";  
	int c,deb,index;  
	struct option opts[]={
		{"username",required_argument,0,0},  
		{"n",required_argument,0,0},  
		{"version",no_argument,0,0},  
		{"v",no_argument,0,0},  
		{"debug",no_argument,0,0},  
		{"d",no_argument,0,0},  
		{"help",no_argument,0,0},  
		{"h",no_argument,0,0},
		{0, 0, 0, 0}
	};  
	while((c=getopt_long_only(argc,argv,optstring,opts,&index))!=-1)  
	{  
		printf("index = %d\n", index);
		switch(index){  
			//-n或者--username  
			case 0:  
			case 1:  
				printf("username:%s\n",optarg);  
				break;  
				//-v或者--version  
			case 2:  
			case 3:  
				printf("version:1.0.0\n");  
				break;  
				//-d or --debug  
			case 4:  
			case 5:  
				printf("debug:yes\n");  
				break;  
				//-h or --help  
			case 6:  
			case 7:  
				printf("Help:?\n");  
				break;  
			default:  
				printf("other:%d\n",index);  
				break;  
		}  
	}  
	return 0;  
}  
