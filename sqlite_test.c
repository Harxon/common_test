#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int main(int argc, const char *argv[])
{
	sqlite3* mydb;
	if(SQLITE_OK != sqlite3_open("my.db", &mydb)){
		perror("sqlite3_open failed");
		return -1;
	}
	sqlite3_close(mydb);
	return 0;
}
