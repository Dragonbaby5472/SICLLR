#include "source.h"
bool SICLLR::memory_image() {
	const char type = 'I'; //Type should be 'I' for image file

	// make sure the program name is 6 characters long
    prog_name.append(6 - prog_name.size(), ' ');

	// header record
    string first_line = format(
        "{}{: <6}{:06X}{:06X}{:06X}",
        type, prog_name, begin_addr, prog_size, trans_addr
    );

    // obj2mem
    for (const auto& [startAddr, objcode] : linked_obj_code) {
        for (int j = 0; j < objcode.size() / 2; ++j) {
            mem[startAddr + j] = objcode.substr(j * 2, 2);
        }
    }

	// check if the file is valid
    ofstream out_file(output_file_name);
    if (!out_file) {
        *tee << "Failed to generate image file \"" << output_file_name << "\"\n";
        error_list.push_back(format("Failed to generate image file {}\n", output_file_name));
        return 0;
    }

    out_file << first_line << "\n";

	// every 32 bytes in a line, store in output file
    constexpr size_t groupSize = 32;
    for (size_t i = 0; i < prog_size; i += groupSize) {
        auto end = min(i + groupSize, mem.size());
        copy(mem.begin() + i, mem.begin() + end, ostream_iterator<string>(out_file));
        out_file << "\n"; // change line
    }
    *tee << "Image file \"" << output_file_name << "\"" << " generate successfully.\n";
    out_file.close();
    return 1;
}