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
	char		missatge[256];
	//int tab[3600];
	int counter = 0;
	int maximum = -9999;
	int minimum = -9999;
	char acquisition[10] = "stop";

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
		printf("\nServer waiting for connections\n");

		/*Esperar conexió. sFd: socket pare, newFd: socket fill*/
		newFd=accept(sFd, (struct sockaddr *) &clientAddr, &sockAddrSize);
		printf("Connection with the client accepted: address %s, port %d\n",	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		/*Rebre*/
		result = read(newFd, buffer, 256);
		
			
		printf("Message received from the client (bytes %d): %s\n",	result, buffer);
		
		/************************ Initialisation Array **********************/
	int tab[3600],i=0;
	time_t t;
	srand((unsigned) time(&t));
	counter = 0;
	for(i=0; i<3600; i++){
		tab[i] = rand() % 30;
		counter++;
	}
	/********* HANDLE THE DATAS **************/

		char* mess = malloc(10*sizeof(char));
		char* acqu_state = malloc(10*sizeof(char));
		acqu_state = "stop"; //initialisation
		
		//to get the minimum temperature
		if(strcmp(buffer,"minimum") == 0) {
			memset(buffer, '\0', sizeof(buffer));
			memset(missatge, '\0', sizeof(missatge));

			if (minimum == -9999) {
				minimum = getMinimum(tab);
			}			
			printf("mini = %d\n", minimum);
			sprintf(mess, "%d", minimum);
			printf("mess = %s\n", mess);
		}
		//to get the maximum temperature
		else if(strcmp(buffer,"maximum") == 0) {
			memset(buffer, '\0', sizeof(buffer));
			memset(missatge, '\0', sizeof(missatge));

			if (maximum == -9999) {
				maximum = getMaximum(tab);
			}			
			printf("maxi = %d\n", maximum);
			sprintf(mess, "%d", maximum);
			printf("mess = %s\n", mess);
		}
		//to get the average temperature
		else if(strcmp(buffer,"average") == 0) {
			memset(buffer, '\0', sizeof(buffer));
			memset(missatge, '\0', sizeof(missatge));

			int avg = getAverage(tab);
			
			printf("avg = %d\n", avg);
			sprintf(mess, "%d", avg);
			printf("mess = %s\n", mess);			
		}
		//to reset the max and mini temperature
		else if(strcmp(buffer,"reset") == 0) {
			/*reset maxi et mini*/
			maximum = -9999;
			minimum = -9999;
		}
		//to get the number of temperature in the array
		else if(strcmp(buffer,"counter") == 0) {
			/*counter*/
			memset(buffer, '\0', sizeof(buffer));
			memset(missatge, '\0', sizeof(missatge));
			//counter = strlen(tab);

			printf("counter = %d\n", counter);	
			sprintf(mess, "%d", counter);
				
			printf("mess = %s\n", mess);
		}
		//to start or stop the acquisition of temperature
		else {
			/*start acqui*/
			//tab = acquisition(&counter);
			
			//to get the differents datas from the message
			char subbuf[10];
			memcpy(subbuf, &buffer, 2);
			subbuf[2] = '\0';
			
			char subbuf2[10];
			memcpy(subbuf2, &buffer[3], 1);
			subbuf2[1] = '\0';
			
			char subbuf3[10];
			memcpy(subbuf3, &buffer[5],1);
			subbuf3[1] = '\0';
			
			
			printf("acquisition = %s\n", acquisition);

			//to start the acquisition of the temperature
			if (strcmp(subbuf3,"1") == 0) {
				printf("\nAcquisition started witch %s seconds of interval.\n",subbuf);
				printf("We calculate median with %s data\n\n. ",subbuf2);
				strcpy(mess,"start");
				strcpy(acquisition,"start");
				printf("mess = %s\n", mess);
			}
			//stop the acquisition of the temperature
			else {
				strcpy(mess,"stop");
				strcpy(acquisition,"stop");
				printf("mess = %s\n", mess);
			}
			printf("acquisition = %s\n", acquisition);
		}
							


		/*Enviar*/
		
		strcpy(missatge,mess);
		memset(buffer, '\0', 256);
		strcpy(buffer,missatge); //Copiar missatge a buffer
		result = write(newFd, buffer, strlen(buffer)+1); //+1 per enviar el 0 final de cadena
		
		printf("Message sent to the client (bytes %d): %s\n\n",	result, missatge);
		
		/*Tancar el socket fill*/
		result = close(newFd);
	}
}


//function not used for the moment
acquisition(int *counter) {
	int tab[3600],i=0;
	time_t t;
	srand((unsigned) time(&t));
	
	for(i=0; i<3600; i++){
		tab[i] = rand() % 30;
		counter++;
	}
}

//function to get the maximum
int getMaximum(int *tab) {
	int ret = -9999;
	int i;
	for (i=0;i<3600; i++){
		if (tab[i]!= NULL){
			if (tab[i] > ret) {
				ret = tab[i];
			}
		}
	}
	return ret;
}

//function to get the minimum
int getMinimum( int *tab) {
	int ret = 9999;	
	int i;
	for (i=0;i<3600; i++){
		if (tab[i]!= NULL){
			if (tab[i] < ret) {
				ret = tab[i];
			}
		}
	}
	return ret;
}

//to get the average of the temperature
int getAverage(int *tab) {
	int ret;
	int count = 0;
	int sum = 0;
	int i;
	
	for (i=0;i<3600; i++){
		if (tab[i]!= NULL){
			sum += tab[i];
			count++;
		}
	}
	ret = sum/count;
	printf("average = %d\n", ret);
	
	return ret;
}


//to analyse the message with the protocol, not used yet, misunderstood from the first task
void analyse_msg(char *msg, char *answer) {
	char order = msg[1];
	int n1, n2, n3, verif;

	switch(order) {
		case 'S':
			strcat(answer,"S");
			n1 = msg[2]-'0';
			n2 = msg[3]-'0';
			n3 = msg[4]-'0';
			
			if(n1 <= 9 && n1 >= 0) {
				if(n2 <= 9 && n2 >= 0) {
					if (n3 == 0 || n3 == 1) {
						verif = 0;
					}else {	verif = 1; }
						if (strlen(msg) == 6){
							verif = 0;
						}else { verif = 1;}
				}else { verif = 1;}
			}else {	verif = 1; }
			
			
			
			if(verif == 1) {
				strcat(answer,"1");
			}else {strcat(answer,"0");}
			break;
		case 'E':
			strcat(answer,"E");
			n1 = msg[2]-'0';
			n2 = msg[3]-'0';

			
			if(n1 <= 9 && n1 >= 0) {
				if(n2 <= 9 && n2 >= 0) {
					if (strlen(msg) == 5){
						verif = 0;
					}else { verif = 1;}
				}else { verif = 1;}
			}else {	verif = 1; }

			if(verif == 1) {
				strcat(answer,"1");
			}else {strcat(answer,"0");}

			strcat(answer,"0");
			
			break;
		case 'C':
			strcat(answer,"C");
			if (strlen(msg) == 3) {
				strcat(answer,"0");
				strcat(answer,"0101");
			}else { 
				strcat(answer,"1"); 
				strcat(answer,"0101");
			}

			break;
		case 'M':
			strcat(answer,"M");
			if(strlen(msg) == 4) {
				if((msg[2]-'0') == 0 || (msg[2]-'0') == 1) {
					verif = 0;
				}
				else {verif = 1;}
			}else {verif = 1; }
			
			if (verif == 0) {
				strcat(answer,"0");
			}else { strcat(answer,"1");}

			break;
		default:
			printf("invalid order\n");
			strcat(answer,"2");
	}
	strcat(answer,"Z");
}



