#include <chrono>
#include <iostream>
#include <string>
#include <cmath>

#include "Synchro.cpp"
using namespace std;


int main(int argc, char* argv[]){

	if(argc != 4+1){
		cout << "Usage is: " << argv[0] << " <src_path> <wmark_path> <dst_path> <nt>" << endl;
		return -1;
	}

	auto src_path = argv[1];
	auto wmark_path = argv[2];
	auto dst_path = argv[3];
	int nt = atoi(argv[4]);

	tdata td(nt,src_path,wmark_path);

    auto start   = chrono::high_resolution_clock::now();
    std::cout << "Start" << std::endl;
    std::cout << "Computing" << std::endl;

	td.threads->init_root(td);
	td.threads->join_root();

    std::cout << "Saving" << std::endl;
	td.src->save_bmp(dst_path);
	std::cout << "Done" << std::endl;

	return 0;
}
