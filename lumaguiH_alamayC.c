/*************************************************************************************************
	This is to certify that this project is our own work, based on our personal efforts in
	studying and applying the concepts learned. We have constructed the functions and their
	respective algorithms and corresponding code by ourselves. The program was run, tested,
	and debugged by our own efforts. We further certify that we have not copied in part or
	whole or otherwise plagiarized the work of other students and/or persons.
					Hans Emilio M. Lumagui, DLSU ID# 12208426
					Carl Justine S. Alamay, DLSU ID# 12274100
*************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define LENGTH 40
#define BUFFERLENGTH 2048

typedef char string[101];
typedef struct personnel 		// Struct that compiles variables responsible for storing user data.
{
    int personnelID;
    char Name[LENGTH];
    char Password[LENGTH];
    int Active;
    int Access; 
} personnel;

typedef struct Task				// Struct that compiles variables that stores TASK DETAILS such as the task description, status, and dates.
{
    int taskID;
    char name[LENGTH];
    int sequence;
    char description[101];
    int projectID;
    char startdate[11];
    char enddate [11];
    float totalDuration;
    int status;
    int assignID;
} Task;


typedef struct Project 			// Struct that compiles project variables such as project status and project ID.
{
    int projectID;
    char name[LENGTH];
    int status;
    float completion;
    string description;
    int assignID;
}   Project;

typedef struct date				// Struct used separate user input for dates in Task struct into day, month, and year.
{
    int day;
    int month;
    int year;
} date;

void export (int action,
 int count);

void count_sched(int ID,
Task t[],
int count);

void count_tasks_of_user(int ID,
Task t[],
int count);

int compareDate(date date1,
date date2);

/*
	manager_screen prints the menu in the terminal that displays options to edit and/or manipulate personnel data, tasks, and projects.
*/
void manager_screen()
{
    printf("Welcome to the manager screen\n[1] Add new project\n[2] Add new task\n[3] Update Task\n[4] Show personnel list\n[5] Assign Task\n[6] Show project details\n[7] Show delayed task\n[8] Run project completion\n[9] Count tasks beyond and within schedule\n[10] Count tasks of the user\n[11] Main menu\n");
}

/*
	updateFile rewrites the contents of the "preloaded" text file with personnel data.
	
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param count - the number of personnel.
		
	Internal variables:
	int i - for loop counter.
    FILE *f - writes into preloaded.txt
*/
void updateFile(personnel p[],
int count)
{
    int i;
    FILE *f;
    f = fopen("preloaded.txt", "w");		// opens preloaded.txt and writes into it with new information.
        for(i = 0; i < count; i++)
        {
            if (i == 0)
            fprintf(f, "%d\n",p[i].personnelID);
            else
            fprintf(f,"\n%d\n",p[i].personnelID);		// printing of the said information into the actual text file.
            fprintf(f,"%s\n",p[i].Name);
            fprintf(f,"%s\n",p[i].Password);
            fprintf(f,"%d\n",p[i].Active);
            fprintf(f,"%d",p[i].Access);
        }
    fclose(f);		// closing of preloaded.txt
}

/*
	put_task_in_file rewrites the contents of the "tasks" texts file with new inputted tasks.
	
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param count - the number of tasks.
	
	Internal variables:
	int i - for loop counter.
    FILE *s - writes into tasks.txt
*/
void put_task_in_file(Task t[], 
int count)
{
    int i;
    FILE *s;
    s = fopen("tasks.txt", "w");			// opens tasks.txt and writes into it with new information.
    for (i =0; i < count; i++)
    {
        if(i == 0)
        fprintf(s, "%d\n",t[i].taskID);
        else
        fprintf(s, "\n%d\n",t[i].taskID);
        fprintf(s,"%s\n",t[i].name);
        fprintf(s,"%d\n",t[i].sequence);
        fprintf(s,"%s\n",t[i].description);			// printing of the said information into the actual text file.
        fprintf(s,"%d\n",t[i].projectID);
        fprintf(s,"%s\n",t[i].startdate);
        fprintf(s,"%s\n",t[i].enddate);
        fprintf(s,"%f\n",t[i].totalDuration);
        fprintf(s,"%d\n",t[i].status);
        fprintf(s,"%d",t[i].assignID);
    }
    fclose(s);			// closing of tasks.txt
}

/*
	count_project gives an integer which shows how many projects are in the "project" text file.
	
	@param *count - variable to be manipulated with respect to the number of projects.
	
	Internal variables
	char temp - temporarily stores the string read from the "project" text file.
    FILE *a - reads project.txt
*/
void count_project(int *count)
{
    FILE *a;
    char temp[BUFFERLENGTH];		// declation of a temporary variable with defined length.
    *count = 0;
    a = fopen("project.txt", "r"); 	// opening of project.txt to read.
    while(!feof(a))
    {
        fgets(temp,BUFFERLENGTH,a);
        *count+= 1;
    }
    *count /= 6;		// the number of projects in project.txt.
    fclose(a);		// closing of project.txt.
}

/*
	count_task gives an integer which shows how many tasks are in the "tasks" text file.
	
	@param *s - to be manipulated with respect to the number of tasks.
	
	Internal variables
    FILE *a - reads tasks.txt
	char temp - temporarily stores the string read from the "tasks" text file.
*/
void count_task(int *s)
{
    FILE *a;
    char temp[BUFFERLENGTH]; 		// declation of a temporary variable with defined length.
    *s = 0;
    a = fopen("tasks.txt", "r"); 	// opening of tasks.txt to read.
    while(!feof(a))
    {
        fgets(temp,BUFFERLENGTH,a);
        *s+= 1;
    }
    *s /= 10;			// the number of tasks in tasks.txt.
    fclose(a);		// closing of tasks.txt.
}

/*
	put_tasks_in_array allocates the contents of the "tasks" text file into an array for ease of use.
	
	@param task_count - the number of tasks in the "tasks" text file.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	
	Internal variables:
    FILE *pointer - reads into tasks.txt
	int rem - temporary variable for array indexing.
	int i - for loop counter.
	char buffer - temporarily stores the string read from the "tasks" text file.
*/
void put_tasks_in_array(int task_count,
Task t[])
{
    FILE *pointer;
    int rem;
    int i;
    char buffer[BUFFERLENGTH];
    pointer = fopen("tasks.txt", "r");		// opening of tasks.txt to read.
    while (!feof(pointer))
    {
        for (i =0; i < task_count; i+= 1)		// for loop involving the number of tasks in the textfile.
        {
            fscanf(pointer,"%d",&t[i].taskID);		// gets the taskID from the text file.
            fgets(buffer,BUFFERLENGTH,pointer);

            fgets(t[i].name,BUFFERLENGTH,pointer);
            rem = strlen(t[i].name);
            t[i].name[rem - 1] = '\0'; 		// removes null value from array.

            fscanf(pointer,"%d",&t[i].sequence); 	// gets the task sequence from the text file.
            fgets(buffer,BUFFERLENGTH,pointer); 

            fgets(t[i].description,BUFFERLENGTH,pointer);
            rem = strlen(t[i].description);
            t[i].description[rem - 1] = '\0'; 

            fscanf(pointer,"%d", &t[i].projectID);	// gets the projectID from the text file.
            fgets(buffer,BUFFERLENGTH,pointer);

            fgets(t[i].startdate,BUFFERLENGTH,pointer);
            rem = strlen(t[i].startdate);
            t[i].startdate[rem - 1] = '\0';

            fgets(t[i].enddate,BUFFERLENGTH,pointer);
            rem = strlen(t[i].enddate);
            t[i].enddate[rem - 1] = '\0';

            fscanf(pointer,"%f",&t[i].totalDuration);	// gets the totalDuration from the text file.
            fscanf(pointer,"%d",&t[i].status);			// gets the task status from the text file.
            fscanf(pointer,"%d",&t[i].assignID);		// gets the assigned ID from the text file.
        }
    }
    fclose(pointer);		// closing of tasks.txt
}

/*
	put_projects_in_array allocates the contents of the "project" text file into an array for ease of use.
	
	@param personnel_count - the number of personnel.
	@param projects[] - gives access to the Project struct array allowing manipulation of the variables.
	
	Internal variables:
	int i - for loop counter.
    FILE *pointer - read into project.txt
	char buffer - temporarily stores the string read from the "project" text file.
*/
void put_projects_in_array(int personnel_count,
Project projects[])
{
    int i;
    FILE *pointer;
    char buffer[BUFFERLENGTH];
    pointer = fopen("project.txt", "r");		// opening of project.txt to read.
    while (!feof(pointer))
    { 
        for (i = 0; i < personnel_count; i+= 1)
        {
            fscanf(pointer,"%d",&projects[i].projectID);		// gets the projectID from the text file.
            fgets(buffer,BUFFERLENGTH,pointer);
            fgets(projects[i].name,BUFFERLENGTH,pointer); 
            fscanf(pointer,"%d",&projects[i].status); 			// gets the project status from the text file.
            fscanf(pointer,"%f",&projects[i].completion);		// gets the project completion percentage from the text file.
            fgets(buffer,BUFFERLENGTH,pointer); 
            fgets(projects[i].description,BUFFERLENGTH,pointer);
            fscanf(pointer,"%d",&projects[i].assignID);			// gets the assigned ID from the text file.
        }
    }
    fclose(pointer);			// closing of project.txt.
}

/*
	find_task_in_array gathers the task from the array through the ID that the user has inputted.
	
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param count - the number of tasks present in the struct.
	
	Internal variables:
	int a - user input for taskID.
	int i - for loop counter.
*/
int find_task_in_array(Task t[],
int count)
{
    int a;
    int i;
    printf("Input task ID: ");
    scanf("%d", &a);
    for(i = 0; i < count; i++)
    {
        if(t[i].taskID == a)
            return i;
    }
    printf("Invalid task ID!\n");
    return -1;
}

/*
	add_user gets user input and generates a unique user that is either an Admin, a Manager, or simply a user.
	
	@param *count - the number of current users saved in the personnel struct.
	@param p[] - gives access to the personnel struct allowing manipulation of the variables.
	
	Internal variables:
	int tries - the amount of times the user input been repeated.
	int repetition - indicator for user input (0 if valid, 1 if invalid)
	int i - for loop counter.
    FILE *fpointer - writes into preloaded.txt
    personnel *a - temporary personnel variable
*/
void add_user(int *count, personnel p[])
{
    int tries = 0;
    int repetition;
    int i;
    FILE *fpointer;
    personnel *a = malloc(sizeof(personnel)); 	// allocates the size of personnel struct.
    srand(time(0));
    a->personnelID = rand()%9999;		// generates a random personnelID with a max of 9999.

        do{
        if(tries != 0)
            printf("Username is too Long\n");
        printf("Input Username: \n");				// asks user to input a unique Username.
        scanf("%s", a->Name);
        repetition = 0;
        tries += 1;
        for(i = 0; i < *count; i++)
        {
            if(strcmp(a->Name, p[i].Name) == 0)
            {
                printf("Username is taken\n");		// warning if username is already taken.
                repetition = 1;
                tries = 0;
            }
        }
        } while ((strlen(a->Name) > 40)|| (repetition == 1));       

        tries = 0;

    do{
        if (tries >= 1)
            printf("Please input 1 - 3");
        printf("Input [1] if the employee is an Admin\nInput [2] if the employee is a Manager\nInput [3] if the employee is a User\n");		// assigns a role to the user.
        scanf("%d", &a->Access);
        tries += 1;
    } while (a->Access < 1 || a->Access > 3);

    
    fpointer = fopen("preloaded.txt", "a");		// opening of preloaded.txt to append.


    fprintf(fpointer,"\n%.4d\n", a->personnelID);
    fprintf(fpointer,"%s\n", a->Name);					// printing of new data into the text file.
    fprintf(fpointer,"tempass\n");
    fprintf(fpointer,"1\n");
    fprintf(fpointer,"%d", a->Access);

    p[*count].personnelID = a->personnelID;
    strcpy(p[*count].Name,a->Name);
    strcpy(p[*count].Password, "tempass");
    p[*count].Active = 1;
    p[*count].Access = a->Access;

    fclose(fpointer);	// closing of preloaded.txt.
    free (a);		// deallocates memory from malloc.
}

/*
	find_projID returns the project ID from user input.
	
	@param p[] - gives access to the Project struct array allowing manipulation of the variables.
	@param p_id - user input for Project ID.
	@param count - the number of projects.
	
	Internal variables:
	int i - for loop counter.
*/
int find_projID(Project p[],
int p_id,
int count)
{
	int i;
    for(i = 0; i < count; i++)
        {
            if (p[i].projectID == p_id)
                return i;          
        }
        printf("Invalid Project ID!\n");
        return -1;
}

/*
	put_proj_in_file allocates project details into the "project" text fie.
	
	@param p[] - gives access to the Project struct array allowing manipulation of the variables.
	@param count - the amount of projects in the struct.
	
	Internal variables:
    FILE *j - writes into project.txt
	int i - for loop counter.
*/
void put_proj_in_file(Project p[], int count)
{
    FILE *j;
    int i;
    j = fopen("project.txt", "w");		// opening of project.txt to write.
    for (i = 0; i<count; i++)
    {
        if (i == 0)
            fprintf(j,"%d\n",p[i].projectID);
        else
            {
            fprintf(j,"\n%d\n",p[i].projectID);
            }
        fprintf(j,"%s",p[i].name);
        fprintf(j,"%d\n",p[i].status);			// printing of information into the text file.
        fprintf(j,"%.2f\n",p[i].completion);
        fprintf(j,"%s",p[i].description);
        fprintf(j,"%d",p[i].assignID);
    }
    fclose(j);		// closing of project.txt
}

/*
	find_ID returns User ID from the user input.
	
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param p_id - user input for User ID.
	@param count - the amount of users in the struct.
	
	Internal variables:
	int i - for loop counter.
*/
int find_ID(personnel p[],int p_id, int count)
{
    int i;
    for(i = 0; i < count; i++)
        {
            if (p[i].personnelID == p_id)
                return i;          
        }
        printf("Invalid User ID!\n");
        return -1;
}

/*
	put_personnels_in_array allocates the contents of the "preloaded" text file into an array for ease of use.
	
	@param personnel_count - the amount of users in the struct.
	@param personnels[] - gives access to the personnel struct array allowing manipulation of the variables.
	
	Internal variables:
    FILE *pointer - reads into preloaded.txt
	int i - for loop counter.
	int remove - temporary variable for array indexing.
	char buffer - temporarily stores the string read from the "preloaded" text file.
*/
void put_personnels_in_array(int personnel_count,
personnel personnels[])
{
    FILE *pointer; 
    int i;
    int remove;
    char buffer[BUFFERLENGTH];				// declaring variable with predefined length.
    pointer = fopen("preloaded.txt", "r");	// opening of preloaded.txt to read.
    while (!feof(pointer))
    { 
        for (i =0; i < personnel_count; i+= 1)
        {
            fscanf(pointer,"%d",&personnels[i].personnelID);		// gets personnelID from text file.
            fgets(buffer,BUFFERLENGTH,pointer); 

            fgets(personnels[i].Name,BUFFERLENGTH,pointer); 		// gets Name from text file.
            remove = strlen(personnels[i].Name);		
            personnels[i].Name[remove-1] = '\0';

            fgets(personnels[i].Password,BUFFERLENGTH,pointer); 	// gets Password from text file.
            remove = strlen(personnels[i].Password);
            personnels[i].Password[remove-1] = '\0';

            fscanf(pointer,"%d",&personnels[i].Active);				// gets personnel status from text file.
            fgets(buffer,BUFFERLENGTH,pointer); 

            fscanf(pointer,"%d",&personnels[i].Access);				// gets personnel access from text file.
            fgets(buffer,BUFFERLENGTH,pointer); 
        }
    }
    fclose(pointer);		// closing of preloaded.txt
}

/*
	verify_login authenticates the user input for Name and Password.
	
	@param count - the number of users in the struct.
	@param a[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param name[] - user input for Name.
	@param pass[] - user input for Password.
	
	Internal variables:
	int i - for loop counter.
*/
int verify_login(int count,
personnel a[],
char name[],
char pass[])
{
    int i;
    for (i = 0; i < count; i+= 1)
    {
        if(strcmp(name,a[i].Name) == 0)
        {
            if (strcmp(pass, a[i].Password) == 0)		// compares user input from stored Password.
            {
                return i;
            }
        }
    }
    printf("INVALID USERNAME OR PASSWORD\n");
    return -1;
}

/*
	count_personnel manipulates a variable with the amount of users in the struct.
	
	@param *count - the number of users in the struct.
	
	Internal variables:
    FILE *a - reads into preloaded.txt.
	char lol - temporarily stores the string read from the "preloaded" text file.
*/
void count_personnel(int *count)
{
    FILE *a = fopen("preloaded.txt", "r");		// opening of preloaded.txt to read.
    char lol[BUFFERLENGTH]; 				// declaring of variable with predefined length.
    *count = 0;
    while (!feof(a))
    {
        fgets(lol,BUFFERLENGTH,a);
        *count+= 1;
    }
    *count/=5;		// the number of personnel in the text file.
    fclose (a);		// closing of preloaded.txt
}

/*
	admin_screen displays the menu if the user is determined to be an Admin.
*/
void admin_screen()
{
    printf("Hello Admin!\n");
    printf("This is the Admin Menu!\n");
    printf("Please Select an Input!\n");
    printf("[1] Add New User\n");
    printf("[2] Update User\n");
    printf("[3] Delete User\n");
    printf("[4] Archive User\n");
    printf("[5] Assign project\n");
    printf("[6] Count tasks within and beyond schedule\n");
    printf("[7] Count the tasks of the User\n");
    printf("[8] Main Menu\n");
}

/*
	update_user edits the already-existing data found in the personnel struct.
	
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param c - the number of users in the struct.
	
	Internal variables:
	int id - user input for the user ID.
	int loc - variable for find_ID function.
	int i - user input for user editing.
	int kreig - temporary variable for switch behavior.
	int j - for loop counter.
    string temp - temporary variable following typedef string.
*/
void update_user(personnel p[],
int c)
{
    int id;
    int loc;
    int i;
    int kreig;
    int j;
    string temp;
    do{
    printf("Input ID: ");			// user input for ID.
    scanf("%d", &id);
    loc = find_ID(p, id, c);		// stores returned integer from find_ID.
    }while (loc == -1);
    do{
        printf("What would you like to edit:\n[1] Personnel Name\n[2] Personnel Password\n[3] Personnel Access\n[4] Done Updating\n");		
        scanf("%d", &i);
        switch(i)
        {
            case 1:
            do{
                kreig = 0;
                printf("Input new username: ");		// user input from new Username.
                scanf("%s", temp);
                for(j = 0; j < c; j+= 1)
                    {
                        if (strcmp(temp, p[j].Name) == 0)		// compares user input to old Username.
                        {
                            printf("Username taken!\n");
                            kreig = 1;
                        }
                    }
                if(kreig == 0)
                {
                    strcpy(p[loc].Name,temp);		// assigning of new Username.
                }
            }while(kreig == 1);
                break;
            case 2:
                printf("Input new password: ");		// user input for new Password.
                scanf("%s", p[loc].Password);
                break;
            case 3:
                do{
                printf("Input new access: \n[1] Admin\n[2] Manager\n[3] User\n");		// changing of user access.
                scanf("%d", &p[loc].Access);
                } while (p[loc].Access < 1 || p[loc].Access > 3);
                break;
            case 4:
            break;
            default:
                printf("Invalid Input!\n");
        }
    }while (i != 4);
}

/*
	delete_user removes an instance in the personnel struct.
	
	@param j[] - gives access to the personnel struct allowing manipulation of the variables.
	@param count - the number of users in the struct.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param task_count - the number of tasks in the struct.
	@param ID - variable for the task ID.
	@param proj[] - gives access to the Project struct array allowing manipulation of the variables.
	@param proj_count - the number of projects in the struct.
	
	Internal variables:
	int c - user input for personnel ID.
	int loc - variable for find_ID function.
	int o - for loop counter.
	int i - for loop counter.
*/
void delete_user(personnel j[],
int count,
Task t[],
int task_count,
int ID,
Project proj[],
int proj_Count)
{
    int c;
    int loc;
    int o;
    int i;
    
    do{
    printf("Input personnel ID: ");			// user input for personnel ID.
    scanf("%d", &c);
    loc = find_ID(j,c,count);		
    }while (loc == -1);

    for (o = 0; o < task_count; o+= 1)		// for loop that compares task ID to personnel ID.
    {
        if (t[o].assignID == j[loc].personnelID)		
        {
            t[o].assignID = ID;
        }
    }

    for (i = 0; i < proj_Count; i+= 1)		// for loop that compares project ID to personnel ID.
    {
        if (proj[i].assignID == j[loc].personnelID)
        {
            proj[i].assignID = ID;
        }
    }

    for(loc = loc; loc < count-1; loc++)
    {
        j[loc].personnelID = j[loc+1].personnelID;		// empty spot in array from deleted user being replaced.
        strcpy(j[loc].Name,j[loc+1].Name);
        strcpy(j[loc].Password,j[loc+1].Password);
        j[loc].Access = j[loc+1].Access;
        j[loc].Active = j[loc+1].Active;
    }

    put_task_in_file(t,task_count);			// calling of user-defined functions.
    put_proj_in_file(proj,proj_Count);
}

/*
	archive_user disables access to the program to the given user ID.
	
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param count- the number of users in the struct.
	
	Interval variables:
	int loc - variable for find_ID function.
	int id - user input for personnel ID.
*/
void archive_user(personnel p[],
int count)
{
    int loc = 0;
    int id;

    do{
    printf("Input Personnel ID: ");
    scanf("%d", &id);
    loc = find_ID(p, id, count);
    } while (loc == -1);
    p[loc].Active = 2;
}

/*
	assign_project allocates a project from the Project struct to a given user.
	
	@param pr[] - gives access to the Project struct array allowing manipulation of the variables.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param p_count - the number of users in the struct.
	@param pr_count - the number of projects in the struct.
	
	Internal variables:
	int ID - user input for personnel ID.
	int index - variable for find_ID function.
	int pID - user input for project ID.
	int index2 - variable for finc_projID.
	int jfer - control  variable for do-while loop.
	int i - for loop counter.
	int j - for loop counter.
*/
void assign_project(Project pr[],
personnel p[],
int p_count,
int pr_count)
{
    int ID;
    int index;
    int pID;
    int index2;
    int jfer = -1;
    int i;
    int j;
    
    printf("Managers List:\n");
    for(i = 0; i < p_count; i++)			// for loop that prints out all users with manager access.
    {
        if (p[i].Access == 2 && p[i].Active == 1)
        {
            printf("ID [%d]: %s\n", p[i].personnelID,p[i].Name);
        }
    } 
    printf("Active Tasks:\n");
    for (j = 0; j < pr_count; j++)			// for loop that prints out all active tasks in the array.
    {
        if (pr[j].status == 1 || pr[j].status == 2)
        {
            printf("ID [%d]: %s", pr[j].projectID,pr[j].name);
        }
    }
    do{
    printf("Input a manager ID: ");		// user input for manager ID.
    scanf("%d", &ID);
    index = find_ID(p,ID,p_count);
    if(index != -1)
        {
            if (p[index].Access == 2)		// checks is user is actually a manager.
                jfer = 0;
        }
    }while(jfer == -1);

    do{
    printf("Input a project ID: ");		// user input for project ID.
    scanf("%d", &pID);
    index2 = find_projID(pr,pID,pr_count);
    }while(index2 == -1);

    pr[index2].assignID = p[index].personnelID;		// assigning of project to user.
    put_proj_in_file(pr, pr_count);
}

/*
	admin_log displays the menu that an Admin use is able to utilize, and allows manipulation of the differents vairables in the different structs.
	
	@param *count - the number of users in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param pr[] - gives access to the Project struct array allowing manipulation of the variables.
	@param p_count - the number of projects in the struct.
	@paam t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param t_count - the number of tasks in the struct.
	@param ID - user input for personnel ID.
	
	Internal variables:
	int choice - the option that the user opts for during the menu screen.
	int i - for loop counter.
	int manager_count - the number of users assigned the Manager role.
*/
void admin_log(int *count,
personnel p[],
Project pr[],
int p_count,
Task t[],
int t_count,
int ID)
{
    int choice = 0;
    int i;
    int manager_count = 0;
    while (choice != 8)
    {
    choice = 0;
    admin_screen();			// printing of admin screen.
    scanf("%d", &choice);	// user input for admin choice.
    if (choice == 1)		// choice for adding new user.
    {
        add_user(count, p);
        *count += 1;
        put_personnels_in_array(*count,p);
        export(3,*count);
    }
    else if (choice == 2)		// choice for updating user information.
    {
        update_user(p, *count);
        updateFile(p, *count);
        export(3,*count);
    }
    else if (choice == 3)		// choice for deleting user.
    {
       delete_user(p,*count,t,t_count,ID,pr,p_count);
       *count -=1;
       updateFile(p,*count);
       export(3,*count);
       
    }
    else if (choice == 4)		// choice for archiving user.
    {
        archive_user(p, *count);
        updateFile(p,*count);
        export(3,*count);
    }
    else if (choice == 5)		// choice for assigning a project.
    {
        for(i = 0; i < *count; i++)
            if (p[i].Access == 2)
                manager_count += 1;
        if(p_count > 0 && manager_count > 1)
        {
            assign_project(pr, p, *count, p_count);
        }
        else
            printf("Create a project or a manager first\n");
    }
    else if (choice == 6)				// choice for displaying tasks within and beyond schedule.
        count_sched(ID,t,t_count);
    else if (choice == 7)				// choice for displying the tasks of a user.
        count_tasks_of_user(ID,t,t_count);
    else if (choice == 8)
        printf("Logging Out\n");
    else
        printf("Invalid choice\n");
    }
}

/*
	show_personnel_list displays all the users in the struct.
	
	@param count - the number of users in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	
	Internal variables:
	int i - for loop counter.
*/
void show_personnel_list (int count,
personnel p[])
{
    int i;
    for (i = 0; i < count; i++)
    {
        if(p[i].Active == 1)
        printf("Id: %d: Name: %s\n",p[i].personnelID,p[i].Name);
    }
}

/*
	assign_task allocates a task to the specific user present in the struct.
	
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param task_count - the number of tasks in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param p_count - the number of users in the struct.
	
	Internal variables:
	int i - variable for find_task_in_array variable, and used for array indexing.
	int j - user input for personnel ID.
	int exist - control variable for do-while loop.
*/
void assign_task(Task t[],
int task_count,
personnel p[],
int p_count)
{
    int i;
    int j;
    int exist;
    do{
    i = find_task_in_array(t,task_count);
    }while(i == -1);

    do{
    printf("Input personnel ID: ");
    scanf("%d", &j);   
    exist = find_ID(p,j,p_count);
    }while(exist == -1);

    t[i].assignID = j;
}

/*
	compute_completion gives a value that shows the percentage of a given task's percentage to completion.
	
	@param task[] - gives access to the Task struct array allowing manipulation of the variables.
	@param task_count - the number of tasks in the struct.
	@param project_id - the ID associated with a given task.
	
	Internal variables:
	int i - for loop counter.
	float total - the number of all active tasks.
	float done - the number of completed tasks.
	float completion - the percentage of total completion of all tasks.
*/
float compute_completion(Task task[],
int task_count,
int project_id)
{   
int i; 
float total =0;
float done = 0; 
float completion;
    for (i =0; i < task_count; i++)		// for loop for counting tasks in the array.
    {
        if(project_id == task[i].projectID)		// comparing of user input to project ID.
        {
            if(task[i].status == 3)		// checks if task is done.
            {
                done += 1;		// number of completed tasks.
            }
            total += 1;		// total number of completed tasks.
        }
    }
    if (total > 0)					// computation of total completion of project.
        completion = done/total;
    else
        completion = 0;
    return completion * 100;
}

/*
	add_project adds a new project into the struct.
	
	@param L[] - gives access to the Project struct array allowing manipulation of the variables.
	@param count - the number of projects in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param personnel_count - the number of personnel in the struct.
	@param ID - the user input for personnel ID.
	
	Internal variables:
	int x - control variable for do-while loop.
	int id - temporary variable for the project ID.
    Project a- temporary varaible for project
    int i - for loop counter
    buffer - temporary variable to hold strings
    int replace - gets the string length to replace \n
    FILE *s - appends into projects.txt
*/
void add_project(Project l[],
int count,
personnel p[],
int personnel_count,
int ID)
{
    int x = 0;
    int id;
    Project a;
    int i;
    char buffer[BUFFERLENGTH];		// declaring of variable with predefined length.
    int replace;
    FILE *s;
    srand(time(0));

    do{
    id = rand()%9999; 		// generates random project ID with a max of 9999.
    x = 0;
    for(i = 0; i < count; i+= 1)
        if (l[i].projectID == id)
            x = 1;
    }while(x != 0);

    x = 0;
    a.projectID = id;		// appoints generated id to the actual struct array.

    do{
    x = 0;
    printf("Input project name: ");		// user input for project name.
    fflush(stdin);
    fgets(buffer, BUFFERLENGTH, stdin);
    for (i = 0; i < count; i++)
    {
        if(strcmp(buffer ,l[i].name) == 0)		// checks if project name is already taken.
        {
            printf("Project in SYS\n");
            x = 1;
        }
    }
    }while(strlen(buffer) > 40 || x == 1);

    replace = strlen(buffer);
    buffer[replace-1] = '\0';			// removes null value from the array.
    strcpy(a.name, buffer);


    do{
    printf("Input project status\n[1] Not started\n[2] In progress\n[3] Done\n");		// user input for project status.
    scanf("%d",&a.status);
    }while (a.status < 1 || a.status > 3);

    do{
        x = 0;
        printf("Input project description: ");		// user input for project description.
        fflush(stdin);
        fgets(buffer, BUFFERLENGTH, stdin);
    }while(strlen(buffer) > 100);

    replace = strlen(buffer);
    buffer[replace-1] = '\0';			// removes null value from the array.
    strcpy(a.description, buffer);
    
    a.assignID = ID;			// appoints ID to the actual struct array.

    s = fopen("project.txt", "a");		// opening of project.txt to append.
        if(count == 0)
        fprintf(s, "%d\n",a.projectID);
        else
        fprintf(s, "\n%d\n",a.projectID);		// printing of information into project.txt
        fprintf(s,"%s\n",a.name);
        fprintf(s,"%d\n",a.status);
        fprintf(s,"%d\n",0);
        fprintf(s,"%s\n",a.description);
        fprintf(s, "%d", a.assignID);
    fclose(s);								// closing of project.txt
}

/*
	days_in_month return the number of days there is in each month of the year.
	
	@param month - the user input for the month of the year.
	@param year - the user input for year.
	
	Internal variables:
	int days - the number of days in each month.
*/
int days_in_month(int month,
int year) 
{
    int days = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {	// checks months that only have 30 days.
        days = 30;
    } else if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {	// checks February if it's in a leap year.
            days = 29;
        } else {
            days = 28;
        }
    }
    return days;		// returns the number of days.
}

/*
	days_between_dates counts the number of between two given dates.
	
	@param date1 - the first date associated with date struct.
	@param date2 - the second date associated with date struct.
	
	Internal variables:
	int days - the number of days between dates.
*/
int days_between_dates(struct date date1,
struct date date2) 
{
    int days = 0;
    while (date1.year < date2.year || (date1.year == date2.year && date1.month < date2.month)) {		// checks if date1 falls behind date2.
        days += days_in_month(date1.month, date1.year);
        date1.month++;
        if (date1.month > 12) {
            date1.month = 1;
            date1.year++;
        }
    }
    days += date2.day - date1.day;		// counts the days between two given dates.
    return days;
}

/*
	add_task creates a new task with necessary details such as task name, start and end dates, description, and ID.
	
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param count - the number of tasks in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param p_count - the number of personnel in the struct.
	@param pr[] - gives access to the Project struct array allowing manipulation of the variables.
	@param pr_count - the number of projects in the struct.
	@param ID - the user input for user ID.
	
	Internal variables:
	int x - control variable for do-while loop.
	int id - temporary variable for storing taskID.
	char buffer - temporarily stores the string read from the "tasks" text file.
	int replace - variable used for array indexing.
	int i - for loop counter.
	int st_month - temporary variable for storing start month value.
	int st_day - temporary variable for storing start day value.
	int st_year - temporary variable for storing start year value.
	int ed_month - temporary variable for storing end month value.
	int ed_day - temporary variable for storing end day value.
	int ed_year - temporary variable for storing end year value.
    date date1 - struct variable for dates
    date date2 - struct variable for dates
    FILE *s - appends into tasks.txt
	int sda - for loop counter.
*/
void add_task(Task t[],
int count,
personnel p[],
int p_count,
Project pr[],
int pr_count,
int ID)
    {
        int x; 
        int id;
        srand(time(0));
        Task *a; 
        char buffer[BUFFERLENGTH];		// declaration of variable with predefined length.
        int replace;
        int i;
        int st_month;
        int st_day;
        int st_year;
        int ed_month;
        int ed_day;
        int ed_year;
        date date1;
        date date2;
        FILE *s;
        int sda;
        s = fopen("tasks.txt", "a");	// opening of tasks.txt to append.
        a = malloc(sizeof(Task));

        do
        {
            x = 0;
            id = rand()%9999;			// generates random id with a maximum of 9999.
            for (i = 0; i<count; i+= 1)
            {
                if(t[i].projectID == id)	// compares random id with project ID.
                x = 1;
            }
        } while (x != 0);
        a->taskID = id;		// appoints generated ID to the struct array.
            
        
        do{
        x = 0;
        printf("Input task name: ");		// user input for task name.
        fflush(stdin);
        fgets(buffer, BUFFERLENGTH, stdin);
        for (i = 0; i < count; i++)
        {
            if(strcmp(buffer ,t[i].name) == 0)		// checks if task is already in the struct array.
            {
                printf("Task in SYS\n");
                x = 1;
            }
            if (strlen(buffer)> 40)				// checks length of the task name.
            {
                printf("Name is too long\n");
                x = 1;
            }
        }
        }while(x == 1);
        replace = strlen(buffer);
        buffer[replace-1] = '\0';		// removes null value from the array.
        strcpy(a->name, buffer);

        a->sequence = count+1;

        do{
            x = 0;
            printf("Input task description: ");			// user input for task description.
            fflush(stdin);
            fgets(buffer, BUFFERLENGTH, stdin);
            if (strlen(buffer) > 100)
            {
                printf("Description is too long\n");		// checks length of the task description.
                x = 1;
            }
        }while(x == 1);
        replace = strlen(buffer);
        buffer[replace-1] = '\0';		// removes null value from the array.
        strcpy(a->description, buffer);


        
        do{
            x = 0;
            printf("Input project ID: ");		// user input for project ID.
            scanf("%d", &a->projectID);
            for (i=0; i < pr_count; i++)
            {
                if(pr[i].projectID == a->projectID)		// checks project ID already exists in the struct array.
                {
                    if(pr[i].assignID == ID)
                        x = 1;
                }
            }
            if (x != 1)
                printf("INVALID PROJECT ID! (DOES NOT EXIST OR ASSIGNED TO A DIFFERENT MANAGER)\n");
        }while(x == 0);
        x = 0;


        do{
            printf("Enter Start date in MM/DD/YYYY format: ");		// user input for start date.
            fflush(stdin);
            fgets(a->startdate, 11, stdin);
            if (strlen(a->startdate) == 10 && a->startdate[2] == '/' && a->startdate[5] == '/')
            {
                sscanf(a->startdate, "%2d/%2d/%4d", &st_month, &st_day, &st_year);
                if(st_month <= 12 && st_month >= 1)
                    {
                        if(st_day > 31)										// if statements to check if the given date is valid.
                        {
                            printf("Invalid day!\n");
                        }
                        else if (st_day == 31 && (st_month == 4 || st_month == 6 || st_month == 9 || st_month == 11))
                        {
                            printf("Invalid day!\n");
                        }
                        else if(st_month == 2 && ((st_day == 29 && st_year % 4 != 0) || st_day > 29))
                        {
                            printf("Invalid day\n");
                        }
                        else
                            x = 1;
                    }
                else 
                    printf("Invalid Month!\n");
            }
            else 
                printf("Invalid date format.\n");
        }while (x == 0);
        x = 0;

        do{
            x = 0;
            printf("Enter end date in MM/DD/YYYY format: ");		// user input for end date.
            fflush(stdin);
            fgets(a->enddate, 11, stdin);
            if (strlen(a->enddate) == 10 && a->enddate[2] == '/' && a->enddate[5] == '/')
            {
                sscanf(a->enddate, "%2d/%2d/%4d", &ed_month, &ed_day, &ed_year);
                if(ed_month <= 12 && ed_month >= 1)
                    {
                        if(ed_day > 31)						// if statements to check if the given date is valid.
                        {
                            printf("Invalid day!\n");
                        }
                        else if (ed_day == 31 && (ed_month == 4 || ed_month == 6 || ed_month == 9 || ed_month == 11))
                        {
                            printf("Invalid day!\n");
                        }
                        else if(ed_month == 2 && ((ed_day == 29 && ed_year % 4 != 0) || ed_day > 29))
                        {
                            printf("Invalid day\n");
                        }
                        else
                        {
                            if(st_year < ed_year)
                                x = 1;
                            else if (st_year == ed_year)
                                {
                                    if (st_month < ed_month)
                                        x = 1;
                                    else if (st_month == ed_month)
                                        {
                                            if (st_day <= ed_day)
                                                x = 1;
                                            else
                                                printf("End date earlier than start\n");
                                        }
                                    else 
                                        printf("End date earlier than start\n");
                                }
                            else   
                                printf("End date earlier than start\n");
                        }
                    }
                else 
                printf("Invalid Month.\n");
            }
            else
                printf("Invalid date format.\n");
        }while (x == 0);

        x = 0;

        date1.day = st_day;				// appoints the temporary variables to their struct array counterparts.
        date1.month = st_month;
        date1.year = st_year;
        date2.day = ed_day;
        date2.month = ed_month;
        date2.year = ed_year;

        a->totalDuration = days_between_dates(date1, date2);

        do{
            printf("What is the status?\n[1] Not Started\n[2] In Progress\n[3] Done\n");		// user input for task status.
            scanf("%d", &a->status);
        }while (a->status < 1 || a->status > 3);

        do{
            x = 0;
            printf("Assign Personnel to the task\n");			// user input for task personnel.
            scanf("%d", &a->assignID);

            for(sda = 0; sda < p_count; sda++)
                {
                    if (a->assignID == p[sda].personnelID)
                    {
                        x=1;
                    }
                }
        }while (x == 0);
 
        if(count == 0)
        fprintf(s, "%d\n",a->taskID);
        else
        fprintf(s, "\n%d\n",a->taskID);			// prints task details into the text file.
        fprintf(s,"%s\n",a->name);
        fprintf(s,"%d\n",a->sequence);
        fprintf(s,"%s\n",a->description);
        fprintf(s,"%d\n", a->projectID);
        fprintf(s,"%s\n",a->startdate);
        fprintf(s,"%s\n",a->enddate);
        fprintf(s,"%f\n",a->totalDuration);
        fprintf(s,"%d\n",a->status);
        fprintf(s,"%d",a->assignID);
    fclose(s);							// closing of tasks.txt
    free (a);

}

/*
	update_task rewrites a given task with new details provided by the user.
	
	@param count - the number of tasks in the struct.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param per[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param per_count - the number of active personnel in the struct.
	@param proj[] - gives access to the Project struct array allowing manipulation of the variables.
	@param pro_count - the number of projects in the struct.
	@param ID_of_manager - user input for the personnel ID that is a manager.
	
	Internal variables:
	int buf - variable for array indexing.
	int choice - the user input for the choice of action for the given task.
	int temp - temporary variable for user input for Project ID.
	int another_temp - 
	int st_month - temporary variable for storing start month value.
	int st_day - temporary variable for storing start day value.
	int st_year - temporary variable for storing start year value.
	int ed_month - temporary variable for storing end month value.
	int ed_day - temporary variable for storing end day value.
	int ed_year - temporary variable for storing end year value.
	int remove - variable for array indexing.
	int i - for loop counter.
	int valid - control variable for do-while loop.
	int x - control variable for do-while loop.
    date date1 - struct array for date.
    date date2 - struct array for date.
*/
void update_task(int count,
Task t[],
personnel per[],
int per_count,
Project proj[],
int pro_count,
int ID_of_manager)
{
    int buf,choice;
    int temp;
    int st_month;
    int st_day;
    int st_year;
    int ed_month;
    int ed_day;
    int ed_year;
    int remove;
    int i;
    int valid;
    int x;
    date date1;
    date date2;
    do{
    buf = find_task_in_array(t,count);
    } while (buf == -1);	
    
    printf("LOADING TASK\n");					// prints all task details into the terminal.
    printf("Name: %s\n",t[buf].name);	
    printf("Sequence: %d\n",t[buf].sequence);
    printf("Description: %s\n",t[buf].description);
    printf("Project: %d\n",t[buf].projectID);
    printf("Start Date: %s\n",t[buf].startdate);
    printf("End Date: %s\n",t[buf].enddate);
    printf("Status: %d\n", t[buf].status);
    printf("Assigned Personnel: %d\n", t[buf].assignID);

    sscanf(t[buf].startdate, "%2d/%2d/%4d", &st_month, &st_day, &st_year);			// scans dates from the struct array.
    sscanf(t[buf].enddate, "%2d/%2d/%4d", &ed_month, &ed_day, &ed_year);

    do {		// user input for which task data to update.
    do{
        printf("What would you like to update?\n[1] Name\n[2] Sequence\n[3] Description\n[4] Project ID\n[5] Start Date\n[6] End Date\n[7] Status\n[8] Assigned ID\n[9] Done Updating\n");
        scanf("%d",&choice);
    }while(choice < 1 || choice > 10);

    if (choice == 1)			// user choice for editing task name.
    {
        do{
            printf("Input Name: ");
            fflush(stdin);
            fgets(t[buf].name,BUFFERLENGTH,stdin);
        }while(strlen(t[buf].name) > 40);
        remove = strlen(t[buf].name);
        t[buf].name[remove - 1] = '\0'; 
    }
    else if (choice == 2)		// user choice for editing task sequence.
    {
        do{
            printf("Task Sequence: ");
            scanf("%d", &temp);
        }while(temp < 1 || temp > count + 1);

        for (i = 0; i < count; i++)
        {
            if (temp == t[i].sequence)
            {
                t[i].sequence = t[buf].sequence;
                t[buf].sequence = temp;
            }
        }
    }
    else if (choice == 3)			// user choice for editing task description.
    {
        do{
            printf("Task Description: ");
            fflush(stdin);
            fgets(t[buf].description,BUFFERLENGTH,stdin);
        }while(strlen(t[buf].description) > 100);
        remove = strlen(t[buf].description);
        t[buf].description[remove - 1] = '\0'; 
    }
    else if (choice == 4)			// user choice for editing project ID.
    {
        valid = 0;
        do {
            printf("Choose a new project ID: ");
            scanf("%d", &temp);
            for(i = 0; i < pro_count; i++)
            {
                if(temp == proj[i].projectID)
                    if (proj[i].assignID == ID_of_manager)
                        valid = 1;
            }
        if (valid == 0)
            printf("Invalid project ID!\n");

        } while (valid != 1);
        t[buf].projectID = temp;
    }
    else if (choice == 5)			// user choice for editing start date.
    {
        x = 0;
        do{
            printf("Enter Start date in MM/DD/YYYY format: ");
            fflush(stdin);
            fgets(t[buf].startdate, 11, stdin);
            if (strlen(t[buf].startdate) == 10 || t[buf].startdate[2] == '/' || t[buf].startdate[5] == '/')
            {
                sscanf(t[buf].startdate, "%2d/%2d/%4d", &st_month, &st_day, &st_year);
                if(st_month <= 12 && st_month >= 1)
                    {
                        if(st_day > 31)
                        {
                            printf("Invalid day!\n");
                        }
                        else if (st_day == 31 && (st_month == 4 || st_month == 6 || st_month == 9 || st_month == 11))
                        {
                            printf("Invalid day!\n");
                        }
                        else if(st_month == 2 && ((st_day == 29 && st_year % 4 != 0) || st_day > 29))
                        {
                            printf("Invalid day\n");
                        }
                        else
                        {
                            if(st_year < ed_year)
                                x = 1;
                                else if (st_year == ed_year)
                                    {
                                    if (st_month < ed_month)
                                        x = 1;
                                    else if (st_month == ed_month)
                                        {
                                        if (st_day <= ed_day)
                                            x = 1;
                                        else
                                            printf("End date earlier than start\n");
                                        }
                                    else 
                                        printf("End date earlier than start\n");
                                    }
                            else   
                                printf("End date earlier than start\n");
                        }
                    }
                else 
                    printf("Invalid Month!\n");
            }
            else 
                printf("Invalid date format.\n");
        date1.day = st_day;
        date1.month = st_month;
        date1.year = st_year;
        date2.day = ed_day;
        date2.month = ed_month;
        date2.year = ed_year;
        t[buf].totalDuration = days_between_dates(date1, date2);

        }while (x == 0);
    }
    else if (choice == 6)				// user choice for editing end date.
    {
        x = 0;
        do{
            printf("Enter End date in MM/DD/YYYY format: ");
            fflush(stdin);
            fgets(t[buf].enddate, 11, stdin);
            if (strlen(t[buf].enddate) == 10 || t[buf].enddate[2] == '/' || t[buf].enddate[5] == '/')
            {
                sscanf(t[buf].enddate, "%2d/%2d/%4d", &ed_month, &ed_day, &ed_year);
                if(ed_month <= 12 && ed_month >= 1)
                    {
                        if(ed_day > 31)
                        {
                            printf("Invalid day!\n");
                        }
                        else if (ed_day == 31 && (ed_month == 4 || ed_month == 6 || ed_month == 9 || ed_month == 11))
                        {
                            printf("Invalid day!\n");
                        }
                        else if(ed_month == 2 && ((ed_day == 29 && ed_year % 4 != 0) || ed_day > 29))
                        {
                            printf("Invalid day\n");
                        }
                        else
                        {
                            if(st_year < ed_year)
                                x = 1;
                            else if (st_year == ed_year)
                            {
                                if (st_month < ed_month)
                                    x = 1;
                                else if (st_month == ed_month)
                                {
                                    if (st_day <= ed_day)
                                        x = 1;
                                    else
                                    printf("End date earlier than start\n");
                                }
                                else 
                                    printf("End date earlier than start\n");
                            }
                            else   
                            printf("End date earlier than start\n");
                            }
                            }
                        else 
                            printf("Invalid Month!\n");
            }
            else 
                printf("Invalid date format.\n");
            
        date1.day = st_day;
        date1.month = st_month;
        date1.year = st_year;
        date2.day = ed_day;
        date2.month = ed_month;
        date2.year = ed_year;
        t[buf].totalDuration = days_between_dates(date1, date2);

            
        }while (x == 0);   
    }
    else if (choice == 7)			// user choice for editing task status.
    {
        do{
            printf("Input status\n[1] Not Started\n[2] In Progress\n[3] Done\n");
            scanf("%d", &t[buf].status);
        }while(t[buf].status < 1 || t[buf].status > 3);
    }
    else if (choice == 8)			// user choice for editing task personnel.
    {
        do {
            valid = 0;
            printf("Choose a new Personnel ID: ");
            scanf("%d", &temp);
            int counter = 0;
            while(counter < per_count)
            {
                if(temp == per[counter].personnelID)
                    {
                        valid = 1;
                    }
                    counter += 1;
            }

        if (valid == 0)
            printf("Invalid personnel ID!\n");

        } while (valid != 1);
        t[buf].assignID = temp;
    }
    }while (choice != 9);

    put_task_in_file(t,count);
}

/*
	show_project_details displays all the contents associated with a given project.
	
	@param p[] - gives access to the Project struct array allowing manipulation of the variables.
	@param proj_count - the number of projects in the struct.
	
	Internal variables:
	int id - user input for project ID.
	int index - control variable for do-while loop.
	int i - for loop counter.
*/
void show_project_details(Project p[],
int proj_count)
{
    int id;
    int index;
    int i;
    for (i =0; i < proj_count; i++)			// for loop for displaying all available projects.
    {
        if (p[i].status == 1 || p[i].status == 2)
        {
            printf("[%d]: %s\n", p[i].projectID, p[i].name);
        }
    }

    do{
    printf("Input a project ID: ");		// user input for project ID.
    scanf("%d",&id);
    index = find_projID(p, id, proj_count);
    } while(index == -1);

    printf("%d\n", p[index].projectID);			// print project details into the terminal.
    printf("%s", p[index].name);
    printf("%d\n", p[index].status);
    printf("%f\n", p[index].completion);
    printf("%s\n", p[index].description);
}

/*
	show_delayed_task displays tasks that are above the end date.
	
	@param task[] - gives access to the Task struct array allowing manipulation of the variables.
	@param task_count - the number of tasks in the struct.
	
	Internal variables:
	char date_str - string variable for storing the date of task.
	time_t t - variable for the time.
	struct tm date - struct vairable for the current time.
	int ed_month - temporary variable for storing end month of task.
	int ed_day - temporary variable for storing end day of task.
	int ed_year - temporary variable for storing end year of task.
	int c_month - temporary variable for storing start month of task.
	int c_day - temporary variable for storing start day of task.
	int c_year - temporary variable for storing start year of task.
	int i - for loop counter.
	struct date date1 - struct variable for the start date.
*/
void show_delayed_task(Task task[],
int task_count)
{
    char date_str[11];
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    sprintf(date_str, "%04d-%02d-%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);

    int ed_month;
    int ed_day;
    int ed_year;
    int c_month;
    int c_day;
    int c_year;
    int i;
    sscanf(date_str, "%4d-%2d-%2d", &c_year, &c_month, &c_day);
    struct date date1 = {c_day, c_month, c_year};

    printf("Delayed Tasks:\n");

    for (i =0; i < task_count; i++)
    {
        sscanf(task[i].enddate,"%2d/%2d/%4d", &ed_month, &ed_day,&ed_year);
        struct date date2 = {ed_day, ed_month, ed_year};
        if (compareDate(date1, date2) == 1 && task[i].status != 3)
        printf("ID:[%d] - %s\n", task[i].taskID, task[i].name);
    }
}

/*
	project_completion computes the percentage of much a given project is close to completion.
	
	@param proj[] - gives access to the Project struct array allowing manipulation of the variables.
	@pram proj_count - the number of projects in the struct.
	@param task[] - gives access to the Task struct array allowing manipulation of the variables.
	@param task_count - the number of tasks in the struct.
	
	Internal variables:
	int project_id - user inpur for project ID.
	int index - variable for find_projID function, array indexing, and control variable for do-while loop.
	float completion - percentage of project completion.

*/
void project_completion(Project proj[],
int proj_count,
Task task[],
int task_count)
{
    int project_id;
    int index;
    float completion;
    do{
    printf("Input project ID: ");
    scanf("%d",&project_id);
    index = find_projID(proj,project_id,proj_count);
    }while(index == -1);

    completion = compute_completion(task, task_count, project_id);
    proj[index].completion = completion;
    printf("Project is: %.2f%% done\n", completion);
}

/*
	manager displays the menu screen if the user is deemed to be a Manager, and allows manipulation of the different structs.
	
	@param l[] - gives access to the Project struct array allowing manipulation of the variables.
	@param *proj_count - the number of projects in the struct.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param *task_count - the number of tasks in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param personnel_count - the number of personnel in the struct.
	@param ID - user input for personnel ID.
	
	Internal variables:
	int choice - the user's input during the menu screen.
*/
void manager(Project l[],
int *proj_count,
Task t[],
int *task_count,
personnel p[],
int personnel_count,
int ID)
{
    int choice = 0;
    while (choice != 11)
    {
    manager_screen();
    scanf("%d", &choice);
    if (choice == 1)			// choice for adding projects.
    {  
        add_project(l,*proj_count,p,personnel_count,ID);
        count_project(proj_count);
        put_projects_in_array(*proj_count, l);
        export(2,*proj_count);
    }
    else if (choice == 2)		// choice for adding tasks.
    {
        if (*proj_count != 0)
        {
        add_task(t, *task_count,p,personnel_count,l,*proj_count,ID);
        count_task(task_count);
        put_tasks_in_array(*task_count, t);
        export(1,*task_count);
        }
        else
        printf("No Projects Available\n");
    }
    else if (choice == 3)		// choice for updating tasks.
    {
        if (*task_count > 0)
        {
        update_task(*task_count,t,p,personnel_count,l,*proj_count,ID);
        export(1,*task_count);
        }
        else 
        printf("Create a task first!\n");
    }
    else if (choice == 4)		// choice for showing personnel list.
    {
        show_personnel_list(personnel_count,p);
    }
    else if (choice == 5)		// choice for assigning tasks.
    {
        assign_task(t,*task_count,p,personnel_count);
        put_task_in_file(t,*task_count);
        export(1,*task_count);
    }
    else if (choice == 6)		// choie for showing project details.
    {
        show_project_details(l,*proj_count);
    }
    else if (choice == 7)		// choice for showing delayed tasks.
    {
        show_delayed_task(t,*task_count);
    }
    else if (choice == 8)		// choice for showing project completion.
    {
        project_completion(l,*proj_count,t,*task_count);
        put_proj_in_file(l,*proj_count);
        export(2,*proj_count);
    }
    else if (choice == 9)				// choice for counting tasks beyond and within schedule.
        count_sched(ID,t,*task_count);
    else if (choice == 10)							// choice for counting tasks of a user.
        count_tasks_of_user(ID,t,*task_count);
    else if (choice == 11)							// choice for going back to the main menu.
    {
        printf("Logging Out!\n");
    }
    else 
        printf("Invalid choice\n");
    }
}

/*
	is_within_dates determines whether or not a given date is between the start and end year of a project.
	
	@param start - the start date according to the date struct.
	@param end - the end date according to the date struct.
	@param current - the current date according to the date struct.
*/
int is_within_dates(date start,
date end,
date current)
{
 if (current.year < start.year || current.year > end.year) {
        return 0;
    }
    if (current.year == start.year && current.month < start.month) {
        return 0;
    }
    if (current.year == end.year && current.month > end.month) {
        return 0;
    }
    if (current.year == start.year && current.month == start.month &&
        current.day < start.day) {
        return 0;
    }
    if (current.year == end.year && current.month == end.month &&
        current.day > end.day) {
        return 0;
    }
    return 1;
}

/*
	sow_todays_task displays the task that is meant to be done on a given date(today).
	
	@param ID - user input for user ID.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables. 
	@param count - the number of tasks in the struct.
	
	Internal variables:
	time_t now - variable for the time.
	struct tm *p - struct variable for the local time.
	date today - struct array for date today.
	date start - struct array for start date.
	date end - struct array for end date.
	today.year - variable for today's year.
	today.month - variable for today's month.
	today.day - variable for today's day.
	int i - for loop counter.
*/
void show_todays_task(int ID,
Task t[],
int count)
{
    time_t now = time(NULL);
    struct tm *p = localtime(&now);
    date today,start,end;
    today.year = p->tm_year + 1900;
    today.month = p->tm_mon + 1;
    today.day = p->tm_mday;
    int i;
    for (i =0; i < count; i+= 1)
    {
        if (t[i].assignID == ID && t[i].status != 3)
        {
            sscanf(t[i].startdate,"%2d/%2d/%4d",&start.month,&start.day,&start.year);
            sscanf(t[i].enddate,"%2d/%2d/%4d",&end.month,&end.day,&end.year);
            if(is_within_dates(start,end,today) == 1)
                printf("[%d]: %s\n", t[i].taskID, t[i].name);
        }
    }
}

/*
	Show_All_Assigned_task displays all the allocated tasks for a given user.
	
	@param ID - user input for user ID.
	@param t[] - gives access to the Task struct allowing manipulation of the variables.
	@param count - the number of active tasks in the struct.
	
	Internal variables:
	int i - for loop counter.
    int counter - the number of assigned tasks.
*/
int Show_All_Assigned_task(int ID,
Task t[],
int count)
{
    int i = 0;
    int counter = 0;
    for (i = 0; i < count; i+=1)
    {
        if(t[i].assignID == ID) 
        {
            printf("[%d]: %s\n", t[i].taskID, t[i].name);
            counter += 1;
        }
    }
    return counter;
}

/*
	Update_Task_Status rewrites the status of a given task.
	
	@param ID- user input for user ID.
	@param t[] - gives access to the Task struct allowing manipulation of the variables.
	@param count - the number of active tasks in the struct.

	Internal variables:
	int task_index - variable for find_task_in_array function.
	int status - user input for task status.
	int the_assign - control variable for do-while loop, and used for array indexing.
*/
void Update_Task_Status(int ID, Task t[], int count)
{
    int task_index;
    int status;
    int the_assign = 0;

    if (Show_All_Assigned_task(ID,t,count) > 0)
    {
    do{
        task_index = find_task_in_array(t,count);
        if (task_index != -1)
            {
                if (t[task_index].assignID == ID)
                    the_assign = 1;
                else
                    printf("Not your task\n");
            }
    }while(the_assign != 1);

    do{
        printf("Input Status\n[1] Not Started\n[2] In Progress\n[3] Done\n");
        scanf("%d", &status);
    }while (status < 1 || status > 3);

    t[task_index].status = status;
    put_task_in_file(t,count);
    }
    else 
        printf("User has no tasks!\n");
}

/*
	compareDate differentiates the start and end dates of a project.
	
	@param date1 - the start date according to the date struct.
	@param date2 - the end date according to the date struct.
*/
int compareDate(date date1,
date date2) {
    if (date1.year < date2.year) {
        return -1;
    } else if (date1.year > date2.year) {
        return 1;
    } else {
        if (date1.month < date2.month) {
            return -1;
        } else if (date1.month > date2.month) {
            return 1;
        } else {
            if (date1.day < date2.day) {
                return -1;
            } else if (date1.day > date2.day) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

/*
	count_sched displays the tasks that are either within schedule or beyond schedule.
	
	@param ID - user input for user ID.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param count - the number of active tasks in the struct.
	
	Internal variables:
	int late - the number of tasks that are beyond schedule.
	int within - the number of tasks that are within schedule.
	int i - for loop counter.
	date date1 - struct array for date1.
	date date2 - struct array for date2.
	date date3 - struct array for date3.
	time_t now - variable for the current time.
	struct tm *p - struct variable for the local time.
	date1.year - variable for date1's year.
	date1.month - variable for date1's month.
	date1.day - variable for date1's day.
*/
void count_sched(int ID,
Task t[],
int count)
{
    int late= 0;
    int within = 0;
    int i = 0;
        date date1;
        date date2;
        date date3;
        time_t now = time(NULL);
        struct tm *p = localtime(&now);
        date1.year = p->tm_year + 1900;
        date1.month = p->tm_mon + 1;
        date1.day = p->tm_mday;
        for(i = 0; i < count; i+= 1)
        {
            if(t[i].assignID == ID && t[i].status != 3) 
            {
                sscanf(t[i].startdate,"%2d/%2d/%4d",&date2.month,&date2.day,&date2.year);
                sscanf(t[i].enddate,"%2d/%2d/%4d",&date3.month,&date3.day,&date3.year);
                if((is_within_dates(date2,date3,date1)) == 1)
                    within += 1;
                else if(compareDate(date1,date3) == 1) 
                    late += 1;               
            }
        }
        printf("[%d] Tasks within sched\n[%d] Tasks beyond sched\n\n",within, late);
}

/*
	count_tasks_of_user displays the amount of tasks assigned to a given user.
	
	@param ID - user input for user ID.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param count - the number of tasks in the struct.
	
	Internal variables:
	int count_of_tasks - the amount of tasks of a given user.
	int i - for loop counter.
*/
void count_tasks_of_user(int ID,
Task t[],
int count)
{
    int count_of_tasks = 0;
    int i;
    for(i =0; i<count;i+= 1)
        if(t[i].assignID == ID)
            count_of_tasks+=1;
    printf("User [%d] has %d tasks!\n",ID,count_of_tasks);
}

/*
	user displays the menu screen if the user is neither a Manager nor Admin, and allows access to contents of Task struct.
	
	@param ID - user input for user ID.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	@param task_count - the number of tasks in the struct.
	
	Internal variables:
	int choice - the option that the user opts in to select.
*/
void user(int ID,
Task t[],
int task_Count)
{
    int choice = 0;
    do{ 
    printf("[1] Show todays task\n[2] Show all assigned task\n[3] Update task status\n[4] Count tasks within and beyond schedule\n[5] Count tasks\n[6] Log out\n");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        show_todays_task(ID,t,task_Count);
        break;
    case 2:
        Show_All_Assigned_task(ID, t,task_Count);
        break;
    case 3:
         Update_Task_Status(ID, t, task_Count);
        break;
    case 4:
        count_sched(ID,t,task_Count);
        break;
    case 5:
        count_tasks_of_user(ID,t,task_Count);
        break;
    case 6:
        printf("Logging out\n");
        break;
    default:
        printf("Invalid choice\n");
    }
    } while(choice != 6);
}

/*
	inttoSTr transforms an int value into a string.
	
	@param l - string that utilizes typedef variable.
	@param val - value of int.
	
	Internal variables:
	int i - for loop counter.
	int num_to_append - end value of the given int.
	int len - length of the int value.
	int n - control variables for val parameter and while loop.
*/
void inttoSTr (string l,
int val)
{
int i;
int num_to_append;
int len = 0;
int n;
 
    n = val;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        num_to_append = val%10;
        val /= 10;
        l[len - (i+1)]= num_to_append + '0';
    }
}

/*
	clearbuffer removes all the contents inside a buffer.
	
	@param buffer - string that utilizes typedef variable.
	@param count - the number of characters inside array.
	
	Internal variables:
	int i - for loop counter.
*/
void clearbuffer(string buffer,
int count)
{
    int i = 0;
    for(i = 0; i <= count; i+= 1)
        buffer[i]='\0';
}

/*
	export allocates all the given information into text files, and properly terminates the program.
	
	@param action - string that utilizes typedef variable.
	@param count - the number of characters inside array.
	
	Internal variables:
	date date1 - struct array for date1;
	time_t now - variable for the current time.
	struct tm *p - struct variable for the local time.
	date1.year - variable for date1's year.
	date1.month - variable for date1's month.
	date1.day - variable for date1's day.
	string task - string variable for task.
	string personnel - string variable for personnel.
	string project - string variable for project.
	FILE *a - reads into tasks.txt, project.txt, and preloaded.txt
	FILE *b - writes into personnel
	int date_today - values associated with a given date.
	string strdate_today - string variable for the date today.
	int i - for loop counter.
	int counter - for loop counter, and control variable for if statement.
	char buffer - temporarily stores the string read from the "tasks" text file, and used for array idexing.
	int remove - variable for array indexing.
*/
void export (int action,
int count)
{
    date date1;
    time_t now = time(NULL);
    struct tm *p = localtime(&now);
    date1.year = p->tm_year + 1900;
    date1.month = p->tm_mon + 1;
    date1.day = p->tm_mday;
    string task = {"task_"};
    string personnel = {"personnel_"};
    string project = {"project_"};
    FILE *a,*b;
    int date_today;
    string strdate_today;
    clearbuffer(strdate_today,100);
    int i;
    int counter;
    date_today = ((date1.year*100+date1.month)*100 + date1.day);
    inttoSTr(strdate_today,date_today);
    strcat(strdate_today,".txt");
    char buffer[BUFFERLENGTH];
    int remove;
    if (action == 1)
    {  
        a = fopen("tasks.txt","r");
        strcat(task,strdate_today);
        b = fopen(task,"w");
        for (i = 0; i < count; i+= 1)
        {
            for (counter = 0; counter < 10; counter += 1)
            {
            fgets(buffer,BUFFERLENGTH,a);
            if((counter == 9 && i == count-1) == 0)
            {
            remove = strlen(buffer);
            buffer[remove - 1] = '\0';
            }
                if (counter == 0)
                    fprintf(b,"%s ", buffer);
                else if (counter == 9)
                    fprintf(b,"%s\n\n", buffer);
                else 
                    fprintf(b,"%s\n",buffer);
            clearbuffer(buffer,remove);
            }
            
        }
        fclose(a);
        fclose(b);
    }
    else if (action == 2)
    {
        a = fopen("project.txt","r");
        strcat(project,strdate_today);
        b = fopen(project,"w");
        for (i = 0; i < count; i+= 1)
        {
            for (counter = 0; counter < 6; counter += 1)
            {
                fgets(buffer,BUFFERLENGTH,a);
            if((counter == 5 && i == count-1) == 0)
            {
                remove = strlen(buffer);
                buffer[remove - 1] = '\0';
            }
                if (counter == 0)
                    fprintf(b,"%s ",buffer);
                else if (counter == 4)
                    fprintf(b,"%s\n\n",buffer);
                else if (counter != 5)
                    fprintf(b,"%s\n",buffer);
                }
        }
        fclose(a);
        fclose(b);
    }
        else if (action == 3)
    {
        a = fopen("preloaded.txt","r");
        strcat(personnel,strdate_today);
        b = fopen(personnel,"w");
        for (i = 0; i < count; i+= 1)
        {
            for (counter = 0; counter < 5; counter += 1)
                {
                fgets(buffer,BUFFERLENGTH,a);
            if((counter == 4 && i == count-1) == 0)
            {
                remove = strlen(buffer);
                buffer[remove - 1] = '\0';
            }
                if (counter == 0)
                    fprintf(b,"%s ",buffer);
                else if (counter == 4)
                    fprintf(b,"%s\n\n",buffer);
                else
                    fprintf(b,"%s\n",buffer);
                }
        }
        fclose(a);
        fclose(b);
    }
}

/*
	main_menu displays the initial start-up screen of the program which asks for Nameame and Password.
	
	@param *count - the number of personnel in the struct.
	@param p[] - gives access to the personnel struct array allowing manipulation of the variables.
	@param l[] - gives access to the Project struct array allowing manipulation of the variables.
	@param *proj_count - the number of projects in the struct.
	@param *task_count - the number of tasks in the struct.
	@param t[] - gives access to the Task struct array allowing manipulation of the variables.
	
	Internal variables:
	int index - variable for verify_login function, and used for array indexing.
	int TName - user input for name.
	int TPassword - user input for password.
    FILE *a - reads into tasks.txt
    FILE *b - reads into project.txt
    FILE *c - reads into preloaded.txt
*/
int main_menu(int *count,
personnel p[],
Project l[],
int * proj_count,
int *task_count,
Task t[])
    {
        int index;
        char TName[LENGTH]; 
        char TPassword[LENGTH];
        FILE *a;
        FILE *b;
        FILE *c;
        if ((a = fopen("tasks.txt", "r")))
        {
            fclose(a);
            export(1,*task_count);
        }
        if ((b = fopen("project.txt", "r")))
        {
            fclose(b);
            export(2,*proj_count);
        }
        if ((c = fopen("preloaded.txt", "r")))
        {
            fclose(c);
            export(3,*count);
        }
        do{
        printf("Username: ");
        scanf("%s", TName);
        printf("Password: ");
        scanf("%s", TPassword);
        
        index = verify_login(*count, p, TName, TPassword);
        if (index != -1 && p[index].Active != 2)
        {
            if (p[index].Access == 1)
                admin_log(count, p, l, *proj_count, t, *task_count, p[index].personnelID);
            else if (p[index].Access == 2)
                manager(l,proj_count,t,task_count,p,*count,p[index].personnelID);
            else
                user(p[index].personnelID, t, *task_count);
        }   
        }while(1);
    }

int main ()
{
    FILE *f;		// declaration of File pointers.
    FILE *d; 
    FILE *p;
    FILE *t;
    int personnel_count;
    int project_count = 0;
    Project projects[100];
    personnel personnels[100];
    Task tasks[100];
    int task_count = 0;
    if(!(f = fopen("preloaded.txt", "r")))		// checks if preloaded.txt exists.
    {
        fclose(f);
        d = fopen("preloaded.txt", "w");		// opens preloaded.txt to write the start information.
        fputs("0000\n", d);
        fputs("a\n", d);
        fputs("a\n", d);
        fputs("1\n", d);
        fputs("1", d);
        fclose(d);					// closing of preloaded.txt
    }
        fclose(f); 
    count_personnel(&personnel_count);
    if((p = fopen("project.txt", "r")))
        {
            fclose(p);
            count_project(&project_count);
            put_projects_in_array(project_count, projects);
        }

    
    if((t = fopen("tasks.txt", "r")))
    {
        fclose(t);
        count_task(&task_count);
        put_tasks_in_array(task_count, tasks);
    }

    put_personnels_in_array (personnel_count, personnels);
    main_menu(&personnel_count,personnels,projects,&project_count,&task_count,tasks);

    return 0;
}
