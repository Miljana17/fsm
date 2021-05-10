#include <systemc>
#include <iostream>
#include <ctime>

using namespace sc_core;
using namespace std;

enum State {idle, state1, state2, state3};

//modul je komponenta sistema
SC_MODULE(Fsm)
{
    public:
        //u okviru ovog modula cemo imati konkurentne metode:
        SC_HAS_PROCESS(Fsm);

        Fsm(sc_module_name, State);

    protected:

        int n;

        void print(const char *str);
        State state;
        sc_event a, b, s0;


        void ab();
        void fsm();

};

Fsm::Fsm(sc_module_name name, State s) : sc_module(name), state(s)
{
    SC_METHOD(fsm);
        sensitive << a << b;

    SC_METHOD(ab)
        dont_initialize();
        sensitive << s0;
}

void Fsm::print(const char *str)
{
    cout << "Trenutno stanje: " << str << sc_time_stamp() << endl;
}

void Fsm::ab()
{
    n = rand() % 2;

    if(n == 0)
    {
        a.notify(5, SC_NS);
        state = state2;
    }
    else
    {
        b.notify(5, SC_NS);
        state = state3;
    }
}

void Fsm::fsm()
{
    switch(state)
    {
        case idle:
            print("idle ");
            next_trigger(5,SC_NS);
            state = state1;
            break;
        case state1:
            print("state1 ");
            next_trigger(a | b);
            s0.notify();
            break;
        case state2:
            print("state2 ");
            next_trigger(5, SC_NS);
            state = idle;
            break;
        case state3:
            print("state3 ");
            next_trigger(5, SC_NS);
            state = idle;
            break;
        default:
            assert(false);

    }
    
  
}

int sc_main(int argc, char* argv[])
{
    srand(time(NULL));

    Fsm uuu ("uuu", idle);

    sc_start(50, SC_NS);

    return 0;
}
