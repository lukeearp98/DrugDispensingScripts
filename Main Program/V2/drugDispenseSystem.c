/*	RTSCS 2020

#TODO
LDR?
Ensure events do not overlap
Remove global variables
Servo and stepper at same time?

Programme Name:		drugDispenseSystem.c
Author Name:		Luke Earp
Development Date:	23/02/2020
General Outline:	This is a system to run timed events to dispense drugs when required.
*/

/*	Compiler Directives */

/*	Standard Library Inclusions */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/* Other Inclusions */
enum eventCodes{
	DRUG_EVENT = 1,
	CONTROLLER_SETTINGS = 2,
	EVENT_ADDED = 3,
	BOOST_ACTIVATED = 4,
	BOOST_SETTINGS = 5,
	DRUG_SETTINGS = 6,
	ADMIN = 7, 
	LOG_SETTINGS = 8
};
/*	Function Prototypes (if not included within a header file) */
void initialiseLogArray(int, int);
void askForDateOnStartup(void);
int validateTimeString(char *);
int validateDateString(char *);
void initialiseTime();
void basicMenuSetup();
void compareAdminPassword();
void setupInitialMenu(void);
void writeToLog(enum eventCodes, char *);
void drugDispenseSetupMenu();
void localControllerSetupMenu();
int validateInput(char *);
void scheduledDrugEventsMenu();
void boostSettingMenu();
void checkEvents();
void runEventWindow();
int getData(char *, int);
int mputchar(unsigned char);
unsigned char mgetchar(void);
void printLogs();
void validateBoost();
int checkDrugInterval();
void timer(void);
void triggerStepper(void);
/*	Globals */
char controllerName[30] = "";
char drugName[30] = "";
char currentDate[15] = "";
char patientID[10] = "";
char lastBoostTime[15] = "00:00:00";
char dosageAmount[10] = "1";
char boostsAvailable[5] = "5";
char intervalBoostTime[30] = "00:00:30";
char recentLogs[20][200];
char drugEvents[11][15];
unsigned char display,hours,mins,secs,*scsr,*scdr,*tmsk1,*tflg1,*tctl1,*pgddr,*pgdr,*porta,*ddra;
unsigned int *tcnt,tics,steps;
volatile unsigned int stepperState = 0;
volatile unsigned int stepperComplete = 1;
unsigned int programStarted = 0;
unsigned int eventsInADay = 2;
unsigned int boostsTaken = 0;
unsigned int authorized = 1;
unsigned int dispenseActive = 1;
unsigned int switchFlag = 0;
unsigned int initialBoostRegistered = 0;
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: initialiseLogArray, askForDateOnStartup, basicMenuSetup
	Purpose: To initialise the program.
	Version: 1.0
	Modifications: None
*/
void main(void)
{	
	tctl1=(unsigned char*)0x20;
	tflg1=(unsigned char*)0x23;
	tcnt=(unsigned int*)0x0e;
	pgddr=(unsigned char*)0x3;
	pgdr=(unsigned char*)0x02;
	tmsk1=(unsigned char*)0x22;
	scsr = (unsigned char *)0x2e;
	ddra=(unsigned char *)0x01;
	porta=(unsigned char *)0x00; /* This is casting values into pointers */
	*pgddr =0xff;
	*tctl1=0x00; 
	*pgdr =0;
	/*Read timer and add offset period based on count of 500 nano sec (E) */
	*tflg1=0xc0; 
	*tmsk1 = 0xc0; 
	hours =0;
	mins=0;
	secs=0;
	display =0;
	tics =0;
	initialiseLogArray(20, 200);
	askForDateOnStartup();
	basicMenuSetup();
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To empty the log array.
	Version: 1.0
	Modifications: None
*/
void initialiseLogArray(int rowSize, int colSize)
{
	int rowCount,colCount = 0;
	for(rowCount = 0; rowCount < rowSize; rowCount++)
	{
		for(colCount = 0; colCount < colSize; colCount++)
		{
			recentLogs[rowCount][colCount] = '\0';
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: validateDateString, validateTimeString, getData, initialiseTime
	Purpose: To initialise the date and time on startup of the system.
	Version: 1.0
	Modifications: None
*/
void askForDateOnStartup()
{
	int timeStringValid = 0;
	int dateStringValid = 0;
	char messageIn[37] = "";
	
	printf("Welcome to The Drug System\n");
	printf("-----------------------------");
	
	while(!dateStringValid)
	{
		printf("\nWhat is the current date?\nCurrent Date (DD/MM/YYYY): \n");
		getData(messageIn, 35);	
		dateStringValid = validateDateString(messageIn);
		if(!dateStringValid)
		{
			printf("\nThe date entered does not match the correct format.\n\n");
		}
		else
		{
			strcpy(currentDate, messageIn);
		}
	}
	
	messageIn[0] = '\0';
	while(!timeStringValid)
	{
		printf("\nWhat is the current time?\nCurrent Time (Hour:Minutes:Seconds): \n");
		getData(messageIn, 35);
		timeStringValid = validateTimeString(messageIn);
		if(!timeStringValid)
		{
			printf("\nThe time entered does not match the correct format.\n\n");
		}
		else
		{
			initialiseTime(messageIn);
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To check the time is valid
	Version: 1.0
	Modifications: None
*/
int validateTimeString(char *eventTime)
{
	int i;
	int stringValid = 1;
	char timeSplit[5] = "";
	int tempHour = 0;
	int tempMin = 0;
	int tempSec = 0;

	if(eventTime[2] == ':' && eventTime[5] == ':')
	{
		if(!isdigit(eventTime[0]) || !isdigit(eventTime[1]))
		{
			stringValid = 0;
		}
		if(!isdigit(eventTime[3]) || !isdigit(eventTime[4]))
		{
			stringValid = 0;
		}
		if(!isdigit(eventTime[6]) || !isdigit(eventTime[7]))
		{
			stringValid = 0;
		}
	}
	else
	{
		stringValid = 0;
	}

	if(stringValid == 1)
	{
		strncpy(timeSplit, &eventTime[0], 2);
		tempHour = atoi(timeSplit);
		timeSplit[0] = '\0';
		strncpy(timeSplit, &eventTime[3], 2);
		tempMin = atoi(timeSplit);
		timeSplit[0] = '\0';
		strncpy(timeSplit, &eventTime[6], 2);
		tempSec = atoi(timeSplit);
		timeSplit[0] = '\0';

		if(tempHour > 23 || tempMin > 59 || tempSec > 59)
		{
			stringValid = 0;
		}
	}
	return stringValid;
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To check the date is valid
	Version: 1.0
	Modifications: None
*/
int validateDateString(char *eventDate)
{
	int i;
	int stringValid = 1;

	if(eventDate[2] == '/' && eventDate[5] == '/')
	{
		if(!isdigit(eventDate[0]) || !isdigit(eventDate[1]))
		{
			stringValid = 0;
		}
		if(!isdigit(eventDate[3]) || !isdigit(eventDate[4]))
		{
			stringValid = 0;
		}
		if(!isdigit(eventDate[6]) || !isdigit(eventDate[7]) || !isdigit(eventDate[8]) || !isdigit(eventDate[9]))
		{
			stringValid = 0;
		}
	}
	else
	{
		stringValid = 0;
	}
	return stringValid;
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To set the time to the entered value.
	Version: 1.0
	Modifications: None
*/
void initialiseTime(char *time)
{
	int nCount;
	char timeSplit[5] = "";
	int tempHour = 0;
	int tempMin = 0;
	int tempSec = 0;
	
	strncpy(timeSplit, &time[0], 2);
	tempHour = atoi(timeSplit);
	timeSplit[0] = '\0';
	
	strncpy(timeSplit, &time[3], 2);
	tempMin = atoi(timeSplit);
	timeSplit[0] = '\0';

	strncpy(timeSplit, &time[6], 2);
	tempSec = atoi(timeSplit);
	timeSplit[0] = '\0';
	
	hours = tempHour;
	mins = tempMin;
	secs = tempSec;
	
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: getData, compareAdminPassword, writeToLog, runEventWindow
	Purpose: Setup the menu that isn't for admin
	Version: 1.0
	Modifications: None
*/
void basicMenuSetup()
{
	char messageIn[37] = "";
	while(1)
	{
		printf("\n\nMain Menu\n");
		printf("-----------------------------\n");
		printf("1 - Enter Admin Password\n");
		printf("2 - Run Event Window\n");
		printf("\nSelection: \n");
		getData(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			compareAdminPassword();
			if(authorized == 0)
			{
				writeToLog(ADMIN, "Correct Password Entered.");
				setupInitialMenu();
			}
		}
		else if(atoi(messageIn) == 2)
		{
			runEventWindow();
		}
	}
}

/* Author: Luke Earp
	Company: Staffordshire University
	Functions used: writeToLog, getData, basicMenuSetup
	Purpose: Compares the entered password to the admin password.
	Version: 1.0
	Modifications: None
*/
void compareAdminPassword()
{
	int result = 0;
	int correctPassword = 1;
	char adminYorN[2] = "";
	char passwordEntered[30] = "";
	char adminPassword[30] = "admin";

	while(correctPassword == 1)
	{
		printf("\nEnter Admin Password:\n");
		getData(passwordEntered, 30);
		result = strcmp(adminPassword, passwordEntered);
		if(result == 0)
		{
			correctPassword = 0;
			authorized = 0;
		}
		else
		{
			writeToLog(ADMIN, "Incorrect Admin Password Entered.");
			printf("\nThat password is not correct. Do you want to try again?(Y/N):\n");
			getData(adminYorN, 5);
			while(1)
			{
				if(adminYorN[0] == 'Y' || adminYorN[0] == 'y')
				{
					break;
				}
				else if(adminYorN[0] == 'N' || adminYorN[0] == 'n')
				{
					basicMenuSetup();
				}
				else
				{
					printf("\nThat option does not exist. Please try again.\n");
				}
			}
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: initialiseLogArray, writeToLog, localControllerSetupMenu, drugDispenseSetupMenu, getData, runEventWindow, printLogs
	Purpose: Setup the admin first menu.
	Version: 1.0
	Modifications: None
*/
void setupInitialMenu()
{
	char messageIn[37] = "";
	while(1)
	{
		printf("\n\nMain Menu\n");
		printf("-----------------------------\n");
		printf("1 - Setup Local Controller\n");
		printf("2 - Setup Drug Dispensing\n");
		printf("3 - Run Event Window\n");
		printf("4 - View Logs\n");
		printf("5 - Delete Logs\n");

		printf("\nSelection: \n");
		getData(messageIn, 5);
		
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
			runEventWindow();
		}
		else if(atoi(messageIn) == 4)
		{
			printLogs();
		}
		else if(atoi(messageIn) == 5)
		{
			initialiseLogArray(20, 200);
			writeToLog(LOG_SETTINGS, "All Recent Logs Deleted.");
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To write values to a log.
	Version: 1.0
	Modifications: None
*/
void writeToLog(enum eventCodes eventCode, char *logMessage)
{
	char logString[200] = "";
	int rowCount,colCount = 0;
	

	switch(eventCode){
		case DRUG_EVENT:
			sprintf(logString, "DRUG_EVENT - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case CONTROLLER_SETTINGS:
			sprintf(logString, "CONTROLLER_SETTINGS - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case EVENT_ADDED:
			sprintf(logString, "EVENT_ADDED - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case BOOST_ACTIVATED:
			sprintf(logString, "BOOST_ACTIVATED - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case BOOST_SETTINGS:
			sprintf(logString, "BOOST_SETTINGS - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case DRUG_SETTINGS:
			sprintf(logString, "DRUG_SETTINGS - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case ADMIN:
			sprintf(logString, "ADMIN - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		case LOG_SETTINGS:
			sprintf(logString, "LOG_SETTINGS - %s - %s %02d:%02d:%02d", logMessage, currentDate , hours, mins, secs);
			break;
		default: 
			printf("Default");
	}
	for(rowCount = 0; rowCount < 20; rowCount++)
	{
		for(colCount = 0; colCount < 200; colCount++)
		{
			if(recentLogs[rowCount][0] == '\0')
			{
				goto foundEmptyArray;
			}
		}
	}	
	foundEmptyArray:
	strncpy(&recentLogs[rowCount][colCount], logString, strlen(logString));
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: getData, writeToLog, scheduledDrugEventsMenu, boostSettingMenu, setupInitialMenu
	Purpose: To edit the drug dispensing.
	Version: 1.0
	Modifications: None
*/
void drugDispenseSetupMenu()
{
	char messageIn[37] = "";
	char tempEventsInDay[5] = "";
	char logMessage[100] = "";
	int isValid = 0;

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
		getData(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			printf("\nDrug Name: \n");
			printf("------------------------------\n");
			getData(drugName, 30);
			sprintf(logMessage, "Drug Name Set To: %s", drugName);
			writeToLog(DRUG_SETTINGS, logMessage);
		}
		else if(atoi(messageIn) == 2)
		{
			while(isValid != 1)
			{
				printf("\nDosage Amount: \n");
				printf("------------------------------\n");
				getData(dosageAmount, 30);
				isValid = validateInput(dosageAmount);
			}
			isValid = 0;
			sprintf(logMessage, "Drug Amount Set To: %s", dosageAmount);
			writeToLog(DRUG_SETTINGS, logMessage);
		}
		else if(atoi(messageIn) == 3)
		{
			while(isValid != 1)
			{
				printf("\nAmount of Scheduled Administrations Allowed: \n");
				printf("------------------------------\n");
				getData(tempEventsInDay, 30);
				isValid = validateInput(tempEventsInDay);
			}
			isValid = 0;
			(atoi(tempEventsInDay) > 10) ? (eventsInADay = 10) : (eventsInADay = atoi(tempEventsInDay));
			sprintf(logMessage, "Events In A Day Set To: %d", eventsInADay);
			writeToLog(DRUG_SETTINGS, logMessage);		
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
			return;
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: getData, writeToLog, setupInitialMenu
	Purpose: To edit the local controller settings.
	Version: 1.0
	Modifications: None
*/
void localControllerSetupMenu()
{
	char messageIn[37] = "";
	char logMessage[100] = "";
	int isValid = 0;
	while(1)
	{
		printf("\nController Settings\n");
		printf("-----------------------------\n");
		printf("1 - Set Controller Name - %s\n", controllerName);
	    printf("2 - Set Patient ID - %s\n", patientID);
		printf("3 - Back\n");
			
		printf("\nSelection: \n");
		getData(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			printf("\nController Name: \n");
			printf("------------------------------\n");
			getData(controllerName, 30);
			sprintf(logMessage, "Controller Name Set To: %s", controllerName);
			writeToLog(CONTROLLER_SETTINGS, logMessage);

		}
		else if(atoi(messageIn) == 2)
		{
			while(isValid != 1)
			{
				printf("\nPatient ID: \n");
				printf("------------------------------\n");
				getData(patientID, 5);	
				isValid = validateInput(patientID);
			}			
			
			sprintf(logMessage, "Patient ID Set To: %s", patientID);
			writeToLog(CONTROLLER_SETTINGS, logMessage);
		}
		else if(atoi(messageIn) == 3)
		{
			return;
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
}
int validateInput(char *input)
{
	int inputValid = 1;
	int nCount = 0;
	
	for(nCount = 0; nCount < strlen(input); nCount++)
	{
		if(!isdigit(input[nCount]))
		{
			inputValid = 0;
		}
	}
	
	return inputValid;
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: getData, writeToLog, validateTimeString
	Purpose: Edit the drug event times.
	Version: 1.0
	Modifications: None
*/
void scheduledDrugEventsMenu()
{
	char messageIn[37] = "";
	char currentDrugEvent[10] = "";
	int nCount = 0;
	int stringValid = 0;
	char logMessage[100] = "";

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
		getData(messageIn, 5);
		if(atoi(messageIn) <= eventsInADay)
		{
			while(!stringValid)
			{
				printf("Enter Time of Event(Hour:Minutes:Seconds):\n");
				getData(currentDrugEvent, 10);
				stringValid = validateTimeString(currentDrugEvent);
				for(nCount = 0; nCount <= strlen(currentDrugEvent); nCount++)
				{
					drugEvents[atoi(messageIn)][nCount] = currentDrugEvent[nCount];
				}
				if(!stringValid)
				{
					printf("\nThe time entered does not match the correct format.\n\n");
				}
			}
			sprintf(logMessage, "Event Added For: %s", drugEvents[atoi(messageIn)]);
			writeToLog(DRUG_EVENT, logMessage);	

			currentDrugEvent[0] = '\0';
		}
		else if(atoi(messageIn) == nCount)
		{
			return;
		}
		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
		stringValid = 0;
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: getData, writeToLog, drugDispenseSetupMenu
	Purpose: To edit the boosts available.
	Version: 1.0
	Modifications: None
*/
void boostSettingMenu()
{
	char messageIn[37] = "";
	int boostsToday = 0;
	char logMessage[100] = "";
	int isValid = 0;
	
	while(1)
	{
		printf("\nBoost Settings\n");
		printf("-----------------------------\n");
		printf("1 - Set Available Boosts - %s\n", boostsAvailable);
		printf("2 - Set Interval Between Boosts - %s\n", intervalBoostTime);
	 	printf("3 - Back\n");
	 	
	 	printf("\nBoosts Used Today: %d\n\n", boostsTaken);
	 	
		printf("\nSelection: \n");
		getData(messageIn, 5);
		
		if(atoi(messageIn) == 1)
		{
			while(isValid != 1)
			{
				printf("\nTotal Boosts Available: \n");
				printf("------------------------------\n");
				getData(boostsAvailable, 30);
				isValid = validateInput(boostsAvailable);
			}
			isValid = 0;
			sprintf(logMessage, "Boosts Allowed In A Day Set To: %s", boostsAvailable);
			writeToLog(BOOST_SETTINGS, logMessage);	
		}
		else if(atoi(messageIn) == 2)
		{	
			while(isValid != 1)
			{
				printf("\nBoost Interval (HH:MM:SS): \n");
				printf("------------------------------\n");
				getData(intervalBoostTime, 30);
				isValid = validateTimeString(intervalBoostTime);
			}
			isValid = 0;
			sprintf(logMessage, "Interval Between Boosts Set To: %s", intervalBoostTime);
			writeToLog(BOOST_SETTINGS, logMessage);	
		}
		else if(atoi(messageIn) == 3)
		{
			return;
		}

		else
		{
			printf("\nThat option does not exist. Please try again.\n");
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: writeToLog
	Purpose: To check any events against the current time to see if they match.
	Version: 1.0
	Modifications: None
*/
void checkEvents()
{
	int nCount = 0;
	char timeSplit[5] = "";
	int tempHour = 0;
	int tempMin = 0;
	int tempSec = 0;
	
	for(nCount = 1; nCount <= eventsInADay; nCount++)
	{
		if(drugEvents[nCount][0] != '\0')
		{
			strncpy(timeSplit, &drugEvents[nCount][0], 2);
			tempHour = atoi(timeSplit);
			timeSplit[0] = '\0';
			strncpy(timeSplit, &drugEvents[nCount][3], 2);
			tempMin = atoi(timeSplit);
			timeSplit[0] = '\0';
			strncpy(timeSplit, &drugEvents[nCount][6], 2);
			tempSec = atoi(timeSplit);
			timeSplit[0] = '\0';
			if(tempHour == hours && tempMin == mins && tempSec == secs)
			{
				printf("\n\nEvent Found for %02d:%02d:%02d\n\nDispensing...\n", hours, mins, secs);
				writeToLog(DRUG_EVENT, "Scheduled Dispense Executed Successfully.");
				dispenseActive = 0;
				stepperState = 1;
			}	
		}
		
	} 
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: setupInitialMenu, basicMenuSetup
	Purpose: To view the current time
	Version: 1.0
	Modifications: None
*/
void runEventWindow()
{
	char input;
	unsigned char in;
	
	printf("\nPress the ESC key to return to menu.\n");
	programStarted = 1;
	do
	{
		in = *porta&0x01; /* Mask off top 7 bits */	
		if(in == 0x1 && switchFlag == 0)
		{
			switchFlag = 1;
			validateBoost();
		}
		if(in == 0x0)
		{
			switchFlag = 0;
		}
		if (display == 1) 
		{
			checkEvents();
			display = 0;
		}		
		if(((*scsr) & 0x20) != 0x0)
        {
            input = *scdr;
            if(input == 0x1b)
			{
				while(stepperComplete == 0)
				{
					if (display == 1) 
					{
						display = 0;
					}	
				}
				programStarted = 0;
				return;
			}
        } 
	}
	while(1);
}


/* 	Author : Mr Luke A Earp
	Company: Staffordshire University
	Functions used: mgetchar, mputchar
	Purpose: This retrieves data strings from the keyboard.
	Version: 1.0
	Modifications: None
*/
int getData(char *dataString, int size)
{		
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
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: mputchar
	Purpose: To create my own getchar
	Version: 1.0
	Modifications: None
*/
unsigned char mgetchar(void)
{
	unsigned char data,data1;
	/* baud rate already defined in monitor program*/
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
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To create my own putchar
	Version: 1.0
	Modifications: None
*/
int mputchar(unsigned char data)
{
	/* baud rate already defined in monitor program*/
	scsr = (unsigned char *)0x2e;
	scdr = (unsigned char *)0x2f;
	/* Loop till bit 7 is '1' indicating free to send more data */
	while (((*scsr) & 0x80) == 0x0);
	/* Transfer data out  */
	*scdr = data;
	return(0);
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To print logs to the screen.
	Version: 1.0
	Modifications: None
*/
void printLogs()
{
	int rowCount, colCount;
	
	printf("\nMost Recent Logs:\n");
	for(rowCount = 0; rowCount < 20; rowCount++)
	{
		if(recentLogs[rowCount][0] != '\0')
		{
			printf("\n%s", recentLogs[rowCount]);
		}
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: writeToLog, checkDrugInterval
	Purpose: To check if a boost is allowed.
	Version: 1.0
	Modifications: None
*/
void validateBoost()
{
	if(boostsTaken >= atoi(boostsAvailable))
	{
		printf("\nBoost rejected due to reaching daily limit.\n");
		writeToLog(BOOST_ACTIVATED, "Boost rejected as reached daily limit.");
	}
	else if(checkDrugInterval() == 1)
	{
		printf("\nBoost rejected due to high number of requests.\n");
		writeToLog(BOOST_ACTIVATED, "Boost rejected due to high number of requests.");
	}
	else
	{
		printf("\n\nBoost Accepted.\n\nDispensing...\n");
		dispenseActive = 0;
		stepperState = 1;
		boostsTaken++;
		writeToLog(BOOST_ACTIVATED, "Drug Boost Executed Successfully.");
		sprintf(lastBoostTime, "%02d:%02d:%02d", hours, mins, secs);
		initialBoostRegistered = 1;
	}
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: 
	Purpose: To check if the patient has requested too many boosts.
	Version: 1.0
	Modifications: None
*/
int checkDrugInterval()
{
	int lastBoostTimeHrs, lastBoostTimeMins, lastBoostTimeSecs;
	int intervalHrs, intervalMins, intervalSecs;
	int hourDiff, minDiff, secDiff;
	int overIntervalTime = 1;
	char timeSplit[5] = "";
	lastBoostTimeHrs = 0;
	lastBoostTimeMins = 0;
	lastBoostTimeSecs = 0;
	intervalHrs = 0;
	intervalMins = 0;
	intervalSecs = 0;
	hourDiff = 0;
	minDiff = 0;
	secDiff = 0;

	strncpy(timeSplit, &lastBoostTime[0], 2);
	lastBoostTimeHrs = atoi(timeSplit);
	timeSplit[0] = '\0';
	
	strncpy(timeSplit, &lastBoostTime[3], 2);
	lastBoostTimeMins = atoi(timeSplit);
	timeSplit[0] = '\0';

	strncpy(timeSplit, &lastBoostTime[6], 2);
	lastBoostTimeSecs = atoi(timeSplit);
	timeSplit[0] = '\0';
	
	strncpy(timeSplit, &intervalBoostTime[0], 2);
	intervalHrs = atoi(timeSplit);
	timeSplit[0] = '\0';
	
	strncpy(timeSplit, &intervalBoostTime[3], 2);
	intervalMins = atoi(timeSplit);
	timeSplit[0] = '\0';

	strncpy(timeSplit, &intervalBoostTime[6], 2);
	intervalSecs = atoi(timeSplit);
	timeSplit[0] = '\0';
	
	hourDiff = hours - lastBoostTimeHrs;
	minDiff = mins - lastBoostTimeMins;
	secDiff = secs - lastBoostTimeSecs;
	
	if(initialBoostRegistered == 1)
	{
		if(hourDiff > intervalHrs)
		{
			overIntervalTime = 0;
		}
		if(hourDiff == intervalHrs && minDiff > intervalMins)
		{
			overIntervalTime = 0;
		}
		if(hourDiff == intervalHrs && minDiff == intervalMins && secDiff >= intervalSecs)
		{
			overIntervalTime = 0;
		}
	}
	else
	{
		overIntervalTime = 0;
	}
		
	return overIntervalTime;
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: checkEvents, writeToLog
	Purpose: To create a real time clock
	Version: 1.0
	Modifications: None
*/
@interrupt void timer(void)
{
	unsigned int *toc2;	
	toc2=(unsigned int*)0x18;
	*ddra=0xfe; /* This is saying that DDRA is to 7 outputs and 1 input */
	display = 0;         
	tics++;
	
	if(tics==100) /* 100 x 10ms = 1sec */
	{        
	 	secs++;
		display =1; /* tell to update display */
		tics=0;
		if(programStarted == 1 && dispenseActive == 1)
		{
			printf("%02d:%02d:%02d\r",hours,mins,secs);
		}
	}
	if(secs==59)
	{ 
		secs=0;
		mins++; 
	}
	
	if(mins==59)
	{ 
		mins=0;
	    hours++;
	}
	
	if(hours==23)
	{
		hours=0;
	} 
	    
	*tflg1=0x40; //Clear TOC2 Flag 
	*toc2=*tcnt+20000; 	/*Read timer and add offset period */
}
/* 	Author: Luke Earp
	Company: Staffordshire University
	Functions used: None
	Purpose: To run the stepper motor.
	Version: 1.0
	Modifications: None
*/
@interrupt void triggerStepper(void)
{
	unsigned int *toc1;
	toc1=(unsigned int*)0x16;
	
	if(stepperState != 0)
	{
		steps++;
		
		if(steps == (4096 * atoi(dosageAmount)))
		{
			stepperState = 0;
			printf("\nDispensing Complete.\n\n");
			steps = 0;
			dispenseActive = 1;
			*porta = 0x00;
			stepperComplete = 1;
			return;
		}
		stepperComplete = 0;
		*porta = 0x02; /* set the light to the first output if the switch is on 0 */
		switch(stepperState){
			case 1:
				*pgdr = 0x01;
				break;
			case 2:
				*pgdr = 0x03;
				break;
			case 3:
				*pgdr = 0x02;
				break;
			case 4:
				*pgdr = 0x06;
				break;
			case 5:
				*pgdr = 0x04;
				break;
			case 6:
				*pgdr = 0x0c;
				break;
			case 7:
				*pgdr = 0x08;
				break;
			case 8:
				*pgdr = 0x09;
				stepperState = 0;
				break;
		}
		stepperState++;
	}

	*tflg1=0x80; //Clear TOC2 Flag 
	*toc1=*tcnt+2000; 	/*Read timer and add offset period */
}

