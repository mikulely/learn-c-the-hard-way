// file name : db.c
// description : ab part
// created at: Fri 13 Dec 2013 02:46:16 PM CST

#include <unistd.h>
#include <apr-1/apr_errno.h>
#include <apr-1/apr_file_io.h>

#include "db.h"
#include "lib/bstrlib.h"
#include "dbg.h"

static FILE *DB_open(const char *path, const char *mode){
        return fopen(path, mode);
}

static void DB_close(FILE *db){
        fclose(db);
}

static bstring DB_load(){
        FILE *db = NULL;
        // bstring 是个结构体指针
        bstring data = NULL;

        db = DB_open(DB_FILE, "r");
        check(db, "Failed to open database: %s", DB_FILE);

        data = bread((bNread)fread, db);
        check(data, "Failed to read from db file: %s", DB_FILE);

        DB_close(db);
        return data;

error:
        if(db) fclose(db);
        if(data) bdestroy(data);
        return NULL;
}

int DB_update(const char *url){
        if(DB_find(url)){
                log_info("Already recorded as installed: %s", url);
        }

        FILE *db = DB_open(DB_FILE, "a+");
        check(db, "Failed to open database: %s", DB_FILE);

        // bfromstr() create a bstring which contains the contents of the '\0'
        // terminated char * buffer str.
        bstring line = bfromcstr(url);
        // bconchar() concatenate the single character c to the bstring b.
        bconchar(line, '\n');
        int rc = fwrite(line->data, blength(line), 1, db);
              // fwrite(buffer,size,count,fp)
        check(rc == 1, "Failed to append to the db.");

        return 0;
error:
        if(db) DB_close(db);
        return -1;
}

int DB_find(const char *url){
        bstring data = NULL;
        bstring line = bfromcstr(url);
        int res = -1;

        data = DB_load();
        check(data, "Failed to load: %s", DB_FILE);
        // binstr(s1, 起始位置, s2) 在 s1 中查找s2,找到的话返回位置，未找到的话返回 BSTR_ERR
        if(binstr(data, 0, line) == BSTR_ERR){
                res = 0;
        } else {
                res = 1;
        }
error:
        if(data) bdestroy(data);
        if(line) bdestroy(line);

        return res;
}


int DB_init(){
        // apr_pool_t 内存池
        apr_pool_t *p = NULL;
        // 初始化内存池，直到遇到 apr_destroy() 后释放。
        apr_pool_initialize();
        apr_pool_create(&p, NULL);

        if(access(DB_DIR, W_OK|X_OK) == -1){
                apr_status_t rc = apr_dir_make_recursive(DB_DIR,
                                                     APR_UREAD|APR_UWRITE|APR_UEXECUTE|
                                                     APR_GREAD|APR_GWRITE|APR_GEXECUTE, p);
                check(rc == APR_SUCCESS, "Failed to make database dir: %s", DB_DIR);
        }

        if(access(DB_FILE, W_OK) == -1){
                FILE *db = DB_open(DB_FILE, "w");
                check(db, "Can not open database %s", DB_FILE);
                DB_close(db);
        }

        apr_pool_destroy(p);
        return 0;

error:
        apr_pool_destroy(p);
        return -1;
}

int DB_list(){
        bstring data = DB_load();
        check(data, "Failed to read load: %s", DB_FILE);

        printf("%s",bdata(data));
        bdestroy(data);
        return 0;

error:
        return -1;
}
