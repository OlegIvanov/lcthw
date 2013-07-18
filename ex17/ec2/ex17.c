// There is a bug in this program because of strncpy being poorly designed. 
// Go read about strncpy then try to find out what happens 
// when the name or address you give is greater than 512 bytes. 
// Fix this by simply forcing the last character to '\0' so that 
// it's always set no matter what (which is what strncpy should do).

// The die function needs to be augmented to let you pass the conn variable so it can close it and clean up.

// Change the code to accept parameters for MAX_DATA and MAX_ROWS,
// store them in the Database struct, and write that to the file, 
// thus creating a database that can be arbitrarily sized.

// Add more operations you can do on the database, like find.

// Add some more fields to the Address and make them searchable.

// Try reworking the program to use a single global for the database connection.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	int age;
	char *name;
	char *email;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address *rows;
};

struct SearchParameter {
	char field[10];
	char value[1000];
};

struct Connection {
	FILE *file;
	struct Database *db;
	struct SearchParameter *sp;
};

struct Connection conn;

void Database_close();

void die(const char *message)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	Database_close();

	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("id: %5d\tname: %20s\tage: %5d\temail: %20s\n",
			addr->id,
			addr->name,
			addr->age,
			addr->email);
}

void Database_load()
{
	struct Database *db = conn.db;
	
	int rc = fread(&db->max_rows, sizeof(int), 1, conn.file);
	if (rc != 1) die("Failed to load max_rows.");

	rc = fread(&db->max_data, sizeof(int), 1, conn.file);
	if (rc != 1) die("Failed to load max_data.");

	db->rows = malloc(db->max_rows * sizeof(struct Address));
	
	int i = 0;
	struct Address *cur_row = NULL;

	for(i = 0; i < db->max_rows; i++) {
		cur_row = db->rows + i;

		rc = fread(&cur_row->id, sizeof(int), 1, conn.file);
		if (rc != 1) die("Failed to load id.");

		rc = fread(&cur_row->set, sizeof(int), 1, conn.file);
		if (rc != 1) die("Failed to load set.");

		rc = fread(&cur_row->age, sizeof(int), 1, conn.file);
		if (rc != 1) die("Failed to load age.");

		cur_row->name = malloc(db->max_data);
		rc = fread(cur_row->name, db->max_data, 1, conn.file);
		if (rc != 1) die("Failed to load name.");

		cur_row->email = malloc(db->max_data);
		rc = fread(cur_row->email, db->max_data, 1, conn.file);
		if (rc != 1) die("Failed to load email.");
	}
}

void Database_open(const char *filename, char mode)
{
	conn.db = malloc(sizeof(struct Database));
	if(!conn.db) die("Memory error");

	if(mode == 'c') {
		conn.file = fopen(filename, "w");
	} else { 
		conn.file = fopen(filename, "r+");

		Database_load(conn);
	}
	
	if(!conn.file) die("Failed to open the file");
}

void Database_close()
{
	if(conn.file) {
		fclose(conn.file);
	}
	if(conn.sp) {
		free(conn.sp);
	}
	struct Database *db = conn.db;
	if(db) {
		struct Address *rows = db->rows;
		if(rows) {
			int i = 0;
			for(i = 0; i < db->max_rows; i++) {
				if((rows + i)->name) {
					free((rows + i)->name);
				}
				if((rows + i)->email) {
					free((rows + i)->email);
				}
			}
			free(rows);
		}
		free(db);
	}
}

void Database_write()
{
	struct Database *db = conn.db;

	rewind(conn.file);

	int rc = fwrite(&db->max_rows, sizeof(int), 1, conn.file);
	if(rc != 1) die("Failed to write max_rows.");

	rc = fwrite(&db->max_data, sizeof(int), 1, conn.file);
	if(rc != 1) die("Failed to write max_data.");

	int i = 0;
	struct Address *cur_row = NULL;

	for(i = 0; i < db->max_rows; i++) {
		cur_row = db->rows + i;

		rc = fwrite(&cur_row->id, sizeof(int), 1, conn.file);
		if(rc != 1) die("Failed to write id.");

		rc = fwrite(&cur_row->set, sizeof(int), 1, conn.file);
		if(rc != 1) die("Failed to write set.");

		rc = fwrite(&cur_row->age, sizeof(int), 1, conn.file);
		if(rc != 1) die("Failed to write age.");

		rc = fwrite(cur_row->name, db->max_data, 1, conn.file);
		if(rc != 1) die("Failed to write name.");

		rc = fwrite(cur_row->email, db->max_data, 1, conn.file);
		if(rc != 1) die("Failed to write email.");
	}

	rc = fflush(conn.file);
	if(rc == -1) die("Cannot flush database.");
}

void Database_create()
{
	struct Database *db = conn.db;

	db->rows = malloc(db->max_rows * sizeof(struct Address));
	
	int i = 0;
	struct Address *cur_row = NULL;	

	for(i = 0; i < db->max_rows; i++) {
		cur_row = db->rows + i;
		
		cur_row->name = malloc(db->max_data);
		memset(cur_row->name, '\0', db->max_data);

		cur_row->email = malloc(db->max_data);
		memset(cur_row->email, '\0', db->max_data);
		
		// make a prototype to initialize it
		struct Address addr = {
			.id = i, 
			.age = 0,
			.set = 0, 
			.name = cur_row->name,
			.email = cur_row->email
		};
		// then just assign it
		*cur_row = addr;
	}
}

void Database_set(int id, int age, const char *name, const char *email)
{
	struct Address *addr = &conn.db->rows[id];
	if(addr->set) die("Already set, delete it first");

	int max_data = conn.db->max_data;

	addr->set = 1;
	addr->age = age;

	// Warning: bug, read the "How To Break It" and fix this
	char *res = strncpy(addr->name, name, max_data);
	// Bug has been fixed.
	*(addr->name + max_data - 1) = '\0';

	// demonstrate the strncpy bug
	if(!res) die("Name copy failed");

	res = strncpy(addr->email, email, max_data);
	// Bug has been fixed.
	*(addr->email + max_data - 1) = '\0';

	if(!res) die("Email copy failed");
}

void Database_get(int id)
{
	struct Address *addr = &conn.db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set");
	}
}

void Database_delete(int id)
{
	struct Address *rows = conn.db->rows;

	(rows + id)->set = 0;
}

void Database_list()
{
	int i = 0;
	struct Database *db = conn.db;
	
	for(i = 0; i < db->max_rows; i++) {
		struct Address *cur = &db->rows[i];

		if(cur->set) {
			Address_print(cur);
		}
	}
}

int Get_search_parameters(int argc, char **argv)
{
	int sp_count = argc - 3;
	conn.sp = malloc(sp_count * sizeof(struct SearchParameter));
	if(!conn.sp) die("Memory error");
	
	struct SearchParameter *sp = conn.sp;
	int i = 0;
	char *equality_sign = NULL;
	int field_len = 0;
	struct SearchParameter *cur_sp = NULL;

	char die_message[30];

	for(i = 0; i < sp_count; i++) {
		cur_sp = sp + i;
		equality_sign = strrchr(argv[i + 3], '=');
		field_len = equality_sign - &argv[i + 3][0];

		strncpy(cur_sp->field, argv[i + 3], field_len);
		cur_sp->field[field_len] = '\0';

		strcpy(cur_sp->value, equality_sign + 1);

		if(!(strcmp(cur_sp->field, "id") == 0) &&
			!(strcmp(cur_sp->field, "age") == 0) &&
		  	!(strcmp(cur_sp->field, "name") == 0) &&
		  	!(strcmp(cur_sp->field, "email") == 0)) {
			
			sprintf(die_message, "There's no such field in database as \"%s\".\n", cur_sp->field);
			die(die_message);
		}
	}

	return sp_count;
}

void Database_find(int sp_count)
{
	int i = 0;
	int j = 0;
	struct Database *db = conn.db;
	
	struct Address *rows = db->rows;
	struct Address *cur_row = NULL;
	
	struct SearchParameter *sp = conn.sp;
	struct SearchParameter *cur_sp = NULL;

	int row_found = 0;

	for(i = 0; i < db->max_rows; i++) {
		cur_row = rows + i;

		if(cur_row->set == 0) continue;
		
		row_found = 1;
		
		for(j = 0; j < sp_count; j++) {
			cur_sp = sp + j;
	
			if(strcmp(cur_sp->field, "id") == 0) {
				int id = atoi(cur_sp->value);
			
				if(!(cur_row->id == id)) row_found = 0;
			}

			if(strcmp(cur_sp->field, "age") == 0) {
				int age = atoi(cur_sp->value);
			
				if(!(cur_row->age == age)) row_found = 0;
			}
			
			if(strcmp(cur_sp->field, "name") == 0) {
				if(!(strcmp(cur_row->name, cur_sp->value) == 0)) row_found = 0;
			}

			if(strcmp(cur_sp->field, "email") == 0) {
				if(!(strcmp(cur_row->email, cur_sp->value) == 0)) row_found = 0;
			}
		}
		
		if (row_found) Address_print(cur_row);
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];
	Database_open(filename, action);
	struct Database *db = conn.db;
	int id = 0;
	int age = 0;
	int sp_count = 0;

	switch(action) {
		case 'c':
			db->max_rows = atoi(argv[3]);
			db->max_data = atoi(argv[4]);

			if(db->max_rows < 1) die("There's too few rows.");
			if(db->max_data < 2) die("There's not enough place to store name or email.");
			break;

		case 's':
			age = atoi(argv[4]);
			if(age < 0) die("Negative age isn't acceptable.");
		case 'g':
		case 'd':
			id = atoi(argv[3]);

			if(id >= db->max_rows) die("There's not that many records.");
			if(id < 0) die("Negative indexes are not acceptable.");
			break;

		case 'f':
			sp_count = Get_search_parameters(argc, argv);
			break;
	}

	switch(action) {
		case 'c':
			if (argc != 5) die("Need max_rows, max_data to make database proxy");
			
			Database_create();
			Database_write();
			break;

		case 'g':
			if(argc != 4) die("Need an id to get");
			
			Database_get(id);
			break;

		case 's':
			if(argc != 7) die("Need id, age, name, email to set");
			
			Database_set(id, age, argv[5], argv[6]);
			Database_write();
			break;

		case 'd':
			if(argc != 4) die("Need id to delete");

			Database_delete(id);
			Database_write();
			break;
		
		case 'l':
			Database_list();
			break;

		case 'f':
			Database_find(sp_count);
			break;

		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find");
	}
	
	Database_close();

	return 0;
}
