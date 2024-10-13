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


//log stuff
void rm_file_str(String name_of_file);
bool file_exists_str(String name_of_file);
void make_directory_str(String name_of_directory);
void setup_log();
void start_new_log();
bool log_currently_open();
void print_current_log();
void save_log();
void close_log();
void write_to_log(char* data, int num_chars);
void write_to_log_bytes(byte* data, int num_bytes);
void run_log_benchmark();
