#include "source.h"

void SICLLR::report() {
    ofstream outfile(report_file_name);
    if (!outfile) {
        *tee << ">> Failed to generate report file \"" << report_file_name << "\"" << ".\n";
        return;
    }

	// report header
    outfile << "        SIC/XE Linking Loader        \n"
            << "       version 1.0 for Windows       \n"
            << "             2025/01/11              \n"
            << "        Author: Tsai Yu-Chen         \n\n";

    // output Linking file list
    outfile << "Linking file list: \n";
    for (const auto& file : file_list) {
        outfile << "  " << file.first << "\n";
    }
    
    //outfile << "---------------------------------------\n";
    outfile << "\n\n";
	// error message
    if (error_list.empty()) {
        outfile << "No error occurred when linking file.\n";
        outfile << format("Image file \"{}\" was generated.\n\n", output_file_name);
    }
    else {
        for (const auto& error : error_list) {
            outfile << error << "\n";
        }
        outfile << format("{} error(s) detected. Therefore, no image file was generated.\n\n", error_list.size());
        outfile.close();
        *tee << "Report has been written to " << report_file_name << "\n\n";
		return;
    }

	// output program information
	outfile << "Program Information:\n";
    outfile << "  Program Name: " << prog_name << "\n"
        << "  Transfer Address: " << hex << trans_addr << "\n"
        << "  Begin Address: " << hex << begin_addr << "\n"
        << "  Size In Bytes: " << hex << prog_size << "\n\n\n";
            

    // Symbol Table
    outfile << "Symbol Table:\n";
    for (const auto& [symbol, address] : symbol_table) {
        outfile << "  Symbol: " << symbol << ", Address: " << hex << address << "\n";
    }
    outfile << "\n";

    // Modification Record
    outfile << "Modification Record:\n";
    for (const auto& [address, modify] : m_record) {
        outfile << "  Address: " << hex << address << ", Modification: " << modify.first << "\n";
    }
    outfile << "\n";

    //Original Linked Object Code
    outfile << "Original Linked Object Code:\n";
    for (const auto& [address, objectCode] : origin_obj_code) {
        outfile << "  Address: " << hex << address << ", Object Code: " << objectCode << "\n";
    }
    outfile << "\n";

    //Modified Linked Object Code
    outfile << "Modified Linked Object Code:\n";
    for (const auto& [address, objectCode] : linked_obj_code) {
        outfile << "  Address: " << hex << address << ", Object Code: " << objectCode << "\n";
    }
    outfile << "\n";

    *tee << "Report has been written to " << report_file_name << "\n\n";
    outfile.close();
}
