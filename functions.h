#include "sqlite\sqlite3.h"
#include <stdbool.h>

// ADD ANOTHER FUNCTION WHERE USER NEEDS TO LOG IN
// ADMIN AND A USER CAN BE ADDED
// ADMIN SHOULD HAVE THE ABILITY TO ADD A NEW USER
// ADMIN HAS A RESET FUNCTION IN ITS UI

bool create_table(sqlite3* db);
bool create_table_login_credentials(sqlite3* db);
void main_menu(sqlite3* db);
void update_inventory(sqlite3* db);
void view_inventory_sales(sqlite3* db);

// functions to add to the database
int add_product(sqlite3* db, const char* name, int quantity, double price);// done
int add_stock(sqlite3* db, int id, int quantity); // done
int update_name(sqlite3* db, int id, const char* name); // done
int update_price(sqlite3* db, int id, double price); // done
int delete_product(sqlite3* db, int id); // done
int view_inventory(sqlite3* db); // done
int view_sales(sqlite3* db); // done
int process_sale(sqlite3* db, int id, int quantity); // done
void alert_for_low_stock(sqlite3* db);


