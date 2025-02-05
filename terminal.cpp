#include "source.h"
static vector<string> split(const string& str, char delimiter) {
	vector<string> result;
	string token;
	size_t start = 0, end;

	while ((end = str.find(delimiter, start)) != string::npos) {
		token = str.substr(start, end - start);
		if (token.size())
			result.emplace_back(token);
		start = end + 1;
	}
	// Add the last token (or the entire string if no delimiter was found)
	if(!result.size())
		result.emplace_back(str.substr(start));
	else {
		token = str.substr(start);
		if (token.size())
			result.emplace_back(token);
	}

	return result;
}

void SICLLR::terminal() {
	bool is_terminated = 0;
	string input;
	while (!is_terminated) {
		*tee << "SICLLR_shell> ";
		getline(cin, input);
		vector<string> command = split(input, ' ');
		if (!command.size()) continue;

		else if (command[0] == "help") {
			*tee << "Available commands: \n"
				<< "  help               : Displays help information for commands.\n"
				<< "  exit               : terminate the program.\n"
				<< "  load filename...   : Load object files to linking list.\n"
				<< "  remove filename... : Remove object file from linking list.\n"
				<< "  clear              : Clear all object files in linking list.\n"
				<< "  list               : List all object files in linking list.\n"
				<< "  link               : Link object files in linking list.\n"
				<< "  img filename       : Set link image file name.\n"
				<< "  report filename    : Generate report to filename.\n";
		}
		else if (command[0] == "exit") {
			*tee << "Writing log file to " << log_file_name << "...\n";
			*tee << "Program terminated.\n";
			*tee << "Thank you...\n";
			is_terminated = 1;
		}
		else if (command[0] == "link") {
			if (file_list.size() == 0) {
				*tee << "Cannot produce linking process: No object file loaded.\n";
				continue;
			}
			else if (!is_T_exist) {
				*tee << "Cannot produce linking process: Main function is not specifed.\n";
				continue;
			}
			else
				linker();
		}
		else if (command[0] == "load") {
			if (command.size() == 1) {
				*tee << "No file loaded.\n";
				continue;
			}
			for (size_t i = 1; i < command.size(); i++) {
				read_file(command[i]);

			}
		}
		else if (command[0] == "remove") {
			if (command.size() == 1) {
				*tee << "No file specified.\n";
				continue;
			}
			else if (file_list.count(command[1]) == 0) {
				*tee << "File not found.\n";
				continue;
			}
			for (size_t i = 1; i < command.size(); i++) {
				file_list.erase(command[i]);
				*tee << format("Object file \"{}\" is excluded.\n", command[i]);
				if (command[i] == top_file_name) {
					is_T_exist = false;
					*tee << format("Top file is not specifed.\n");
				}
			}
		}
		else if (command[0] == "clear") {
			file_list.clear();
			is_T_exist = false;
			*tee << "All object files are excluded.\n";
		}
		else if (command[0] == "list") {
			*tee << "Object file list: \n";
			for (const auto& [file_name, file] : file_list) {
				*tee << "  " << file_name << "\n";
			}
		}
		else if (command[0] == "img") {
			if (command.size() > 1) {
				*tee << format("Set output image file name to {}\n", command[1]);
				output_file_name = command[1];
			}
			else {
				*tee << "Invalid command: No file name.\n";
			}
		}
		else if (command[0] == "report") {
			if (command.size() > 1) {
				*tee << format("Set output report file name to {}\n", command[1]);
				report_file_name = command[1];
			}
			else {
				*tee << "Invalid command: No file name.\n";
			}
		}
		else {
			*tee << "Invalid command, enter \"help\" to check command list.\n";
		}
	}
	
}