#include "sqlite\sqlite3.h"
#include <stdbool.h>

// Database Functions
bool create_table(sqlite3* db);
bool create_table_login_credentials(sqlite3* db);
void main_menu(sqlite3* db);
void update_inventory(sqlite3* db);
void view_inventory_sales(sqlite3* db);

// functions to update table in database
int add_product(sqlite3* db, const char* name, int quantity, double price, const char* category);
int add_stock(sqlite3* db, int id, int quantity);
int update_name(sqlite3* db, int id, const char* name);
int update_price(sqlite3* db, int id, double price);
int delete_product(sqlite3* db, int id);
int view_inventory(sqlite3* db);
int view_sales(sqlite3* db);
int process_sale(sqlite3* db, int id, int quantity);
void alert_for_low_stock(sqlite3* db);


// Integer Validation Function

bool parse_int(char* string, int* integer);

// Permission Based-Access Control
// Admin

void permission_based_access_control(sqlite3* db);
int reset_inventory(sqlite3* db);
void create_new_user(sqlite3* db, const char* name, int idNumber, const char* password);
static void empty_stdin(void);
void admin_menu(sqlite3* db);
int get_Number(char* to_print);
void removeaccount(sqlite3* db, int idNumber);
void view_users(sqlite3* db);
