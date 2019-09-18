#include<iostream>
#include<thread>
#include<vector>

//#include "Image.cpp"
#include "Reader.cpp"

using namespace std;

#define ROOT 0

class Threads{
  private:
    vector<Image> *_imagevec;
    Image *_wmark;

    /*inline thread* operator[](int wid) 
    { 
        return this->_thr.data() + wid*nw;
    }*/

  public:
    const int nw;
    const int nt;

    Threads(int nw, int nt, vector<Image> *vimage, Image *wmark);
    //~Threads();
    void do_job();

  private:
    struct map_pattern{

        static void sum(int a,int b){cout << (a+b) << endl;}

        inline void init_map(Threads *outer, thread *threads, int wid, int tid){
            int tid1,tid2;
            tid1 = 2*tid+1;
            tid2 = tid1+1;
            if(tid1 >= outer->nt)
                return;
            threads[0] = thread(map_pattern(),outer,wid,tid1);
            if(tid2 >= outer->nt)
                return;
            threads[1] = thread(map_pattern(),outer,wid,tid2);
        }

        inline void exit_map(Threads *outer, thread *threads, int wid, int tid){
            int tid1,tid2;
            tid1 = 2*tid+1;
            tid2 = tid1+1;
            if(tid1 >= outer->nt)
                return;
            threads[0].join();
            if(tid2 >= outer->nt)
                return;
            threads[1].join();
        }

        void operator() (Threads *outer, int wid, int tid)
        {
            vector<thread> v(2);
            init_map(outer,v.data(),wid,tid);
            cout << to_string(wid) + "-->" + to_string(tid) + " eccoci\n";
            int num_images = outer->_imagevec->size();
            for(int img=wid;img<num_images;img+=outer->nw){
                Image src = (*(outer->_imagevec))[img];
                for (int r=tid;r<src.height();r+=outer->nt){
                    for(int c=0;c<src.width();c++){
                        int is_dark;
                        is_dark =
                          outer->_wmark->red()[r][c] &
                          outer->_wmark->green()[r][c] &
                          outer->_wmark->blue()[r][c];
                        is_dark = ~is_dark;
                        is_dark >>= 7;
                        is_dark &= 0x1;

                        src.red()[r][c] /= (is_dark+1);
                        src.green()[r][c] /= (is_dark+1);
                        src.blue()[r][c] /= (is_dark+1);
                    }
                }
            }
            exit_map(outer,v.data(),wid,tid);
        }   
    };//end struct map_pattern

    struct farm_pattern{
        inline void init_farm(Threads *outer,thread *threads,int wid){
            int wid1,wid2;
            wid1 = 2*wid+1;
            wid2 = wid1+1;
            if(wid1 >= outer->nw)
                return;
            threads[0] = thread(farm_pattern(),outer,wid1);
            if(wid2 >= outer->nw)
                return;
            threads[1] = thread(farm_pattern(),outer,wid2);
        }

        inline void exit_farm(Threads *outer,thread *threads,int wid){
            int wid1,wid2;
            wid1 = 2*wid+1;
            wid2 = wid1+1;
            if(wid1 >= outer->nw)
                return;
            threads[0].join();
            if(wid2 >= outer->nw)
                return;
            threads[1].join();
        }

        void operator() (Threads *outer,int wid) 
        {
            vector<thread> v(2);
            init_farm(outer,v.data(),wid);
            map_pattern()(outer,wid,ROOT);
            exit_farm(outer,v.data(),wid);
        }   
    };//end struct farm_pattern
    
}; //end class Threads

Threads::Threads(int nw,int nt,vector<Image> *vimage,Image *wmark) :
  nw(nw),
  nt(nt),
  _imagevec(vimage),
  _wmark(wmark)
{

}

void Threads::do_job(){
    farm_pattern()(this,ROOT);
}

int main(int argc, char* argv[]){
	if(argc != 2+1){
		cout << "Usage is: " << argv[0] << " <nw> nt>" << endl;
        return -1;
	}
    int nw,nt;
    nw = atoi(argv[1]);
    nt = atoi(argv[2]);

    char *src_dir_path = "/home/ftosoni/Desktop/spm/repo/small";
    char *wmark_path = "/home/ftosoni/Desktop/spm/repo/small/logo_small.png";
	imagevec *images = load_images(src_dir_path);
    Image *wmark = new Image(wmark_path);
    char *dst_path = "./qui";

    cout << "Computing" << endl;
    Threads t(nw,nt,images,wmark);
    t.do_job();

    cout << "Saving" << endl;
	int counter = 0;
	for(auto img : *images){
		string img_path = string(dst_path) + "/" + to_string(counter) + ".bmp";
		char ch[img_path.size() + 1];
		std::strcpy(ch, img_path.c_str());
		img.save_bmp(ch);
		counter++;
	}
    return 0;
}

