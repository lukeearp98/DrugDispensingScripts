#include <stdio.h>
void askForDateOnStartup(void);
void askForLoginDetails();
void checkUserCredentials(void);
void updateRole(void);
void setupInitialMenu(void);
int validateDateString(char *);
int mputchar(unsigned char);
unsigned char mgetchar(void);
void drugDispenseSetupMenu();
void localControllerSetupMenu();
void scheduledDrugEventsMenu();

char controllerName[30] = "";
char drugName[30] = "";
char totalBoosts[30] = "";
char intervalBoostTime[30] = "";
char dosageAmount[10] = "";
char drugEvents[11][8];
int eventsInADay = 10;
int isUserNurseOrAdmin = 0;
char patientID[10] = "";
char password[30] = "";

void main(void)
{
	int configExists = 0;
	
	if(!configExists)
	{
		askForDateOnStartup();
		askForLoginDetails();
	}
	
	setupInitialMenu();
}
void askForDateOnStartup()
{
	char messageIn[37] = "";
	
	printf("Welcome to The Drug System\n");
	printf("-----------------------------");
	
	
	printf("\nWhat is the current date?\nCurrent Time (Day/Month/Year): \n");
	getdata(messageIn, 35);
	printf("\n\nlength: %d", sizeof(messageIn));

	
	printf("\nWhat is the current time?\nCurrent Time (Hour/Minutes/Seconds): \n");
	getdata(messageIn, 35);
	printf("You entered this: %s\n\n", messageIn);
}
void askForLoginDetails()
{
	printf("Login Credentials\n");
	printf("-----------------------------\n");
	
	printf("\nPlease Enter Your Patient/Nurse ID: \n");
	getdata(patientID, 15);
	printf("\nPlease Enter Your Password: \n");
	getdata(password, 30);
	checkUserCredentials();
	updateRole();
	
}
void checkUserCredentials()
{
	
}
void updateRole()
{
	
}
void setupInitialMenu()
{
	char messageIn[37] = "";
	while(1)
	{
		printf("\nMain Menu\n");
		printf("-----------------------------\n");
		printf("1 - Setup Local Controller\n");
		printf("2 - Setup Drug Dispensing\n");
		printf("3 - Toggle Night Mode ON/OFF\n");
		
		printf("\nSelection: \n");
		getdata(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			localControllerSetupMenu();
		}
		else if(atoi(messageIn) == 2)
		{
			drugDispenseSetupMenu();
		}
		else if(atoi(messageIn) == 3)
		{
			printf("\nNight Mode Toggled\n");
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
	
}
int validateDateString(char *dateString)
{
}
int getdata(char *dataString, int size)
{
/* 	
	Author : Mr Luke A Earp
	Company: Staffordshire University
	Date:	11th December 2017
	Function: This retrieves data strings from the keyboard.
	Version: 1.0
	Modifications: None
	Functions used: 
	mygetchar - returns chars to this function to complete string.
	mputchar - inserts a char
	Key Variables: 
	*dataString  - string inputted
	nCharCount - allows for filling neccesary amount of spaces and prints correct amount of new characters in the string which has been backspaced
	singleChar - the individual char which has been pressed
	
*/

	char *string,singleChar;
	int nCount, nCharCount;
	
	nCharCount = 0;
	string = dataString;

	for (nCount = 0;nCount < size; nCount++)
	{
		if ((singleChar = mgetchar()) == EOF)
		{
			return -1; /* if reaches end of buffer end return null */
		}
		if (singleChar == '\n')
		{
			break; /* break for loop on new line press */
		}
		if(singleChar == 0x08 && nCharCount > 0) /* checks for backspace and more than 0 key entries. */
		{
			string--;
			nCharCount--;
			nCount--;
			*string = ' ';
			mputchar('\r');
			nCount = 0;
			while(nCount <= nCharCount) 
			{
				mputchar(' '); /* fills current amount of chars with blank spaces. */
				nCount++;
			}
			
			mputchar('\r'); /* Puts back to start of line so that new string can overwrite */
			nCount = 0;
			while(nCount < nCharCount)
			{
				mputchar(dataString[nCount]); /* Overwrites spaces with the new string */
				nCount++;
			}	
			
		}
		else if(nCharCount < size)
		{
			*string = singleChar; /* the char is set to current location of string. */	
			string++; /* Increases location so it moves pointer forward by 1 */
			nCharCount++;
		}	
	}
	if (dataString == string)
	{
		return -1;	
	}
	
	*string = '\0'; /* puts null terminator at end of string */
	return 0;
}
void localControllerSetupMenu()
{
	char messageIn[37] = "";
	while(1)
	{
		printf("\nController Settings\n");
		printf("-----------------------------\n");
		printf("1 - Set Controller Name - %s\n", controllerName);
	    printf("2 - Set Patient ID - %s\n", patientID);
		printf("3 - Back\n");
			
		printf("\nSelection: \n");
		getdata(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			printf("\nController Name: \n");
			printf("------------------------------\n");
			getdata(controllerName, 30);
		}
		else if(atoi(messageIn) == 2)
		{
			printf("\nPatient ID: \n");
			printf("------------------------------\n");
			getdata(patientID, 5);
		}
		else if(atoi(messageIn) == 3)
		{
			setupInitialMenu();
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
	
}
void drugDispenseSetupMenu()
{
	char messageIn[37] = "";
	char tempEventsInDay[5] = "";
	while(1)
	{
		printf("\nDrug Settings\n");
		printf("-----------------------------\n");
		printf("1 - Set Drug Name - %s\n", drugName);
		printf("2 - Set Dosage Amount - %s\n", dosageAmount);
		printf("3 - Amount of Scheduled Administrations Allowed - %d\n", eventsInADay); 
		printf("4 - Scheduled Drug Administrations\n");
		printf("5 - Boost Settings\n");
	 	printf("6 - Back\n");
	 	
		printf("\nSelection: \n");
		getdata(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			printf("\nDrug Name: \n");
			printf("------------------------------\n");
			getdata(drugName, 30);
		}
		else if(atoi(messageIn) == 2)
		{
			printf("\nDosage Amount: \n");
			printf("------------------------------\n");
			getdata(dosageAmount, 30);
		}
		else if(atoi(messageIn) == 3)
		{
			printf("\nAmount of Scheduled Administrations Allowed: \n");
			printf("------------------------------\n");
			getdata(tempEventsInDay, 30);
			(atoi(tempEventsInDay) > 10) ? (eventsInADay = 10) : (eventsInADay = atoi(tempEventsInDay));		
		}
		else if(atoi(messageIn) == 4)
		{
			scheduledDrugEventsMenu();
		}
		else if(atoi(messageIn) == 5)
		{
			boostSettingMenu();
		}
		else if(atoi(messageIn) == 6)
		{
			setupInitialMenu();
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
}
void boostSettingMenu()
{
	char messageIn[37] = "";
	int boostsToday = 0;
	while(1)
	{
		printf("\nBoost Settings\n");
		printf("-----------------------------\n");
		printf("1 - Set Total Boosts - %s\n", totalBoosts);
		printf("2 - Set Interval Between Boosts - %s\n", intervalBoostTime);
	 	printf("3 - Back\n");
	 	
	 	printf("\nBoosts Used Today: %d\n\n", boostsToday);
	 	
		printf("\nSelection: \n");
		getdata(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			printf("\nTotal Boosts Available: \n");
			printf("------------------------------\n");
			getdata(totalBoosts, 30);
		}
		else if(atoi(messageIn) == 2)
		{
			printf("\nBoost Interval (HH:MM:SS): \n");
			printf("------------------------------\n");
			getdata(intervalBoostTime, 30);
		}
		else if(atoi(messageIn) == 3)
		{
			drugDispenseSetupMenu();
		}

		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
	
}
void scheduledDrugEventsMenu()
{
	char messageIn[37] = "";
	char currentDrugEvent[10] = "";
	int nCount = 0;
	while(1)
	{
		printf("\nDrug Scheduler Menu\n");
		printf("------------------------------\n");
		for(nCount = 1; nCount <= eventsInADay; nCount++)
		{
			printf("%d - Drug Event (%s)\n", nCount, drugEvents[nCount]);
		} 

	 	printf("%d - Back\n", nCount);
		printf("\nSelection: \n");
		getdata(messageIn, 5);
		if(atoi(messageIn) <= eventsInADay)
		{
			printf("Enter Time of Event(Hour:Minutes:Seconds):\n");
			getdata(currentDrugEvent, 10);
			for(nCount = 0; nCount <= strlen(currentDrugEvent); nCount++)
			{
				drugEvents[atoi(messageIn)][nCount] = currentDrugEvent[nCount];
			}
		}
		else if(atoi(messageIn) == nCount)
		{
			drugDispenseSetupMenu();
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
		
	}
}
unsigned char mgetchar(void)
{
/* Author: Luke Earp
Company: Staffordshire University
Functions used: mputchar
Purpose: To create my own getchar
Version: 1.0
Modifications: None
*/
	unsigned char *scsr, *scdr, data,data1;
	/* baud rate already defined in monitor program*/
	scsr = (unsigned char *)0x2e;
	scdr = (unsigned char *)0x2f;
	/* Loop till bit 5 is '1' indicating free to send more data */
	while (((*scsr) & 0x20) == 0x0);
	data = *scdr;
	mputchar(data);
	if(data == 0xd)
	{
		mputchar('\n');
		mputchar('\r');
		data = '\n';
	}
	
	
	/* return  data out  */
	return(data);
}
int mputchar(unsigned char data)
{
/* Author: Luke Earp
Company: Staffordshire University
Functions used: None
Purpose: To create my own putchar
Version: 1.0
Modifications: None
*/
	unsigned char *scsr, *scdr;
	/* baud rate already defined in monitor program*/
	scsr = (unsigned char *)0x2e;
	scdr = (unsigned char *)0x2f;
	/* Loop till bit 7 is '1' indicating free to send more data */
	while (((*scsr) & 0x80) == 0x0);
	/* Transfer data out  */
	*scdr = data;
	return(0);
}
