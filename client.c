#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


#define REQUEST_MSG_SIZE	1024
#define REPLY_MSG_SIZE		500
#define SERVER_PORT_NUM		5001

/************************
*
*
* ImprimirMenu
*
*
*/
void ImprimirMenu(void)
{
	printf("\n\nMenu:\n");
	printf("--------------------\n");
	printf("1: Show minimum\n");
	printf("2: Show maximum\n");
	printf("3: Show average\n");
	printf("4: Reset maximum and minimum\n");
	printf("5: Show counter\n");
	printf("6: Start acquisition\n");
	printf("s: Exit\n");
	printf("--------------------\n");
}

void connection_server(char *message) {
	struct sockaddr_in serverAddr;
	char serverName[] = "127.0.0.1"; //Adreça IP on est� el servidor
	int sockAddrSize;
	int sFd;
	int mlen;
	int result;
	char buffer[256];
	char *missatge = message;

	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(serverName);

	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error when establishing the connection at address %s, port %d\n");
		exit(-1);
	}
	//printf("\nConnection with the server established: address %s, port %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	/*Enviar*/
	strcpy(buffer,missatge); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Message sent to the server(bytes %d): %s\n", result, missatge);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	printf("The server answered (bytes %d): %s\n", result, buffer);

	/*Tancar el socket*/
	close(sFd);
}


/************************
*
*
* main
*
*
*/
int main(int argc, char **argv)                                                               
{                                                                          
	char input;

	ImprimirMenu();                             
	input = getchar();

	while (input != 's')
	{
		switch (input)
		{
			case '1':
				printf("menu 1\n");
				connection_server("minimum");	
				ImprimirMenu();                             
				break;
			case '2':
				printf("menu 2\n");
				connection_server("maximum");	
				ImprimirMenu();                             
				break;
			case '3':
				printf("menu 3\n");
				connection_server("average");	
				ImprimirMenu();                             
				break;
			case '4':
				printf("menu 4\n");
				connection_server("reset");	
				ImprimirMenu();                             
				break;
			case '5':
				printf("menu 5\n");
				connection_server("counter");	
				ImprimirMenu();                             
				break;
			case '6':
				printf("menu 6\n");
				connection_server("start");	
				ImprimirMenu();                             
				break;
			case 0x0a:
				break;
			default:
				printf("Wrong option\n");	
				printf("You entered 0x%hhx \n",input);
				ImprimirMenu();
				break;
		}

		input = getchar();

	}
	
	return 0;
}



