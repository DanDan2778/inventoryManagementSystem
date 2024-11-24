#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite\sqlite3.h"
#include "login.h"

// getting the login details

void enter_login_details(int *idNumber, char *password) {
	int i = 0;
	char pass;
	printf("LOG-IN\n");
	printf("Enter ID Number: ");
	while (scanf("%d", &idNumber) != 1) {
		printf("Invalid input. Please enter an integer ID number: ");
		fflush(stdin); 
	}
	printf("Enter your password: ");
	while ((pass = _getch()) != 13) {
		password[i] = pass;
		i++;
		printf("*");
	}
	printf("\n");
	password[i] = '\0';
}


// Check if the user is in the database
void login_verification(sqlite3* db, int idNumber, char* password) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT Name, IdNumber, Password FROM LoginCredentials WHERE IdNumber = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	sqlite3_bind_int(stmt, 1, idNumber);

	rc = sqlite3_step(stmt);
	printf("Code in\n");
	if (rc == SQLITE_ROW) {
		const char* name = sqlite3_column_text(stmt, 0);
		int idNumber = (int)sqlite3_column_int(stmt, 1);
		const unsigned char* db_password = (const char*)sqlite3_column_text(stmt, 2);
		
		if (strcmp(password, db_password) == 0) {
			printf("Login Successfully!!\n");
			printf("Welcome Back, %s\n", name);
			sqlite3_finalize(stmt);
			return 0;
		}
		else {
			printf("Incorrect Id Number and Password, Please Re-Enter Login Details\n\n");
			sqlite3_finalize(stmt);
			enter_login_details(&idNumber, password);
		}
	}
	else {
		printf("Incorrect Id Number and Password, Please Re-Enter Login Details\n\n");
		sqlite3_finalize(stmt);
		enter_login_details(&idNumber, password);
	}

}