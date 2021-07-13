#define MAX_COMMAND 5
#define TRUE 1
#define FALSEE -1
#define MIN_LENGTH_OF_PASSWORD 6
#define LENGTH_OF_PHONE_NUMBER 11
#define SIZE_OF_DATE_STRING 17
#define CLOSE -2
#define LOGOUT -3
#define UNREAD 8
#define READ 9

struct user
{
	char *username;
	char *password;
	char *phone_number;
	char *signup_date;
	struct user *next;
};
struct message
{
	char *sender_username;
	char *receiver_username;
	char *message_content;
	char *submit_date;
	int flag;
	int yekta;
	struct message *next;
};

/*divided command functions to check command , return FALSEE means can't allocate memory or vlaue is wrong*/
int Command_Menu(struct user *user_head, struct message *message_head);
/*get command from user*/
char ** Get_Command(int* counter);
/*check the command and pass it , first command*/
int Check_Command_First_Menu(char **command, int counter, struct user *user_head, struct message *message_head);
/*ckeck the signup command*/
int Check_Command_Signup(char **command, struct user *user_head);
/*check the password value , check the length of password , when length of paasword is true return true and else return FALSEE , get *command*/
int Check_Password_Value(char *command);
/*check username exist or no , when username exists return true and if username doesn't exist return FALSEE*/
int Ckeck_Username_Exist(char *command, struct user *user_head);
/*check the phone number to signup , check phone number to signup*/
int Check_Phone_Number_Value(char **command, struct user *user_head);
/*check phone number exists or no , if exists return true else return FALSEE*/
int Check_Phone_Number_Exist(char **command, struct user *user_head);
/*sign up user , if can't allocate memory return NULL, then return address of node of new user*/
struct user* Signup(char **command, struct user* user_head);
/*add user to struct*/
struct user* Add_User(char **command, struct user* user_head);
/*divided command functions to check command for second menu*/
int Second_Command_Menu_Of_Signup_User(struct user *user_head, struct user *new_user, struct message *message_head);
/*check command of signup user , if command is wrong return FALSEE , if command is close return close , command & counter & user_head & new_user*/
int Check_Command_Second_Menu_Of_Signup_User(char **command, int counter, struct user *user_head, struct user *new_user, struct message *message_head);
/*edit username , command & user_head & new_user*/
void Edit_Username(char **command, struct user* user_head, struct user* new_user);
/*edit password , command & new_user*/
void Edit_Pass(char **command, struct user* new_user);
/*ckeck the login command , if can log in return address of user node else return NULL*/
struct user* Check_Command_Login(char **command, struct user *user_head);
/*find username node , if can find node of username return address of user node else return NULL*/
struct user* Find_Username(char **command, struct user *user_head);
/*save time in char* of strcut*/
void Insert_Time(char *date_with_time);
/*info , get user_head , print username of users*/
void Info(struct user *user_head);
/*about print informatoin of that user , get command & user_head*/
void About(char **command, struct user* user_head);
/*send message , get : command & user_head & new_user & message_head*/
void Send_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head);
/*make node at the end of message linked list and insert information in that , get command & new_user & message_head*/
void Add_Message(char **command, struct user *new_user, struct message *message_head);
/*show sent messages , if messages is unread from receiver show in other color , get : command & user_head & new_user & message_head*/
void Show_Sent_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head);
/*print sent messages , return true if message exist and if message doesn't exist return FALSEE , get : command & user_head & new_user & message_head*/
int Print_Sent_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head);
/*show received messages , get : command & user_head & new_user & message_head*/
void Show_Received_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head);
/*print received messages , return true if message exist and if message doesn't exist return FALSEE , get : command & user_head & new_user & message_head*/
int Print_Received_Message(char **command, struct user *user_head, struct user *new_user, struct message *message_head);
/*show unread messages when user login , get: current_user & message_head*/
void Show_Unread_Message(struct user  *current_user, struct message *message_head);
/*free users node and users char* */
void Free_Users_Node(struct user *user_head);
/*free messages node and messages char* */
void Free_Messages_Node(struct message *message_head);
/*save information of users struct in file*/
void Save_Users_Information(struct user *user_head);
/*save information of messages struct in file*/
void Save_Messages_Information(struct message *message_head);
/*read users information from file*/
void Read_Users_Information_From_File(struct user *user_head);
/*read messages information from file and put in nodes*/
void Read_Messages_Information_From_File(struct message *message_head);