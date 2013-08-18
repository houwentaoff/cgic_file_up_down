/* Change this if the SERVER_NAME environment variable does not report
	the true name of your web server. */
#if 1
#define SERVER_NAME cgiServerName
#endif
#if 0
#define SERVER_NAME "www.boutell.com"
#endif

/* You may need to change this, particularly under Windows;
	it is a reasonable guess as to an acceptable place to
	store a saved environment in order to test that feature. 
	If that feature is not important to you, you needn't
	concern yourself with this. */

#ifdef WIN32
#define SAVED_ENVIRONMENT "c:\\cgicsave.env"
#else
#define SAVED_ENVIRONMENT "/tmp/cgicsave.env"
#endif /* WIN32 */

#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void HandleSubmit();
void ShowForm();
void File();

char *correctName(char *p)
{
	char *delimPos = NULL;

	delimPos = strtok(p, "\\");

	while(NULL != (delimPos = strtok(NULL, "\\"))){
		p = delimPos;
	}
	return p;
}

//#define CGICDEBUG
int cgiMain() {
#ifdef DEBUG
	LoadEnvironment();
#endif /* DEBUG */
	char command[100];
	char name[128];
	/* Send the content type, letting the browser know this is HTML */
	cgiHeaderContentType("text/html");
	/* Top of the page */
	fprintf(cgiOut, "<HTML><HEAD>\n");
	fprintf(cgiOut, "<meta charset=\"utf-8\">\n");
	fprintf(cgiOut, "<TITLE>UPLOAD PAGE</TITLE></HEAD>\n");
	fprintf(cgiOut, "<BODY><center><H1>UPLOAD</H1></center>\n");
	/* If a submit button has already been clicked, act on the 
		submission of the form. */
	if ((cgiFormSubmitClicked("uploadcgic") == cgiFormSuccess) /*||
		cgiFormSubmitClicked("saveenvironment") == cgiFormSuccess*/)
	{
#ifdef MYADDED		
			//get file name 
		if (cgiFormFileName("file", name, sizeof(name)) != cgiFormSuccess) 
		{
			printf("<p>No file was uploaded.<p>\n");
			return;
		} 
		strcat(command, "checkfname.sh ");
		strcat(command, name);
		strcat(command, "../");
		if (EXIST == system(command))//filename exist
		{
			fprintf(cgiOut, "");//
		}
#endif
		HandleSubmit();
		fprintf(cgiOut, "<hr>\n");
	}
	/* Now show the form */
	ShowForm();
	/* Finish up the page */
	fprintf(cgiOut, "</BODY></HTML>\n");
	return 0;
}

void HandleSubmit()
{
	File();
}
	
void File()
{
	cgiFilePtr file;
	char name[1024];
	char contentType[1024];
	char buffer[1024];
	int size;
	int got;
	char nameOnServer[1024]="../uploads/";
	char *correctedName = NULL;
	/*原以为与操作系统有关，后来查了相关资料 才知道是 浏览器的安全权限 ie的太低，会显示绝对路径,其他的不会*/
	int fd;

	if (cgiFormFileName("file", name, sizeof(name)) != cgiFormSuccess) {
		printf("<p>No file was uploaded.<p>\n");
		return;
	} 
	fprintf(cgiOut, "The filename submitted was: ");

	/*处理操作系统或者由于浏览器引起的上传的文件的绝对路径 引起的上传后的名字问题*/
	//correct the name of file
	correctedName = correctName(name);

	cgiHtmlEscape(correctedName);

	fprintf(cgiOut, "<p>\n");
	cgiFormFileSize("file", &size);
	fprintf(cgiOut, "The file size was: %.2f k<p>\n", (size*1.0)/1024);
	cgiFormFileContentType("file", contentType, sizeof(contentType));
	fprintf(cgiOut, "The alleged content type of the file was: ");
	cgiHtmlEscape(contentType);
	fprintf(cgiOut, "<p>\n");
	fprintf(cgiOut, "Of course, this is only the claim the browser made when uploading the file. Much like the filename, it cannot be trusted.<p>\n");
	if (cgiFormFileOpen("file", &file) != cgiFormSuccess) {
		fprintf(cgiOut, "Could not open the file.<p>\n");
		return;
	}
	strcat(nameOnServer, correctedName);

	if((fd = open(nameOnServer, O_RDWR|O_CREAT|O_APPEND))<0){
		fprintf(cgiOut, "could not creat the new file on the server,%s\n", nameOnServer);
		return;
	}
	fprintf(cgiOut, "<pre>\n");
	while (cgiFormFileRead(file, buffer, sizeof(buffer), &got) ==
		cgiFormSuccess)
	{
		write(fd, buffer, got);
	//	cgiHtmlEscapeData(buffer, got);//display the buffer
	}
	fprintf(cgiOut, "</pre>\n");
	close(fd);
	cgiFormFileClose(file);
}

void ShowForm()
{
	fprintf(cgiOut, "<!-- 2.0: multipart/form-data is required for file uploads. -->");
	fprintf(cgiOut, "<form method=\"POST\" enctype=\"multipart/form-data\" ");
	fprintf(cgiOut, "	action=\"");
#ifndef CGICDEBUG
	cgiValueEscape(cgiScriptName);
#else
//	fprintf(cgiOut, "capture");
#endif	
	fprintf(cgiOut, "\">\n");
	fprintf(cgiOut,"<table width=\"100%%\" height=\"100%%\" cellpadding=\"0\" cellspacing=\"0\">"
	"<tr>"
	"<td valign=\"middle\" align=\"center\">"
	 );


	fprintf(cgiOut, "<p>File Upload:\n");
	//多个文件上传
	fprintf(cgiOut, "<input type=\"file\" name=\"file\"  value=\"\"> (Select A Local File)\n");
	fprintf(cgiOut, "<p>\n");
	fprintf(cgiOut, "<input type=\"submit\" name=\"uploadcgic\" value=\"Submit Request\">\n");
	fprintf(cgiOut, "<input type=\"reset\" value=\"Reset Request\">\n");
	fprintf(cgiOut, "</form>\n");

	fprintf(cgiOut, "</td>\n</tr></table>");

}
