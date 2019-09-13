#include <iostream>
using namespace std;

#include "CImg.h"
using namespace cimg_library;

class Channel{
  private:
    unsigned char *start;
    int width;
  public:
    Channel(){}
    Channel(unsigned char *start, int width) : start(start), width(width){}
    inline unsigned char *operator[](int row){
        return this->start + row*this->width;
    }
};

class Image{
  private:
    CImg<unsigned char> cimage;
    Channel r,g,b;
    int w,h;
  public:
    Image();
    Image(char *path);
    Image(int width, int height);
    inline int width();
    inline int height();
    inline Channel red();
    inline Channel green();
    inline Channel blue();
    inline void save_bmp(char *dst_path);
};

Image::Image(){}

Image::Image(char *src_path)
{
    this->cimage = CImg<>(src_path);
    this->w = this->cimage.width();
    this->h = this->cimage.height();
    unsigned char *data = this->cimage.data();
    int offset = h*w;
    this->r = Channel(data,w);
    this->g = Channel(data+offset,w);
    this->b = Channel(data+2*offset,w);
}

Image::Image(int width, int height){
    this->cimage = CImg<>(width,height,1,3);
    this->w = this->cimage.width();
    this->h = this->cimage.height();
    unsigned char *data = this->cimage.data();
    int offset = h*w;
    this->r = Channel(data,w);
    this->g = Channel(data+offset,w);
    this->b = Channel(data+2*offset,w);
}

inline void Image::save_bmp(char *dst_path){
    this->cimage.save_bmp(dst_path);
}

inline int Image::width(){return this->w;}
inline int Image::height(){return this->h;}
inline Channel Image::red(){return this->r;}
inline Channel Image::green(){return this->g;}
inline Channel Image::blue(){return this->b;}

