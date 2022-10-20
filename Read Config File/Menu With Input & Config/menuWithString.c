#include <stdio.h>
void setupDevice(void);
void setupInitialMenu(void);
int checkConfigExists(const char *);
void readConfig(void);
int mputchar(unsigned char);
unsigned char mgetchar(void);
void drugDispenseSetupMenu();
void localControllerSetupMenu();
void scheduledDrugEventsMenu();

char controllerName[30] = "";
char patientID[5] = "";
char drugName[30] = "";
char dosageAmount[10] = "";
char drugEvents[11][8];

void main(void)
{
	if(checkConfigExists("testCSV.csv"))
	{
		readConfig();
	}
	else
	{
		setupDevice();
	}
	setupInitialMenu();
}
int checkConfigExists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
void readConfig()
{
	FILE *fp = fopen("testCSV.csv", "r");

    if (!fp) 
	{
        printf("Can't open file\n");
        return 0;
    }

    char buf[1024];
    int row_count = 0;
    int field_count = 0;
    while (fgets(buf, 1024, fp)) 
	{
        field_count = 0;
        row_count++;

        if (row_count == 1) 
		{
            continue;
        }

        char *field = strtok(buf, ",");
        while (field) {
            if (field_count == 0) {
                printf("Date: ");
            }
            if (field_count == 1) {
                printf("Time: ");
            }
            if (field_count == 2) {
                printf("Controller Name: ");
            }
            if (field_count == 3) {
                printf("Patient ID: ");
            }
            if (field_count == 4) {
                printf("Drug Name: ");
            }
            if (field_count == 5) {
                printf("Dosage Amount: ");
            }

            printf("%s\n", field);
            field = strtok(NULL, ",");

            field_count++;
        }
        printf("\n");
    }

    fclose(fp);
}
void setupDevice()
{
	char messageIn[37] = "";
	
	printf("Welcome to The Drug System\n");
	printf("-----------------------------");
	
	
	printf("\nWhat is the current date?\nCurrent Time (Day/Month/Year): \n");
	getdata(messageIn, 35);
	printf("You entered this: %s\n\r", messageIn);
	
	
	printf("\nWhat is the current time?\nCurrent Time (Hour/Minutes/Seconds): \n");
	getdata(messageIn, 35);
	printf("You entered this: %s\n\n", messageIn);
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
	
	while(1)
	{
		printf("\nDrug Settings\n");
		printf("-----------------------------\n");
		printf("1 - Set Drug Name - %s\n", drugName);
		printf("2 - Set Dosage Amount - %s\n", dosageAmount);
		printf("3 - Scheduled Drug Administrations\n");
	 	printf("4 - Back\n");
	 	
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
			scheduledDrugEventsMenu();
		}
		else if(atoi(messageIn) == 4)
		{
			setupInitialMenu();
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
	int eventsInADay = 10;
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
