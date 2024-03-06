#include "input.h"
#include <Arduino.h>


bool whitespace_or_null(char c) {
	return c==NULL_CHAR || c == ' ' || c == '\n' || c == '\r';
}

bool attempt_parse_command(char* input, const char* command_to_parse) {
  //we assume that both are null terminated.
  for(int i = 0; i < MAX_COMMAND_LEN; i++) {
    if(command_to_parse[i] == NULL_CHAR) {
      //we have run out of command.
      if(whitespace_or_null(input[i])) {
        //and also input, therefore command == input.
        return true;
      } else {
        //but we still have more input.
        //e.g. command=="ls", input=="lsasd sdf"
        return false;
      }
    } else if(whitespace_or_null(input[i])) {
      //but we dont have more input.
      return false;
    } else if(input[i] != command_to_parse[i]) {
    	return false;
    }
  }
  return false;
}


Command parse_command(char* input) {
	for(auto c = commands.begin(); c != commands.end(); ++c) {
		if(attempt_parse_command(input, c->second)) {
			return c->first;
		}
	}
  return NOT_KNOWN;
}

void handle_input() {
  if(Serial.available()) {
    int num_chars = Serial.available();
    char* chars = new char[num_chars+1];
    Serial.readBytes(chars, num_chars);
    chars[num_chars] = NULL_CHAR;
    
    Serial.println("processing input: " + String(chars));

    Command c = parse_command(chars);
    switch(c) {
      case(LS):
        Serial.println("handling ls!");
        break;
      case(TOUCH):
      	Serial.println("handling touch!");
      	break;
      case(NOT_KNOWN):
        Serial.println("Command not recognised.");
        break;
      default:
        Serial.println("We somehow parsed something else?");
    }
  }
}