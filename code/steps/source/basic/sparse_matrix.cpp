#include "header/basic/sparse_matrix.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;

SPARSE_MATRIX::SPARSE_MATRIX()
{
    //constructor
    update_clock_when_LU_factorization_is_performed();
    update_clock_when_matrix_is_changed();
}

SPARSE_MATRIX::~SPARSE_MATRIX()
{
    // destructor
}


bool SPARSE_MATRIX::matrix_in_compressed_column_form() const
{
    return not matrix_in_triplet_form();
}

void SPARSE_MATRIX::add_entry(int row, int col, double value)
{
    add_entry(row, col, complex<double>(value,0.0));
}

int SPARSE_MATRIX::get_column_number_of_entry_index(int index) const
{
    int n = get_matrix_size();
    if(index<=get_starting_index_of_column(n))
    {
        int k_start, k_end;
        for(int col = 0; col!=n; ++col)
        {
            k_start = get_starting_index_of_column(col);
            k_end = get_starting_index_of_column(col+1)-1;
            if(index>=k_start and index<=k_end)
                return col;
        }
        return INDEX_NOT_EXIST;
    }
    else
        return INDEX_NOT_EXIST;
}

complex<double> SPARSE_MATRIX::get_entry_value(int row, int col) const
{
    return get_entry_value(get_entry_index(row, col));
}

complex<double> SPARSE_MATRIX::get_entry_value(int index) const
{
    //return complex<double>(get_real_entry_value(index), get_imag_entry_value(index));
    return get_complex_entry_value(index);
}

void SPARSE_MATRIX::change_entry_value(int row, int col, double value)
{
    change_entry_value(row, col, complex<double>(value, 0.0));
}

void SPARSE_MATRIX::change_entry_value(int index, double value)
{
    change_entry_value(index, complex<double>(value, 0.0));
}

void SPARSE_MATRIX::change_entry_value(int row, int col, const complex<double>& value)
{
    // get entry index of a compress matrix
    int index = get_entry_index(row, col); // if it is triplet format, will be converted into compressed format
    change_entry_value(index, value);
}

void SPARSE_MATRIX::change_entry_value(int index, const complex<double>& value)
{
    if(index != INDEX_NOT_EXIST)
    {
        change_real_entry_value(index, value.real());
        change_imag_entry_value(index, value.imag());
    }
}


bool SPARSE_MATRIX::LU_factorization_is_performed() const
{
    if(get_clock_when_LU_factorization_is_performed() > get_clock_when_matrix_is_changed())
        return true;
    else
        return false;
}
void SPARSE_MATRIX::update_clock_when_matrix_is_changed()
{
    clock_when_matrix_is_changed = clock();
}

void SPARSE_MATRIX::update_clock_when_LU_factorization_is_performed()
{
    clock_when_LU_factorization_is_performed = clock();
}

clock_t SPARSE_MATRIX::get_clock_when_matrix_is_changed() const
{
    return clock_when_matrix_is_changed;
}

clock_t SPARSE_MATRIX::get_clock_when_LU_factorization_is_performed() const
{
    return clock_when_LU_factorization_is_performed;
}
