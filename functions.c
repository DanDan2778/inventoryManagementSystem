#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite\sqlite3.h"
#include "functions.h"
#include <windows.h>
#include "table\fort.h"
#define BUFFER_SIZE 4096


bool create_table(sqlite3* db) {
	const char* sql = "CREATE TABLE IF NOT EXISTS Inventory (\
		Id INTEGER PRIMARY KEY AUTOINCREMENT, \
		Product TEXT, \
		Stock INTEGER, \
		Price REAL, \
		Sold INTEGER, \
		Total REAL, \
		Category TEXT);";

	char* err_msg = 0;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK) {
		printf("SQL ERROR: %s\n", err_msg);
		sqlite3_free(err_msg);
		return false;
	}
	return true;
}

// Main Menu
void main_menu(sqlite3* db) {
	int choice, id, sold;
	while (true) {
		printf("MAIN MENU\n");
		printf("1. Update Inventory\n");
		printf("2. View Inventory/Sales\n");
		printf("3. Proccess Sale\n");
		printf("4. Permission Based-Access Control\n"); // New create switch, function, and verification
		printf("5. Close/Exit\n");
		printf("Enter Choice: ");
		if (scanf("%d", &choice) != 1) {
			fprintf(stderr, "\nInvalid Input\n");
			while (getchar() != '\n') {}
			continue;
		}
		system("cls");

		switch (choice) {
		case 1: {
			update_inventory(db);
			break;
		}
		case 2: {
			view_inventory_sales(db);
			break;
		}
		case 3: {
			// Process Sale
			view_inventory(db);
			id = get_Number("Enter Product Id: ");
			sold = get_Number("Enter Items to be Sold: ");
			process_sale(db, id, sold);
			break;
		}
		case 4: {
			permission_based_access_control(db);
		}
		case 5: {
			printf("\nExiting Program!!!\n");
			Sleep(2000);
			system("cls");
			exit(0);
			return;
		}
		default:
			printf("Invalid Choice");
			break;
		}
	}
}

// Update Inventory
void update_inventory(sqlite3* db) {
	int choice, quantity, id;
	char name[100];
	double price;
	char* category = NULL;

	while (true) {
		printf("Update Inventory\n");
		printf("1. Add New Item/Product\n");
		printf("2. ADD STOCK\n");
		printf("3. Change Item/Product Name\n");
		printf("4. Change Price\n");
		printf("5. Delete Item/Product\n");
		printf("6. Back to Main Menu\n");
		printf("7. Close/Exit\n");
		printf("Enter Choice: ");
		if (scanf("%d", &choice) != 1) {
			printf("\nInvalid Input\n");
			while (getchar() != '\n') {}
			continue;
		}
		system("cls");

		switch (choice) {
		case 1:
		{ // New Product
			printf("Add New Product\n");
			while (true) {
				printf("Product Category: \n");
				printf("1. Mobile Phones\n");
				printf("2. Computer & Laptops\n");
				printf("3. Tablets\n");
				printf("4. Accessories\n");
				int choice = get_Number("Enter Choice: ");
				switch (choice) {
				case 1: // Mobile Phones
					category = "Mobile Phones";
					break;
				case 2: // Computer & Laptops
					category = "Computer & Laptops";
					break;
				case 3: // Tablets
					category = "Tablets";
					break;
				case 4: // Accessories
					category = "Accessories";
					break;
				default:
					system("cls");
					printf("Invalid choice!\n");
					continue;
				}

				break;
			}
			printf("Enter Product Name: ");
			while (getchar() != '\n');
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = 0;
			while (true) {
				printf("Enter Quantity: ");
				int res = scanf("%d", &quantity);
				if (res == 0) {
					printf("Invalid Input\n");
					empty_stdin();
					continue;
				}
				break;
			}
			while (true) {
				printf("Enter Product Selling Price: ");
				int res = scanf("%lf", &price);
				if (res == 0) {
					printf("Invalid Input\n");
					empty_stdin();
					continue;
				}
				break;
			}
			add_product(db, name, quantity, price, category);
			break;
		}
		case 2:// Working
		{ // Update Stock
			view_inventory(db);
			id = get_Number("Enter Id Number: ");
			quantity = get_Number("Enter Quantity to be Added: ");
			add_stock(db, id, quantity);
			break;
		}
		case 3:
		{ // Update Product Name
			view_inventory(db);
			printf("Update Product Name\n");
			printf("Enter Product Id: ");
			(void)scanf("%d", &id);
			printf("Enter New Product Name: ");
			while (getchar() != '\n');
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = 0;
			update_name(db, id, name);
			break;
		}
		case 4:
		{ // Update Price
			view_inventory(db);
			printf("\nUpdate Price\n");
			printf("Enter Product Id: ");
			(void)scanf("%d", &id);
			printf("Enter New Price: ");
			(void)scanf("%lf", &price);
			update_price(db, id, price);
			break;
		}
		case 5:
		{ // Delete Product
			view_inventory(db);
			printf("\nDelete Product\n");
			printf("Enter Product Id: ");
			(void)scanf("%d", &id);
			delete_product(db, id);
			break;
		}
		case 6:
		{
			system("cls");
			main_menu(db);
			printf("\n");
			return;
		}
		case 7:
		{
			system("cls");
			printf("\nExiting Program!!!\n");
			Sleep(2000);
			system("cls");
			exit(0);
		}
		default:
		{
			printf("\nInvalid Input!!\n");
		}
		}

		printf("\n");
	}
}

// View Inventory
void view_inventory_sales(sqlite3* db) {
	int choice;
	while (true) {
		printf("View Inventory/Sales\n");
		printf("1. View Inventory\n");
		printf("2. View Sales Report\n");
		printf("3. Back to Main Menu\n");
		printf("4. Close/Exit\n");
		printf("Enter Choice: ");
		if (scanf("%d", &choice) != 1) {
			fprintf(stderr, "Invalid Choice");
			while (getchar() != '\n') {}
			continue;
		}
		system("cls");

		switch (choice)
		{
		case 1:
		{
			view_inventory(db);
			alert_for_low_stock(db);
			break;
		}
		case 2:
		{
			printf("Sales Report\n");
			view_sales(db);
			alert_for_low_stock(db);
			//view_sales();
			break;
		}
		case 3:
		{
			system("cls");
			main_menu(db);
			printf("\n");
			return;
		}
		case 4:
		{
			system("cls");
			printf("\nExiting Program!!!\n");
			Sleep(2000);
			system("cls");
			exit(0);
		}
		default:
		{
			printf("Invalid Choice\n");
		}
		}

		printf("\n");
	}
}

// DataBase
// Add Product
int add_product(sqlite3* db, const char* name, int quantity, double price, const char* category) {
	sqlite3_stmt* stmt;
	const char* sql = "INSERT INTO Inventory(Product, Stock, Price, Sold, Total, Category) VALUES (?, ?, ?, 0, 0, ?);";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, quantity);
	sqlite3_bind_double(stmt, 3, price);
	sqlite3_bind_text(stmt, 4, category, -1, SQLITE_STATIC);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Failed to insert Product: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}
	system("cls");
	printf("Adding Product...");
	Sleep(2000);
	system("cls");
	view_inventory(db);
	printf("Product added successfully\n");
	sqlite3_finalize(stmt);
	return 0;
}


// Add Stock
// Continue from here
int add_stock(sqlite3* db, int id, int quantity) {
	sqlite3_stmt* stmt;
	const char* check_query = "SELECT 1 FROM Inventory WHERE Id = ?";
	int rc;

	rc = sqlite3_prepare_v2(db, check_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare check query: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char* update_query = "UPDATE Inventory SET Stock = Stock + ? WHERE Id = ?";

		sqlite3_finalize(stmt);

		rc = sqlite3_prepare_v2(db, update_query, -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot prepare update query: %s\n", sqlite3_errmsg(db));
			return 1;
		}

		sqlite3_bind_int(stmt, 1, quantity);
		sqlite3_bind_int(stmt, 2, id);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			fprintf(stderr, "Failed to update stock: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return 1;
		}
		system("cls");
		printf("Adding Stock...");
		Sleep(2000);
		system("cls");
		printf("Stock updated successfully\n");

		sqlite3_finalize(stmt);
		view_inventory(db);
		update_inventory(db);
	}
	else if (rc == SQLITE_DONE) {
		system("cls");
		printf("Adding Stock...");
		Sleep(2000);
		system("cls");
		printf("CANNOT FIND PRODUCT ID IN THE INVENTORY\n");
	}
	else {
		fprintf(stderr, "Failed to check if ID exists: %s\n", sqlite3_errmsg(db));
	}

	// Finalize the statement before returning
	sqlite3_finalize(stmt);
	return 0;
}

// Update Name
int update_name(sqlite3* db, int id, const char* name) {
	sqlite3_stmt* stmt;
	const char* check_query = "SELECT 1 FROM Inventory WHERE Id = ?";
	int rc;

	rc = sqlite3_prepare_v2(db, check_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare check query: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char* update_query = "UPDATE Inventory SET Product = ? WHERE Id = ?";

		sqlite3_finalize(stmt);

		rc = sqlite3_prepare_v2(db, update_query, -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot prepare update query: %s\n", sqlite3_errmsg(db));
			return 1;
		}

		sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt, 2, id);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			fprintf(stderr, "Failed to update stock: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return 1;
		}
		system("cls");
		printf("Updating Product Name...");
		Sleep(2000);
		system("cls");
		printf("Product Name updated successfully\n");

		sqlite3_finalize(stmt);
		view_inventory(db);
		update_inventory(db);
	}
	else if (rc == SQLITE_DONE) {
		system("cls");
		printf("Updating Product Name...");
		Sleep(2000);
		system("cls");
		printf("CANNOT FIND PRODUCT ID IN THE INVENTORY\n");
	}
	else {
		fprintf(stderr, "Failed to check if ID exists: %s\n", sqlite3_errmsg(db));
	}

	// Finalize the statement before returning
	sqlite3_finalize(stmt);
	return 0;
}

// Update Price
int update_price(sqlite3* db, int id, double price) {
	sqlite3_stmt* stmt;
	const char* check_query = "SELECT 1 FROM Inventory WHERE Id = ?";
	int rc;

	rc = sqlite3_prepare_v2(db, check_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare check query: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char* update_query = "UPDATE Inventory SET Price = ? WHERE Id = ?";

		sqlite3_finalize(stmt);

		rc = sqlite3_prepare_v2(db, update_query, -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot prepare update query: %s\n", sqlite3_errmsg(db));
			return 1;
		}

		sqlite3_bind_double(stmt, 1, price);
		sqlite3_bind_int(stmt, 2, id);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			fprintf(stderr, "Failed to update Price: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return 1;
		}
		system("cls");
		printf("Updating Price...");
		Sleep(2000);
		system("cls");
		printf("Price updated successfully\n");

		sqlite3_finalize(stmt);
		view_inventory(db);
		update_inventory(db);
	}
	else if (rc == SQLITE_DONE) {
		system("cls");
		printf("Updating Price...");
		Sleep(2000);
		system("cls");
		printf("CANNOT FIND PRODUCT ID IN THE INVENTORY\n");
	}
	else {
		fprintf(stderr, "Failed to check if ID exists: %s\n", sqlite3_errmsg(db));
	}

	// Finalize the statement before returning
	sqlite3_finalize(stmt);
	return 0;
}

// Delete Product
int delete_product(sqlite3* db, int id) {
	sqlite3_stmt* stmt;
	const char* check_query = "SELECT 1 FROM Inventory WHERE Id = ?";
	int rc;

	rc = sqlite3_prepare_v2(db, check_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare check query: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char* update_query = "DELETE FROM Inventory WHERE Id = ?";

		sqlite3_finalize(stmt);

		rc = sqlite3_prepare_v2(db, update_query, -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot prepare update query: %s\n", sqlite3_errmsg(db));
			return 1;
		}

		sqlite3_bind_int(stmt, 1, id);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			fprintf(stderr, "Failed to Delete Product: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return 1;
		}
		system("cls");
		printf("Deleting Product...");
		Sleep(2000);
		system("cls");
		printf("Product Removed successfully\n");

		sqlite3_finalize(stmt);
		view_inventory(db);
		update_inventory(db);
	}
	else if (rc == SQLITE_DONE) {
		system("cls");
		printf("Deleting Product...");
		Sleep(2000);
		system("cls");
		printf("CANNOT FIND PRODUCT ID IN THE INVENTORY\n");
	}
	else {
		fprintf(stderr, "Failed to check if ID exists: %s\n", sqlite3_errmsg(db));
	}

	// Finalize the statement before returning
	sqlite3_finalize(stmt);
	return 0;
}

// VIEW INVENTORY
int view_inventory(sqlite3* db) {
	sqlite3_stmt* stmt;
	ft_table_t* table = ft_create_table();
	ft_set_border_style(table, FT_BASIC_STYLE);
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	printf("%-30s%s", " ", "INVENTORY\n");
	ft_write_ln(table, "ID", "Product", "Category", "Stock", "Price");

	const char* sql = "SELECT Id, Product, Stock, Price, Category FROM Inventory ORDER BY CASE WHEN category = 'Compute & Laptops' THEN 2 WHEN category = 'Tablets' THEN 3 WHEN category = 'Mobile Phones' THEN 1 WHEN category = 'Accessories' THEN 4 ELSE 4 END, price ASC;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		printf("Cannot Prepare Statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}
	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const char* product = sqlite3_column_text(stmt, 1);
		int stock = sqlite3_column_int(stmt, 2);
		double price = sqlite3_column_double(stmt, 3);
		const char* category = sqlite3_column_text(stmt, 4);

		char id_str[5];
		sprintf(id_str, "%d", id);
		char stock_str[20];
		sprintf(stock_str, "%d", stock);
		char price_str[25];
		sprintf(price_str, "P %.2lf", price);


		ft_write_ln(table, id_str, product, category, stock_str, price_str);
	}
	printf("%s\n", ft_to_string(table));
	ft_destroy_table(table);
	sqlite3_finalize(stmt);
	return 0;
}

// VIEW SALES
int view_sales(sqlite3* db) {
	sqlite3_stmt* stmt;
	ft_table_t* table = ft_create_table();
	ft_set_border_style(table, FT_BASIC_STYLE);
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_write_ln(table, "ID", "PRODUCT", "TOTAL SALES", "TOTAL REVENUE");
	const char* sql = "SELECT Id, Product, Sold, Total FROM Inventory WHERE Sold > 0 ORDER BY CASE WHEN category = 'Computer & Laptops' THEN 2 WHEN category = 'Tablets' THEN 3 WHEN category = 'Mobile Phones' THEN 1 WHEN category = 'Accessories' THEN 4 ELSE 5 END, Sold DESC;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		printf("Cannot Prepare Statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const char* product = sqlite3_column_text(stmt, 1);
		int sold = sqlite3_column_int(stmt, 2);
		double total = sqlite3_column_double(stmt, 3);

		char id_str[5];
		sprintf(id_str, "%d", id);
		char sold_str[10];
		sprintf(sold_str, "%d", sold);
		char total_str[20];
		sprintf(total_str, "P %.2lf", total);
		ft_write_ln(table, id_str, product, sold_str, total_str);
	}
	printf("%s", ft_to_string(table));
	ft_destroy_table(table);
	sqlite3_finalize(stmt);
	return 0;
}

// PROCCESS SALES
int process_sale(sqlite3* db, int id, int sold) {
	sqlite3_stmt* stmt;
	const char* check_query = "SELECT 1 FROM Inventory WHERE Id = ?";
	int rc;

	rc = sqlite3_prepare_v2(db, check_query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare check query: %s\n", sqlite3_errmsg(db));
		return 1;  
	}

	sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char* sale_query = "UPDATE Inventory SET Sold = Sold + ?, Total = Total + ? * Price, Stock = Stock - ? WHERE Id = ?";

		sqlite3_finalize(stmt);

		rc = sqlite3_prepare_v2(db, sale_query, -1, &stmt, 0);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Cannot prepare sale query: %s\n", sqlite3_errmsg(db));
			return 1;  
		}

		sqlite3_bind_int(stmt, 1, sold);  
		sqlite3_bind_int(stmt, 2, sold);  
		sqlite3_bind_int(stmt, 3, sold);  
		sqlite3_bind_int(stmt, 4, id);   

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			fprintf(stderr, "Failed to process sale: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return 1;
		}

		system("cls");
		printf("Processing Sale...");
		Sleep(2000); 
		system("cls");

		printf("Sale processed successfully!!!\n");

		sqlite3_finalize(stmt);

		view_inventory(db);
		main_menu(db);
	}
	else if (rc == SQLITE_DONE) {
		system("cls");
		printf("Processing Sale...");
		Sleep(2000);  
		system("cls");
		printf("CANNOT FIND PRODUCT ID IN THE INVENTORY\n");
	}
	else {
		fprintf(stderr, "Failed to check if ID exists: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);
	return 0;  

}

// Alert for low stock
void alert_for_low_stock(sqlite3* db) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT Id, Product, Stock FROM Inventory WHERE Stock < 5;";
	int prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	printf("\nLOW STOCK: Please Restock!!!\n");
	printf("%-10s%-20s%-20s\n", "Id", "Product", "Available Stock");
	while ((prepare = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const char* product = sqlite3_column_text(stmt, 1);
		int stock = sqlite3_column_int(stmt, 2);

		printf("%-10d%-20s%-20d\n", id, product, stock);
	}
	sqlite3_finalize(stmt);
	return;
}

// Creating Table for Log-in Credentials
bool create_table_login_credentials(sqlite3* db) {
	const char* sql = "CREATE TABLE IF NOT EXISTS LoginCredentials (\
		Name TEXT, \
		IdNumber INTEGER, \
		Password TEXT); ";

	const char* err_msg = 0;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK) {
		printf("SQL ERROR: %s\n", err_msg);
		sqlite3_free(err_msg);
		return false;
	}
	return true;
}

// Integer Validation Function
bool parse_int(char* string, int* integer)
{
	int i = 0;
	while (isspace(string[i])) i++;

	int length = (int)strlen(string);
	if (length == i) return false;

	char integer_buffer[BUFFER_SIZE];
	int integer_chars = 0;

	if (string[i] == '-')
	{
		integer_buffer[integer_chars] = '-';
		integer_chars++;
		i++;

		if (!isdigit(string[i])) return false;

		while (i < length && !isspace(string[i])) {
			if (!isdigit(string[i])) return false;
			integer_buffer[integer_chars] = string[i];

			integer_chars++;

			i++;
		}

		integer_buffer[integer_chars] = '\0';

		while (isspace(string[i])) i++;

		if (string[i] != '\0') return false;

		*integer = atoi(integer_buffer);
		return true;
	}

	if (!isdigit(string[i])) return false;

	while (i < length && !isspace(string[i])) {
		if (!isdigit(string[i])) return false;
		integer_buffer[integer_chars] = string[i];

		integer_chars++;

		i++;
	}

	integer_buffer[integer_chars] = '\0';

	while (isspace(string[i])) i++;

	if (string[i] != '\0') return false;

	*integer = atoi(integer_buffer);
	return true; // Ensure the function returns a value
}

// Permission Based-Access Control
void permission_based_access_control(sqlite3* db){
	char pass;
	int i = 0;
	char password[50];

	while (1) {
		printf("Enter ADMIN Password to Continue!!\n");
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

		// Check if the password is correct
		if (strcmp(password, "Admin123") == 0) {
			printf("Permission Granted\n");
			admin_menu(db);
			break; // Exit loop if permission is granted
		}
		else {
			printf("Incorrect Password...\n");
			main_menu(db);
		}
	}
}

// Admin Menu
void admin_menu(sqlite3* db) {
	int choice;

	while (true) {
		printf("Permission Based-Access Control\n");
		printf("\n1. Reset Inventory\n");
		printf("2. Create New Account for Employee\n");
		printf("3. Remove Employee Account\n");
		printf("4. View Employee Accounts\n");
		printf("5. Back to Main Menu\n");
		printf("6. Exit Program\n");
		printf("Enter Choice: ");
		int res = scanf("%d", &choice);
		if (res == 0) {
			system("cls");
			printf("Invalid Input\n\n");
			empty_stdin();
			continue;
		}
		break;
	}
	switch (choice) {
	case 1: {// Reset Inventory
		system("cls");
		printf("Resetting Inventory");
		Sleep(3000);
		system("cls");
		reset_inventory(db);
		admin_menu(db);
		break;
	}
	case 2: {// Create New Account For Employee
		char* name[100], password[100];

		system("cls");
		printf("Creating New User\n");
		printf("Enter New User Credentials:\n");
		printf("Enter Full Name FN LN: ");
		while (getchar() != '\n');
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = 0;
		int idNumber = get_Number("Enter New User Id Number: ");
		printf("Create New Password: ");
		while (getchar() != '\n');
		fgets(password, sizeof(password), stdin);
		password[strcspn(password, "\n")] = 0;
		create_new_user(db, name, idNumber, password);
		system("cls");
		admin_menu(db);
	}
	case 3:
	{ // Remove Employee Account
		system("cls");
		view_users(db);
		printf("Remove Employee Account\n");
		int idNumber = get_Number("Enter Id Number: ");
		removeaccount(db, idNumber);
	}
	case 4: {
		system("cls");
		view_users(db);
	}
	case 5:
	{ // Back to Main Menu
		system("cls");
		main_menu(db);
	}
	case 6:
	{
		system("cls");
		printf("Exiting Program...");
		Sleep(2000);
		system("cls");
		exit(0);
		break;
	}
	default:
	{
		printf("Invalid Input\n");
	}
	}
}

// Empty stdin
static void empty_stdin(void) {
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

// CONTINUE FROM HERE
int get_Number(char* to_print) {
	int number;
	while (true) {
		printf(to_print);
		int res = scanf("%d", &number);
		if (res == 0) {
			printf("Invalid Input\n");
			empty_stdin();
			continue;
		}

		break;
	}
	return number;
}

// Reset Inventory
int reset_inventory(sqlite3* db) {
	char* errmsg;
	const char* sql = "DELETE FROM Inventory;";
	int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);

	if (rc != SQLITE_OK) {
		printf("Cannot Prepare Statement: %s", errmsg);
		return;
	}
	printf("Inventory Successfully Resetted...\n");
}

// Create New User
void create_new_user(sqlite3* db, char* name, int idNumber, char* password) {
	char* user[200];
	char* errmsg;

	const char* sql = "INSERT INTO LoginCredentials(Name, IdNumber, Password) VALUES ('%s', %d, '%s')";
	sprintf(user, sql, name, idNumber, password);
	int rc = sqlite3_exec(db, user, NULL, NULL, &errmsg);

	if (rc != SQLITE_OK) {
		printf("Cannot Execute SQL: %s\n", errmsg);
		return 1;
	}

	system("cls");
	printf("Creating New User...\n");
	Sleep(2000);
	system("cls");
	printf("New User Created: \n");
	printf("Name: %s", name);
	printf("Idnumber: %d\n", idNumber);
	admin_menu(db);
}

// Remove Account
void removeaccount(sqlite3* db, int idNumber) {
	char Remove[200];
	char* errmsg;
	const char* sql = "DELETE FROM LoginCredentials WHERE IdNumber = %d";
	sprintf(Remove, sql, idNumber);

	int rc = sqlite3_exec(db, Remove, NULL, NULL, &errmsg);

	if (rc != SQLITE_OK) {
		printf("Cannot Execute SQL: %s\n", errmsg);
		return 1;
	}

	system("cls");
	printf("Account Removed: %d\n", idNumber);
	admin_menu(db);
}

// View Users
void view_users(sqlite3* db) {
	sqlite3_stmt* stmt;
	ft_table_t* table = ft_create_table();
	ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
	ft_set_border_style(table, FT_BASIC_STYLE); 

	ft_write_ln(table, "Name", "IdNumber", "Password");  

	const char* sql = "SELECT Name, IdNumber, Password FROM LoginCredentials;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		printf("Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return;  
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		const char* name = sqlite3_column_text(stmt, 0);      
		int idNumber = sqlite3_column_int(stmt, 1);          
		const char* password = sqlite3_column_text(stmt, 2); 

		char id_str[5];
		sprintf(id_str, "%d", idNumber);  

		ft_write_ln(table, name, id_str, password);
	}
	printf("Employee Accounts\n");
	printf("%s", ft_to_string(table));

	ft_destroy_table(table);
	sqlite3_finalize(stmt);

	admin_menu(db);
	return;
}
