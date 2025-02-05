#include "source.h";
void SICLLR::read_file(const string& file_name) {
    ifstream* file = new ifstream(file_name);
    if (!*file) {
        *tee << "Failed to read object file \"" << file_name << "\"\n";
		return;
    }
    // open file successfully
    //find E record
	string line;
	if (file_name == top_file_name)
		is_T_exist = false;
	while (getline(*file, line)) {
		if (line[0] == 'E' && line.length() > 1) {
            if (!is_T_exist) {
                is_T_exist = true;
                top_file_name = file_name;
                break;
            }
            else{
				*tee << format("Error occurred when loading file \"{}\": Main function has already define.\n", file_name);	
				return;
            }
		}
	}
    file->clear();
    file->seekg(0, ios::beg);

	if (file_list.count(file_name)) {
		*tee << "Reload file \"" << file_name << "\".\n";
		file_list.erase(file_name);
	}
    else
        *tee << "Load file \"" << file_name << "\".\n";

    if(top_file_name == file_name)
        *tee << format("Top file is \"{}\".\n", file_name);

	file_list.emplace(file_name, file);
}

void SICLLR::parse_obj_file(ifstream* file, string file_name) {

    string line, temp_name;
    set<string> symbol_ref;
    while (getline(*file, line)) {
        char record_type = line.empty() ? '\0' : line[0];

        switch (record_type) {
        case 'H': { // Header
            temp_name = line.substr(1, 6);
            temp_name.erase(temp_name.find_last_not_of(" \t\r\n") + 1);
			symbol_ref.emplace(temp_name);
            int start_addr = stoi(line.substr(7, 6), nullptr, 16);
            int prog_length = stoi(line.substr(13, 6), nullptr, 16);

            if (firstfile) {
                begin_addr = start_addr;
                firstfile = false;
            }

            current_addr = next_addr;
            next_addr = current_addr + prog_length;
            prog_size += prog_length;

            if (symbol_table.count(temp_name)) {
                error_list.emplace_back(
                    format("Error occurred when linking program \"{}\" in file \"{}\": Duplicate program name.", temp_name, file_name));
            }
            else {
                symbol_table[temp_name] = current_addr;
            }
			if (file_name == top_file_name)
				prog_name = temp_name;
            break;
        }
        case 'D': { // Define
            for (size_t i = 1; i + 11 < line.size(); i += 12) {
                string symbol = line.substr(i, 6);
                symbol.erase(symbol.find_last_not_of(" \t\r\n") + 1);
                symbol_ref.emplace(symbol);
                int address = stoi(line.substr(i + 6, 6), nullptr, 16) + current_addr;

                if (symbol_table.count(symbol)) {
                    error_list.emplace_back(
                        format("Error occurred when linking program \"{}\" in file \"{}\": Duplicate symbol definition \"{}\".", temp_name, file_name, symbol));
                }
                else {
                    symbol_table[symbol] = address;
                }
            }
            break;
        }
        case 'T': { // Text
            int start_addr = stoi(line.substr(1, 6), nullptr, 16) + current_addr;
            int length = stoi(line.substr(7, 2), nullptr, 16);
            linked_obj_code.emplace_back(start_addr, line.substr(9, length * 2));
            break;
        }
        case 'M': { // Modify
            int address = stoi(line.substr(1, 6), nullptr, 16) + current_addr;
            string symbol = line.length() >= 16 ? line.substr(7, 9) : line.substr(7);
            symbol.erase(symbol.find_last_not_of(" \t\r\n") + 1);
            if (symbol.size() <= 2) {
                m_record.emplace(address, pair<string, string>({ symbol + "+" + temp_name, temp_name}));
            }
            else if(symbol_ref.count(symbol.substr(3)))
                m_record.emplace(address, pair<string, string>({ symbol, temp_name }));
			else { 
				error_list.emplace_back(
					format("Error occurred when linking program \"{}\" in file \"{}\": Undefined symbol reference \"{}\".", temp_name, file_name, symbol.substr(3)));
			}
            /*if (line.length() >= 16) {
                symbol = line.substr(7, 9);
                symbol.erase(symbol.find_last_not_of(" \t\r\n") + 1);
                m_record.emplace(address, pair<string, string>({ symbol, temp_name }));
            }
            else if (line.length() > 9) {
                symbol = line.substr(7);
                symbol.erase(symbol.find_last_not_of(" \t\r\n") + 1);
                m_record.emplace(address, pair<string, string>({ symbol, temp_name }));
            }
            else {
                m_record.emplace(address, pair<string, string>({ symbol + "+" + temp_name, temp_name }));
            }*/
            break;
        }
        case 'E': { // End
			//is done by read_file();
            break;
        }
		case 'R': { // Reference
			string temp_ref = line.substr(1);
            for (size_t i = 0; i < temp_ref.size(); i += 6) {
                string symbol = temp_ref.substr(i, 6);
                symbol.erase(symbol.find_last_not_of(" \t\r\n") + 1);
				symbol_ref.emplace(symbol);
            }
            break; 
        }
		default:
            error_list.emplace_back(
                format("Error occurred when linking program \"{}\" in file \"{}\": Unknown record type.", temp_name, file_name));
			break;
        }
    }
    //file->close();
}