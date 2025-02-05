#include "source.h";
const int mem_size = 0xFFFF + 1;
SICLLR::SICLLR()
{
	firstfile = true;
	is_T_exist = false;
	begin_addr = 0;
	current_addr = 0;
	prog_size = 0;
	next_addr = 0;
	trans_addr = 0;
	mem = vector<string>(mem_size, "FF");
	ofstream log_file(log_file_name);
	tee = new TeeStream(std::cout, log_file);

	*tee << "            SIC/XE Linking Loader        \n"
         << "           version 1.0 for Windows       \n"
         << "                 2025/01/11              \n"
         << "            Author: Tsai Yu-Chen         \n\n";
	terminal();
}
SICLLR::SICLLR(const vector<string>& in) 
{
    firstfile = true;
    is_T_exist = false;
    begin_addr = 0;
    current_addr = 0;
    prog_size = 0;
    next_addr = 0;
    trans_addr = 0;
    mem = vector<string>(mem_size, "FF");
	ofstream log_file(log_file_name);
    tee = new TeeStream(std::cout, log_file);
    // title
    *tee << "        SIC/XE Linking Loader        \n"
        << "       version 1.0 for Windows       \n"
        << "             2025/01/11              \n"
        << "        Author: Tsai Yu-Chen         \n\n\n";

	// read input instruction
	bool link_flag = false;
    for (size_t i = 0; i < in.size(); i++) {
		if ((in)[i] == "-o" && i + 1 < in.size()) { // img file name
            output_file_name = (in)[++i];
        }
		else if ((in)[i] == "-r" && i + 1 < in.size()) { // report file name
            report_file_name = (in)[++i];
        }
		else if ((in)[i] == "-l" && i + 1 < in.size()) { // log file name
			log_file_name = (in)[++i];
		}
		else if ((in)[i] == "-link") {
			link_flag = true;
		}
        else {
            read_file((in)[i]);
        }
    }
	if (link_flag) 
		if (file_list.size() == 0) {
			*tee << "Cannot produce linking process: No object file loaded.\n";
		}
		else
			linker();
	terminal();
}

SICLLR::~SICLLR()
{
	for (auto& [file_name, file] : file_list) {
		file->close();
		delete file;
	}
	delete tee;
}