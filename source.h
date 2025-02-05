#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>  
#include <map>
#include <set>
//#include <multimap>
#include <sstream>
#include <fstream>
#include <utility>
#include <format>
#include <stdio.h>
using namespace std;
class TeeStream;
class SICLLR;


class SICLLR
{
public:
	SICLLR();
	SICLLR(const vector<string>&);
	~SICLLR();

private:
	//variables
    //vector<string> in_file;
    string prog_name;//output obj name
    bool firstfile = true;
    bool is_T_exist = false;
	string top_file_name;
    int current_addr, prog_size, next_addr, trans_addr, begin_addr = 0;
    map<string, int> symbol_table;
	map<string, ifstream*> file_list;
	multimap<int, pair<string, string>> m_record;//first=address,second=<modifyRecord, prog_name>
    vector<pair<int, string>>linked_obj_code;//first=address,second=obj code
    vector<pair<int, string>>origin_obj_code;
    vector<string> mem;
    //output file
	string log_file_name = "SICLLR.log";
    string output_file_name = "DEVF2";
    string report_file_name = "SICLLR_report.txt";
    //log buffer
	TeeStream* tee;
    //report variables
    vector<string> error_list;

	//functions
	void linker();
	void parse_obj_file(ifstream*, string);
	void read_file(const string&);
    bool memory_image();
    void report();
    void terminal();
	void data_reset();
};

class TeeStream {

private:
	ostream& stream1; // first output stream
	ostream& stream2; // second output stream

public:
    TeeStream(ostream& s1, ostream& s2): stream1(s1), stream2(s2) {}

    template <typename T>
    TeeStream& operator<<(const T& value) {
		stream1 << value; // output to the first stream
		stream2 << value; // output to the second stream
        return *this;
    }
};