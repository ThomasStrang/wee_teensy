void setup_log();
void start_new_log();
bool log_currently_open();
void print_current_log();
void save_log();
void close_log();
void write_to_log(char* data, int num_chars);
void write_to_log_bytes(byte* data, int num_bytes);
void run_log_benchmark(int num_bytes_per_flush, int num_flushes);
