#define MAX_COMMAND_LEN 1000
#define NULL_CHAR '\0'

#include <map>

enum Command {
  LS,
  TOUCH,
  NOT_KNOWN
};

#if !defined(COMMAND_MAP)
#define COMMAND_MAP 1

const std::map<Command,const char*> commands = {
	{LS, "ls"},
	{TOUCH, "touch"},
};

#endif


bool attempt_parse_command(char* input, const char* command_to_parse);
Command parse_command(char* input);
void handle_input();