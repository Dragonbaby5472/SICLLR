#include "source.h";
void SICLLR::linker() {
	// parse object file
    //parse top file first
    parse_obj_file(file_list[top_file_name], top_file_name);
	for (auto& f : file_list) {
		if (f.first == top_file_name)
			continue;
		parse_obj_file(f.second, f.first);
	}
	if (error_list.size() > 0) {
		for (const auto err : error_list)
			*tee << err << "\n";
		*tee << "Failed to link due to errors above.\n";
		*tee << "No image file generated.\n";
        *tee << format("Writing linking report to {}...\n", report_file_name);
        report();
        data_reset();
		return;
	}
	// check if transfer address exist
    if (!is_T_exist) {
        error_list.emplace_back(
            "Error occurred when linking all file: No transfer address in all program.");

    }

	// copy object code
    origin_obj_code = linked_obj_code;

	// do relocation
    for (const auto& [address, record] : m_record) {
		// find the T record that contains the address
        int nth_Trecord = -1;
        for (size_t i = 0; i < linked_obj_code.size(); ++i) {
            int start = linked_obj_code[i].first;
            int end = start + static_cast<int>(linked_obj_code[i].second.length()) / 2;
            if (address >= start && address < end) {
                nth_Trecord = static_cast<int>(i);
                break;
            }
        }

        if (nth_Trecord == -1) {
            error_list.emplace_back(format(
                "Error occurred when linking program \"{}\": Address \"{:X}\" not found in any T record.",
                record.second, address));
            continue;
        }

        // count the offset and length of the modification
        int offset = address - linked_obj_code[nth_Trecord].first;
        int length = stoi(record.first.substr(0, 2), nullptr, 16);
        int num_hbyte = offset * 2 + (length % 2 == 1 ? 1 : 0);

		// catch the origin value
        string hexValue = linked_obj_code[nth_Trecord].second.substr(num_hbyte, length);
        int modifyValue = stoi(hexValue, nullptr, 16);

		//no need to count m record
		if (record.first.size() < 3) {
            // replace the origin value with the modified value
            modifyValue += symbol_table[record.second];
            linked_obj_code[nth_Trecord].second.replace(num_hbyte, length, format("{:0{}X}", modifyValue, length));
			continue;
		}
        // count the modification value
        char operation = record.first[2];
        string key = record.first.substr(3);

        if (symbol_table.count(key)) {
            if (operation == '+') {
                modifyValue += symbol_table[key];
            }
            else if (operation == '-') {
                modifyValue -= symbol_table[key];
            }
            else {
                error_list.emplace_back(format(
                    "Error occurred when linking program \"{}\": Invalid modification operator \"{}\" in M record Address \"{:X}\".",
                    record.second, operation, address));
                continue;
            }
        }
        else {
            error_list.emplace_back(format(
                "Error occurred when linking program \"{}\": Unresolved symbol reference \"{}\" in M record Address \"{:X}\".",
                record.second, key, address));
            continue;
        }


		// replace the origin value with the modified value
        linked_obj_code[nth_Trecord].second.replace(num_hbyte, length, format("{:0{}X}", modifyValue, length));
    }
    if (error_list.size() > 0) {
        for (const auto err : error_list)
            *tee << err << "\n";
        error_list.clear();
        *tee << "Failed to link due to errors above.\n";
        *tee << "No image file generated.\n";
    }
    else {
		*tee << format("Linking completed. Writing image file {}...\n", output_file_name);
		memory_image();
    }
    *tee << format("Writing linking report to {}...\n", report_file_name);
    report();
	data_reset();

}

void SICLLR::data_reset() {
    firstfile = true;
    is_T_exist = false;
    begin_addr = 0;
    current_addr = 0;
    prog_size = 0;
    next_addr = 0;
    trans_addr = 0;
	symbol_table.clear();
	m_record.clear();
	linked_obj_code.clear();
	origin_obj_code.clear();
	//mem.clear();
	error_list.clear();
	// clear all file buffer
    for (auto& f : file_list) {
		f.second->clear();
		f.second->seekg(0, ios::beg);
    }

}