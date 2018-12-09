#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

extern "C"
{
    #include "cs.h"
}
#include <vector>
#include <ctime>
#include <complex>
using namespace std;

class SPARSE_MATRIX
{
public:
    SPARSE_MATRIX();
    virtual ~SPARSE_MATRIX();

    bool LU_factorization_is_performed() const;
    void update_clock_when_matrix_is_changed();
    void update_clock_when_LU_factorization_is_performed();
    clock_t get_clock_when_matrix_is_changed() const;
    clock_t get_clock_when_LU_factorization_is_performed() const;

    void add_entry(int row, int col, double value);
    bool matrix_in_compressed_column_form()  const;

    complex<double> get_entry_value(int row, int col)  const;
    complex<double> get_entry_value(int index)  const;

    void change_entry_value(int row, int col, double value);
    void change_entry_value(int row, int col, complex<double> value);
    void change_entry_value(int index, double value);
    void change_entry_value(int index, complex<double> value);

    int get_column_number_of_entry_index(int index)  const;

    vector<double> solve_Ax_eq_b(vector<double> b);


    virtual void add_entry(int row, int col, complex<double> value) = 0;

    virtual void convert_to_triplet_form() = 0;

    virtual bool matrix_in_triplet_form()  const = 0;

    virtual void compress_and_merge_duplicate_entries() = 0;

    virtual void transpose() = 0;

    virtual int get_matrix_size()  const = 0;
    virtual int get_matrix_entry_count()  const = 0;
    virtual int get_starting_index_of_column(int col)  const = 0;
    virtual int get_row_number_of_entry_index(int index)  const = 0;

    virtual int    get_entry_index(int row, int col)  const = 0;

    virtual double get_real_entry_value(int index)  const = 0;
    virtual double get_imag_entry_value(int index)  const = 0;

    virtual void change_real_entry_value(int index, double value) = 0;
    virtual void change_imag_entry_value(int index, double value) = 0;

    virtual void clear() = 0;

    virtual vector<size_t> get_reorder_permutation() = 0;

    virtual void LU_factorization(int order=1, double tolerance = 1e-6) = 0;

    virtual void solve_Lx_eq_b(vector<double>& b) = 0;
    virtual void solve_xU_eq_b(vector<double>& b) = 0;

    virtual void report_brief()  const = 0;
    virtual void report_full()  const = 0;
    virtual void save_matrix_to_file(string filename)  const = 0;
private:
    clock_t clock_when_matrix_is_changed;
    clock_t clock_when_LU_factorization_is_performed;
};
#endif // SPARSE_MATRIX_H
