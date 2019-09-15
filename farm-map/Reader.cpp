#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sys/types.h>
#include <dirent.h>
#include"Image.cpp"

typedef std::vector<std::string> stringvec;
typedef std::vector<Image> imagevec;


bool ends_with(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return false;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return false;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string image_to_path(string dir_path, string image){
    if(ends_with(dir_path,"/")){
        return dir_path + image;
    }else{
        return dir_path + "/" + image;
    }
}
 
void read_jpg(char *dir_path, stringvec& v)
{
    DIR* dirp = opendir(dir_path);
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        if(ends_with(dp->d_name,".jpg")){
            auto img_path = image_to_path(dir_path,dp->d_name);
            std::cout << "img_path: " << img_path << std::endl;
            v.push_back(img_path);
        }
    }
    closedir(dirp);
}

imagevec *load_images(char *dir_path){
    stringvec v;
    read_jpg(dir_path,v);
    auto result = new imagevec(v.size());
    std::transform<>(v.begin(),v.end(),(*result).begin(),[](string path) -> Image {
        std::cout << "prima: " << path;
        char ch[path.size() + 1];
        std::strcpy(ch, path.c_str());
        std::cout << ", dopo: " << ch << std::endl;
        return Image(ch);
    });
    return result;
}

/*int main(int argc, char* argv[]){
    stringvec v;
    read_jpg(argv[1], v);
    std::copy(v.begin(), v.end(),
         std::ostream_iterator<std::string>(std::cout, "\n"));
}*/