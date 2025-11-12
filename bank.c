#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

char option[30];

void text_menu(){
    printf("-------------------------------------------\n");
    printf("Please choose an option (1-6):\n");
    printf("1. Create New Bank Account\n");
    printf("2. Delete Bank Account\n");
    printf("3. Deposit\n");
    printf("4. Withdrawal\n");
    printf("5. Remittance\n");
    printf("6. Exit\n");
    printf("-------------------------------------------\n");
    printf("\nSelect Option: ");
    scanf("%s", &option);
}

void create_account(){
    int account_type;
    char account_type_name[20];
    char account_file_name[50];
    char name[50];
    char id[20];
    int pin;

    // user input
    printf("Enter Your Name: ");
    scanf("%s", name);
    printf("Enter Your Identification Number(ID): ");
    scanf("%s", id);
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

    // account logging

    if (account_type == 1){
        strcpy(account_type_name, "Savings");
    }
    else if (account_type == 2){
        strcpy(account_type_name, "Current");
    }

    sprintf(account_file_name, "database/%s_%s.txt", id, account_type_name);

    // check if account already exists
    FILE *file_check = fopen(account_file_name, "r");
    if (file_check != NULL) {
        printf("Account already exists!\n");
        fclose(file_check);
        return;
    }

    FILE *account_fp = fopen(account_file_name, "w+");
    if (account_fp == NULL) {
        printf("Error creating account file!\n");
        return;
    }
    fprintf(account_fp, "Name: %s\nID: %s\nAccount Type: %s\nPIN: %d\nBalance: 0\n", name, id, account_type_name, pin);
    fclose(account_fp);

    // general logging
    FILE *fp = fopen("database/accounts.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%s %s %d %d\n", name, id, account_type, pin);
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
    char account_type_name[20];
    char account_file_name[50];
    char id[20];
    char account_type[20];
    int pin;
    int pin_check;
    // user input
    printf("Enter Your Identification Number(ID) to Delete Account: ");
    scanf("%s", &id);
    printf("Enter the Type of Account to Delete (1: Savings/2: Current): ");
    scanf("%s", &account_type);

    if (account_type == 1){
        strcpy(account_type_name, "Savings");
    }
    else if (account_type == 2) {
        strcpy(account_type_name, "Current");
    }

    // check if account exists
    FILE *file_check = fopen(account_file_name, "r");
    if (file_check = NULL) {
        printf("Account doesn't exist!\n");
        fclose(file_check);
        return;
    }
    else (file_check != NULL){
        printf("Account found!\n");
        fclose(file_check);
    }

    printf("Enter your 4 Digit PIN: \n");
    scanf("%d", &pin);


    sprintf(account_file_name, "database/%s_%s.txt", id, account_type_name);
    FILE *pin_fp = fopen(account_file_name, "r");
    if (pin_fp == NULL) {
        printf("Account doesn't exist!\n");
        return;
    }


}

void deposit(){

}

void withdrawal(){

}

void remittance(){

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
        sleep(1);
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