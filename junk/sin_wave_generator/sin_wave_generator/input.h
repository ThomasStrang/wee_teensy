#include <map>

#define MAX_COMMAND_LEN 1000


typedef void (*handler_function)(char*);
void handle_lsc(char* args);
void handle_ls(char* args);
void handle_touch(char* args);
void handle_mkdir(char* args);
void handle_rm(char* args);
void handle_rmdir(char* args);
void handle_cat(char* args);
void handle_set_clock(char* args);
void handle_view_clock(char* args);
void handle_current_log(char* args);
void handle_make_log(char* args);
void handle_write_to_log(char* args);
void handle_save_log(char* args);
void handle_close_log(char* args);
void handle_log_benchmark(char* args);
void handle_unknown_command(char* args);

#if !defined(COMMAND_MAP)
#define COMMAND_MAP 1
const std::map<const char*,handler_function> commands = {
  {"lsc", &handle_lsc},
  {"ls", &handle_ls},
  {"rm", &handle_rm},
  {"rmdir", &handle_rmdir},
  {"touch", &handle_touch},
  {"mkdir", &handle_mkdir},
  {"cat", &handle_cat},
  {"time_set", &handle_set_clock},
  {"time_view", &handle_view_clock},
  {"log_make", &handle_make_log},
  {"log_view", &handle_current_log},
  {"log_write", &handle_write_to_log},
  {"log_save", &handle_save_log},
  {"log_close", &handle_close_log},
  {"log_benchmark", &handle_log_benchmark}
};
#endif

void handle_input();
