#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<time.h>
#include <stdlib.h>
#include <windows.h>
#include "headerfile.h"


int main()
{
	int command_value = 0;
	struct user *user_head;
	user_head = (struct user*)malloc(sizeof(struct user));
	user_head->next = NULL;
	struct message *message_head;
	message_head = (struct message*)malloc(sizeof(struct message));
	message_head->next = NULL;
	FILE *usersfile;
	if ((usersfile = fopen("usersinformation.txt", "r")))
	{
		fclose(usersfile);
		Read_Users_Information_From_File(user_head);
	}
	FILE *messagesfile;
	if ((messagesfile = fopen("messagesinformation.txt", "r")))
	{		
		fclose(messagesfile);
		Read_Messages_Information_From_File(message_head);
	}
	printf("Welcome to UT messenger \n");
	printf("Please enter your command \n");
	printf("************ \n");
	while (TRUE)
	{
		command_value = Command_Menu(user_head, message_head);
		if (command_value == FALSEE || command_value == LOGOUT)/*it means the command is FALSEE*/
			continue;/*go to the start of while*/
		else if (command_value == CLOSE)
			break;
	}
	printf("Close successfully \n");
	if(user_head->next != NULL)
		Save_Users_Information(user_head);
	if(message_head->next != NULL)
		Save_Messages_Information(message_head);
	Free_Users_Node(user_head);
	Free_Messages_Node(message_head);
}/*end of main function*/

/*divided command functions to check command , return FALSEE means can't allocate memory or vlaue is wrong*/
int Command_Menu(struct user *user_head, struct message *message_head)
{
	int counter = 0, command_value = 0;
	char **command;
	command = Get_Command(&counter);
	if (counter == 5)
	{
		printf("Wrong input \n");
		free(command[0]);
		free(command[1]);
		free(command[2]);
		free(command[3]);
		free(command[4]);
		while (getchar() != '\n');
		return FALSEE;
	}
	else if (**command == '\0')
	{
		return FALSEE;
	}
	else
	{
		command_value = Check_Command_First_Menu(command, counter, user_head, message_head);
	}
	if (command_value == FALSEE)
	{
		printf("Wrong input \n");
		return FALSEE;
	}
	else if (command_value == LOGOUT)
	{
		return LOGOUT;
	}
	else if (command_value == CLOSE)
	{
		return CLOSE;
	}
	return 0;
}/*end of Command_Menu function*/

/*get command from user*/
char ** Get_Command(int* counter)
{
	char **command;
	int i = 1, j = 0;
	command = (char**)malloc(MAX_COMMAND*(sizeof(char*)));
	if (*command == NULL)/*can not allocate memory to get command*/
	{
		printf("Can't allocate the memory \n");
		return NULL;
	}
	else
	{/*allocate memory and get command from user*/
		printf(">> ");
		do
		{
			i = 1;
			*(command + j) = (char*)malloc(i * sizeof(char));
			while (command[j][i - 2] != ' ' && command[j][i - 2] != '\n')
			{
				*((*(command + j)) + i - 1) = getc(stdin);
				i++;
				*(command + j) = (char*)realloc(*(command + j), i * sizeof(char));
			}
			if (command[j][i - 2] != '\n' && i >= 3)
			{
				command[j][i - 2] = '\0';
			}
			j++;
		} while (j < MAX_COMMAND && command[j - 1][i - 2] != '\n');
		command[j - 1][i - 2] = '\0';
		(*counter) = j;
		return command;
	}
}/*end of Get_Command function*/

/*check the command and pass it , first command*/
int Check_Command_First_Menu(char **command, int counter, struct user *user_head, struct message *message_head)
{
	int check = 0, i = 0;
	if ((strcmp(*command, "signup")) == 0 && counter == 4)
	{
		if ((Check_Command_Signup(command, user_head)) == FALSEE)
			return FALSEE;
		else
		{
			struct user *new_user = Signup(command, user_head);
			if (new_user == NULL)
				return FALSEE;
			while (TRUE)
			{
				check = Second_Command_Menu_Of_Signup_User(user_head, new_user, message_head);
				if (check == CLOSE)
					return CLOSE;
				else if (check == LOGOUT)
					return LOGOUT;
			}
		}
	}
	else if (strcmp(*command, "login") == 0 && counter == 3)
	{
		struct user *current_user = Check_Command_Login(command, user_head);
		if (current_user == NULL)
		{
			return FALSEE;
		}
		else
		{
			Show_Unread_Message(current_user, message_head);
			while (TRUE)
			{
				check = Second_Command_Menu_Of_Signup_User(user_head, current_user, message_head);
				if (check == CLOSE)
					return CLOSE;
				else if (check == LOGOUT)
					return LOGOUT;
			}
		}
	}
	else if (strcmp(*command, "close") == 0 && counter == 1)
	{
		return CLOSE;
	}
	else
	{
		while (i<counter)
		{
			free(command[i]);
			i++;
		}
		return FALSEE;
	}
	return LOGOUT;
}/*end of Check_Command_First_Menu function*/

/*ckeck the signup command*/
int Check_Command_Signup(char **command, struct user *user_head)
{
	 if ((Ckeck_Username_Exist(command[1], user_head)) == TRUE)
	 {
		 printf("Username exist\n");
		 free(command[0]);
		 free(command[1]);
		 free(command[2]);
		 free(command[3]);
		 return FALSEE;
	 }
	 else if ((Check_Password_Value(command[2])) == FALSEE || (Check_Phone_Number_Value(command, user_head)) == FALSEE)
	 {
		 free(command[0]);
		 free(command[1]);
		 free(command[2]);
		 free(command[3]);
		 return FALSEE;
	 }
	 else
	 {
		 return TRUE;
	 }
}/*end of Check_Command_Signup function*/

 /*check the password value , check the length of password , when length of paasword is true return true and else return FALSEE , get *command*/
int Check_Password_Value(char *command)
{
	 if ((strlen(command)) < MIN_LENGTH_OF_PASSWORD)/*ckeck the length of password*/
	 {
		 printf("short password length \n");
		 return FALSEE;
	 }
	 else
	 {
		 return TRUE;
	 }
 }/*end of Check_Password_Value function*/

/*check username exist or no , when username exists return true and if username doesn't exist return FALSEE */
 int Ckeck_Username_Exist(char *command, struct user *user_head)
 {
	 struct user *ptr = user_head->next;
	 while (ptr != NULL)/*navigate in linked list to check the username exist or no*/
	 {
		 if (strcmp(ptr->username, command) == 0)
		 {/*username exist*/
			 return TRUE;
		 }
		 else
		 {/*go to next node to check*/
			 ptr = ptr->next;
		 }
	 }
	 return FALSEE;/*username does't exist*/
 }/*end of Ckeck_Username_Exist function*/

/*check the phone number to signup , check phone number to signup*/
 int Check_Phone_Number_Value(char **command, struct user *user_head)
 {
	 int i = 0, phone_number_exist = 0;
	 if(command[3][0]!='0')
	 {/*user doesn't use 0 at the first of phone number*/
		 printf("Phone number must start with 0 \n");
		 return FALSEE;
	 }
	 else if ((strlen(command[3])) != LENGTH_OF_PHONE_NUMBER)
	 {/*length of phone number is FALSEE*/
		 printf("Phone number must be 11 character \n");
		 return FALSEE;
	 }
	 while (i < LENGTH_OF_PHONE_NUMBER)
	 {
		 if ((((int)(command[3][i])) - ((int)('0'))) < 0 || (((int)(command[3][i])) - ((int)('0'))) > 9)
		 {/*user insert char instead of number*/
			 printf("Must use number in phone number \n");
			return FALSEE;
		 }
		 i++;
	 }
	 phone_number_exist = Check_Phone_Number_Exist(command, user_head);
	 if (phone_number_exist == TRUE)
	 {/*phone number exists before*/
		 printf("Phone number exists \n");
		 return FALSEE;
	 }
	 else
	 {
		 return TRUE;
	 }
 }/*end of Check_Phone_Number_Value function*/

/*check phone number exists or no , if exists return true else return FALSEE*/
 int Check_Phone_Number_Exist(char **command, struct user *user_head)
 {
	 struct user *ptr = user_head->next;
	 struct user *temp = NULL;
	 while (ptr != NULL)/*navigate in linked list to check the phone number exist or no*/
	 {
		 if ((strcmp(ptr->phone_number, command[3])) == 0)
		 {/*phone number exist*/
			 return TRUE;
		 }
		 else
		 {/*go to next node to check*/
			 ptr = ptr->next;
		 }
	 }
	 return FALSEE;/*phone number does't exist*/
 }/*end of Check_Phone_Number_Exist function*/

/*sign up user , if can't allocate memory return NULL, then return address of node of new user*/
 struct user* Signup(char **command, struct user* user_head)
 {
	 struct user *ptr = (struct user*)malloc(sizeof(struct user));
	 if (ptr == NULL)
	 {
		 printf("OVERFLOW\n");
		 free(command[0]);
		 free(command[1]);
		 free(command[2]);
		 free(command[3]);
		 return NULL;
	 }
	 else
	 {
		 ptr = Add_User(command, user_head);
		 printf("Sign up successfullly \n");
		 return ptr;
	 }
 }/*end of Signup function*/

/*add user to struct*/
 struct user* Add_User(char **command, struct user* user_head)
 {
	 char* date_with_time = (char*)malloc(sizeof(char)*SIZE_OF_DATE_STRING);
	 struct user *newnode = (struct user*)malloc(sizeof(struct user));
	 newnode->next = NULL;
	 newnode->phone_number = (char*)malloc(sizeof(char)*((strlen(command[3])) + 1));
	 memcpy(newnode->phone_number, command[3], strlen(command[3]) + 1);
	 free(command[3]);
	 newnode->password = (char*)malloc(sizeof(char)*((strlen(command[2])) + 1));
	 memcpy(newnode->password, command[2], strlen(command[2]) + 1);
	 free(command[2]);
	 newnode->username = (char*)malloc(sizeof(char)*((strlen(command[1])) + 1));
	 memcpy(newnode->username, command[1], strlen(command[1]) + 1);
	 free(command[1]);
	 free(command[0]);
	 Insert_Time(date_with_time);
	 newnode->signup_date = (char*)malloc(sizeof(char)*SIZE_OF_DATE_STRING);
	 memcpy(newnode->signup_date, date_with_time, strlen(date_with_time) + 1);
	 free(date_with_time);
	 if (user_head->next == NULL)
		 user_head->next = newnode;
	 else
	 {
		 struct user *lastUserNode = user_head;
		 while (lastUserNode->next != NULL)
		 {
			 lastUserNode = lastUserNode->next;
		 }
		 lastUserNode->next = newnode;
	 }
	 return newnode;
 }/*end of Add_User function*/

/*divided command functions to check command for second menu*/
 int Second_Command_Menu_Of_Signup_User(struct user *user_head , struct user *new_user, struct message *message_head)
 {
	 int counter = 0, command_value = 0;
	 char **command;
	 command = Get_Command(&counter);
	 if (counter == 5)
	 {
		 printf("Wrong input \n");
		 free(command[0]);
		 free(command[1]);
		 free(command[2]);
		 free(command[3]);
		 free(command[4]);
		 while (getchar() != '\n');
		 return FALSEE;
	 }
	 else if (**command == '\0')
	 {
		 return FALSEE;
	 }
	 else
	 {
		 command_value = Check_Command_Second_Menu_Of_Signup_User(command, counter, user_head, new_user, message_head);
	 }
	 if (command_value == FALSEE)
	 {
		 printf("Wrong input \n");
		 return FALSEE;
	 }
	 else if (command_value == CLOSE)
		 return CLOSE;
	 else if (command_value == LOGOUT)
		 return LOGOUT;
	 return 0;
 }/*end of Second_Command_Menu_Of_Signup_User function*/

 /*check command of signup user , if command is wrong return FALSEE , if command is close return close , command & counter & user_head & new_user & message_head*/
int Check_Command_Second_Menu_Of_Signup_User(char **command, int counter, struct user *user_head, struct user *new_user, struct message *message_head)
{
	if (strcmp(*command, "close") == 0 && counter == 1)
	{
		free(command[0]);
		return CLOSE;
	}
	else if (strcmp(*command, "logout") == 0 && counter == 1)
	{
		printf("Logout successfully \n");
		free(command[0]);
		return LOGOUT;
	}
	else if (strcmp(*command, "edit_username") == 0 && counter == 2)
	{
		Edit_Username(command, user_head, new_user);
		return TRUE;
	}
	else if (strcmp(*command, "edit_pass") == 0 && counter == 2)
	{
		Edit_Pass(command, new_user);
		return TRUE;
	}
	else if (strcmp(*command, "info") == 0 && counter == 1)
	{
		Info(user_head);
		free(command[0]);
		return TRUE;
	}
	else if (strcmp(*command, "about") == 0 && counter == 2)
	{
		About(command, user_head);
		return TRUE;
	}
	else if (strcmp(*command, "send") == 0 && counter == 3)
	{
		Send_Message(command, user_head, new_user, message_head);
		return TRUE;
	}
	else if (strcmp(*command, "show_sent") == 0 && counter == 2)
	{
		Show_Sent_Message(command, user_head, new_user, message_head);
		return TRUE;
	}
	else if (strcmp(*command, "show_received") == 0 && counter == 2)
	{
		Show_Received_Message(command, user_head, new_user, message_head);
		return TRUE;
	}
	else
	{
		int i = 0;
		while (i<counter)
		{
			free(command[i]);
			i++;
		}
		return FALSEE;
	}
}/*end of Check_Command_Second_Menu_Of_Signup_User function*/

/*edit username , command & user_head & new_user*/
void Edit_Username(char **command,struct user* user_head,struct user* new_user)
{
	if ((Ckeck_Username_Exist(command[1], user_head)) == TRUE)
	{
		printf("Your new username exists \n");
		return;
	}
	free(new_user->username);
	new_user->username = (char*)malloc(sizeof(char)*((strlen(command[1])) + 1));
	memcpy(new_user->username, command[1], strlen(command[1]) + 1);
	free(command[1]);
	free(command[0]);
	printf("Edit username successsfully \n");
}/*end of Edit_Username function*/

/*edit password , command & new_user*/
void Edit_Pass(char **command,struct user* new_user)
{
	if ((Check_Password_Value(command[1])) == TRUE)		
	{
		if ((strcmp(command[1], new_user->password)) == 0)
		{
			printf("Your password is same to your last password \n");
			free(command[1]);
			free(command[0]);
			return;
		}
		else
		{
			free(new_user->password);
			new_user->password = (char*)malloc(sizeof(char)*((strlen(command[1])) + 1));
			memcpy(new_user->password, command[1], strlen(command[1]) + 1);
			free(command[1]);
			free(command[0]);
			printf("Your password change successfully \n");
			return;
		}
	}
	else
	{
		return;
	}
}/*end of Edit_Pass function*/

/*ckeck the login command , if can log in return address of user node else return NULL*/
struct user* Check_Command_Login(char **command, struct user *user_head)
{
	struct user *node_of_user = Find_Username(command, user_head);
	if (node_of_user == NULL)
	{
		printf("Username doesn't exist \n");
		free(command[2]);
		free(command[1]);
		free(command[0]);
		return NULL;
	}
	if ((Check_Password_Value(command[2])) == FALSEE)
	{
		free(command[2]);
		free(command[1]);
		free(command[0]);
		return NULL;
	}
	else
	{
		if ((strcmp(node_of_user->password, command[2])) == 0)
		{
			printf("Login successfully \n");
			return node_of_user;
		}
		printf("Wrong password \n");
		free(command[2]);
		free(command[1]);
		free(command[0]);
		return NULL;
	}
}/*end of Check_Command_Login function*/

/*find username node , if can find node of username return address of user node else return NULL*/
struct user* Find_Username(char **command, struct user *user_head)
{
	struct user *temp = user_head;
	if (temp->next == NULL)
	{
		return NULL;
	}
	temp = temp->next;
	while (temp != NULL)
	{
		if ((strcmp(temp->username,command[1]))==0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}/*end of Find_Username function*/

/*save time in char* of strcut*/
void Insert_Time(char *date_with_time)
{
	time_t rawtime;
	struct tm *info;
	char buffer[SIZE_OF_DATE_STRING];
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(buffer, SIZE_OF_DATE_STRING, "%Y/%m/%d-%H:%M", info);
	strcpy(date_with_time, buffer);
}/*end of Insert_Time function*/

/*info , get user_head , print username of users*/
void Info(struct user *user_head)
{
	user_head = user_head->next;
	if (user_head == NULL)
	{
		printf("Empty \n");
		return;
	}
	while (user_head != NULL)
	{
		printf("%s \n", user_head->username);
		user_head = user_head->next;
	}
}/*end of Info function*/

/*about print informatoin of that user , get command & user_head*/
void About(char **command, struct user* user_head)
{
	struct user *node_of_user = Find_Username(command, user_head);
	if (node_of_user == NULL)
	{
		printf("Username is wrong \n");
	}
	else
	{
		printf("Username: %s \n", node_of_user->username);
		printf("Phone number: %s \n", node_of_user->phone_number);
	}
	free(command[1]);
	free(command[0]);
}/*end of About function*/

 /*send message , get : command & user_head & new_user & message_head*/
void Send_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head)
{
	if (Ckeck_Username_Exist(command[1],user_head) == FALSEE)
	{
		printf("Username to send your message doesn't exist \n");
		return;
	}
	else
	{
		Add_Message(command, new_user, message_head);
		printf("Send message successfully \n");
	}
}/*end of Send_Message function*/

/*make node at the end of message linked list and insert information in that , get command & new_user & message_head*/
void Add_Message(char **command, struct user *new_user, struct message *message_head)
{
	int i = 0;
	char* date_with_time = (char*)malloc(sizeof(char)*SIZE_OF_DATE_STRING);
	struct message *newnode = (struct message*)malloc(sizeof(struct message));
	newnode->next = NULL;
	newnode->sender_username = (char*)malloc(sizeof(char)*((strlen(new_user->username)) + 1));
	memcpy(newnode->sender_username, new_user->username, strlen(new_user->username) + 1);
	newnode->receiver_username = (char*)malloc(sizeof(char)*((strlen(command[1])) + 1));
	memcpy(newnode->receiver_username, command[1], strlen(command[1]) + 1);
	newnode->message_content = (char*)malloc(sizeof(char)*((strlen(command[2])) + 1));
	memcpy(newnode->message_content, command[2], strlen(command[2]) + 1);
	free(command[2]);
	free(command[1]);
	free(command[0]);
	Insert_Time(date_with_time);
	newnode->submit_date = (char*)malloc(sizeof(char)*SIZE_OF_DATE_STRING);
	memcpy(newnode->submit_date, date_with_time, strlen(date_with_time) + 1);
	free(date_with_time);
	if (message_head->next == NULL)
	{
		message_head->next = newnode;
	}
	else
	{
		struct message *lastMessageNode = message_head;
		while (lastMessageNode->next != NULL)
		{
			lastMessageNode = lastMessageNode->next;
			i++;
		}
		lastMessageNode->next = newnode;
	}
	newnode->yekta = (i + 1);
	newnode->flag = UNREAD;
}/*end of Add_Message function*/

/*show sent messages , if messages is unread from receiver show in other color , get : command & user_head & new_user & message_head*/
void Show_Sent_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head)
{
	if (Ckeck_Username_Exist(command[1], user_head) == FALSEE)
	{
		printf("That username doesn't exist \n");
	}
	else
	{
		if (Print_Sent_Message(command, user_head, new_user, message_head) == FALSEE)
		{
			printf("Empty \n");
		}
	}
	free(command[1]);
	free(command[0]);
}/*end of Show_Sent_Message function*/

/*print sent messages , return true if message exist and if message doesn't exist return FALSEE , get : command & user_head & new_user & message_head*/
int Print_Sent_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head)
{
	int check = 0;
	struct message *temp = message_head;
	if (temp->next == NULL)
	{
		return FALSEE;
	}
	temp = temp->next;
	while (temp != NULL)
	{
		if ((strcmp(temp->receiver_username, command[1])) == 0 && (strcmp(temp->sender_username, new_user->username)) == 0)
		{
			check = 1;
			if (temp->flag == UNREAD)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			}
			printf("message to: %s \n", temp->receiver_username);
			printf("in: %s \n", temp->submit_date);
			printf("%s \n", temp->message_content);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}
		temp = temp->next;
	}
	if (check == 0)
	{
		return FALSEE;
	}
	else
	{
		return TRUE;
	}
}/*end of Print_Sent_Message function*/

 /*show received messages , get : command & user_head & new_user & message_head*/
void Show_Received_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head)
{
	if (Ckeck_Username_Exist(command[1], user_head) == FALSEE)
	{
		printf("That username doesn't exist \n");
	}
	else
	{
		if (Print_Received_Message(command, user_head, new_user, message_head) == FALSEE)
		{
			printf("Empty \n");
		}
	}
	free(command[1]);
	free(command[0]);
}/*end of Show_Received_Message function*/

 /*print received messages , return true if message exist and if message doesn't exist return FALSEE , get : command & user_head & new_user & message_head*/
int Print_Received_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head)
{
	int check = 0;
	struct message *temp = message_head;
	if (temp->next == NULL)
	{
		return FALSEE;
	}
	temp = temp->next;
	while (temp != NULL)
	{
		if ((strcmp(temp->sender_username, command[1])) == 0 && (strcmp(temp->receiver_username, new_user->username)) == 0)
		{
			check = 1;
			printf("message from: %s \n", temp->receiver_username);
			printf("in: %s \n", temp->submit_date);
			printf("%s \n", temp->message_content);
			if (temp->flag == UNREAD)
			{
				temp->flag = READ;
			}
		}
		temp = temp->next;
	}
	if (check == 0)
	{
		return FALSEE;
	}
	else
	{
		return TRUE;
	}
}/*end of Print_Received_Message function*/

/*show unread messages when user login , get: current_user & message_head*/
void Show_Unread_Message(struct user *current_user, struct message *message_head)
{
	int check = 0;
	struct message *temp = message_head;
	temp = temp->next;
	while (temp != NULL)
	{
		if ((strcmp(temp->receiver_username, current_user->username)) == 0 && temp->flag == UNREAD)
		{
			if (check == 0)
			{
				printf("Unread messages: \n");
				check = 1;
			}
			printf("message from: %s ", temp->sender_username);
			printf("in: %s \n", temp->submit_date);
		}
		temp = temp->next;
	}
}/*end of Show_Unread_Message function*/

/*free users node and users char* */
void Free_Users_Node(struct user *user_head)
{
	struct user* tmp;
	while (user_head->next != NULL)
	{
		tmp = user_head;
		user_head = user_head->next;
		free(tmp->next->username);
		free(tmp->next->password);
		free(tmp->next->phone_number);
		free(tmp->next->signup_date);
		free(tmp);
	}
}/*end of Free_Users_Node function*/

 /*free messages node and messages char* */
void Free_Messages_Node(struct message *message_head)
{
	struct message* tmp;
	while (message_head->next != NULL)
	{
		tmp = message_head;
		message_head = message_head->next;
		free(tmp->next->receiver_username);
		free(tmp->next->sender_username);
		free(tmp->next->message_content);
		free(tmp->next->submit_date);
		free(tmp);
	}
}/*end of Free_Messages_Node function*/

 /*save information of users struct in file*/
void Save_Users_Information(struct user *user_head)
{
	struct user *put = user_head->next;
	FILE * fileptr = fopen("usersinformation.txt", "w");
	if (fileptr == NULL)
	{
		printf("Error can't save users information in file \n");
		return;
	}
	while (put != NULL) 
	{
		fprintf(fileptr, "%s ", put->username);
		fprintf(fileptr, "%s ", put->password);
		fprintf(fileptr, "%s ", put->phone_number);
		fprintf(fileptr, "%s", put->signup_date);
		fprintf(fileptr, "\n");
		put = put->next;
	}
	fclose(fileptr);
}/*end of Save_Users_Information function*/

 /*save information of messages struct in file*/
void Save_Messages_Information(struct message *message_head)
{
	struct message *put = message_head->next;
	FILE * fileptr = fopen("messagesinformation.txt", "w");
	if (fileptr == NULL)
	{
		printf("Error can't save messages information in file \n");
		return;
	}
	while (put != NULL)
	{
		fprintf(fileptr, "%s ", put->sender_username);
		fprintf(fileptr, "%s ", put->receiver_username);
		fprintf(fileptr, "%s ", put->message_content);
		fprintf(fileptr, "%s ", put->submit_date);
		fprintf(fileptr, "%d ", put->yekta);
		fprintf(fileptr, "%d", put->flag);
		fprintf(fileptr, "\n");
		put = put->next;
	}
	fclose(fileptr);
}/*end of Save_Messages_Information function*/

/*read users information from file and put in nodes*/
void Read_Users_Information_From_File(struct user *user_head)
{
	int i = 0, end = 0;
	FILE *filePointer;
	char in;
	filePointer = fopen("usersinformation.txt", "r");
	while (TRUE)
	{
		i = 0;
		char *get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			if (((int)(in)) == -1)
			{
				end = 1;
				free(get_from_file);
				break;
			}
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		if (end == 1)
			break;
		get_from_file[i] = '\0';
		struct user* current = user_head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = (struct user*)malloc(sizeof(struct user));
		current->next->username = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->username, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		i = 0;
		get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		get_from_file[i] = '\0';
		current->next->password = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->password, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		i = 0;
		get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		get_from_file[i] = '\0';
		current->next->phone_number = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->phone_number, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		i = 0;
		get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != '\n')
		{
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		get_from_file[i] = '\0';
		current->next->signup_date = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->signup_date, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		current->next->next = NULL;
		get_from_file = (char*)malloc(sizeof(char));
	}
	fclose(filePointer);
}/*end of Read_Users_Information_From_File function*/

/*read messages information from file and put in nodes*/
void Read_Messages_Information_From_File(struct message *message_head)
{
	int i = 0, id = 0, end = 0;
	FILE *filePointer;
	char in;
	filePointer = fopen("messagesinformation.txt", "r");
	while (TRUE)
	{
		i = 0;
		char *get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			if (((int)(in)) == -1)
			{
				end = 1;
				free(get_from_file);
				break;
			}
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		if (end == 1)
			break;
		get_from_file[i] = '\0';
		struct message* current = message_head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = (struct message*)malloc(sizeof(struct message));
		current->next->sender_username = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->sender_username, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		i = 0;
		get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		get_from_file[i] = '\0';
		current->next->receiver_username = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->receiver_username, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		i = 0;
		get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		get_from_file[i] = '\0';
		current->next->message_content = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->message_content, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		i = 0;
		get_from_file = (char*)malloc(sizeof(char));
		while ((in = fgetc(filePointer)) != ' ')
		{
			get_from_file[i] = in;
			i++;
			get_from_file = (char*)realloc(get_from_file, (sizeof(char)*(i + 1)));
		}
		get_from_file[i] = '\0';
		current->next->submit_date = (char*)malloc(sizeof(char)*((strlen(get_from_file)) + 1));
		memcpy(current->next->submit_date, get_from_file, strlen(get_from_file) + 1);
		free(get_from_file);
		fscanf(filePointer, "%d", &id);
		current->next->yekta = id;
		in = fgetc(filePointer);
		fscanf(filePointer, "%d", &id);
		current->next->flag = id;
		in = fgetc(filePointer);
		current->next->next = NULL;
	}
	fclose(filePointer);
}/*end of Read_Messages_Information_From_File function*/