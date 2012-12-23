#include <iostream>
#include <string>
#include "sudoku.h"

BaseNineBox::BaseNineBox()
{
    // single_box is an array.
    // it contains 9 items which are (unsigned int *) point.
    // single_box = address of this array.
    // *(single_box+i) = value of array[i], also means an address (unsigned int *)
    // **(single_box+i) = value of array[i] pointed. real value!
    single_box = new unsigned int*[9];
}

BaseNineBox::~BaseNineBox()
{
    delete [] single_box;
}

bool BaseNineBox::has_same_value(const unsigned int &box) const
{
    for(unsigned int i=0; i<9; i++)
    {
        if(i!=box && **(single_box+i) == **(single_box+box)) return true;
    }
    return false;
}

bool BaseNineBox::has_same_value() const
{
    for(int i=0; i<8; i++)
    {
        for(int j=i+1; j<9; j++)
        {
            if(**(single_box+i)!=0 && **(single_box+j)!=0
               && **(single_box+i) == **(single_box+j))
               return true;
        }
    }
    return false;
}

void BaseNineBox::print_value() const
{
    for(int i=0; i<9; i++)
    {
        cout << **(single_box+i) << ' ';
    }
    cout << endl;
}


void BaseNineBox::print_all_value() const
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<10; j++)
        {
            cout << *(*(single_box+i) + j) << ' ';
        }
        cout << endl;
    }
    cout << endl;
}


unsigned int * BaseNineBox::get_value_addr(const unsigned int &box) const
{
    return *(single_box+box);
}

unsigned int BaseNineBox::get_value(const  unsigned int &box) const
{
    return *(get_value_addr(box));
}


void BaseNineBox::clean_candidate(const unsigned int &value)
{
    for(int box=0; box<9; box++)
    {
        if(!have_init_value(box)) *(*(single_box+box)+value) = 0;
    }
    return;
}


bool BaseNineBox::find_absolute_value()
{
    int undefined_count = 0;
    int sum = 45;  // 1+2+3+...+9
    int pos = 0;
    for(int box=0; box<9; box++)
    {
        if(!get_value(box))
        {
            undefined_count++;
            pos = box;
        }
        else
        {
            sum -= get_value(box);
        }
    }
    if(undefined_count == 1)
    {
        *(get_value_addr(pos)) = sum;
        new_defined_value(pos);
        return true;
    }
    return false;
}


bool BaseNineBox::have_init_value(const unsigned int &box)
{
    return init_value[box];
}


void BaseNineBox::new_defined_value(const unsigned int &box)
{
    init_value[box] = true;
    return;
}

////////////////////////////////////////////////////////


OneLine::OneLine(const string &s)
{
    string _s("0123456789");
    // 9 box, 10 values per box.
    line = new unsigned int[90];
    unsigned int *box_start = NULL;
    string::size_type s_len = s.size();
    string::size_type safe_len = s_len<=9 ? s_len : 9;
    for(string::size_type i=0; i<safe_len; i++)
    {
        box_start = line+i*10;
        if(s[i]=='0')
        {
            // this box has not defined.
            // fix it with 0~9 ,this nums for candidate.
            for (int j=0; j<10; j++)
            {
                *(box_start+j) = j;
            }
            init_value[i] = false;
        }
        else
        {
            // this box has a initial value.
            // so it's format like this: {VALUE, 0 ,0 ,0 ...}
            *box_start = _s.find(s[i]);
            init_value[i] = true;
        }
        *(single_box+i) = box_start;
    }
}

OneLine::~OneLine()
{
    delete [] line;
}




void OneLine::auto_set_value(const unsigned int &box)
{
    unsigned int * addr = get_value_addr(box);
    for(int i=*addr+1; i<10; i++)
    {
        if(*(addr+i))
        {
            *addr = *(addr+i);
            break;
        }
    }
    return;
}


void OneLine::empty_box_value(const unsigned int &box)
{
    *(get_value_addr(box)) = 0;
    return;
}


bool OneLine::have_defined_new_box()
{
    bool flag = false;
    int count;
    unsigned int candidate_value = 0;
    for(int box=0; box<9; box++)
    {
        count = 0;
        for(int i=1; i<10; i++)
        {
            if(*(get_value_addr(box)+i))
            {
                candidate_value = *(get_value_addr(box)+i);
                count++;
            }
        }
        if(count == 1)
        {
            // new defined value.
            *(get_value_addr(box)) = candidate_value;
            new_defined_value(box);
            flag = true;
        }
    }
    return flag;
}


bool OneLine::already_max(const unsigned &box)
{
    if(!get_value(box)) return false;
    if(have_init_value(box)) return false;
    unsigned int max_num = 0;
    for(int i=9; i>0; i--)
    {
        if(*(get_value_addr(box)+i))
        {
            max_num = *(get_value_addr(box)+i);
            break;
        }
    }
    if(get_value(box) == max_num) return true;
    return false;
}

//////////////////////////////////////////////////////////////////


OneRow::OneRow(OneLine **line, const unsigned int &pos)
{
    for(int i=0; i<9; i++)
    {
        // copy relevant address from line[pos] to row.
        // *(line+i) means the "i" line's value(address)
        *(single_box+i) = (*(line+i))->get_value_addr(pos);
    }
}


/////////////////////////////////////////////////////////////////


SmallBox::SmallBox(OneLine **line, const unsigned int &pos,
                   const unsigned int &line_pos)
{
    unsigned int real_pos = pos == 0 ? 0 : pos == 1 ? 3 : 6;
    unsigned int real_line = line_pos == 0 ? 0 : line_pos == 1 ? 3 : 6;
    for(unsigned int end_line = real_line + 3, n=0; real_line<end_line;
        real_line++)
    {
        for(int j=0; j<3; j++, n++)
        {
            *(single_box+n) = (*(line+real_line))->get_value_addr(real_pos+j);
        }
    }
}
