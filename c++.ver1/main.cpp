#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "sudoku.h"

using namespace std;

void print_value(BaseNineBox **line); // print sudodu.

void print_all_value(BaseNineBox **line); // for debug.
//define which 3x3 area that the given line & row box belongs to
unsigned int get_area_index(const int &line, const int &row);
//when input a sudoku, clean candicates
void clean_value(OneLine **line, OneRow **row, SmallBox **smbox);
// This is the recursive function. to find out the final sudodu.
void find_value(OneLine **line, OneRow **row, SmallBox **smbox,
               const unsigned int &line_index,
               const unsigned int &row_index);


/////////////////////////////////////////////////////////////

void print_value(BaseNineBox **line)
{
    for(int i=0; i<9; i++)
    {
        (*(line+i))->print_value();
    }
    cout << endl;
    return;
}

void print_all_value(BaseNineBox **line)
{
    for(int i=0; i<9; i++)
    {
        (*(line+i))->print_all_value();
    }
    cout << endl;
    return;
}

unsigned int get_area_index(const int &line, const int &row)
{
    if(line<3)
    {
        if(row<3) return 0;
        else if(row<6) return 1;
        else return 2;
    }
    else if(line<6)
    {
        if(row<3) return 3;
        else if(row<6) return 4;
        else return 5;
    }
    else
    {
        if(row<3) return 6;
        else if(row<6) return 7;
        else return 8;
    }
}


void clean_value_assistant(BaseNineBox **item)
{
    int this_value, i, j;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            this_value = (*(item+i))->get_value(j);
            if(this_value == 0) continue;
            (*(item+i))->clean_candidate(this_value);
        }
    }
    return;
}


void clean_value(OneLine **line, OneRow **row, SmallBox **smbox)
{
    int i=0;
    bool res1, res2;
    do
    {
        clean_value_assistant((BaseNineBox **)line);
        clean_value_assistant((BaseNineBox **)row);
        clean_value_assistant((BaseNineBox **)smbox);
        for(i=0; i<9; i++)
        {
            res1 = (*(line+i))->have_defined_new_box();
            if(res1) break;
        }
//        for(i=0; i<9; i++)
//        {
//            cout << i;
//            res2 = (*(line+i))->find_absolute_value()
//                    || (*(row+i))->find_absolute_value()
//                    || (*(smbox+i))->find_absolute_value();
//            if(res2) break;
//        }
    }while(res1);

    return;
}

int loop_count = 0;

void find_value(OneLine **line, OneRow **row, SmallBox **smbox,
               const unsigned int &line_index,
               const unsigned int &row_index)
{
    int next_line;
    int next_row;
    int pre_line;
    int pre_row;
    unsigned int var_line = line_index;
    unsigned int var_row = row_index;

    while(var_line<9)
    {
//        cout << "in " << var_line << ',' << var_row << endl;
//        if(var_row > 8)
//        {
//            var_row = 0;
//            var_line++;
//        }
        next_line = var_row == 8 ? var_line+1 : var_line;
        next_row = var_row == 8 ? 0 : var_row+1;
//        cout << "init status " << (*(line+var_line))->have_init_value(var_row) << endl;
        if((*(line+var_line))->have_init_value(var_row))
        {
            var_row++;
//            cout << "var_row++ at " << var_line << ',' << var_row << endl;
                    if(var_row > 8)
        {
            var_row = 0;
            var_line++;
        }
            continue;
        }
        (*(line+var_line))->auto_set_value(var_row);
        if((*(line+var_line))->has_same_value(var_row)
            || (*(row+var_row))->has_same_value(var_line)
            || (*(smbox+get_area_index(var_line, var_row)))->has_same_value())
        {
//            cout << "same" << endl;
            if((*(line+var_line))->already_max(var_row))
            {
                (*(line+var_line))->empty_box_value(var_row);

                pre_line = var_row == 0 ? var_line-1 : var_line;
                pre_row = var_row == 0 ? 8 : var_row-1;
                if(pre_line<0)
                {
                    cout << "you have input a wrong sudoku. exit" << endl;
                    exit(0);
                }
                while((*(line+pre_line))->have_init_value(pre_row)
                      || (*(line+pre_line))->already_max(pre_row))
                    {
                        if((*(line+pre_line))->already_max(pre_row))
                        {
                            (*(line+pre_line))->empty_box_value(pre_row);
                        }
                        pre_row--;
                        if(pre_row < 0)
                        {
                            pre_row = 8;
                            pre_line--;
                        }
                        if(pre_line < 0)
                        {
                            cout << "you have input a wrong sudoku. exit" << endl;
                            exit(0);
                        }
                    }
                var_line = pre_line;
                var_row = pre_row;
//                cout << "back at " << var_line << ',' << var_row << endl;
                continue;
            }
            else continue;
        }
        else
        {
            var_line = next_line;
            var_row = next_row;
                    if(var_row > 8)
        {
            var_row = 0;
            var_line++;
        }
        }
    }
}
////    loop_count++;
////    if(loop_count>2000) return;
//    cout << "in " << line_index << ',' << row_index << endl;
//        print_value((BaseNineBox **)line);
//    if (line_index > 8) return;
//
//    if((*(line+line_index))->have_init_value(row_index))
//    {
////        cout << "move from " << line_index << ',' << row_index << endl;
////        cout << "move to " << next_line << ',' << next_row << endl;
//        find_value(line, row, smbox, next_line, next_row);
//    }
//    else
//    {
//        cout << (*(line+line_index))->get_value(row_index) << endl;
//        if(!(*(line+line_index))->already_max(row_index))
//        {
//            // this box's value not difined, set it.
//            (*(line+line_index))->auto_set_value(row_index);
//            cout << "set value done " << (*(line+line_index))->get_value(row_index) << endl;
//        }
//
//        if((*(line+line_index))->has_same_value(row_index)
//            || (*(row+row_index))->has_same_value(line_index)
//            || (*(smbox+get_area_index(line_index, row_index)))->has_same_value())
//        {
//            // set a wrong value just now. reset it.
//            cout << "same" << endl;
//            if((*(line+line_index))->already_max(row_index))
//            {
//                //roll back
//                cout << "already max" << endl;
//                (*(line+line_index))->empty_box_value(row_index);
//
//                int pre_line = row_index == 0 ? line_index-1 : line_index;
//                int pre_row = row_index == 0 ? 8 : row_index-1;
//                if(pre_line<0)
//                {
//                    cout << "you have input a wrong sudoku. exit" << endl;
//                    exit(0);
//                }
//
//                while((*(line+pre_line))->have_init_value(pre_row)
//                      || (*(line+pre_line))->already_max(pre_row))
//                    {
//                        if((*(line+pre_line))->already_max(pre_row))
//                        {
//                            (*(line+pre_line))->empty_box_value(pre_row);
//                        }
//                        pre_row--;
//                        if(pre_row < 0)
//                        {
//                            pre_row = 8;
//                            pre_line--;
//                        }
//                        if(pre_line < 0)
//                        {
//                            cout << "you have input a wrong sudoku. exit" << endl;
//                            exit(0);
//                        }
//
//
//                    }
//                cout << "back from " << line_index << ',' << row_index << " to " << pre_line << ',' << pre_row << endl;
//                find_value(line, row, smbox, pre_line, pre_row);
//            }
//            else
//            {
//                find_value(line, row, smbox, line_index, row_index);
//            }
//        }
//        else
//        {
//            find_value(line, row, smbox, next_line, next_row);
//        }
//    }
//}



int main()
{
    string s;
    unsigned int i=0, n=0;

    // input 9 lines , to initial a sudoku.
    OneLine **line = new OneLine*[9];

    ifstream f("sudoku.txt");
    if(!f)
    {
        cerr << "open error" << endl;
        exit(1);
    }
    for(i=0; i<9; i++)
    {
        getline(f, s);
        *(line+i) = new OneLine(s);
    }
    f.close();


    // use OneLine initial OneRow.
    OneRow **row = new OneRow*[9];
    for(i=0; i<9; i++)
    {
        *(row+i) = new OneRow(line, i);
    }


    // initial SmallBox
    SmallBox **smbox = new SmallBox*[9];
    for(i=0; i<3; i++)
    {
        for(int j=0; j<3; j++, n++)
        {
            *(smbox+n) = new SmallBox(line, j, i);
        }
    }

//    print_all_value((BaseNineBox **)line);
//    clean_value(line, row, smbox);
//    cout << endl;
    print_value((BaseNineBox **)line);
//    print_all_value((BaseNineBox **)line);
    find_value(line, row, smbox, 0 ,0);
    print_value((BaseNineBox **)line);

    delete [] line;
    delete [] row;
    delete [] smbox;
    return 0;
}
