#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite\sqlite3.h"
#include "functions.h"

bool create_table(sqlite3* db) {
	const char* sql = "CREATE TABLE IF NOT EXISTS Inventory (\
		Id INTEGER PRIMARY KEY AUTOINCREMENT, \
		Product TEXT, \
		Stock INTEGER, \
		Price REAL, \
		Sold INTEGER, \
		Total REAL);";

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
		printf("4. Close/Exit\n");
		printf("Enter Choice: ");
		if (scanf("%d", &choice) != 1) {
			fprintf(stderr, "\nInvalid Input\n");
			while (getchar() != '\n') {}
			continue;
		}

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
			printf("Enter Product Id: ");
			(void)scanf("%d", &id);
			printf("Enter Items to be Sold: ");
			(void)scanf("%d", &sold);
			process_sale(db, id, sold);
			break;
		}
		case 4: {
			printf("\nExiting Program!!!\n");
			return;
		}
		default:
			printf("Invalid Choice");
			break;
		}
	}
}

// Update Inventory
void update_inventory(sqlite3* db){
	int choice, quantity, id;
	char name[100];
	double price;

	while (true) {
		printf("\nUpdate Inventory\n");
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

		switch (choice) {
		case 1: // Working
		{ // New Product
			printf("Enter New Product Name: ");
			while (getchar() != '\n');
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = 0;
			printf("Enter Quantity: ");
			(void)scanf("%d", &quantity);
			printf("Enter Product Selling Price: ");
			(void)scanf("%lf", &price);
			add_product(db, name, quantity, price);
			break;
		}
		case 2:// Working
		{ // Update Stock
			view_inventory(db);
			printf("Enter Product Id: ");
			(void)scanf("%d", &id);
			printf("Enter Stock to be Added: ");
			(void)scanf("%d", &quantity);
			add_stock(db, id, quantity);
			break;
		}
		case 3:
		{ // Update Product Name
			printf("\nUpdate Product Name\n");
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
			printf("\nDelete Product\n");
			printf("Enter Product Id: ");
			(void)scanf("%d", &id);
			delete_product(db, id);
			break;
		}
		case 6:
		{
			printf("\n");
			return;
		}
		case 7:
		{
			printf("\nExiting Program!!!\n");
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
		printf("\nView Inventory/Sales\n");
		printf("1. View Inventory\n");
		printf("2. View Sales Report\n");
		printf("3. Back to Main Menu\n");
		printf("4. Close/Exit\n");
		printf("Enter Choice: ");
		if (scanf("%d", &choice) != 1) {
			fprintf(stderr, "Invalid Choice");
			while (getchar() != '\n'){}
			continue;
		}

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
//view_sales();
			break;
		}
		case 3:
		{
			printf("\n");
			return;
		}
		case 4:
		{
			printf("\nExiting Program!!!\n");
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
int add_product(sqlite3* db, const char* name, int quantity, double price) {
	sqlite3_stmt* stmt;
	const char* sql = "INSERT INTO Inventory(Product, Stock, Price, Sold, Total) VALUES (?, ?, ?, 0, 0);";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, quantity);
	sqlite3_bind_double(stmt, 3, price);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Failed to insert Product: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}

	printf("Product added successfully\n");
	sqlite3_finalize(stmt);
	return 0;
}


// Add Stock
// Continue from here
int add_stock(sqlite3* db, int id, int quantity) {
	sqlite3_stmt* stmt;
	const char* sql = "UPDATE Inventory SET Stock = Stock + ? WHERE Id = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_int(stmt, 1, quantity);
	sqlite3_bind_int(stmt, 2, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		printf("Failed to update Stock: %s\n", sqlite3_errmsg(db));
		fprintf(stderr, "Failed to update Stock: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}

	printf("Stock updated successfully\n");
	sqlite3_finalize(stmt);
	return 0;
}

// Update Name
int update_name(sqlite3* db, int id, const char* name) {
	sqlite3_stmt* stmt;
	const char* sql = "UPDATE Inventory SET Product = ? WHERE Id = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Failed to update Product Name: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}

	printf("Product Name updated successfully\n");
	sqlite3_finalize(stmt);
	return 0;
}

// Update Price
int update_price(sqlite3* db, int id, double price) {
	sqlite3_stmt* stmt;
	const char* sql = "UPDATE Inventory SET Price = ? WHERE Id = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_double(stmt, 1, price);
	sqlite3_bind_int(stmt, 2, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Failed to update Product Price: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}

	printf("\nProduct Price updated successfully\n");
	sqlite3_finalize(stmt);
	return 0;
}

int delete_product(sqlite3* db, int id) {
	sqlite3_stmt* stmt;
	const char* sql = "DELETE FROM Inventory WHERE Id = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot Prepare Statement: %s\n", sqlite3_errmsg(db));
		return 1;
	}

	sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		fprintf(stderr, "Failed to delete Product: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}

	printf("Product deleted successfully\n");
	sqlite3_finalize(stmt);
	return 0;
}

// VIEW INVENTORY
int view_inventory(sqlite3* db) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT Id, Product, Stock, Price FROM Inventory;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		printf("Cannot Prepare Statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}
	printf("%-22s%s", " ", "Inventory\n");
	printf("------------------------------------------------------------\n");
	printf("%-5s%-6s%-7s%-14s%-6s%-10s%-3s%-8s%s\n", "|", "ID", "|","PRODUCT","|", "STOCK", "|", "PRICE", "|");
	printf("------------------------------------------------------------\n");

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const char* product = sqlite3_column_text(stmt, 1);
		int stock = sqlite3_column_int(stmt, 2);
		double price = sqlite3_column_double(stmt, 3);
		printf("%s%-10d%s%-20s%s%-15d%s%-10.2lf%s\n", "|", id, "|", product, "|", stock, "|", price, "|");
	}
	
	printf("------------------------------------------------------------\n");

	sqlite3_finalize(stmt);
	return 0;
}

// VIEW SALES
int view_sales(sqlite3* db) {
	sqlite3_stmt* stmt;
	const char* sql = "SELECT Id, Product, Sold, Total FROM Inventory;";
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
		printf("%-10d%-20s%-10d%-15.2lf\n", id, product, sold, total);
	}
	sqlite3_finalize(stmt);
	return 0;
}

// PROCCESS SALES
int process_sale(sqlite3* db, int id, int sold) {
	sqlite3_stmt* stmt;
	const char* sql = "UPDATE Inventory SET Sold = Sold + ?, Total = Total + ? * Price, Stock = Stock - ? WHERE Id = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		printf("Cannot Prepare Statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}

	sqlite3_bind_int(stmt, 4, id);
	sqlite3_bind_int(stmt, 3, sold);
	sqlite3_bind_int(stmt, 2, sold);
	sqlite3_bind_int(stmt, 1, sold);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		printf("Failed to Proccess Sale: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 1;
	}

	printf("Sale Proccessed!!!\n");
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