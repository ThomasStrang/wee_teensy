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
void handle_unknown_command(char* args);

#if !defined(COMMAND_MAP)
#define COMMAND_MAP 1
const std::map<const char*,handler_function> commands = {
  {"lsc",&handle_lsc},
  {"ls",&handle_ls},
  {"rm",&handle_rm},
  {"rmdir", &handle_rmdir},
  {"touch",&handle_touch},
  {"mkdir",&handle_mkdir},
  {"cat",&handle_cat},
  {"set_clock",&handle_set_clock},
  {"view_clock",&handle_view_clock}
};
#endif

void handle_input();
