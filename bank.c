#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/stat.h>
#include <ctype.h>
#include <io.h>
#include <time.h>
// PROBLEMS: ACCOUNT TYPE NAME AND ACCOUNT TYPE VARIABLE MISMATCHING
// FUTURE: DEPOSIT/WITHDRAWAL/REMITTANCE FUNCTIONS
char option[30];
void text_menu(){
    printf("\n-------------------------------------------\n");
    printf("Please choose an option (1-6):\n");
    printf("1. Create New Bank Account\n");
    printf("2. Delete Bank Account\n");
    printf("3. Deposit\n");
    printf("4. Withdrawal\n");
    printf("5. Remittance\n");
    printf("6. Exit\n");
    printf("-------------------------------------------\n");
    printf("\nSelect Option: ");
    scanf("%29s", &option);
}

void create_account(){
    int account_type;
    int account_number;
    char account_type_name[20];
    char account_file_name_s[50], account_file_name_c[50];
    char name[50];
    char id[20], id_temp[20];
    int pin, digits, lower, upper;
    srand(time(NULL));

    // user input
    printf("Enter Your Name: ");
    scanf(" %49[^\n]", name);
    printf("Enter Your Identification Number(ID): ");
    scanf("%19s", id);
    printf("Enter the Type of Account (1: Savings/2: Current): ");
    scanf("%d", &account_type);
    if (account_type != 1 && account_type != 2){
        printf("Invalid Account Type Selected. Please try again.\n");
        return;
    }
    printf("Enter a 4 Digit PIN for your Account: ");
    scanf("%d", &pin);
    if (pin < 1000 || pin > 9999){
        printf("Invalid PIN entered. Please try again.\n");
        return;
    }
    // lowercase name
    for (int i = 0; name[i]; i++){
        name[i] = tolower(name[i]);
    }
    // account logging

    if (account_type == 1){
        strcpy(account_type_name, "Savings");
    }
    else if (account_type == 2){
        strcpy(account_type_name, "Current");
    }
while(1) {
    digits = (rand() % 3) + 7; // 7–9 digits
    lower = 1;

    for(int i = 1; i < digits; i++)
        lower *= 10;

    upper = lower * 10 - 1;
    account_number = (rand() % (upper - lower + 1)) + lower;

    sprintf(account_file_name_s, "database/%d_Savings.txt", account_number);
    sprintf(account_file_name_c, "database/%d_Current.txt", account_number);

    FILE *s = fopen(account_file_name_s,"r");
    FILE *c = fopen(account_file_name_c,"r");

    if(!s && !c) break; // Unique number found

    if(s) fclose(s);
    if(c) fclose(c);
}
    printf("Your Bank Account Number is %d, Please Remember it!\n", account_number);
    // choose correct filename based on type
    char final_file_name[60];
    if (account_type == 1) 
        strcpy(final_file_name, account_file_name_s);
    else 
        strcpy(final_file_name, account_file_name_c);

    // create file
    FILE *account_fp = fopen(final_file_name, "w+");
    if (!account_fp) {
        printf("Error creating account file!\n");
        return;
    }

    fprintf(account_fp, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nPIN: %d\nBalance: 0\n", name, id, account_type_name, account_number, pin);
    fclose(account_fp);


    // general logging
    FILE *fp = fopen("database/accounts.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp,"\"%s\" %s %d %d %d\n", name, id, account_number, account_type, pin);
    fclose(fp);

    FILE *log_fp = fopen("database/transactions.log", "a");
    if (log_fp == NULL) {
        printf("Error opening log file!\n");
        return;
    }
    fprintf(log_fp, "Account created for %s with ID %s and account type %d\n", name, id, account_type);
    fclose(log_fp);

    printf("Account Created Successfully!\n");
}

void delete_account(){
    char name[50], name_temp[50], name_input[50];
    char account_type_name[20];
    char account_file_name[50];
    char number[20], number_temp[20];
    char id[20], id_temp[20], id4[5];
    char account_type[20], account_type_temp_copy[20];
    int account_type_temp;
    int pin, pin_temp;
    char line[100];
    int extracted_pin = 0;
    // missing last 4 id ending digits check and retreive all banking accounts from index file
    // retreive name and id for logging, done
    printf("Retrieving account information...\n");
    FILE *fp_list = fopen("database/accounts.txt", "r");
    if (fp_list == NULL) {
        printf("Error opening accounts.txt\n");
        return;
    }

    while (fgets(line, sizeof(line), fp_list)) {
        if(sscanf(line, "\"%49[^\"]\" %19s %19s %d %d", name_temp, id_temp, number_temp, &account_type_temp, &pin_temp) == 5) {
            printf("%s\n", number_temp);
        }
    }

    fclose(fp_list);
    printf("Enter Your Bank Number to Delete Account: ");
    scanf("%19s", &number);
    printf("Enter the Type of Account to Delete (1: Savings/2: Current): ");
    scanf("%19s", &account_type);
    if (strcmp(account_type, "1") == 0 || strcmp(account_type, "Savings") == 0 || strcmp(account_type, "savings") == 0 || strcmp(account_type, "save") == 0){
        strcpy(account_type_name, "Savings");
    }
    else if (strcmp(account_type, "2") == 0 || strcmp(account_type, "Current") == 0 || strcmp(account_type, "current") == 0 || strcmp(account_type, "curr") == 0) {
        strcpy(account_type_name, "Current");
    }
    else {
        printf("Invalid Account Type Selected. Please try again.\n");
        return;
    }
    sprintf(account_file_name, "database/%s_%s.txt", number, account_type_name);
    FILE *name_fp = fopen(account_file_name, "r");
    if (name_fp == NULL) {
        printf("Account doesn't exist!\n");
        return;
    }
    while(fgets(line, sizeof(line), name_fp)) {
        if (sscanf(line, "Name: %49[^\n]", name) == 1) {
            break;
        }
    }
    while(fgets(line, sizeof(line), name_fp)) {
        if (sscanf(line, "ID: %s", id) == 1) {
            break;
        }
    }

        fclose(name_fp);
    printf("Enter Your Name As Confirmation: ");
    scanf(" %49[^\n]", name_input);
    for (int i = 0; name[i]; i++){
        name[i] = tolower(name[i]);
    }
    if (strcmp(name, name_input) != 0){
        printf("Name does not match our records. Account deletion failed.\n");
        return;
    }
    else {
        printf("Name found in the database.\n");
    }
    // not done yet
    printf("Enter The Last 4 Digits of Your Identification Number(ID): ");
    scanf("%4s", id4);
    if (strcmp(id + strlen(id) - 4, id4) == 0) {
        printf("ID digits verified.\n");
    } else {
        printf("ID digits do not match our records. Account deletion failed.\n");
        return;
    }

    // check if account exists
    FILE *file_check = fopen(account_file_name, "r");
    if (file_check == NULL) {
        printf("Account doesn't exist!\n");
        fclose(file_check);
        return;
    }
    else {
        printf("Account found!\n");
        fclose(file_check);
    }

    printf("Enter your 4 Digit PIN: ");
    scanf("%d", &pin);

    // read PIN from account file, done
    FILE *pin_fp = fopen(account_file_name, "r");
    if (pin_fp == NULL) {
        printf("Account doesn't exist!\n");
        return;
    }

    while(fgets(line, sizeof(line), pin_fp)) {
        if (sscanf(line, "PIN: %d", &extracted_pin) == 1) {
            break;
        }
    }
    fclose(pin_fp);

    if (extracted_pin == pin){
        remove(account_file_name);
        printf("Account deleted successfully!\n");
    }
    else {
        printf("Incorrect PIN. Account deletion failed.\n");
    }

    // general logging
    FILE *fp_del = fopen("database/accounts.txt", "r");
    FILE *temp_fp = fopen("database/temp_accounts.txt", "w");
    if (fp_del == NULL || temp_fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while(fscanf(fp_del, "\"%49[^\"]\" %19s %19s %19s %d", name_temp, id_temp, number_temp, account_type_temp_copy, &pin_temp) == 5) {

    if(strcmp(number_temp, number) == 0 && strcmp(account_type_temp_copy, account_type) == 0)
        continue;

    fprintf(temp_fp, "\"%s\" %s %s %s %d\n", name_temp, id_temp, number_temp, account_type_temp_copy, pin_temp);
}
    fclose(fp_del);
    fclose(temp_fp);
    remove("database/accounts.txt");
    rename("database/temp_accounts.txt", "database/accounts.txt");

    FILE *log_fp = fopen("database/transactions.log", "a");
    if (log_fp == NULL) {
        printf("Error opening log file!\n");
        return;
    }
    fprintf(log_fp, "Account deleted for %s with ID %s and account type %s\n", name, id, account_type);
    fclose(log_fp);
}

void deposit(){
    float amount;
    int account_number, account_type;
    int pin;

    printf("Enter Your Account Number: ");
    scanf("%d", &account_number);
    printf("Enter Your Bank Account Type (1: Savings/2: Current): ");
    scanf("%d", &account_type);
    // check if account exists then return if not
    printf("Enter your PIN: ");
    scanf("%d", &pin);
    printf("Enter the amount to deposit: ");
    scanf("%f", &amount);
    if (amount <= 0 || amount > 50000){
        printf("Invalid amount entered. Please try again.\n");
        return;
    }
    
}

void withdrawal(){
    float amount;
    int account_number, account_type;
    int pin;
    printf("Enter Your Account Number: ");
    scanf("%d", &account_number);
    printf("Enter Your Bank Account Type (1: Savings/2: Current): ");
    scanf("%d", &account_type);
    // check if account exists then return if not

    //
    printf("Enter your PIN: ");
    scanf("%d", &pin);
    // Show the amount of money available before withdrawal
    
    printf("Enter the amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0){
        printf("Invalid amount entered. Please try again.\n");
        return;
    }
}

void remittance(){
    int sender_account_number, sender_account_type;
    int receiver_account_number, receiver_account_type;
    int pin;
    float amount;

    printf("Enter the Sender's Account Number: ");
    scanf("%d", &sender_account_number);
    printf("Enter the Sender's Bank Account Type (1: Savings/2: Current): ");
    scanf("%d", &sender_account_type);
    // check if sender account exists then return if not
    printf("Enter the Receiver's Account Number: ");
    scanf("%d", &receiver_account_number);
    printf("Enter the Receiver's Bank Account Type (1: Savings/2: Current): ");
    scanf("%d", &receiver_account_type);
    // check if receiver account exists then return if not

    printf("Enter Sender's PIN: ");
    scanf("%d", &pin);
    printf("Enter the amount to remit: ");
    scanf("%f", &amount);
    if (amount <= 0){
        printf("Invalid amount entered. Please try again.\n");
        return;
    }
}

void options(){
    if (strcmp(option, "1") == 0 || strcmp(option, "create") == 0 || strcmp(option, "new") == 0 || strcmp(option, "make") == 0) {
        create_account();
    } else if (strcmp(option, "2") == 0 || strcmp(option, "delete") == 0 || strcmp(option, "remove") == 0) {
        delete_account();
    } else if (strcmp(option, "3") == 0 || strcmp(option, "deposit") == 0) {
        deposit();
    } else if (strcmp(option, "4") == 0 || strcmp(option, "withdraw") == 0 || strcmp(option, "withdrawal") == 0) {
        withdrawal();
    } else if (strcmp(option, "5") == 0 || strcmp(option, "remit") == 0 || strcmp(option, "remittance") == 0) {
        remittance();
    } else if (strcmp(option, "6") == 0 || strcmp(option, "exit") == 0 || strcmp(option, "quit") == 0) {
    const char *message = "Program will exit soon";
    for (int i = 0; i < 3; i++) {
        printf("\r%s", message);
        for (int j = 0; j < i % 4; j++)
            printf(".");
        fflush(stdout);
        Sleep(1000);
    }
    exit(0);
    } else {
        printf("Invalid Option. Please try again.\n");
    }
}
int main(){
    struct stat st = {0};
    if (stat("database", &st) == -1) {
        system("mkdir database");
    }
    while (1){
        text_menu();
        options();
    }
    return 0;
}