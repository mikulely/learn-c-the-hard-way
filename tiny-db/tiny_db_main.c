// file name : tiny-db.c
// description: Based on ex17,remove strncpy, str copy by myself.
// created at: Thu 05 Dec 2013 03:47:08 PM CST

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define SUCCESS         0
#define FAILURE         1

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address{
        int id;
        int set;
        char name[MAX_DATA];
        char email[MAX_DATA];
};

struct Database{
        struct Address rows[MAX_ROWS];
};

struct Connection{
        FILE *file;
        struct Database *db;
};

void die(const char *message){
        // extern var errno to say what error happened, jsut number.
        if(errno){
                perror(message);
        } else {
                printf("ERROR: %s\n", message);
        }
        exit(FAILURE);
}

void Address_print(struct Address *addr){
        printf("%d, %s, %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection* conn){
        // 读取的元素个数= fread(接受数据的缓存，每个单元的大小，元素个数，文件指针)
        int rc= fread(conn->db, sizeof(struct Database), 1, conn->file);
        if (rc != 1) die("Failed to load Database");
}

// 在open 分配统一分配内存
struct Connection *Database_open(const char* filename, char mode){
        struct Connection *conn = malloc(sizeof(struct Connection));
        // 当然你可以显式的比较 conn == NULL ,NULL 在布尔上下文中表示 0
        if (!conn) die("Memory error.");

        conn->db = malloc(sizeof(struct Database));
        if(!conn->db) die("Memory error.");

        if (mode == 'c')
        {
                conn->file = fopen(filename, "w");
        } else {
                conn->file = fopen(filename, "r+");
                if (conn->file) Database_load(conn);
        }
        if (!conn->file) die("Failed to open the file.");

        return conn;
}

void Database_close(struct Connection *conn){
        if (conn)
        {
                if (conn->file) fclose(conn->file);
                if (conn->db) free(conn->db);
                free(conn);
        }
}

void Database_write(struct Connection *conn){
        rewind(conn->file); /* 使文件fp的位置指针指向文件开始 */

        int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
        if (rc != 1) die("Failed to write database");

        rc = fflush(conn->file);
        if(rc == -1) die("Can not flush databae");
}

void Database_create(struct Connection *conn){
        int i = 0;

        for (i = 0; i < MAX_ROWS; i++)
        {
                // make a prototype to initialize it.
                struct Address addr = {.id = i, .set = 0};
                // then just assign it
                conn->db->rows[i] = addr;
        }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email){
        struct Address *addr = &conn->db->rows[id];
        if(addr->set) die("Already set, delete it first.");

        addr->set = 1;
        // 不用 strncpy
        int i = 0 ;
        for (i = 0; i < MAX_DATA; i++)
        {
                if (*(name + i) != '\0')
                {
                        addr->name[i] = *(name + i);
                } else {
                        addr->name[i] = '\0';
                }
        }

        for (i = 0; i < MAX_DATA; i++)
        {
                if (*(email + i) != '\0')
                {
                        addr->email[i] = *(email + i);
                } else {
                        addr->email[i] = '\0';
                }
        }

        /* char * res = strncpy(addr->email, email, MAX_DATA); */
        /* if(!res) die("Email copy failed."); */
}

void Database_get(struct Connection *conn, int id){
        struct Address *addr = &conn->db->rows[id];

        if (addr->set)
        {
                Address_print(addr);
        }else{
                die("ID is not set.");
        }
}

void Database_delete(struct Connection *conn, int id){
        // 使用临时变量，初始化它的id 和 set
        struct Address addr = {.id = id, .set = 0};
        // 你不应该使用 memcpy，C容许你将一个结构体赋值给另一个结构体，它会处理拷贝的事
        conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn){
        int i = 0;
        struct Database *db = conn->db;

        for (i = 0; i < MAX_ROWS; i++)
        {
                struct Address *cur = &db->rows[i];
                if (cur->set)
                {
                        Address_print(cur);
                }
        }
}

int main(int argc, char *argv[]){
        if (argc < 3) die("USAGE: small_db <dbfile> <action> [action params]");

        char *filename = argv[1];
        char action = argv[2][0];
        struct Connection *conn = Database_open(filename, action);
        int id = 0;

        // atoi 进行类型转换
        if (argc > 3) id = atoi(argv[3]);
        if (id >= MAX_ROWS) die("There's not that many records.");

        switch (action) {
        case 'c':
                Database_create(conn);
                Database_write(conn);
                break;

        case 'g':
                if (argc != 4) die("Need an id to get.");

                Database_get(conn, id);
                break;
        case 's':
                if (argc != 6) die("Need id, name, email to set.");

                Database_set(conn, id, argv[4], argv[5]);
                Database_write(conn);
                break;

        case 'd':
                if(argc != 4) die("Need id to delete");

                Database_delete(conn, id);
                Database_write(conn);
                break;

        case 'l':
                Database_list(conn);
                break;
        default:
                die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
        }

        Database_close(conn);

        return 0;
}
