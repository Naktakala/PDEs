#include "sparse_cholesky.h"

#include "vector.h"
#include "Sparse/sparse_matrix.h"

#include "macros.h"

#include <cmath>


using namespace Math;

//################################################## Constructors


LinearSolver::SparseCholesky::
SparseCholesky() : DirectSolverBase<SparseMatrix>()
{}


//################################################## Methods


void
LinearSolver::SparseCholesky::factorize()
{
  size_t n = A.n_rows();

  // Compute the factorization column by column
  for (size_t j = 0; j < n; ++j)
  {
    // Accessor for the diagonal element
    double* d = A.locate(j, j);
    Assert(d && *d != 0.0, "Singular matrix error.");

    // Compute the new diagonal term
    double sum = 0.0;
    for (const auto el : A.const_row(j))
      if (el.column < j)
        sum += el.value * el.value;
    *d = std::sqrt(*d - sum);

    // Set the lower-diagonal components
    for (size_t i = j + 1; i < n; ++i)
    {
      // Go through row i and j, add to sum when columns are equal
      sum = 0.0;
      for (const auto a_ik : A.const_row(i))
        if (a_ik.column < j)
          for (const auto a_jk : A.const_row(j))
            if (a_jk.column == a_ik.column)
              sum += a_ik.value * a_jk.value;

      // Set element i, j
      double* a_ij = A.locate(i, j);
      double value = (a_ij)? (*a_ij - sum) / *d : -sum / *d;
      if (std::fabs(value) != 0.0)
        A.set(i, j, value);
    }
  }
  factorized = true;
}


void
LinearSolver::SparseCholesky::solve(Vector& x, const Vector& b) const
{
  size_t n = A.n_rows();
  Assert(factorized, "Matrix must be factorized before solving.")
  Assert(b.size() == n, "Dimension mismatch error.")
  Assert(x.size() == n, "Dimension mismatch error.")

  //======================================== Forward solve
  for (size_t i = 0; i < n; ++i)
  {
    double value = b[i];
    for (const auto el : A.const_row(i))
      if (el.column < i)
        value -= el.value * x[el.column];
    x[i] = value / *A.diagonal(i);
  }

  //======================================== Backward solve
  for (size_t i = n - 1; i != -1; --i)
  {
    x[i] /= *A.diagonal(i);
    for (const auto a_ij : A.const_row(i))
      if (a_ij.column < i)
        x[a_ij.column] -= a_ij.value * x[i];
  }
}
