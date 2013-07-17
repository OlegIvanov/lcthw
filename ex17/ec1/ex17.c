// There is a bug in this program because of strncpy being poorly designed. 
// Go read about strncpy then try to find out what happens 
// when the name or address you give is greater than 512 bytes. 
// Fix this by simply forcing the last character to '\0' so that 
// it's always set no matter what (which is what strncpy should do).

// The die function needs to be augmented to let you pass the conn variable so it can close it and clean up.

// Change the code to accept parameters for MAX_DATA and MAX_ROWS,
// store them in the Database struct, and write that to the file, 
// thus creating a database that can be arbitrarily sized.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	char* name;
	char* email;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
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
	}
	
	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
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

		rc = fread(&cur_row->set, sizeof(int), 1, conn->file);
		if(rc != 1) die("Failed to write set.", conn);

		rc = fread(cur_row->name, db->max_data, 1, conn->file);
		if(rc != 1) die("Failed to write name.", conn);

		rc = fread(cur_row->email, db->max_data, 1, conn->file);
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
		cur_row->email = malloc(db->max_data);
		
		// make a prototype to initialize it
		struct Address addr = {.id = i, .set = 0};
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
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;
	
	for(i = 0; i < conn->db->max_rows; i++) {
		struct Address *cur = &db->rows[i];

		if(cur->set) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;
	int max_rows, max_data = 0;

	switch(action) {
		case 'c':
			max_rows = atoi(argv[3]);
			max_data = atoi(argv[4]);
			break;
		case 'g':
		case 's':
		case 'd':
			id = atoi(argv[3]);
			if(id >= conn->db->max_rows) die("There's not that many records.", conn);
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
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}

	Database_close(conn);

	return 0;
}