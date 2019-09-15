#include<thread>
#include<vector>
#include "Reader.cpp"

using namespace std;

#define ROOT 0


struct thread_data;

class Threads{
  private:
    vector<thread> thr;

  public:
    const int nt,nw;

    Threads(int nt,int nw);
    ~Threads();
    inline void init(struct thread_data &td,int tid,int wid);
    inline void exit(int tid);
    inline void init_root(struct thread_data &td,int wid);
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
};

typedef struct thread_data{
	Threads *threads;
	imagevec *images;
    Barrier *barrier;
    Image *wmark;

    thread_data(int nt, int nw, imagevec *images, char *wmark_path) : images(images){
        this->threads = new Threads(nt,nw);
        this->barrier = new Barrier(nt);
        this->wmark = new Image(wmark_path);
    }

    thread_data(const thread_data &td){
        this->threads = td.threads;
        this->images = td.images;
        this->barrier = td.barrier;
        this->wmark = td.wmark;
    }

    void operator() (int tid, int wid) 
    {
        cout << wid << "pizza" << endl;
		this->threads->init((*this),tid,wid);
		for(int index=wid; index<this->images->size(); index+=threads->nw){
            for (int r=tid;r<(*images)[index].height();r+=threads->nt){
                for(int c=0;c<(*images)[index].width();c++){
                    compute_pixel(r,c,(images->data() + index),wmark);
                }
		    }
        }
		this->threads->exit(tid);
    }
   
} tdata;

typedef struct worker_data{
    tdata *td;
    const int nw;
  
    worker_data(int nt, int nw, imagevec *images, char *wmark_path) : nw(nw){
        this->td = (tdata*)malloc(nw*sizeof(tdata));
        for(int wid=0;wid<nw;wid++){
            this->td[wid] = tdata(nt,nw,images, wmark_path);
        }
    }

    ~worker_data(){
        free(td);
    }

    inline void init(struct worker_data &istr,int wid);
    inline void exit(int wid);
    inline void init_root(struct worker_data &istr);
    inline void join_root();
} wdata;

inline void worker_data::init(struct worker_data &istr,int wid){
    int wid1,wid2;
    wid1 = 2*wid+1;
    wid2 = wid1+1;
    if(wid1 >= this->nw)
        return;
    this->td[wid1].threads->init_root(this->td[wid1],wid1);
    if(wid2 >= this->nw)
        return;
    this->td[wid2].threads->init_root(this->td[wid2],wid2);
}

inline void worker_data::exit(int wid){
    int wid1,wid2;
    wid1 = 2*wid+1;
    wid2 = wid1+1;
    if(wid1 >= this->nw)
        return;
    this->td[wid1].threads->join_root();
    if(wid2 >= this->nw)
        return;
    this->td[wid2].threads->join_root();
}

inline void worker_data::init_root(struct worker_data &istr){
    this->td[ROOT].threads->init_root(td[ROOT],ROOT);
}

inline void worker_data::join_root(){
    this->td[ROOT].threads->join_root();
}

Threads::Threads(int nt,int nw) : nt(nt),nw(nw){
    this->thr = vector<thread>(nt);
}

Threads::~Threads(){}

inline void Threads::init(struct thread_data &td,int tid,int wid){
    int tid1,tid2;
    tid1 = 2*tid+1;
    tid2 = tid1+1;
    if(tid1 >= this->nt)
        return;
    this->thr[tid1] = thread(td,tid1,wid);
    if(tid2 >= this->nt)
        return;
    this->thr[tid2] = thread(td,tid2,wid);
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

inline void Threads::init_root(struct thread_data &td,int wid){
    this->thr[ROOT] = thread(td,ROOT,wid);
}

inline void Threads::join_root(){
    this->thr[ROOT].join();
}