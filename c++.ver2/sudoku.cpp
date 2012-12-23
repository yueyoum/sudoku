#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;



class Dot
{
    public:
        Dot(int, int, int);
        Dot(int, int, int, bool);
        void set_new_value(void);
        int get_value(void);
        bool arrival_max(void);
        void reset_value(void);
        bool is_editable(void);
        int get_x(void);
        int get_y(void);

    private:
        int value;
        int x;
        int y;
        bool editable;
};

Dot::Dot(int v, int x, int y):value(v), x(x), y(y), editable(false){};
Dot::Dot(int v, int x, int y, bool e):value(v), x(x), y(y), editable(e){};

inline int Dot::get_value(void){ return value; }
inline bool Dot::arrival_max(void){ return (value == 9); }
inline void Dot::reset_value(void) { value = 0; }
inline void Dot::set_new_value(void){ if(editable) ++value; }
inline bool Dot::is_editable(void){ return editable; }
inline int Dot::get_x(void){ return x; }
inline int Dot::get_y(void){ return y; }


typedef vector<Dot> line_t;
typedef pair<vector<line_t>::iterator, line_t::iterator> iter_pos_t;
typedef map<int, iter_pos_t> run_step_t;


class Sudoku
{
    public:
        Sudoku(): _step(0){};
        void initialize(const vector< vector<int> >&);
        void run(void);
        bool check_not_same(const line_t::iterator);
        bool check_row(int);
        bool check_column(int);
        bool check_square(int, int);

        void add_step(const iter_pos_t&);
        iter_pos_t get_pos(void);
        void erase_error_step(void);

        void output(void);

    private:
        vector<line_t> sudoku;
        run_step_t sudoku_step;
        int _step;

        bool _check(const vector<int>&);
};

inline void Sudoku::add_step(const iter_pos_t &pos)
{
    iter_pos_t t(pos.first, pos.second);
    sudoku_step[_step++] = t;
}

inline iter_pos_t Sudoku::get_pos(void){ return sudoku_step[_step-1]; }
inline void Sudoku::erase_error_step(void){ sudoku_step.erase(--_step); }


void Sudoku::initialize(const vector< vector<int> >& vvv)
{
    line_t line;
    int x=0, y=0;
    for(auto vv: vvv)
    {
        for(auto v: vv)
        {
            Dot d(v, x++, y, v==0);
            line.push_back(d);
        }
        x=0;
        y++;

        sudoku.push_back(line);
        line.clear();
    }
}

void Sudoku::run(void)
{
    bool rollback = false;

    auto rollback_hanlder = 
        [this, &rollback](vector<line_t>::iterator &y_iter, line_t::iterator &x_iter){
            x_iter->reset_value();
            iter_pos_t pos = this->get_pos();
            y_iter = pos.first;
            x_iter = pos.second;
            this->erase_error_step();
            rollback = true;
        };

    for(vector<line_t>::iterator it=sudoku.begin(); it!=sudoku.end();)
    {
        for(line_t::iterator _it=it->begin(); _it!=it->end();)
        {
            rollback = false;
            if(_it->is_editable())
            {
                while(true)
                {
                    if(_it->arrival_max())
                    {
                        rollback_hanlder(it, _it);
                        break;
                    }

                    _it->set_new_value();

                    if(check_not_same(_it))
                    {
                        add_step(make_pair(it, _it));
                        break;
                    }

                    if(_it->arrival_max())
                    {
                        rollback_hanlder(it, _it);
                        break;
                    }
                }
            }
            if(!rollback) _it++;
        }
        if(!rollback) it++;
    }
}


inline bool Sudoku::check_not_same(const line_t::iterator d)
{
    return (check_row(d->get_y()) && check_column(d->get_x()) 
            && check_square(d->get_y(), d->get_x())
            );
}


inline bool Sudoku::check_row(int y)
{
    vector<int> row;
    for(auto v: sudoku[y])
        row.push_back(v.get_value());
    return _check(row);
}


inline bool Sudoku::check_column(int x)
{
    vector<int> column;
    for(auto v: sudoku)
        column.push_back(v[x].get_value());
    return _check(column);
}

inline bool Sudoku::check_square(int y, int x)
{
    vector<int> square;
    int y_min = y / 3 * 3;
    int y_max = y_min + 2;
    int x_min = x / 3 * 3;
    int x_max = x_min + 2;

    for(int _y=y_min; _y<=y_max; _y++)
    {
        for(int _x=x_min; _x<=x_max; _x++)
            square.push_back( sudoku[_y][_x].get_value() );
    }

    return _check(square);
}


inline bool Sudoku::_check(const vector<int> &values)
{
    vector<int> tmp;
    for(auto v: values)
    {
        if(v!=0) tmp.push_back(v);
    }
    sort(tmp.begin(), tmp.end());
    vector<int>::iterator unique_iter = unique(tmp.begin(), tmp.end());
    return (unique_iter == tmp.end());
}


void Sudoku::output(void)
{
    for(auto line: sudoku)
    {
        for(auto dot: line)
            cout << dot.get_value() << " ";
        cout << endl;
    }
}



int main(void)
{
    vector<int> input_line;
    vector< vector<int> > input_sudoku;
    string line;
    stringstream ss;
    int num;
    
    ifstream f("sudoku.txt");
    if(!f)
    {
        cerr << "Unable open sudoku.txt" << endl;
        exit(2);
    }

    while(getline(f, line))
    {
        for(auto s: line)
        {
            ss << s;
            ss >> num;
            ss.clear();
            input_line.push_back(num);
        }
        input_sudoku.push_back(input_line);
        input_line.clear();
    }
    f.close();

    Sudoku sudoku;
    sudoku.initialize(input_sudoku);
    sudoku.output();
    sudoku.run();
    cout << endl;
    sudoku.output();

    return 0;
}
