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
#define PROCESSING 2

struct event{
    int time;
    int event_type;

    event(int t, int et) : 
      time(t),
      event_type(et) {}

    string to_string(){
        string color;
        switch (event_type)
        {
        case FARM_OPS:
            color = "red";
            break;
        case MAP_OPS:
            color = "green";
            break;
        case PROCESSING:
            color = "blue";
            break;
        default:
            color = "#00000000";
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
    const int nt,nw;

  public:

    Timer(int nw,int nt) :
      origin(std::chrono::system_clock::now().time_since_epoch().count()),
      nt(nt), nw(nw)
    {
        this->times = vector<queue<struct event>>(nw*nt);
        for(int i=0;i<nw*nt;i++){
            this->times[i] = queue<struct event>();
        }
    }

    inline void register_event(int wid, int tid, int event_type) {
        long int elapsed = std::chrono::system_clock::now().time_since_epoch().count() - this->origin;
        struct event ev(static_cast<int>(elapsed),event_type);
        this->times[nt*wid + tid].push(ev);
    }

    void print_data(){
        string out_file_path = "farm-map_" + to_string(nw) + "_" + to_string(nt);
        ofstream outfile (out_file_path,ofstream::binary);
        for(int wid=0;wid<nw;wid++){
            for(int tid=0;tid<nt;tid++){
                queue<event> q = this->times[nt*wid + tid];
                outfile << "@thr(" << to_string(wid) << "," << to_string(tid) << ")" << endl;
                while (!q.empty()) {
                    auto ev = q.front();
                    outfile << ev.to_string() << endl; 
                    q.pop(); 
                } 
            }
        }
    }
};

int main()
{
    Timer tim(2,2);
    tim.register_event(0,0,FARM_OPS);
    tim.register_event(0,0,FARM_OPS);

    tim.register_event(1,0,PROCESSING);
    tim.register_event(1,0,FARM_OPS);

    tim.register_event(0,1,FARM_OPS);
    tim.register_event(0,1,FARM_OPS);

    tim.register_event(1,1,FARM_OPS);
    tim.register_event(1,1,FARM_OPS);
    tim.register_event(1,1,MAP_OPS);
    tim.register_event(1,1,FARM_OPS);

    tim.print_data();
}
