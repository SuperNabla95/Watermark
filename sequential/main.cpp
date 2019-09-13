#include <chrono>
#include <iostream>
#include <string>
#include <cmath>

#include "Image.cpp"
using namespace std;

inline void compute_pixel(
	int r,
	int c,
	Image &src,
	Image &wmark
	)
{
	int is_dark;
	is_dark = wmark.red()[r][c] & wmark.green()[r][c] & wmark.blue()[r][c];
	is_dark = ~is_dark;
	is_dark >>= 7;
	is_dark &= 0x01;

	src.red()[r][c] /= (is_dark+1);
	src.green()[r][c] /= (is_dark+1);
	src.blue()[r][c] /= (is_dark+1);

}

int main(int argc, char* argv[]){

	if(argc != 3+1){
		cout << "Usage is: " << argv[0] << " <src_path> <wmark_path> <dst_path>" << endl;
		return -1;
	}

	auto src_path = argv[1];
	auto wmark_path = argv[2];
	auto dst_path = argv[3];

	Image src, wmark;
	int width, height;
	
	//src
	try{
		src = Image(src_path);
	}
	catch(const CImgIOException &ioe){
		cout << "Not able to read from " << src_path << endl;
		return -8;
	}
    width = src.width();
    height = src.height();

	//wmark
	try{
		wmark = Image(wmark_path);
	}
	catch(const CImgIOException &ioe){
		cout << "Not able to read from " << wmark_path << endl;
		return -8;
	}
	if(width > wmark.width() || height > wmark.height()){
		cout << "Dimensions of source image and watermark do not correspond: "
		<< width << "x" << height << " <---> " << wmark.width() << "x" << wmark.height() << endl;
		return -2;
	}


    auto start   = chrono::high_resolution_clock::now();
    std::cout << "Start" << std::endl;
    std::cout << "Computing" << std::endl;

	//computing
	for (int r=0;r<height; r++){
		for(int c=0;c<width;c++){
			compute_pixel(r,c,src,wmark);
		}
	}

    std::cout << "Saving" << std::endl;
	src.save_bmp(dst_path);
	std::cout << "Done" << std::endl;

	return 0;
}
