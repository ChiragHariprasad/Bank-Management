#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// Structure definitions
struct pass {
    char username[50];
    char password[50];
    int date, month, year;
    char pnumber[15];
    char adharnum[20];
    char fname[20];
    char lname[20];
    char fathname[20];
    char mothname[20];
    char address[50];
    char typeaccount[20];
};

struct money {
    char usernameto[50];
    char userpersonfrom[50];
    long int money1;
};

// Function declarations
void checkbalance(char*);
void transfermoney(void);
void display(char*);
void login(void);
void loginsu(void);
void account(void);
void accountcreated(void);
void logout(void);

// Cursor position function
void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int main()
{
    system("cls");
    int choice;

    while(1) {
        system("cls");
        gotoxy(20, 3);
        printf("WELCOME TO BANK ACCOUNT SYSTEM\n\n");
        gotoxy(18, 5);
        printf("**********************************");
        gotoxy(25, 7);
        printf("BANK MANAGEMENT SYSTEM");

        gotoxy(20, 10);
        printf("1.... CREATE A BANK ACCOUNT");
        gotoxy(20, 12);
        printf("2.... ALREADY A USER? SIGN IN");
        gotoxy(20, 14);
        printf("3.... EXIT\n\n");

        printf("\n\nENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                printf("\n\n USERNAME 50 CHARACTERS MAX!!");
                printf("\n\n PASSWORD 50 CHARACTERS MAX!!");
                account();
                break;
            case 2:
                login();
                break;
            case 3:
                exit(0);
            default:
                printf("\nInvalid Choice! Press any key to continue...");
                getch();
        }
    }
    return 0;
}

void account(void)
{
    FILE *fp;
    struct pass u1;
    int i;

    fp = fopen("username.txt", "ab");
    
    if (fp == NULL) {
        printf("\nError opening file!");
        return;
    }

    system("cls");
    printf("\n\n!!!!!CREATE ACCOUNT!!!!!");

    printf("\n\nFIRST NAME: ");
    scanf("%s", u1.fname);

    printf("\nLAST NAME: ");
    scanf("%s", u1.lname);

    printf("\nFATHER's NAME: ");
    scanf("%s", u1.fathname);

    printf("\nMOTHER's NAME: ");
    scanf("%s", u1.mothname);

    printf("\nADDRESS: ");
    scanf("%s", u1.address);

    printf("\nACCOUNT TYPE (savings/current): ");
    scanf("%s", u1.typeaccount);

    printf("\nDATE OF BIRTH");
    printf("\nDATE (1-31): ");
    scanf("%d", &u1.date);
    printf("MONTH (1-12): ");
    scanf("%d", &u1.month);
    printf("YEAR: ");
    scanf("%d", &u1.year);

    printf("\nADHAR NUMBER: ");
    scanf("%s", u1.adharnum);

    printf("\nPHONE NUMBER: ");
    scanf("%s", u1.pnumber);

    printf("\nUSERNAME: ");
    scanf("%s", u1.username);
    
    // Clear input buffer before password
    fflush(stdin);
    
    printf("\nPASSWORD: ");
    i = 0;
    while (1) {
        char ch = getch();
        if (ch == 13) // Enter key
            break;
        if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
            continue;
        }
        if (i < 49) { // Prevent buffer overflow
            u1.password[i++] = ch;
            printf("*");
        }
    }
    u1.password[i] = '\0';

    fwrite(&u1, sizeof(u1), 1, fp);
    fclose(fp);
    accountcreated();
}

void accountcreated(void)
{
    system("cls");
    printf("PLEASE WAIT....\n\nYOUR DATA IS PROCESSING....");
    Sleep(2000);

    gotoxy(30, 10);
    printf("ACCOUNT CREATED SUCCESSFULLY....");
    gotoxy(0, 20);
    printf("Press enter to login");

    getch();
    login();
}

void login(void)
{
    system("cls");
    char username[50];
    char password[50];
    int i;
    FILE *fp;
    struct pass u1;

    fp = fopen("username.txt", "rb");
    if (fp == NULL) {
        printf("ERROR IN OPENING FILE");
        return;
    }

    gotoxy(34, 2);
    printf(" ACCOUNT LOGIN ");
    gotoxy(7, 5);
    printf("***********************************************");

    gotoxy(35, 10);
    printf("==== LOG IN ====");

    gotoxy(35, 12);
    printf("USERNAME: ");
    scanf("%s", username);
    
    // Clear input buffer
    fflush(stdin);

    gotoxy(35, 14);
    printf("PASSWORD: ");
    
    // Read password with backspace support
    i = 0;
    while (1) {
        char ch = getch();
        if (ch == 13) // Enter key
            break;
        if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
            continue;
        }
        if (i < 49) { // Prevent buffer overflow
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    int found = 0;
    while (fread(&u1, sizeof(u1), 1, fp)) {
        if (strcmp(username, u1.username) == 0) {
            if (strcmp(password, u1.password) == 0) {
                found = 1;
                loginsu();
                display(username);
                break;
            }
        }
    }
    
    if (!found) {
        printf("\n\nInvalid Username or Password!");
        printf("\nPress any key to try again...");
        getch();
        login();
    }

    fclose(fp);
}

void loginsu(void)
{
    system("cls");
    printf("Fetching account details.....\n");
    Sleep(1000);

    gotoxy(30, 10);
    printf("LOGIN SUCCESSFUL....");
    gotoxy(0, 20);
    printf("Press enter to continue");

    getch();
}

void display(char username1[])
{
    system("cls");
    FILE* fp;
    int choice;
    fp = fopen("username.txt", "rb");
    struct pass u1;

    if (fp == NULL) {
        printf("Error opening file");
        return;
    }

    while (fread(&u1, sizeof(u1), 1, fp)) {
        if (strcmp(username1, u1.username) == 0) {
            gotoxy(30, 1);
            printf("WELCOME, %s %s", u1.fname, u1.lname);
            gotoxy(28, 2);
            printf("..........................");
            gotoxy(55, 6);
            printf("==== YOUR ACCOUNT INFO ====");
            gotoxy(55, 8);
            printf("***************************");
            gotoxy(55, 10);
            printf("NAME: %s %s", u1.fname, u1.lname);
            gotoxy(55, 12);
            printf("FATHER's NAME: %s", u1.fathname);
            gotoxy(55, 14);
            printf("MOTHER's NAME: %s", u1.mothname);
            gotoxy(55, 16);
            printf("ADHAR CARD NUMBER: %s", u1.adharnum);
            gotoxy(55, 18);
            printf("MOBILE NUMBER: %s", u1.pnumber);
            gotoxy(55, 20);
            printf("DATE OF BIRTH: %d-%d-%d", u1.date, u1.month, u1.year);
            gotoxy(55, 22);
            printf("ADDRESS: %s", u1.address);
            gotoxy(55, 24);
            printf("ACCOUNT TYPE: %s", u1.typeaccount);
            break;
        }
    }

    fclose(fp);

    gotoxy(0, 6);
    printf(" HOME ");
    gotoxy(0, 7);
    printf("******");
    gotoxy(0, 9);
    printf(" 1....CHECK BALANCE");
    gotoxy(0, 11);
    printf(" 2....TRANSFER MONEY");
    gotoxy(0, 13);
    printf(" 3....LOG OUT\n\n");
    gotoxy(0, 15);
    printf(" 4....EXIT\n\n");

    printf(" ENTER YOUR CHOICE: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            checkbalance(username1);
            break;
        case 2:
            transfermoney();
            break;
        case 3:
            logout();
            login();
            break;
        case 4:
            exit(0);
        default:
            printf("\nInvalid Choice!");
            Sleep(1000);
            display(username1);
    }
}

void transfermoney(void)
{
    FILE *fm, *fp;
    struct pass u1;
    struct money m1;
    char usernamet[50];
    char usernamep[50];
    int found = 0;

    system("cls");
    fp = fopen("username.txt", "rb");
    fm = fopen("mon.txt", "ab");

    if (fp == NULL || fm == NULL) {
        printf("Error opening files!");
        return;
    }

    gotoxy(33, 4);
    printf("---- TRANSFER MONEY ----");
    gotoxy(33, 5);
    printf("========================");

    gotoxy(33, 11);
    printf("FROM (your username): ");
    scanf("%s", usernamet);

    gotoxy(33, 13);
    printf("TO (username of person): ");
    scanf("%s", usernamep);

    while (fread(&u1, sizeof(u1), 1, fp)) {
        if (strcmp(usernamep, u1.username) == 0) {
            found = 1;
            strcpy(m1.usernameto, u1.username);
            strcpy(m1.userpersonfrom, usernamet);
            break;
        }
    }

    if (!found) {
        printf("\nRecipient username not found!");
        Sleep(1000);
        fclose(fp);
        fclose(fm);
        display(usernamet);
        return;
    }

    gotoxy(33, 16);
    printf("ENTER THE AMOUNT TO BE TRANSFERRED: ");
    scanf("%ld", &m1.money1);

    fwrite(&m1, sizeof(m1), 1, fm);

    printf("\nTransferring amount, Please wait...");
    Sleep(2000);
    printf("\nAMOUNT SUCCESSFULLY TRANSFERRED....");
    getch();

    fclose(fp);
    fclose(fm);
    display(usernamet);
}

void checkbalance(char username2[])
{
    system("cls");
    FILE* fm;
    struct money m1;
    long int summoney = 0;
    int i = 1;

    fm = fopen("mon.txt", "rb");
    if (fm == NULL) {
        printf("Error opening file!");
        return;
    }

    gotoxy(30, 2);
    printf("==== BALANCE DASHBOARD ====");
    gotoxy(30, 3);
    printf("***************************");
    gotoxy(5, 10);
    printf("S no.");
    gotoxy(30, 10);
    printf("TRANSACTION ID");
    gotoxy(60, 10);
    printf("AMOUNT");

    int row = 12;
    while (fread(&m1, sizeof(m1), 1, fm)) {
        if (strcmp(username2, m1.usernameto) == 0) {
            gotoxy(5, row);
            printf("%d", i++);
            gotoxy(30, row);
            printf("%s", m1.userpersonfrom);
            gotoxy(60, row);
            printf("%ld", m1.money1);
            summoney += m1.money1;
            row++;
        }
    }

    gotoxy(30, row + 2);
    printf("TOTAL BALANCE: %ld", summoney);

    fclose(fm);
    printf("\n\nPress any key to continue...");
    getch();
    display(username2);
}

void logout(void)
{
    system("cls");
    printf("Logging out");
    for (int i = 0; i < 3; i++) {
        Sleep(500);
        printf(".");
    }

    gotoxy(30, 10);
    printf("Sign out successfully..\n");
    gotoxy(0, 20);
    printf("Press any key to continue..");
    getch();
}
