#define _CRT_SECURE_NO_WARNINGS
#include "sqlite\sqlite3.h"
#include "functions.h"
#include "login.h"
#include <string.h>


int main() {
	sqlite3* db;
	int idNumber;
	char password[100], input_password[100];
	int rc = sqlite3_open("inventory.db", &db);
	if (rc != SQLITE_OK) {
		printf("Can't open database: %s\n", sqlite3_errmsg(db)); 
		sqlite3_errmsg(db);
		return 0;
	}
	if (!create_table(db)) {
		return;
	}
	if (!create_table_login_credentials(db)) {
		return;
	}
	enter_login_details(&idNumber, password);
	login_verification(db, idNumber, password);
	
	
	// Main Menu has all the functions and attributes needed to run this fucking program...
}
		