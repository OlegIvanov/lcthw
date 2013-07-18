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

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
	struct SearchParameter *sp;
};

struct SearchParameter {
	char field[10];
	char value[1000];
};

void Database_close(struct Connection*);

void die(const char *message, struct Connection *conn)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	Database_close(conn);

	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n",
			addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	struct Database *db = conn->db;
	
	int rc = fread(&db->max_rows, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to load max_rows.", conn);

	rc = fread(&db->max_data, sizeof(int), 1, conn->file);
	if (rc != 1) die("Failed to load max_data.", conn);

	db->rows = malloc(db->max_rows * sizeof(struct Address));
	
	int i = 0;
	struct Address *cur_row = NULL;

	for(i = 0; i < db->max_rows; i++) {
		cur_row = db->rows + i;

		rc = fread(&cur_row->id, sizeof(int), 1, conn->file);
		if (rc != 1) die("Failed to load id.", conn);

		rc = fread(&cur_row->set, sizeof(int), 1, conn->file);
		if (rc != 1) die("Failed to load set.", conn);

		cur_row->name = malloc(db->max_data);
		rc = fread(cur_row->name, db->max_data, 1, conn->file);
		if (rc != 1) die("Failed to load name.", conn);

		cur_row->email = malloc(db->max_data);
		rc = fread(cur_row->email, db->max_data, 1, conn->file);
		if (rc != 1) die("Failed to load email.", conn);
	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error", conn);
	
	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Memory error", conn);

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
	} else { 
		conn->file = fopen(filename, "r+");

		Database_load(conn);
	}
	
	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) {
			fclose(conn->file);
		}
		if(conn->sp) {
			free(conn->sp);
		}
		struct Database *db = conn->db;
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
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	struct Database *db = conn->db;

	rewind(conn->file);

	int rc = fwrite(&db->max_rows, sizeof(int), 1, conn->file);
	if(rc != 1) die("Failed to write max_rows.", conn);

	rc = fwrite(&db->max_data, sizeof(int), 1, conn->file);
	if(rc != 1) die("Failed to write max_data.", conn);

	int i = 0;
	struct Address *cur_row = NULL;

	for(i = 0; i < db->max_rows; i++) {
		cur_row = db->rows + i;

		rc = fwrite(&cur_row->id, sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to write id.", conn);

		rc = fwrite(&cur_row->set, sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to write set.", conn);

		rc = fwrite(cur_row->name, db->max_data, 1, conn->file);
		if(rc != 1) die("Failed to write name.", conn);

		rc = fwrite(cur_row->email, db->max_data, 1, conn->file);
		if(rc != 1) die("Failed to write email.", conn);
	}

	rc = fflush(conn->file);
	if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
	struct Database *db = conn->db;

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
			.set = 0, 
			.name = cur_row->name,
			.email = cur_row->email
		};
		// then just assign it
		*cur_row = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first", conn);

	int max_data = conn->db->max_data;

	addr->set = 1;
	// Warning: bug, read the "How To Break It" and fix this
	char *res = strncpy(addr->name, name, max_data);
	// Bug has been fixed.
	*(addr->name + max_data - 1) = '\0';

	// demonstrate the strncpy bug
	if(!res) die("Name copy failed", conn);

	res = strncpy(addr->email, email, max_data);
	// Bug has been fixed.
	*(addr->email + max_data - 1) = '\0';

	if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address *rows = conn->db->rows;

	(rows + id)->set = 0;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;
	
	for(i = 0; i < db->max_rows; i++) {
		struct Address *cur = &db->rows[i];

		if(cur->set) {
			Address_print(cur);
		}
	}
}

int Get_search_parameters(int argc, char **argv, struct Connection *conn)
{
	int sp_count = argc - 3;
	conn->sp = malloc(sp_count * sizeof(struct SearchParameter));
	if(!conn->sp) die("Memory error", conn);
	
	struct SearchParameter *sp = conn->sp;
	int i = 0;
	char *equality_sign = NULL;
	int field_len = 0;
	struct SearchParameter *cur_sp = NULL;	

	for(i = 0; i < sp_count; i++) {
		cur_sp = sp + i;
		equality_sign = strrchr(argv[i + 3], '=');
		field_len = equality_sign - &argv[i + 3][0];

		strncpy(cur_sp->field, argv[i + 3], field_len);
		cur_sp->field[field_len] = '\0';

		strcpy(cur_sp->value, equality_sign + 1);
	}

	return sp_count;
}

void Database_find(struct Connection *conn, int sp_count)
{
	int i = 0;
	struct Database *db = conn->db;
	struct Address *rows = db->rows;
	struct Address *cur_row = NULL;
	struct SearchParameter *sp = conn->sp;
	int j = 0;
	int row_found = 0;
	struct SearchParameter *cur_sp = NULL;

	for(i = 0; i < db->max_rows; i++) {
		cur_row = rows + i;

		if(cur_row->set == 0) continue;
		
		int row_found = 0;
		
		for(j = 0; j < sp_count; j++) {
			cur_sp = sp + j;
				
			if(strcmp(cur_sp->field, "id")) {
				
			}
		}

		Address_print(cur_row);
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	struct Database *db = conn->db;
	int id = 0;
	int sp_count = 0;

	switch(action) {
		case 'c':
			db->max_rows = atoi(argv[3]);
			db->max_data = atoi(argv[4]);

			if(db->max_rows < 1) die("There's too few rows.", conn);
			if(db->max_data < 2) die("There's not enough place to store name or email.", conn);
			break;
		case 'g':
		case 's':
		case 'd':
			id = atoi(argv[3]);

			if(id >= db->max_rows) die("There's not that many records.", conn);
			if(id < 0) die("Negative indexes are not acceptable.", conn);
			break;
		case 'f':
			sp_count = Get_search_parameters(argc, argv, conn);
			break;
	}

	switch(action) {
		case 'c':
			if (argc != 5) die("Need max_rows, max_data to make database proxy", conn);
			
			Database_create(conn);
			Database_write(conn);
			break;

		case 'g':
			if(argc != 4) die("Need an id to get", conn);
			
			Database_get(conn, id);
			break;

		case 's':
			if(argc != 6) die("Need id, name, email to set", conn);
			
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;

		case 'd':
			if(argc != 4) die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
			break;
		
		case 'l':
			Database_list(conn);
			break;

		case 'f':
			Database_find(conn, sp_count);
			break;

		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}
	
	Database_close(conn);

	return 0;
}
