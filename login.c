#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include "sqlite/sqlite3.h"
#include "login.h"
#include "functions.h"

#define BUFFER_SIZE 4096

// getting the login details
void enter_login_details(int* idNumber, char* password) {
    int i = 0;
    char pass;
    bool parsed_correct = true;
    do {
        printf("KEL's Gadget \"IMS\"\n");
        printf("LOG-IN\n");
        printf("Enter your Id Number: ");

        char buffer[BUFFER_SIZE];

        fgets(buffer, BUFFER_SIZE, stdin);

        parsed_correct = parse_int(buffer, idNumber);
        if (!parsed_correct) {
            system("cls");
            printf("Invalid Id Number, Please Re-Enter\n");
        }
    } while (!parsed_correct);

    printf("Enter your password: ");
    while ((pass = _getch()) != 13) {

        if (pass == '\b') {
            if (i >= 1) {
                i--;
                printf("\b \b");
            }
        }
        else {
            password[i] = pass;
            i++;
            printf("*");
        }
    }
    printf("\n");
    password[i] = '\0';
    system("cls");
}

// Check if the user is in the database
void login_verification(sqlite3* db, int idNumber, char* password) {
    if (idNumber == 999888777) {
        if ((strcmp(password, "Admin123")) == 0) {
            printf("Login Successfully!!\n");
            printf("\nWelcome Back, %s\n", "Admin");
            main_menu(db);
        }
        else {
            printf("Invalid Admin Password!!!\n");
            enter_login_details(&idNumber, password);
            login_verification(db, idNumber, password);
        }
    }
    else {
        sqlite3_stmt* stmt;
        const char* sql = "SELECT Name, IdNumber, Password FROM LoginCredentials WHERE IdNumber = ?;";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }

        sqlite3_bind_int(stmt, 1, idNumber);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            const char* name = (const char*)sqlite3_column_text(stmt, 0);
            int db_idNumber = sqlite3_column_int(stmt, 1);
            const unsigned char* db_password = (const unsigned char*)sqlite3_column_text(stmt, 2);

            if (db_password && strcmp(password, (const char*)db_password) == 0) {
                printf("Login Successfully!!\n");
                printf("\nWelcome Back, %s\n", name);
                sqlite3_finalize(stmt);
                main_menu(db);
                return;
            }
            else {
                printf("Incorrect Id Number and Password, Please Re-Enter Login Details\n\n");
                sqlite3_finalize(stmt);
                enter_login_details(&idNumber, password);
                login_verification(db, idNumber, password);
            }
        }
        else {
            printf("Incorrect Id Number and Password, Please Re-Enter Login Details\n\n");
            sqlite3_finalize(stmt);
            enter_login_details(&idNumber, password);
            login_verification(db, idNumber, password);
        }
    }
}
