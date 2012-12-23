#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED


using namespace std;

// BaseNineBox is the Base Class.
class BaseNineBox
{
    public:
        BaseNineBox();
        ~BaseNineBox();
        bool has_same_value(const unsigned int &) const;
        bool has_same_value() const;
        void print_value() const;    // print current nine values
        void print_all_value() const;// print 9 * 10 values.
        unsigned int * get_value_addr(const unsigned int &) const;
        unsigned int get_value(const unsigned int &) const;
        void clean_candidate(const unsigned int &);
        bool find_absolute_value();
        bool have_init_value(const unsigned int &);
        void new_defined_value(const unsigned int &);

    protected:
        unsigned int **single_box;  // unsigned int *single_box[9]
        bool init_value[9];
};



// OneLine contains boxes which from left to right.

class OneLine: public BaseNineBox
{
    public:
        OneLine(const string &s="000000000");
        ~OneLine();
        void auto_set_value(const unsigned int &);
        void empty_box_value(const unsigned int &);
        bool have_defined_new_box();
        bool already_max(const unsigned int &);
    private:
        unsigned int *line;
};



// OneRow use OneLine to initial.
class OneRow: public BaseNineBox
{
    public:
        OneRow(OneLine **, const unsigned int &);
};



// SmallBox, 3 x 3 area.
class SmallBox: public BaseNineBox
{
    public:
        SmallBox(OneLine **, const unsigned int &,
                 const unsigned int &);
};


#endif // SUDOKU_H_INCLUDED
