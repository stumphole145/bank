#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    fgets(option, sizeof(option), stdin);
    option[strcspn(option, "\n")] = 0;
}
int cancel_process(const char input[]){
    if (input[0] == '\n' || input[0] == '\0'){
        printf("Process Cancelled.\n");
        return 1;
    }
    return 0;
}
void acccompare(char str[]){
    str[strcspn(str, "\n")] = '\0';
    if (strcmp(str, "1") == 0 || strcmp(str, "Savings") == 0 || strcmp(str, "savings") == 0 || strcmp(str, "save") == 0){
        strcpy(str, "Savings");
    }
    else if (strcmp(str, "2") == 0 || strcmp(str, "Current") == 0 || strcmp(str, "current") == 0 || strcmp(str, "curr") == 0) {
        strcpy(str, "Current");
    }
    else {
        printf("Invalid Account Type Selected. Please try again.\n");
        str[0] = '\0';
    }
}
void create_account(){
    char account_type[20], name[50], id[20], pin_str[10];
    int account_number, pin, digits, lower, upper;
    char account_type_name[20];
    char account_file_name_s[50], account_file_name_c[50], final_file_name[60];

    // user input
    printf("Press Enter without Keying in Anything to Cancel at any time...\n");
    printf("Enter Your Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    if (cancel_process(name)) return;
    printf("Enter Your Identification Number(ID): ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    if (cancel_process(id)) return;
    printf("Enter the Type of Account (1: Savings/2: Current): ");
    fgets(account_type, sizeof(account_type), stdin);
    account_type[strcspn(account_type, "\n")] = '\0';
    if (cancel_process(account_type)) return;
    acccompare(account_type);
    if(strlen(account_type)==0) return;
    printf("Enter a 4 Digit PIN for your Account: ");
    fgets(pin_str, sizeof(pin_str), stdin);
    pin_str[strcspn(pin_str, "\n")] = '\0';
    if (cancel_process(pin_str)) return;
    if (sscanf(pin_str, "%d", &pin) != 1 || pin < 1000 || pin > 9999) {
        printf("Invalid PIN entered. Please try again.\n");
        return;
    }
    // lowercase name
    for (int i = 0; name[i]; i++){
        name[i] = tolower(name[i]);
    }
    // account logging

    strcpy(account_type_name, account_type);
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
    // create file
    sprintf(final_file_name, "database/%d_%s.txt", account_number, account_type_name);
    FILE *account_fp = fopen(final_file_name, "w+");
    fprintf(account_fp, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %d\nPIN: %d\nBalance: RM 0\n", name, id, account_type_name, account_number, pin);
    fclose(account_fp);

    // general logging
    FILE *fp = fopen("database/accounts.txt", "a");
    fprintf(fp,"\"%s\" %s %d %s %d\n", name, id, account_number, account_type_name, pin);
    fclose(fp);
    FILE *log_fp = fopen("database/transactions.log", "a");
    fprintf(log_fp, "Account created for %s with ID %s and account type %s\n", name, id, account_type_name);
    fclose(log_fp);

    printf("Account Created Successfully!\n");
}

void delete_account(){
    char name[50], name_temp[50], name_input[50];
    char account_type_name[20];
    char account_file_name[60];
    char number[20], number_temp[20];
    char id[20], id_temp[20], id4[5];
    char account_type[20], type_temp[20];
    int pin, pin_temp;
    char line[200];
    int extracted_pin = 0;

    printf("Retrieving account information...\n");
    FILE *fp_list = fopen("database/accounts.txt", "r");
    if (fp_list == NULL) {
        printf("Error opening accounts.txt\n");
        return;
    }

    while (fgets(line, sizeof(line), fp_list)) {
        if(sscanf(line, "\"%49[^\"]\" %19s %19s %19s %d", name_temp, id_temp, number_temp, type_temp, &pin_temp) == 5) {
            printf("%s\n", number_temp);
        }
    }

    fclose(fp_list);
    printf("Press Enter without Keying in Anything to Cancel at any time...\n");
    printf("Enter Your Bank Number to Delete Account: ");
    fgets(number, sizeof(number), stdin);
    number[strcspn(number, "\n")] = '\0';
    if (cancel_process(number)) return;
    printf("Enter the Type of Account to Delete (1: Savings/2: Current): ");
    fgets(account_type, sizeof(account_type), stdin);
    account_type[strcspn(account_type, "\n")] = '\0';
    if (cancel_process(account_type)) return;
    acccompare(account_type);
    if(strlen(account_type)==0) return;
    strcpy(account_type_name, account_type);
    sprintf(account_file_name, "database/%s_%s.txt", number, account_type_name);
    // get name and id from account file, done
    FILE *account_fp = fopen(account_file_name, "r");
    if (account_fp == NULL) {
        printf("Account doesn't exist!\n");
        return;
    }
    while(fgets(line, sizeof(line), account_fp)) {
        if (sscanf(line, "Name: %49[^\n]", name) == 1) {
            break;
        }
    }
    while(fgets(line, sizeof(line), account_fp)) {
        if (sscanf(line, "ID: %19s", id) == 1) {
            break;
        }
    }
    while(fgets(line, sizeof(line), account_fp)) {
        if (sscanf(line, "PIN: %d", &extracted_pin) == 1) {
            break;
        }
    }
        fclose(account_fp);
    printf("Enter Your Name As Confirmation: ");
    fgets(name_input, sizeof(name_input), stdin);
    name_input[strcspn(name_input, "\n")] = '\0';
    for (int i = 0; name_input[i]; i++){
        name_input[i] = tolower(name_input[i]);
    }
    if (strcmp(name, name_input) != 0){
        printf("Name does not match our records. Account deletion failed.\n");
        return;
    }

    printf("Enter The Last 4 Digits of Your Identification Number(ID): ");
    fgets(id4, sizeof(id4), stdin);
    id4[strcspn(id4, "\n")] = '\0';
    if (strlen(id) < 4 || strcmp(id + strlen(id) - 4, id4) != 0) {
        printf("ID digits do not match our records. Account deletion failed.\n");
        return;
    }

    printf("Enter your 4 Digit PIN: ");
    scanf("%d", &pin);
    while(getchar() != '\n');
    if (pin != extracted_pin){
        printf("Incorrect PIN. Account deletion failed.\n");
        return;
    }
    if (remove(account_file_name) != 0) {
        printf("Error deleting account file.\n");
        return;
    }
    printf("Account deleted successfully!\n");

    // general logging
    FILE *fp_del = fopen("database/accounts.txt", "r");
    FILE *temp_fp = fopen("database/temp_accounts.txt", "w");
    if (!fp_del || !temp_fp) {
        printf("Error opening file!\n");
        return;
    }

    while(fgets(line, sizeof(line), fp_del)) {
    if (sscanf(line, "\"%49[^\"]\" %19s %19s %19s %d", name_temp, id_temp, number_temp, type_temp, &pin_temp) == 5) {
        if(strcmp(number_temp, number) == 0 && strcmp(type_temp, account_type_name) == 0)
            continue;
    }
    fprintf(temp_fp, "\"%s\" %s %s %s %d\n", name_temp, id_temp, number_temp, type_temp, pin_temp);
}
    fclose(fp_del);
    fclose(temp_fp);
    remove("database/accounts.txt");
    rename("database/temp_accounts.txt", "database/accounts.txt");

    FILE *log_fp = fopen("database/transactions.log", "a");
    if (log_fp) {
        fprintf(log_fp, "Account deleted for %s with ID %s and account type %s\n", name, id, account_type);
        fclose(log_fp);
    }

}

void deposit(){
    char account_number[20], account_type[20], account_type_name[50], account_file_name[60];
    char line[200];
    char name[50], id[20];
    int stored_pin = -1, pin;
    float amount, balance = 0.0;

    printf("Press Enter without Keying in Anything to Cancel at any time...\n");
    printf("Enter Your Account Number: ");
    fgets(account_number, sizeof(account_number), stdin);
    if (cancel_process(account_number)) return;
    account_number[strcspn(account_number, "\n")] = '\0';
    printf("Enter Your Bank Account Type (1: Savings/2: Current): ");
    fgets(account_type, sizeof(account_type), stdin);
    account_type[strcspn(account_type, "\n")] = '\0';
    if (cancel_process(account_type)) return;
    acccompare(account_type);
    if(strlen(account_type)==0) return;
    strcpy(account_type_name, account_type);
    // check if account exists then return if not
    sprintf(account_file_name,"database/%s_%s.txt", account_number, account_type_name);
    FILE *fp = fopen(account_file_name, "r");
    if (fp == NULL) {
        printf("Account doesn't exist!\n");
        return;
    }
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "Name: %49[^\n]", name);
        sscanf(line, "ID: %19s", id);
        sscanf(line, "PIN: %d", &stored_pin);
        sscanf(line, "Balance: RM %f", &balance);
    }
    fclose(fp);
    printf("Enter your PIN: ");
    scanf("%d", &pin);
    while(getchar() != '\n');
    if (pin != stored_pin){
        printf("Incorrect PIN.\n");
        return;
    }
    printf("Enter the amount to deposit: ");
    scanf("%f", &amount);
    while(getchar() != '\n');
    if (amount <= 0 || amount > 50000){
        printf("Invalid amount entered. Please try again.\n");
        return;
    }
    balance += amount;
    fp = fopen(account_file_name, "w");
    if (fp == NULL) {
        printf("Error opening account file for writing.\n");
        return;
    }
    fprintf(fp, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %s\nPIN: %d\nBalance: RM %.2f\n", name, id, account_type_name, account_number, stored_pin, balance);
    fclose(fp);
    printf("Successfully deposited RM %.2f to your account. New balance is RM %.2f\n", amount, balance);

    FILE *log_fp = fopen("database/transactions.log", "a");
    if (log_fp == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(log_fp, "Deposited %.2f to account %s of type %s\n", amount, account_number, account_type_name);
    fclose(log_fp);
    
}

void withdrawal(){
    char account_number[20], account_type[20], account_type_name[50], account_file_name[60];
    char line[200];
    char name[50], id[20];
    int stored_pin = -1, pin;
    float amount, balance = 0.0;

    printf("Press Enter without Keying in Anything to Cancel at any time...\n");
    printf("Enter Your Account Number: ");
    fgets(account_number, sizeof(account_number), stdin);
    account_number[strcspn(account_number, "\n")] = '\0';
    if (cancel_process(account_number)) return;
    printf("Enter Your Bank Account Type (1: Savings/2: Current): ");
    fgets(account_type, sizeof(account_type), stdin);
    account_type[strcspn(account_type, "\n")] = '\0';
    if (cancel_process(account_type)) return;
    acccompare(account_type);
    if(strlen(account_type)==0) return;
    strcpy(account_type_name, account_type);
    // check if account exists then return if not
    sprintf(account_file_name,"database/%s_%s.txt", account_number, account_type_name);
    FILE *fp = fopen(account_file_name, "r");
    if (fp == NULL) {
        printf("Account doesn't exist!\n");
        return;
    }
    while(fgets(line, sizeof(line), fp)) {
        sscanf(line, "Name: %49[^\n]", name);
        sscanf(line, "ID: %19s", id);
        sscanf(line, "PIN: %d", &stored_pin);
        sscanf(line, "Balance: RM %f", &balance);
    }
    fclose(fp);
    printf("Enter your PIN: ");
    scanf("%d", &pin);
    while(getchar() != '\n');
    if (pin != stored_pin){
        printf("Incorrect PIN.\n");
        return;
    }
    printf("Balance: RM %.2f\n", balance);
    printf("Enter the amount to withdraw: ");
    scanf("%f", &amount);
    while(getchar() != '\n');
    if (amount <= 0 || amount > balance){
        printf("Invalid amount entered. Please try again.\n");
        return;
    }
    balance -= amount;
    fp = fopen(account_file_name, "w");
    if (fp == NULL) {
        printf("Error opening account file for reading.\n");
        return;
    }
    fprintf(fp, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %s\nPIN: %d\nBalance: RM %.2f\n", name, id, account_type_name, account_number, stored_pin, balance);
    fclose(fp);
    printf("Successfully withdrew RM %.2f to your account. New balance is RM %.2f\n", amount, balance);

    FILE *log_fp = fopen("database/transactions.log", "a");
    if (log_fp == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(log_fp, "Withdrew %.2f to account %s of type %s\n", amount, account_number, account_type_name);
    fclose(log_fp);
    
}

void remittance(){
    char sender_account_number[20], sender_account_type[20], sender_account_type_name[50], sender_account_file_name[60];
    char receiver_account_number[20], receiver_account_type[20], receiver_account_type_name[50], receiver_account_file_name[60];
    char line_send[200], line_receive[200];
    char name_send[50], name_receive[50];
    char id_send[20], id_receive[20];
    int stored_pin_send = -1, stored_pin_receive = -1, pin;
    float amount, balance_send = 0.0, balance_receive = 0.0;
    float fee_rate = 0.0;
    int same_account;
    float total_deduction;

    printf("Enter the Sender's Account Number: ");
    fgets(sender_account_number, sizeof(sender_account_number), stdin);
    sender_account_number[strcspn(sender_account_number, "\n")] = '\0';
    if (cancel_process(sender_account_number)) return;
    printf("Enter the Sender's Bank Account Type (1: Savings/2: Current): ");
    fgets(sender_account_type, sizeof(sender_account_type), stdin);
    sender_account_type[strcspn(sender_account_type, "\n")] = '\0';
    if (cancel_process(sender_account_type)) return;
    acccompare(sender_account_type);
    if(strlen(sender_account_type)==0) return;
    strcpy(sender_account_type_name, sender_account_type);
    // check if sender account exists then return if not
    sprintf(sender_account_file_name,"database/%s_%s.txt", sender_account_number, sender_account_type_name);
    FILE *fp = fopen(sender_account_file_name, "r");
    if (fp == NULL) {
        printf("Sender Account doesn't exist!\n");
        return;
    }
    while(fgets(line_send, sizeof(line_send), fp)) {
        sscanf(line_send, "Name: %49[^\n]", name_send);
        sscanf(line_send, "ID: %19s", id_send);
        sscanf(line_send, "PIN: %d", &stored_pin_send);
        sscanf(line_send, "Balance: RM %f", &balance_send);
    }
    fclose(fp);
    printf("Enter the Receiver's Account Number: ");
    fgets(receiver_account_number, sizeof(receiver_account_number), stdin);
    receiver_account_number[strcspn(receiver_account_number, "\n")] = '\0';
    if (cancel_process(receiver_account_number)) return;
    printf("Enter the Receiver's Bank Account Type (1: Savings/2: Current): ");
    fgets(receiver_account_type, sizeof(receiver_account_type), stdin);
    receiver_account_type[strcspn(receiver_account_type, "\n")] = '\0';
    if (cancel_process(receiver_account_type)) return;
    acccompare(receiver_account_type);
    if(strlen(receiver_account_type)==0) return;
    strcpy(receiver_account_type_name, receiver_account_type);
    // check if receiver account exists then return if not
    sprintf(receiver_account_file_name,"database/%s_%s.txt", receiver_account_number, receiver_account_type_name);
    FILE *fp_r = fopen(receiver_account_file_name, "r");
    if (fp_r == NULL) {
        printf("Receiver Account doesn't exist!\n");
        return;
    }
    while(fgets(line_receive, sizeof(line_receive), fp_r)) {
        sscanf(line_receive, "Name: %49[^\n]", name_receive);
        sscanf(line_receive, "ID: %19s", id_receive);
        sscanf(line_receive, "PIN: %d", &stored_pin_receive);
        sscanf(line_receive, "Balance: RM %f", &balance_receive);
    }
    fclose(fp_r);


    printf("Enter Sender's PIN: ");
    scanf("%d", &pin);
    while(getchar() != '\n');
    if (pin != stored_pin_send){
        printf("Incorrect PIN.\n");
        return;
    }
    printf("Enter the amount to remit: ");
    scanf("%f", &amount);
    while(getchar() != '\n');
    if (amount <= 0){
        printf("Invalid amount entered. Please try again.\n");
        return;
    }
    same_account = (strcmp(sender_account_number, receiver_account_number) == 0 && strcmp(sender_account_type_name, receiver_account_type_name) == 0);
    if (!same_account){
        if (strcmp(sender_account_type_name, "Savings") == 0 && strcmp(receiver_account_type_name, "Current") == 0){
            fee_rate = 0.02;
        }
        else if (strcmp(sender_account_type_name, "Current") == 0 && strcmp(receiver_account_type_name, "Savings") == 0){
            fee_rate = 0.03;
        }
    }
    total_deduction = amount * (1 + fee_rate);
    if (total_deduction > balance_send){
        printf("Insufficient funds in sender's account including fees. Please try again.\n");
        return;
    }
    balance_send -= total_deduction;
    balance_receive += amount;
    fp = fopen(sender_account_file_name, "w");
    if (fp == NULL) {
        printf("Error opening sender account file for writing.\n");
        return;
    }
    fprintf(fp, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %s\nPIN: %d\nBalance: RM %.2f\n", name_send, id_send, sender_account_type_name, sender_account_number, stored_pin_send, balance_send);
    fclose(fp);
    fp_r = fopen(receiver_account_file_name, "w");
    if (fp_r == NULL) {
        printf("Error opening receiver account file for reading.\n");
        return;
    }
    fprintf(fp_r, "Name: %s\nID: %s\nAccount Type: %s\nAccount Number: %s\nPIN: %d\nBalance: RM %.2f\n", name_receive, id_receive, receiver_account_type_name, receiver_account_number, stored_pin_receive, balance_receive);
    fclose(fp_r);
    printf("Successfully remitted RM %.2f from account %s to account %s, fee %.2f\n", amount, sender_account_number, receiver_account_number, amount * fee_rate);
    FILE *log_fp = fopen("database/transactions.log", "a");
    if (log_fp == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(log_fp, "Remitted %.2f from account %s of type %s to account %s of type %s, fee %.2f\n", amount, sender_account_number, sender_account_type_name, receiver_account_number, receiver_account_type_name, amount * fee_rate);
    fclose(log_fp);
}
void sleep_seconds(int seconds) {
    clock_t start_time = clock();
    while (clock() < start_time + seconds * CLOCKS_PER_SEC);
}
void options(){
    if (strcmp(option, "1") == 0 || strcmp(option, "create") == 0 || strcmp(option, "new") == 0 || strcmp(option, "make") == 0 || strcmp(option, "open") == 0 || strcmp(option, "add") == 0) {
        create_account();
    } else if (strcmp(option, "2") == 0 || strcmp(option, "delete") == 0 || strcmp(option, "remove") == 0 || strcmp(option, "close") == 0 || strcmp(option, "del") == 0 || strcmp(option, "rm") == 0) {
        delete_account();
    } else if (strcmp(option, "3") == 0 || strcmp(option, "deposit") == 0 || strcmp(option, "addfund") == 0 || strcmp(option, "addfunds") == 0 || strcmp(option, "add money") == 0 || strcmp(option, "addmoney") == 0 || strcmp(option, "topup") == 0 || strcmp(option, "top up") == 0 || strcmp(option, "fund") == 0 || strcmp(option, "funds") == 0) {
        deposit();
    } else if (strcmp(option, "4") == 0 || strcmp(option, "withdraw") == 0 || strcmp(option, "withdrawal") == 0 || strcmp(option, "takeout") == 0 || strcmp(option, "take out") == 0 || strcmp(option, "removefund") == 0 || strcmp(option, "removefunds") == 0 || strcmp(option, "remove money") == 0 || strcmp(option, "removemoney") == 0) {
        withdrawal();
    } else if (strcmp(option, "5") == 0 || strcmp(option, "remit") == 0 || strcmp(option, "remittance") == 0 || strcmp(option, "transfer") == 0 || strcmp(option, "sendmoney") == 0 || strcmp(option, "send money") == 0 || strcmp(option, "pay") == 0 || strcmp(option, "payment") == 0 || strcmp(option, "paymoney") == 0 || strcmp(option, "pay money") == 0) {
        remittance();
    } else if (strcmp(option, "6") == 0 || strcmp(option, "exit") == 0 || strcmp(option, "quit") == 0 || strcmp(option, "closeapp") == 0 || strcmp(option, "close app") == 0 || strcmp(option, "bye") == 0 || strcmp(option, "goodbye") == 0) {
    const char *message = "Program will exit soon";
    for (int i = 0; i < 3; i++) {
        printf("\r%s", message);
        for (int j = 0; j < i % 4; j++)
            printf(".");
        fflush(stdout);
        sleep_seconds(1);
    }
    exit(0);
    } else {
        printf("Invalid Option. Please try again.\n");
    }
}
int main(){
    srand(time(NULL));
    system("mkdir database >nul 2>nul");
    while (1){
        text_menu();
        options();
    }
    return 0;
}
