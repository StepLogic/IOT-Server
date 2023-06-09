/*
 * http_ssi.c
 *
 *  Created on: 11-Oct-2021
 *      Author: controllerstech
 */


#include"http_ssi.h"
#include "string.h"
#include "stdio.h"

#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"

#include "stm32f7xx_hal.h"

int indx = 0;
/* we will use character "x", "y","z" as tag for SSI */
char const* TAGCHAR[]={"x", "y", "z"};
char const** TAGS=TAGCHAR;

uint16_t ssi_handler (int iIndex, char *pcInsert, int iInsertLen)
{
	switch (iIndex) {
		case 0:
			indx+=1;
			sprintf(pcInsert, "%d", indx);
			return strlen(pcInsert);
			break;
		case 1:
			indx+=1;
			sprintf(pcInsert, "%d", indx);
			return strlen(pcInsert);
			break;
		case 2:
			indx+=1;
			sprintf(pcInsert, "%d", indx);
			return strlen(pcInsert);
			break;
		default :
			break;
	}

	return 0;
}

/************************ CGI HANDLER ***************************/
const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

const tCGI FORM_CGI = {"/form.cgi", CGIForm_Handler};
const tCGI LED_CGI = {"/led.cgi", CGILED_Handler};

char name[30];

tCGI CGI_TAB[2];

const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 0)
	{
		for (int i=0; i<iNumParams; i++)
		{
                       char led1[50];
                       char led2[50];
			if (strcmp(pcParam[i], "led1") == 0)
      // if the led1 string is found
			{
				memset(led1, '\0', 30);
				strcpy(led1, pcValue[i]);
                                if(strcmp(led1, "true")==0){
 /* Switch on the corresponding led in this case GPIO_PIN_4 is led1 */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);}else{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);}
			}

			else if (strcmp(pcParam[i], "led2") == 0) 
                               // if the led2 string is found
			{
				strcat(led2, " ");
				strcat(led2, pcValue[i]);
                                 if(strcmp(led2,"true")==0)  if(strcmp(led1, "true")==0){
/* Switch on the corresponding led in this case GPIO_PIN_5 is led2 */

HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);}else{
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);}
			}
		}
	}

	return "/cgiform.html";}

const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 1)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "fname") == 0)  // if the fname string is found
			{
				memset(name, '\0', 30);
				strcpy(name, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "lname") == 0)  // if the fname string is found
			{
				strcat(name, " ");
				strcat(name, pcValue[i]);
			}
		}
	}

	return "/cgiled.html";
}

void http_server_init (void)
{
	httpd_init();

	http_set_ssi_handler(ssi_handler, (char const**) TAGS, 3);

	CGI_TAB[0] = FORM_CGI;
	CGI_TAB[1] = LED_CGI;

//	http_set_cgi_handlers (&FORM_CGI, 1);
	http_set_cgi_handlers (CGI_TAB, 2);
}
