#include<stdio.h>
#include<stdlib.h>
#include"cgic.h"

int cgiMain()
{
//	 cgiHeaderContentType("text/html");
//	fprintf(cgiOut, "");
	system("./downlist.sh");
	fprintf(cgiOut, "end#################3");
	
return 0;
}
