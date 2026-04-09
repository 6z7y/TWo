#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>


#include "GAME_DATA.h"
#include "Episodes/include_helper.h"

#define NAME_FILE "/tmp/TWo_save.db"

static sqlite3 *sql;

int my_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data; (void)argc; (void)col_name;

    int val = atoi(argv[0]);  // ← val now, not id
    if      (val == 1) game_ctx.ep = EP1;
    else if (val == 2) game_ctx.ep = EP2;
    else if (val == 3) game_ctx.ep = EP3;
    else               game_ctx.ep = EP4;
    return 0;
}

void save_sql_file(void)
{
    sqlite3_open(NAME_FILE, &sql);

    // add val column
    sqlite3_exec(sql,
        "CREATE TABLE IF NOT EXISTS ep "
        "(id INTEGER PRIMARY KEY, val INTEGER);",
        NULL, NULL, NULL);

    // default = EP1
    sqlite3_exec(sql,
        "INSERT OR IGNORE INTO ep (id, val) VALUES (1, 1);",
        NULL, NULL, NULL);

    sqlite3_close(sql);
}

int check_file_save()
{
  FILE *f = fopen(NAME_FILE, "r");

  if (f) {fclose(f); return 0;}

  return -1; // not found
}

void save_episode(int num)
{
  sqlite3_open(NAME_FILE, &sql);

  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(sql,
      "UPDATE ep SET val = ? WHERE id = 1;",
      -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, num);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  sqlite3_close(sql);
}

void load_current_episode(Define_Episode *ep)
{
    if (check_file_save() < 0) save_sql_file();

    sqlite3_open(NAME_FILE, &sql);
    sqlite3_exec(sql, "SELECT val FROM ep WHERE id = 1;", my_callback, NULL, NULL);
    sqlite3_close(sql);

    *ep = game_ctx.ep;

    // now the checks work
    if (*ep == EP1)  { 
      load_episode1(&game_ctx.map);
    }
    if (*ep == EP2) {
      load_episode2(&game_ctx.map);
    }

    if (*ep == EP3) {
      load_episode2(&game_ctx.map);
    }

    if (*ep == EP4) {
      load_episode2(&game_ctx.map);
    }
}
