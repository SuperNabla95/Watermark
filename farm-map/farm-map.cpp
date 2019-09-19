#include <chrono>
#include <iostream>
#include <string>
#include <cmath>

#include "Threads.cpp"
using namespace std;


int main(int argc, char* argv[]){

	if(argc != 5+1){
		cout << "Usage is: " << argv[0] << " <src_dir_path> <wmark_path> <dst_path> <nw> <nt>" << endl;
		return -1;
	}

	auto src_dir_path = argv[1];
	auto wmark_path = argv[2];
	auto dst_path = argv[3];
	int nw = atoi(argv[4]);
	int nt = atoi(argv[5]);

	imagevec *images = load_images(src_dir_path);
	Image *wmark = new Image(wmark_path);
	Timer *timer = new Timer(nw,nt);
	Threads threads(nw,nt,images,wmark,timer);

    //auto start   = chrono::high_resolution_clock::now();
    std::cout << "Computing" << std::endl;

	threads.do_job();

    std::cout << "Saving" << std::endl;
	int counter = 0;
	for(auto img : *images){
		string img_path = string(dst_path) + "/" + to_string(counter) + ".bmp";
		char ch[img_path.size() + 1];
		std::strcpy(ch, img_path.c_str());
		img.save_bmp(ch);
		counter++;
	}
	if(PRINT_STATS){
    	std::cout << "Printing statistics" << std::endl;
		timer->print_data();
	}
	std::cout << "Done" << std::endl;

	return 0;
}
