#include "input.h"
#include <Arduino.h>
#include <tuple>
#include "rtc.h"
#include "sd_card.h"
#include "log.h"

#define NULL_CHAR '\0'

bool whitespace_or_null(char c) {
	return c==NULL_CHAR || c == ' ' || c == '\n' || c == '\r';
}

char* attempt_parse_command(char* input, const char* command_to_parse) {
  //we assume that both are null terminated.
  for(int i = 0; i < MAX_COMMAND_LEN; i++) {
    if(command_to_parse[i] == NULL_CHAR) {
      //we have run out of command.
      if(whitespace_or_null(input[i])) {
        //and also input, therefore command == input.
        if(input[i] == NULL_CHAR) {
          return input+i;
        } else {
          return input+i+1;
        }
      } else {
        //but we still have more input.
        //e.g. command=="ls", input=="lsasd sdf"
        return NULL;
      }
    } else if(whitespace_or_null(input[i])) {
      //but we dont have more input.
      return NULL;
    } else if(input[i] != command_to_parse[i]) {
    	return NULL;
    }
  }
  return NULL;
}

std::tuple<handler_function, char*> parse_command(char* input) {
  handler_function handle_f = &handle_unknown_command;
  char* handler_args = NULL;
  for(auto c = commands.begin(); c != commands.end(); ++c) {
    handler_args = attempt_parse_command(input, c->first);
    if(handler_args != NULL) {
      handle_f = c->second;
      Serial.println(c->first + String("..."));
      break;
    }
  }
  return std::make_tuple(handle_f, handler_args);
}

void print_invalid_arg() {
  Serial.println("Invalid arg.");
}

bool has_arg(char* args) {
  return (args[0] != NULL_CHAR);
}

void handle_ls(char* args) {
  print_file_tree();
}

void handle_lsc(char* args) {
  for(auto c = commands.begin(); c != commands.end(); ++c) {
    Serial.print(String(c->first) + " ");
  }
  Serial.println();
}

uint32_t stoi(char* a){
  int i = 0;
  uint32_t num = 0;
  while (!whitespace_or_null(a[i]) && i < 20) {
    uint32_t char_val = a[i]-'0';
    if(char_val<0 || char_val > 9) {
      print_invalid_arg();
      return 0;
    }
    num = char_val + (num*10);
    i++;
  }
  return num;
}

void handle_unknown_command(char* args) {
  Serial.println("Command not recognised.");
}

void handle_touch(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  make_file(args);
}

void handle_mkdir(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  make_directory(args);
}

void handle_rm(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  rm_file(args);
}

void handle_rmdir(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  rm_directory(args);
}

void handle_cat(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  print_file_contents(args);
}

void handle_set_clock(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  uint32_t val_of_arg = stoi(args);
  set_clock(val_of_arg);
  print_rtc();
}

void handle_view_clock(char* args) {
  if(has_arg(args)) {
    print_invalid_arg();
    return;
  }
  print_rtc();
}

void handle_current_log(char* args) {
  if(has_arg(args)) {
    print_invalid_arg();
    return;
  }
  print_current_log();
}

void handle_make_log(char* args) {
  if(has_arg(args)) {
    print_invalid_arg();
    return;
  }
  start_new_log();
}

void handle_write_to_log(char* args) {
  if(!has_arg(args)) {
    print_invalid_arg();
    return;
  }
  int len_of_args = 0;
  for(int i=0; i< 511; i++) {
    if(args[i] == '\r' || args[i] == 'n') {
      len_of_args=i;
      break;
    }
  }
  write_to_log(args, len_of_args);
}

void handle_save_log(char* args) {
  if(has_arg(args)) {
    print_invalid_arg();
    return;
  }
  save_log();
}

void handle_close_log(char* args) {
  if(has_arg(args)) {
    print_invalid_arg();
    return;
  }
  close_log();
}

void handle_log_benchmark(char* args) {
  run_log_benchmark();
}

void handle_input() {
  if(Serial) {
    if(Serial.available()) {
      int num_chars = Serial.available();
      char* chars = new char[num_chars];
      Serial.readBytes(chars, num_chars);
      chars[num_chars-1] = NULL_CHAR;//we replace the last character instead of appending after the last character because the last character is \r
      auto tup = parse_command(chars);
      (*(std::get<0>(tup)))(std::get<1>(tup));
      Serial.println("-");
      delete[] chars;
    }
  }
}
