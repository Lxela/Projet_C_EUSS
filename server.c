/***************************************************************************
                          main.c  -  server
                             -------------------
    begin                : lun feb  4 15:30:41 CET 2002
    copyright            : (C) 2002 by A. Moreno
    email                : amoreno@euss.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT_NUM		5001
#define SERVER_MAX_CONNECTIONS	4

#define REQUEST_MSG_SIZE	1024


/************************
*
*
* tcpServidor
*
*
*/



int main(int argc, char *argv[])
{
	struct sockaddr_in	serverAddr;
	struct sockaddr_in	clientAddr;
	int			sockAddrSize;
	int			sFd;
	int			newFd;
	int			nRead;
	int 		result;
	char		buffer[256];
	char		missatge[] = "";
	int maximum = -9999;
	int minimum = -9999;

	/*Preparar l'adreça local*/
	sockAddrSize=sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*Crear un socket*/
	sFd=socket(AF_INET, SOCK_STREAM, 0);
	
	/*Nominalitzar el socket*/
	result = bind(sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	
	/*Crear una cua per les peticions de connexió*/
	result = listen(sFd, SERVER_MAX_CONNECTIONS);
	
	/*Bucle s'acceptació de connexions*/
	while(1){
		printf("\nServidor esperant connexions\n");

		/*Esperar conexió. sFd: socket pare, newFd: socket fill*/
		newFd=accept(sFd, (struct sockaddr *) &clientAddr, &sockAddrSize);
		printf("Connexión acceptada del client: adreça %s, port %d\n",	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		/*Rebre*/
		result = read(newFd, buffer, 256);
		
			
		printf("Missatge rebut del client(bytes %d): %s\n",	result, buffer);
		
		/************************ Initialisation Array **********************/
	int tab[3600],i=0;
	time_t t;
	srand((unsigned) time(&t));
	
	for(i=0; i<3600; i++)
		{
		tab[i] = rand() % 30;
		}
		
		/************************ Average**********************/
	int average;
	int count = 0;
	int sum = 0;
	for (i=0;i<3600; i++){
		if (tab[i]!= NULL){
			sum += tab[i];
			count++;
		}
	}
	average = sum/count;
	
	char average_c;
	average_c = (char)average;

	/******* fin average ************/


		char* mess = malloc(10*sizeof(char));
		
		if(strcmp(buffer,"minimum") == 0) {
			*buffer = '\0';
			*missatge = '\0';

			if (minimum == -9999) {
				minimum = getMinimum(tab);
			}			
			printf("mini = %d\n", minimum);
				
			sprintf(mess, "%d", minimum);
				
			printf("mess = %s\n", mess);
			printf("buffer = %s\n\n", buffer);
			printf("missaje = %s\n\n", missatge);
		}
		else if(strcmp(buffer,"maximum") == 0) {
			*buffer = '\0';
			*missatge = '\0';

			if (maximum == -9999) {
				maximum = getMaximum(tab);
			}			
			printf("maxi = %d\n", maximum);
				
			sprintf(mess, "%d", maximum);
				
			printf("mess = %s\n", mess);
			printf("buffer = %s\n\n", buffer);
			printf("missaje = %s\n\n", missatge);
		}
		else if(strcmp(buffer,"average") == 0) {
			mess = average_c;
		}
		else if(strcmp(buffer,"reset") == 0) {
			/*reset maxi et mini*/
			maximum = -9999;
			minimum = -9999;
		}
		else if(strcmp(buffer,"counter") == 0) {
			/*counter*/
		}
		else if(strcmp(buffer,"start") == 0) {
			/*start acqui*/
		}
							


		/*Enviar*/
		
		strcpy(missatge,mess);
		printf("missaje 1 = %s\n\n", missatge);
		printf("buffer 1 = %s\n\n", buffer);
		strcpy(buffer,missatge); //Copiar missatge a buffer
		printf("missaje 2 = %s\n\n", missatge);
		printf("buffer 2 = %s\n\n", buffer);
		result = write(newFd, buffer, strlen(buffer)+1); //+1 per enviar el 0 final de cadena
		printf("retest\n");

		
		printf("Missatge enviat a client(bytes %d): %s\n\n",	result, missatge);
		
		printf("after\n");

		printf("after missaje = %s\n", missatge);
		/*Tancar el socket fill*/
		result = close(newFd);
	}
}





int getMaximum(int *tab) {
	int ret = -9999;
	int i;
	for (i=0;i<3600; i++){
		if (tab[i]!= NULL){
			if (tab[i] > ret) {
				printf("tab = %d\n", tab[i]);
				ret = tab[i];
			}
		}
	}
	return ret;
}

int getMinimum( int *tab) {
	int ret = 9999;	
	int i;
	for (i=0;i<3600; i++){
		if (tab[i]!= NULL){
			if (tab[i] < ret) {
				printf("tab = %d\n", tab[i]);
				ret = tab[i];
			}
		}
	}
	return ret;
}


