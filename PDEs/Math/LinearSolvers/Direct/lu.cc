#include "lu.h"

#include "vector.h"
#include "matrix.h"

#include "macros.h"

#include <cmath>


using namespace Math;

//################################################## Constructors

LinearSolver::LU::
LU(Matrix& A, const bool pivot) :
  A(A), row_pivots(A.n_rows()), pivot_flag(pivot)
{
  Assert(A.n_rows() == A.n_cols(), "Square matrix required.");
  factorize();
}


//################################################## Properties

void
LinearSolver::LU::
pivot(const bool flag)
{ pivot_flag = flag; }


bool
LinearSolver::LU::
pivot() const
{ return pivot_flag; }

//################################################## Methods

void
LinearSolver::LU::
factorize()
{
  size_t n = A.n_rows();

  // Initialize the pivot mappings such that each row maps to itself
  for (size_t i = 0; i < n; ++i)
    row_pivots[i] = i;

  //======================================== Apply Doolittle algorithm
  for (size_t j = 0; j < n; ++j)
  {
    /* Find the row containing the largest magnitude entry for column j.
     * This is only done for the sub-diagonal elements. */
    if (pivot_flag)
    {
      size_t argmax = j;
      double max = std::fabs(A(j, j));
      for (size_t k = j + 1; k < n; ++k)
      {
        const double a_kj = A(k, j);
        if (std::fabs(a_kj) > max)
        {
          argmax = k;
          max = std::fabs(a_kj);
        }
      }

      // If the sub-diagonal is uniformly zero, throw error
      Assert(max != 0.0, "Singular matrix error.");

      /* Swap the current row and the row containing the largest magnitude
       * entry corresponding for the current column. This is done to improve
       * the numerical stability of the algorithm. */
      if (argmax != j)
      {
        std::swap(row_pivots[j], row_pivots[argmax]);
        A.swap_row(j, argmax);
      }
    }//if pivoting

    const double* a_j = A.data(j); // accessor for row j
    const double a_jj = a_j[j]; // diagonal element for row j

    // Compute the elements of the LU decomposition.
    for (size_t i = j + 1; i < n; ++i)
    {
      double* a_i = A.data(i); // accessor for row i
      double& a_ij = a_i[j]; // accessor for element i, j

      /* Lower triangular components. This represents the row operations
       * performed to attain the upper-triangular, row-echelon matrix. */
      a_ij /= a_jj;

      a_i += j + 1; // increment to the correct element

      /* Upper triangular components. This represents the row-echelon form of
       * the original matrix. */
      for (size_t k = j + 1; k < n; ++k)
        *a_i++ -= a_ij * a_j[k];
    }
  }
  factorized = true;
}


void
LinearSolver::LU::
solve(Vector& x, const Vector& b) const
{
  size_t n = A.n_rows();
  Assert(factorized, "Matrix must be factorized before solving.");
  Assert(n == b.size(), "Dimension mismatch error.");
  Assert(n == x.size(), "Dimension mismatch error.");

  //================================================== Forward solve
  for (size_t i = 0; i < n; ++i)
  {
    const double* a_i = A.data(i); // accessor for row i

    double value = b[row_pivots[i]];
    for (size_t j = 0; j < i; ++j)
      value -= *a_i++ * x[j];
    x[i] = value;
  }

  //================================================== Backward solve
  for (size_t i = n - 1; i != -1; --i)
  {
    const double* a_i = A.data(i); // accessor for row i
    const double a_ii = a_i[i]; // diagonal element value.
    a_i += i + 1; // increment to first element after diagonal

    double value = x[i];
    for (size_t j = i + 1; j < n; ++j)
      value -= *a_i++ * x[j];
    x[i] = value / a_ii;
  }
}
