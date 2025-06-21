#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

struct data{
	int height;
	int day;
	int month;
	int year;
	int price;
	char description[50];
	struct data *left;
	struct data *right;
} *root=NULL;

struct credential{
	char username[30];
	char password[30];
	struct credential *next;
} *head=NULL, *tail=NULL;

const char *months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const char *fullMonths[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

long long totalPrice;
long long frequencyPrice;
int freq;
int i=0;
int signIn(char username[], char password[], int mode);
void signUp();
void popAll();
void input(int mode, int month, int year);
void deleteAll(struct data *N);
void frequency(int mode, int month, int year);
void viewSaved();
void view(int choice);
void loadSaveExport(char username[], int mode, int month, int year);
void header(int N);

int main(){
	int choice;
	do{
		int mark;
		char username[30], password[30], confirm;
		do{
			do{
				header(0);
			    printf(" 1. Log in\n");
			    printf(" 2. Reset Password\n");
			    printf(" 3. Delete Account\n");
			    printf(" 4. Register\n");
			    printf(" 0. Close Program\n");
			    printf(" >> ");
			    scanf("%d",&choice);
				fflush(stdin);
			    system("cls");
			}while(choice<0||choice>4);
			if(choice>0 && choice<4) header(0);
			switch(choice){
				case 1:
					mark = signIn(username,password,0);
					break;
				case 2:
					mark = signIn(username,password,1);
					break;
				case 3:
					mark = signIn(username,password,2);
					break;
			}
			if(choice==0){
				header(2);
				return 0;
			}
			else if(choice==4||mark==1) signUp();
			system("cls");
			popAll();
		}while(choice!=0 && mark!=2);
		if(choice==0){
			header(2);
			return 0;
		}
		if(mark==2){
			char usernameCopy[30];
			strcpy(usernameCopy,username);
			loadSaveExport(usernameCopy,0,0,0);
			do{
				do{
					header(1);
					printf(" Welcome back, %s!\n\n",username);
					printf(" 1. Input A New Data\n");
					printf(" 2. View Monthly Data\n");
					printf(" 3. View Yearly Data\n");
					printf(" 4. View All Data\n");
					printf(" 5. View Frequency\n");
					printf(" 6. View A Saved Data\n");
					printf(" 7. Export Data\n");
					printf(" 8. Log Out\n");
					printf(" 0. Close Program\n");
					printf(" >> ");
					scanf("%d",&choice);
					fflush(stdin);
					system("cls");
				}while(choice<0||choice>8);
				if(choice==1){
					header(1);
					input(0,0,0);
				}else if(choice>1 && choice<5){
					view(choice);
				}else if(choice==5){
					frequency(1,0,0);
				}else if(choice==6){
					viewSaved();
					loadSaveExport(username,0,0,0);
				}else if(choice==7){
					loadSaveExport(username,2,0,0);
				}
				system("cls");
				loadSaveExport(username,1,0,0);
			}while(choice!=0 && choice!=8);
			mark = 0;
		}
		deleteAll(root);
		root = NULL;
	}while(choice!=0);
	header(2);
	return 0;
}

void getPass(char password[]){
    int index = 0;
	char letter;
	while(1){
        letter = _getch();
        if(letter=='\r') break;
        else if(letter=='\b' && index>0){
            index--;
            printf("\b \b");
        }else if(index<30){
            password[index++] = letter;
            printf("*");
        }
    }
    password[index] = '\0';
}

void push(char username[], char password[]){
	struct credential *curr = (struct credential*)malloc(sizeof(struct credential));
	strcpy(curr->username,username);
	strcpy(curr->password,password);
	curr->next = NULL;
	if(!head){
		head = tail = curr;
	}else{
		if(strcmp(curr->username,head->username)<0){
			curr->next = head;
			head = curr;
		}else if(strcmp(curr->username,tail->username)>=0){
			tail->next = curr;
			tail = curr;
		}else{
			struct credential *temp = head;
			while(strcmp(temp->next->username,curr->username)<0){
				temp = temp->next;
			}
			curr->next = temp->next;
			temp->next = curr;
		}
	}
}

struct credential *search(char username[]){
	struct credential *curr = head;
	while(curr!=NULL && strcmp(curr->username,username)!=0){
		curr = curr->next;
	}
	return curr;
}

void pop(char username[]){
	struct credential *curr = search(username);
	if(!curr) return;
	if(curr==head){
		if(head==tail){
			free(head);
			head = tail = NULL;
		}else{
			head = head->next;
			free(curr);
		}
	}else{
		struct credential *temp = head;
		while(temp->next!=curr){
			temp = temp->next;
		}
		temp->next = curr->next;
		if(curr==tail) tail = temp;
		free(curr);
	}
}

void popAll(){
    struct credential *temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp);
    }
    head = tail = NULL;
}

int signIn(char username[], char password[], int mode){
	char confirm;
	FILE *F = fopen("Credential","r");
	int flag = 0;
	if(F){
		while(fscanf(F,"%[^#]#%[^\n]\n",username,password)==2){
			push(username,password);
			flag = 1;
		}	
		fclose(F);
	}
	if(!F||!flag){
		printf(" There is no data yet.\n");
		do{
			printf(" Register a new account? [Y/N]: ");
			scanf("%c",&confirm);
			fflush(stdin);
		}while(confirm!='Y' && confirm!='N');
		system("cls");
		return confirm=='Y'? 1:0;
	}else{
		strcpy(username,"");
		strcpy(password,"");
		printf("\t\t\t\t\t\t       Input \"0\" to exit\n\n");
		do{
			printf(" Username [more than 1 character and less than 30 characters]: ");
			scanf("%[^\n]",username);
			fflush(stdin);
		}while((strlen(username)<2||strlen(username)>30) && strcmp(username,"0")!=0);
		if(strcmp(username,"0")!=0){
			struct credential *exist = search(username);
			if(!exist){
				do{
					printf(" Invalid username!\n");
					printf(" Username [more than 1 character and less than 30 characters]: ");
					scanf("%[^\n]",username);
					fflush(stdin);
					exist = search(username);
				}while((strlen(username)<2||strlen(username)>30||!exist) && strcmp(username,"0")!=0);
			}
			if(exist){
				do{
					printf(" Password [more than 1 character and less than 30 characters]: ");
					getPass(password);
					puts("");
				}while((strlen(password)<2||strlen(password)>30) && strcmp(password,"0")!=0);
				if(strcmp(exist->password,password)!=0 && strcmp(password,"0")!=0){
					do{
						printf(" Invalid password!\n");
						printf(" Password [more than 1 character and less than 30 characters]: ");
						getPass(password);
						puts("");
					}while((strlen(password)<2||strlen(password)>30||strcmp(exist->password,password)!=0) && strcmp(password,"0")!=0);
				}
				if(strcmp(exist->password,password)==0){
					if(mode==1){
						do{
							printf(" Input new password [more than 1 character and less than 30 characters]: ");
							getPass(password);
							puts("");
						}while((strlen(password)<2||strlen(password)>30) && strcmp(password,"0")!=0);
						char newPass[30];
						if(strcmp(password,"0")!=0){
							do{
								printf(" Confirm new password: ");
								getPass(newPass);
								puts("");
							}while(strcmp(password,newPass)!=0 && strcmp(password,"0")!=0);
							if(strcmp(password,newPass)==0){
								strcpy(exist->password,password);
								F = fopen("Credential","w");
								while(head){
									fprintf(F,"%s#%s\n",head->username,head->password);
									head = head->next;
								}
								fclose(F);
							}
						}
						return 0;
					}else if(mode==2){
						do{
							printf(" Are you sure you want to delete this account? [Y/N]: ");
							scanf("%c",&confirm);
							fflush(stdin);
						}while(confirm!='Y' && confirm!='N');
						if(confirm=='Y'){
							pop(username);
							F = fopen("Credential","w");
							struct credential *curr = head;
							while(curr){
								fprintf(F,"%s#%s\n",curr->username,curr->password);
								curr = curr->next;
							}
							fclose(F);
						}
						return 0;
					}
					return 2;
				}
			}
		}
	}
	return 0;
}

void signUp(){
	char username[30], password[30];
	header(0);
	printf("\t\t\t\t\t\t       Input \"0\" to exit\n\n");
	FILE *F = fopen("Credential","r");
	while(fscanf(F,"%[^#]#%[^\n]\n",username,password)==2) push(username,password);
	fclose(F);
	do{
		printf(" Username [more than 1 character and less than 30 characters]: ");
		scanf("%[^\n]",username);
		fflush(stdin);
	}while((strlen(username)<2||strlen(username)>30) && strcmp(username,"0")!=0);
	if(strcmp(username,"0")==0) return;
	struct credential *exist = search(username);
	while((strlen(username)<2||strlen(username)>30||exist) && strcmp(username,"0")!=0){
		printf(" Username already exists.\n");
		printf(" Username [more than 1 character and less than 30 characters]: ");
		scanf("%[^\n]",username);
		fflush(stdin);
		exist = search(username);
	}
	if(strcmp(username,"0")==0) return;
	do{
		printf(" Password [more than 1 character and less than 30 characters]: ");
		getPass(password);
		puts("");
	}while((strlen(password)<2||strlen(password)>30) && strcmp(password,"0")!=0);
	if(strcmp(password,"0")!=0){
		F = fopen("Credential","a");
		fprintf(F,"%s#%s\n",username,password);
		fclose(F);
	}
	return;
}

int heightBalanced(struct data *N, int mode){
	if(mode==0) return N? N->height:0;
	else return N? heightBalanced(N->left,0)-heightBalanced(N->right,0):0;
}

struct data *rotation(struct data *A, int mode){
	struct data *B = mode==0? A->left:A->right;
	struct data *C = mode==0? B->right:B->left;
	(mode==0)? (B->right=A):(B->left=A);
	(mode==0)? (A->left=C):(A->right=C);
	A->height = (heightBalanced(A->left,0)>heightBalanced(A->right,0))? heightBalanced(A->left,0):heightBalanced(A->right,0)+1;
	B->height = (heightBalanced(B->left,0)>heightBalanced(B->right,0))? heightBalanced(B->left,0):heightBalanced(B->right,0)+1;
	return B;
}

struct data *insert(struct data *node, int day, int month, int year, int price, char description[]){
	if(!node){
		struct data *curr = (struct data*)malloc(sizeof(struct data));
		curr->day = day;
		curr->month = month;
		curr->year = year;
		curr->price = price;
		strcpy(curr->description,description);
		curr->left = curr->right = NULL;
		curr->height = 1;
		return curr;
	}
	if(year<node->year) node->left = insert(node->left,day,month,year,price,description);
	else if(year>node->year) node->right = insert(node->right,day,month,year,price,description);
	else if(month<node->month) node->left = insert(node->left,day,month,year,price,description);
	else if(month>node->month) node->right = insert(node->right,day,month,year,price,description);
	else if(day<node->day) node->left = insert(node->left,day,month,year,price,description);
	else node->right = insert(node->right,day,month,year,price,description);
	node->height = (heightBalanced(node->left,0)>heightBalanced(node->right,0))? heightBalanced(node->left,0):heightBalanced(node->right,0)+1;
	int balance = heightBalanced(node,1);
	if(balance>1 && node->left->left) return rotation(node,0);
	if(balance>1 && node->left->right){
		node->left = rotation(node->left,1);
		return rotation(node,0);
	}
	if(balance<-1 && node->right->right) return rotation(node,1);
	if(balance<-1 && node->right->left){
		node->right = rotation(node->right,0);
		return rotation(node,1);
	}
	return node;
}

struct data *deleteNode(struct data *node, int day, int month, int year, char description[]){
	if(!node) return node;
	if(year<node->year) node->left = deleteNode(node->left,day,month,year,description);
	else if(year>node->year) node->right = deleteNode(node->right,day,month,year,description);
	else if(month<node->month) node->left = deleteNode(node->left,day,month,year,description);
	else if(month>node->month) node->right = deleteNode(node->right,day,month,year,description);
	else if(day<node->day) node->left = deleteNode(node->left,day,month,year,description);
	else if(day>node->day) node->right = deleteNode(node->right,day,month,year,description);
	else if(strcmp(description,node->description)==0){
		if(!node->left||!node->right){
			struct data *temp = node->left? node->left:node->right;
			if(!temp){
				temp = node;
				node = NULL;
			}else{
				*node = *temp;
			}
			free(temp);
		}else{
			struct data *temp = node->left;
			while(temp->right) temp = temp->right;
			node->day = temp->day;
			node->month = temp->month;
			node->year = temp->year;
			node->price = temp->price;
			strcpy(node->description,temp->description);
			node->left = deleteNode(node->left,temp->day,temp->month,temp->year,temp->description);
		}
	}else{
		node->left = deleteNode(node->left,day,month,year,description);
		node->right = deleteNode(node->right,day,month,year,description);
	}
	if(!node) return node;
	node->height = (heightBalanced(node->left,0)>heightBalanced(node->right,0))? heightBalanced(node->left,0):heightBalanced(node->right,0)+1;
	int balance = heightBalanced(node,1);
	if(balance>1 && heightBalanced(node->left,1)>=0) return rotation(node,0);
	if(balance>1 && heightBalanced(node->left,1)<0){
		node->left = rotation(node->left,1);
		return rotation(node,0);
	}
	if(balance<-1 && heightBalanced(node->right,1)<=0) return rotation(node,1);
	if(balance<-1 && heightBalanced(node->right,1)>0){
		node->right = rotation(node->right,0);
		return rotation(node,1);
	}
	return node;
}

void deleteAll(struct data *N){
	if(N){
		deleteAll(N->left);
		deleteAll(N->right);
		free(N);
	}
}

void enter(){
	printf(" Press Enter to continue...");
	getchar();
	system("cls");
}

void input(int mode, int month, int year){
	int day, price;
	char description[50];
	if(mode==0){
		do{
			printf(" Input date [day-month-year]: ");
			scanf("%d-%d-%d",&day,&month,&year);
			fflush(stdin);
		}while(day<1||day>31||month<1||month>12||year<2020||year>2100);
		month--;
	}else{
		do{
			printf(" Input day: ");
			scanf("%d",&day);
			fflush(stdin);
		}while(day<1||day>31);
	}
	do{
		printf(" Input description [max. 50 characters]: ");
		scanf("%[^\n]",description);
		fflush(stdin);
	}while(strlen(description)<1||strlen(description)>50);
	do{ 
		printf(" Input price: ");
		scanf("%d",&price);
		fflush(stdin);
	}while(price<1000);
	root = insert(root,day,month,year,price,description);
	printf("\n Data successfully inserted!\n");
	enter();
	return;
}

void checkMonth(struct data *N, int month, int year, int *check, int mode){
	if(N){
		checkMonth(N->left,month,year,check,mode);
		if(((mode==2 && N->month==month) || mode==3) && N->year==year){
			*check = 1;
			return;
		}
		checkMonth(N->right,month,year,check,mode);
	}
}

void displayMonth(struct data *N, int *prevDay, int month, int year, char description[], int mode){
	if(N){
		displayMonth(N->left,prevDay,month,year,description,mode);
		if((mode==0 || (mode==1 && strcmp(N->description,description)==0)) && N->month==month && N->year==year){
			if(*prevDay!=N->day){
				printf(" | %02d %s %d",N->day,months[N->month],N->year);
				*prevDay = N->day;
			}else{
				printf(" |            ");
			}
			printf("  | %-50s  | %-8d  |\n",N->description,N->price);
			mode==0? (totalPrice += N->price) : (frequencyPrice += N->price);
			if(mode==1) freq++;
		}
		displayMonth(N->right,prevDay,month,year,description,mode);
	}
}

void viewMonth(int month, int year, char description[], int mode){
	printf(" Showing the data in %s %d.\n",fullMonths[month],year);
	printf(" "); for(i=0; i<82; i++) printf("="); puts("");
	printf(" |     DATE     | %-20s%-31s |   PRICE   |\n"," ","DESCRIPTION");
	printf(" "); for(i=0; i<82; i++) printf("="); puts("");
	int prevDay = -1;
	displayMonth(root,&prevDay,month,year,description,mode);
	printf(" "); for(i=0; i<82; i++) printf("="); puts("");
	printf(" |     %-62s | %-9lld |\n", "TOTAL", mode==0? totalPrice:frequencyPrice);
	printf(" "); for(i=0; i<82; i++) printf("="); printf("\n\n");
	return;
}

void displayAll(struct data *N, int *prevDay, int *prevMonth, int *prevYear, int *monthBorder, int *yearBorder, int year, char description[], int mode){
	if(N){
		displayAll(N->left,prevDay,prevMonth,prevYear,monthBorder,yearBorder,year,description,mode);
		if((mode==5 && N->year==year && strcmp(N->description,description)==0) || (mode==6 && strcmp(N->description,description)==0) || mode==4 || (mode==3 && N->year==year)){
			if((*monthBorder!=N->month||*yearBorder!=N->year) && *monthBorder!=-1 && *yearBorder!=-1){
				printf(" |"); for(i=0; i<14; i++) printf("="); printf("|"); for(i=0; i<53; i++) printf("=");
				printf("|"); for(i=0; i<11; i++) printf("="); printf("|"); puts("");
				*monthBorder = N->month;
				*yearBorder = N->year;
			}else{
				*monthBorder = N->month;
				*yearBorder = N->year;
			}
			if(*prevDay!=N->day||*prevMonth!=N->month||*prevYear!=N->year){
				printf(" | %02d %s %d",N->day,months[N->month],N->year);
				*prevDay = N->day;
				*prevMonth = N->month;
				*prevYear = N->year;
			}else{
				printf(" |            ");
			}
			printf("  | %-50s  | %-8d  |\n",N->description,N->price);
			if(mode==5||mode==6){
				frequencyPrice += N->price;
				freq++;
			}else{
				totalPrice += N->price;
			}
		}
		displayAll(N->right,prevDay,prevMonth,prevYear,monthBorder,yearBorder,year,description,mode);
	}
}

void viewAll(int year, char description[], int mode){
	printf(" "); for(i=0; i<82; i++) printf("="); puts("");
	printf(" |     DATE     | %-20s%-31s |   PRICE   |\n"," ","DESCRIPTION");
	printf(" "); for(i=0; i<82; i++) printf("="); puts("");
	int prevDay = -1, prevMonth = -1, prevYear = -1, monthBorder = -1, yearBorder = -1;
	displayAll(root,&prevDay,&prevMonth,&prevYear,&monthBorder,&yearBorder,year,description,mode);
	printf(" "); for(i=0; i<82; i++) printf("="); puts("");
	printf(" |     %-62s | %-9lld |\n", "TOTAL", mode==3? totalPrice : (mode==4? totalPrice:frequencyPrice));
	printf(" "); for(i=0; i<82; i++) printf("="); printf("\n\n");
	return;
}

int checkData(struct data *N, int month, int year, char description[], int mode){
	if(N){
		if(checkData(N->left,month,year,description,mode)) return 1;
			if(((mode==3||(mode==4 && strcmp(N->description,description)==0)) && N->month==month && N->year==year) ||
			((mode==2||(mode==5 && strcmp(N->description,description)==0)) && N->year==year) ||
			(mode==6 && strcmp(N->description,description)==0)) return 1;
		if(checkData(N->right,month,year,description,mode)) return 1;
	}
}

void frequency(int mode, int month, int year){
	if(!root){
		header(1);
		printf(" There's no data yet.\n");
		enter();
		return;
	}
	int anotherMode;
	freq = 0;
	frequencyPrice = 0;
	if(mode==1){
		header(1);
		char type[10];
		do{
			printf(" Which data do you wanna see? [Monthly | Yearly | All]: ");
			scanf("%[^\n]",type);
			fflush(stdin);
		}while(strcmpi(type,"Monthly")!=0 && strcmpi(type,"Yearly")!=0 && strcmpi(type,"All")!=0);
		anotherMode = 6;
		if(strcmpi(type,"All")!=0){
			if(strcmpi(type,"Monthly")==0){
				do{
					printf(" Input month [in number]: ");
					scanf("%d",&month);
					fflush(stdin);
				}while(month<1||month>12);
				month--;
			}
			do{
				printf(" Input year: ");
				scanf("%d",&year);
				fflush(stdin);
			}while(year<2020||year>2100);
			anotherMode = strcmpi(type,"Monthly")==0? 4:5;
		}
	}
	if(mode!=1) anotherMode = mode + 2;
	char description[50];
	do{
		printf(" Input description [max. 50 characters]: ");
		scanf("%[^\n]",description);
		fflush(stdin);
	}while(strlen(description)<1||strlen(description)>50);
	int exist = checkData(root,month,year,description,anotherMode);
	if(exist==1){
		system("cls");
		if(anotherMode==4){
			viewMonth(month,year,description,1);
		}else{
			viewAll(year,description,anotherMode);
		}
		printf(" %s was spend %d times with the total price of %lld.\n",description,freq,frequencyPrice);
	}else{
		printf(" The data \"%s\" is not found.\n",description);
	}
	enter();
	return;
}

void printSaved(struct data *N, FILE *F, int *prevDay, int *prevMonth, int *prevYear, int *monthBorder, int *yearBorder){
	if(N){
		printSaved(N->left,F,prevDay,prevMonth,prevYear,monthBorder,yearBorder);
		if((*monthBorder!=N->month||*yearBorder!=N->year) && *monthBorder!=-1 && *yearBorder!=-1){
			fprintf(F,"|"); for(i=0; i<14; i++) fprintf(F,"="); fprintf(F,"|"); for(i=0; i<53; i++) fprintf(F,"=");
			fprintf(F,"|"); for(i=0; i<11; i++) fprintf(F,"="); fprintf(F,"|"); fprintf(F,"\n");
			*monthBorder = N->month;
			*yearBorder = N->year;
		}else{
			*monthBorder = N->month;
			*yearBorder = N->year;
		}
		if(*prevDay!=N->day||*prevMonth!=N->month||*prevYear!=N->year){
			fprintf(F,"| %02d %s %d",N->day,months[N->month],N->year);
			*prevDay = N->day;
			*prevMonth = N->month;
			*prevYear = N->year;
		}else{
			fprintf(F,"|            ");
		}
		fprintf(F,"  | %-50s  | %-8d  |\n",N->description,N->price);
		totalPrice += N->price;
		printSaved(N->right,F,prevDay,prevMonth,prevYear,monthBorder,yearBorder);
	}
}

void viewSaved(){
	header(1);
	char file[30];
	do{
		printf(" Input file name [more than 1 character and less than 30 characters]: ");
		scanf("%[^\n]",file);
		fflush(stdin);
	}while(strlen(file)<2||strlen(file)>30);
	FILE *F = fopen(file,"r");
	if(F){
		system("cls");
		int day, month, year, price;
		char description[50];
		deleteAll(root);
		root = NULL;
		while(fscanf(F,"%d#%d#%d#%d#%[^\n]\n",&day,&month,&year,&price,description)==5) root = insert(root,day,month,year,price,description);
		fclose(F);
		totalPrice = 0;
		viewAll(0,description,4);
		char confirm;
		do{
			printf(" Write into a printable file? [Y/N]: ");
			scanf("%c",&confirm);
			fflush(stdin);
		}while(confirm!='Y' && confirm!='N');
		if(confirm=='Y'){
			totalPrice = 0;
			strcat(file,"_Print");
			FILE *F = fopen(file,"w");
			for(i=0; i<82; i++) fprintf(F,"="); fprintf(F,"\n");
			fprintf(F,"|     DATE     | %-20s%-31s |   PRICE   |\n"," ","DESCRIPTION");
			for(i=0; i<82; i++) fprintf(F,"="); fprintf(F,"\n");
			int prevDay = -1, prevMonth = -1, prevYear = -1, monthBorder = -1, yearBorder = -1;
			printSaved(root,F,&prevDay,&prevMonth,&prevYear,&monthBorder,&yearBorder);
			for(i=0; i<82; i++) fprintf(F,"="); fprintf(F,"\n");
			fprintf(F,"|     %-62s | %-9lld |\n","TOTAL",totalPrice);
			for(i=0; i<82; i++) fprintf(F,"=");
			fclose(F);
			printf(" File successfully printed!\n");
			enter();
		}
	}else{
		printf(" File does not exist!\n");
		enter();
	}
	deleteAll(root);
	root = NULL;
	return;
}

int updateData(struct data *N, int day, int month, int year, char description[], int mode){
	if(!N) return 0;
	int found = updateData(N->left,day,month,year,description,mode);
	if(!found && N->day==day && N->month==month && N->year==year && strcmp(N->description,description)==0){
		if(mode==0){
			do{
				printf("\n Input new description [max. 50 characters]: ");
				scanf("%[^\n]",description);
				fflush(stdin);
			}while(strlen(description)<1||strlen(description)>50);
			strcpy(N->description,description);
		}else if(mode==1){
			int price;
			do{
				printf("\n Input new price: ");
				scanf("%d",&price);
				fflush(stdin);
			}while(price<1000);
			N->price = price;
		}else{
			root = deleteNode(root,day,month,year,description);
		}
		return 1;
	}
	if(!found) found = updateData(N->right,day,month,year,description,mode);
	return found;
}

void update(int mode, int choice, int menu, int month, int year){
	int day;
	char description[50];
	if(choice==2){
		do{
			printf(" Input day: ");
			scanf("%d",&day);
			fflush(stdin);
		}while(day<1||day>31);
	}else{
		do{
			choice==3? printf(" Input date [day-month]: "):printf(" Input date [day-month-year]: ");
			choice==3? scanf("%d-%d",&day,&month):scanf("%d-%d-%d",&day,&month,&year);
			fflush(stdin);
		}while(day<1||day>31||month<1||month>12||year<2020||year>2100);
		month--;
	}
	do{
		printf(" Input description [max. 50 characters]: ");
		scanf("%[^\n]",description);
		fflush(stdin);
	}while(strlen(description)<1||strlen(description)>50);
	int found = updateData(root,day,month,year,description,mode);
	if(found) printf("\n Data successfully %s\n", menu==4? "deleted!":"updated!");
	else printf("\n Data not found!\n");
	enter();
	return;
}

void view(int choice){
	int menu, month = -1, year = -1;
	char username[30];
	do{
		totalPrice = 0;
		if(!root){
			header(1);
			printf(" There's no data yet.\n");
			enter();
			return;
		}
		if(((choice==2 && month==-1) || choice==3) && year==-1){
			header(1);
			if(choice==2){
				do{
					printf(" Input month [in number]: ");
					scanf("%d",&month);
					fflush(stdin);
				}while(month<1||month>12);
				month--;
			}
			do{
				printf(" Input year: ");
				scanf("%d",&year);
				fflush(stdin);
			}while(year<2020||year>2100);
			system("cls");
			int check = 0;
			checkMonth(root,month,year,&check,choice);
			if(check==0){
				header(1);
				printf(" There's no data in %s %d.\n",fullMonths[month],year);
				enter();
				return;
			}
		}
		choice==2? viewMonth(month,year,username,0):viewAll(year,username,choice);
		printf(" 1. Input A New Data\n");
		printf(" 2. Update Description\n");
		printf(" 3. Update Price\n");
		printf(" 4. Delete A Data\n");
		printf(" 5. View Frequency\n");
		printf(" 6. Export the Data\n");
		printf(" 0. Exit\n");
		do{
			printf(" >> ");
			scanf("%d",&menu);
			fflush(stdin);
		}while(menu<0||menu>6);
		system("cls");
		totalPrice = 0;
		choice==2? viewMonth(month,year,username,0):viewAll(year,username,choice);
		switch(menu){
			case 1:
				choice==2? input(1,month,year):input(0,0,0);
				break;
			case 2:
				update(0,choice,menu,month,year);
				break;
			case 3:
				update(1,choice,menu,month,year);
				break;
			case 4:
				update(2,choice,menu,month,year);
				break;
			case 5:
				frequency(choice,month,year);
				break;
			case 6:
				loadSaveExport(username,(choice==2? 4:5),month,year);
				break;
		}
	}while(menu!=0);
}

void saveRecursion(struct data *N, FILE *F, int mode, int month, int year){
	if(N){
		saveRecursion(N->left,F,mode,month,year);
		if(mode==1 || (mode==2&&N->year==year) || ((mode==3||mode==4)&&N->month==month&&N->year==year) || mode==5){
			fprintf(F,"%d#%d#%d#%d#%s\n",N->day,N->month,N->year,N->price,N->description);
		}
		saveRecursion(N->right,F,mode,month,year);
	}
}

void loadSaveExport(char username[], int mode, int month, int year){
	int day, price;
	char confirm;
	char fileName[35];
	strcpy(fileName,username);
	strcat(fileName,"_Data");
	if(mode==0){
		FILE *F = fopen(fileName,"r");
		if(F){
			char description[50];
			while(fscanf(F,"%d#%d#%d#%d#%[^\n]\n",&day,&month,&year,&price,description)==5) root = insert(root,day,month,year,price,description);
			fclose(F);
		}
		return;
	}else if(mode==1){
		FILE *F = fopen(fileName,"w");
		saveRecursion(root,F,mode,month,year);
		fclose(F);
		return;
	}else if(mode==2){
		if(!root){
			header(1);
			printf(" There's no data yet.\n");
			enter();
			return;
		}
		header(1);
		int exist;
		char type[10];
		do{
			printf(" Which data do you wanna export? [Monthly | Yearly | All]: ");
			scanf("%[^\n]",type);
			fflush(stdin);
		}while(strcmpi(type,"Monthly")!=0 && strcmpi(type,"Yearly")!=0 && strcmpi(type,"All")!=0);
		if(strcmpi(type,"All")!=0){
			if(strcmpi(type,"Monthly")==0){
				do{
					printf(" Input month [in number]: ");
					scanf("%d",&month);
					fflush(stdin);
				}while(month<1||month>12);
				month--;
				mode = 3;
			}
			do{
				printf(" Input year: ");
				scanf("%d",&year);
				fflush(stdin);
			}while(year<2020||year>2100);
			exist = checkData(root,month,year,type,mode);
		}else{
			mode = 5;
			exist = 1;
		}
		if(exist==1){
			do{
				printf(" File name [more than 1 character and less than 30 characters]: ");
				scanf("%[^\n]",fileName);
				fflush(stdin);
			}while(strlen(fileName)<2||strlen(fileName)>30);
			FILE *F = fopen(fileName,"w");
			saveRecursion(root,F,mode,month,year);
			fclose(F);
			printf(" Data succesfully exported!\n");
		}else{
			printf(" There's no data in %s %d.\n",fullMonths[month],year);
		}
		enter();
		return;
	}else{
		if(mode==4){
			do{
				printf(" Export the data in %s %d? [Y/N]: ",fullMonths[month],year);
				scanf("%c",&confirm);
				fflush(stdin);
			}while(confirm!='Y' && confirm!='N');
		}else if(mode==5){
			do{
				printf(" Export all data? [Y/N]: ");
				scanf("%c",&confirm);
				fflush(stdin);
			}while(confirm!='Y' && confirm!='N');
		}
	}
	if(confirm=='Y'){
		char fileName[40];
		do{
			printf(" File name [more than 1 character and less than 30 characters]: ");
			scanf("%[^\n]",fileName);
			fflush(stdin);
		}while(strlen(fileName)<2||strlen(fileName)>30);
		FILE *F = fopen(fileName,"w");
		saveRecursion(root,F,mode,month,year);
		fclose(F);
		printf(" Data succesfully exported!\n");
		enter();
	}
	return;
}

void header(int N){
	if(N==0){
		printf("   ____  _____  _____  _____  _____  __   _ _______ _____  _____  _\n");
		printf("  / __ \\|  _  \\|  ___||  __ \\|  ___||  \\ | |___ ___|__ __|/  _  \\| |\n");
	    printf(" | |  \\_| |_) || |___ | |  | | |___ |   \\| |  | |    | | |  / \\  | |\n");
	    printf(" | |   _|  _  /|  ___|| |  | |  ___|| |  | |  | |    | | | |___| | |\n");
	    printf(" | |__/ | | \\ \\| |___ | |__| | |___ | |\\   |  | |   _| |_|  ___  | |___\n");
	    printf("  \\____/|_| |_||_____||_____/|_____||_| \\__|  |_|  |_____|_/   \\_|_____|\n\n");
	}else if(N==1){
		printf("  _____  _     _____ __   _  _   __    _____   _____  __   _  _   __\n");
		printf(" |  _  \\| |   |__ __|  \\ | || | / /   |  _  \\ /  _  \\|  \\ | || | / /\n");
	    printf(" | |_)  | |     | | |   \\| || |/ /    | |_)  |  / \\  |   \\| || |/ /\n");
	    printf(" |  _  <| |     | | | |  | ||   <     |  _  <| |___| | |  | ||   <\n");
	    printf(" | |_)  | |___ _| |_| |\\   || |\\ \\    | |_)  |  ___  | |\\   || |\\ \\\n");
	    printf(" |_____/|_____|_____|_| \\__||_| \\_\\   |_____/|_/   \\_|_| \\__||_| \\_\\\n\n");
	}else{
		printf("  _______ _    _  _____  __   _  _   __   __   __ ___  _   _\n");
		printf(" |___ ___| |  | |/  _  \\|  \\ | || | / /   \\ \\ / // _ \\| | | |\n");
	    printf("    | |  | |__| |  / \\  |   \\| || |/ /     \\ V /| | | | | | |\n");
	    printf("    | |  |  __  | |___| | |  | ||   <       \\ / | | | | | | |\n");
	    printf("    | |  | |  | |  ___  | |\\   || |\\ \\      | | | |_| | \\_/ |\n");
	    printf("    |_|  |_|  |_|_/   \\_|_| \\__||_| \\_\\     |_|  \\___/ \\___/\n");
	}
}
