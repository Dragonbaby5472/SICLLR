#include "source.h"
//Main RDREC WRREC -o output.txt -r report.txt
int main(int argc, char** argv) {
	//variables
	vector<string> input;
	for (int i = 1; i < argc; i++) {
		input.emplace_back(argv[i]);
	}
	try {
		SICLLR main_prog(input);
	 }
	catch (const exception& e) {
		cerr << "Fatal error occurred due to following reason: " << e.what() << endl;
		cout << "Program terminated.\n";
	}

    return 0;
}