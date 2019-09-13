#include<thread>
#include<vector>
#include "Image.cpp"

using namespace std;

#define ROOT 0


struct thread_data;

class Threads{
  private:
    vector<thread> thr;

  public:
    const int nt;

    Threads(int dim);
    ~Threads();
    inline void init(struct thread_data &td,int tid);
    inline void exit(int tid);
    inline void init_root(struct thread_data &td);
    inline void join_root();
};

inline void compute_pixel(
	int r,
	int c,
	Image *src,
	Image *wmark
	)
{
	int is_dark;
	is_dark = wmark->red()[r][c] & wmark->green()[r][c] & wmark->blue()[r][c];
	is_dark = ~is_dark;
	is_dark >>= 7;
	is_dark &= 0x01;

	src->red()[r][c] /= (is_dark+1);
	src->green()[r][c] /= (is_dark+1);
	src->blue()[r][c] /= (is_dark+1);

}

typedef struct thread_data{
	Threads *threads;
	Image *src, *wmark;

	thread_data(int nt,char *src_path,char *wmark_path)
	{
        this->threads = new Threads(nt);
        this->src = new Image(src_path);
        this->wmark = new Image(wmark_path);
    }

    thread_data(const thread_data &td){
        this->threads = td.threads;
        this->src = td.src;
        this->wmark = td.wmark;
    }

    void operator() (int tid) 
    {
		this->threads->init((*this),tid);
		for (int r=tid;r<src->height();r+=threads->nt){
			for(int c=0;c<src->width();c++){
				compute_pixel(r,c,src,wmark);
			}
		}
		this->threads->exit(tid);
    }
   
} tdata;

struct pizza {

    void operator() (int n){
        cout << "Molto buona" << endl;
    }
};

/*
class Barrier{
  private:
	volatile bool *b;
    const int dim;
  public:
	Barrier(int dim) : dim(dim){
        this->b = (volatile bool *) malloc(dim * sizeof(volatile bool));
        for(int i=0;i<dim;++i)
            b[i] = false;
    }

	~Barrier() {
        free((void*)this->b);
    }

    inline bool is_root(int tid){
        return tid==0;
    }

    inline void set_done(int tid){
        this->b[tid] = true;
    }

    inline bool is_left_child_done(int tid){
        return (2*tid+1 >= dim) || this->b[2*tid+1];
    }

    inline bool is_right_child_done(int tid){
        return (2*tid+2 >= dim) || this->b[2*tid+2];
    }

    inline void reset_children(int tid){
        if(2*tid+1<dim){
            this->b[2*tid+1] = false;
            if(2*tid+2<dim)
                this->b[2*tid+2] = false;
        }
    }
};*/

Threads::Threads(int dim) : nt(dim){
    this->thr = vector<thread>(dim);
}

Threads::~Threads(){}

inline void Threads::init(struct thread_data &td,int tid){
    int tid1,tid2;
    tid1 = 2*tid+1;
    tid2 = tid1+1;
    if(tid1 >= this->nt)
        return;
    this->thr[tid1] = thread(td,tid1);
    if(tid2 >= this->nt)
        return;
    this->thr[tid2] = thread(td,tid2);
}

inline void Threads::exit(int tid){
    int tid1,tid2;
    tid1 = 2*tid+1;
    tid2 = tid1+1;
    if(tid1 >= this->nt)
        return;
    this->thr[tid1].join();
    if(tid2 >= this->nt)
        return;
    this->thr[tid2].join();
}

inline void Threads::init_root(struct thread_data &td){
    this->thr[ROOT] = thread(td,ROOT);
    //struct pizza p;
    //this->thr[ROOT] = thread(p,ROOT);
}

inline void Threads::join_root(){
    this->thr[ROOT].join();
}