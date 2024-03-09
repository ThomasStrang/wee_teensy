#include <SD.h>
void setup_sd_card();
void print_directory(File dir, int numTabs);
void make_directory(char* name_of_directory);
void rm_directory(char* name_of_directory);
void print_file_tree();
bool file_exists(char* name_of_file);
void make_file(char* name_of_file);
void rm_file(char* name_of_file);
void print_file_contents(char* name_of_file);

void rm_file_str(String name_of_file);
bool file_exists_str(String name_of_file);
void make_directory_str(String name_of_directory);
