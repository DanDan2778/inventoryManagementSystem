#include <stdio.h>
#include "sqlite\sqlite3.h"

void enter_login_details(int* idNumber, char* password);
void login_verification(sqlite3* db, int id, char* password);
