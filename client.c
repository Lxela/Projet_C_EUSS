#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
				ImprimirMenu();                             
				break;
			case '2':
				printf("menu 2\n");	
				ImprimirMenu();                             
				break;
			case '3':
				printf("menu 3\n");	
				ImprimirMenu();                             
				break;
			case '4':
				printf("menu 4\n");	
				ImprimirMenu();                             
				break;
			case '5':
				printf("menu 5\n");	
				ImprimirMenu();                             
				break;
			case '6':
				printf("menu 6\n");	
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



