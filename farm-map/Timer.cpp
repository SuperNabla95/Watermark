#include <thread>
#include <iostream>
#include <fstream>
#include <atomic>
#include <chrono>
#include <future>
#include <vector>
#include <queue>
#include <string>

using namespace std;

#define FARM_OPS 0
#define MAP_OPS 1
#define NEW_IMAGE 2
#define DONE 3

struct event{
    long int time;
    int event_type;

    event(long int t, int et) : 
      time(t),
      event_type(et) {}

    string to_string(bool is_even){
        string color;
        switch (event_type)
        {
        case FARM_OPS:
            color = "red";
            break;
        case MAP_OPS:
            color = "green";
            break;
        case NEW_IMAGE:
            color = is_even ? "blue" : "royalblue";
            break;
        default:
            color = "useless";
            break;
        }
        return ::to_string(time) + "@" + color;
    }
};

// stopwatch. Returns time in seconds
class Timer {
  private:
    const long int origin;
    vector<queue<struct event>> times;
    const int nw,nt;

  public:

    Timer(int nw,int nt) :
      origin(std::chrono::system_clock::now().time_since_epoch().count()),
      nw(nw), 
      nt(nt)
    {
        this->times = vector<queue<struct event>>(nw*nt);
        for(int i=0;i<nw*nt;i++){
            this->times[i] = queue<struct event>();
        }
    }

    inline void register_event(int wid, int tid, int event_type) {
        long int elapsed = std::chrono::system_clock::now().time_since_epoch().count() - this->origin;
        struct event ev(elapsed,event_type);
        this->times[nt*wid + tid].push(ev);
    }

    void print_data(){
        string out_file_path = "farm-map_" + to_string(nw) + "_" + to_string(nt) + ".txt";
        ofstream outfile (out_file_path,ofstream::binary);
        for(int wid=0;wid<nw;wid++){
            for(int tid=0;tid<nt;tid++){
                bool is_even = true;
                queue<event> q = this->times[nt*wid + tid];
                outfile << "@thr(" << to_string(wid) << "," << to_string(tid) << ")" << endl;
                while (!q.empty()) {
                    auto ev = q.front();
                    is_even = ev.event_type!=NEW_IMAGE ? is_even : !is_even;
                    outfile << ev.to_string(is_even) << endl; 
                    q.pop(); 
                } 
            }
        }
    }
};

/*int main()
{
    Timer tim(2,2);
    tim.register_event(0,0,FARM_OPS);
    tim.register_event(0,0,FARM_OPS);

    tim.register_event(1,0,NEW_IMAGE);
    tim.register_event(1,0,FARM_OPS);

    tim.register_event(0,1,FARM_OPS);
    tim.register_event(0,1,FARM_OPS);

    tim.register_event(1,1,FARM_OPS);
    tim.register_event(1,1,FARM_OPS);
    tim.register_event(1,1,MAP_OPS);
    tim.register_event(1,1,FARM_OPS);

    tim.print_data();
}*/
