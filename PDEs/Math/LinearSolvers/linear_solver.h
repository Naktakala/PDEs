#ifndef LINEAR_SOLVER_BASE_H
#define LINEAR_SOLVER_BASE_H

#include <cstddef>
#include <string>

//########## Forward declarations
namespace pdes::Math
{
  class Vector;
  class Matrix;
  class SparseMatrix;
}

namespace pdes::Math::LinearSolver
{

/**
 * Available types of linear solvers.
 */
enum class LinearSolverType
{
  LU            = 0,
  CHOLESKY      = 1,
  JACOBI        = 2,
  GAUSS_SEIDEL  = 3,
  SOR           = 4,
  SSOR          = 5,
  CG            = 6
};


/**
 * Struct for linear solver options.
 */
struct Options
{
  bool verbose = false;
  double tolerance = 1.0e-8;
  size_t max_iterations = 1000;
  double omega = 1.5;
};


/**
 * Base class from which all linear solvers must derive.
 */
class LinearSolverBase
{
public:
  virtual void
  solve(Vector& x, const Vector& b) const = 0;

  Vector
  solve(const Vector& b) const;
};


/**
 * Base class for iterative solvers.
 */
class IterativeSolverBase : public LinearSolverBase
{
protected:
  const std::string solver_name;
  bool verbose;

protected:
  const SparseMatrix& A;

  double tolerance;
  size_t max_iterations;

public:
  /**
   * Default constructor using Options struct to set parameters.
   *
   * \note The Options struct contains all parameters necessary for all
   *       implemented iterative solvers. Each derived class should set
   *       any and all appropriate parameters from this.
   */
  IterativeSolverBase(const SparseMatrix& A,
                      const Options& opts = Options(),
                      const std::string name = "Undefined");

protected:
  /**
   * Throw an error when convergence criteria is not met.
   */
  void
  throw_convergence_error(const size_t iteration,
                          const double difference) const;
};

}
#endif //LINEAR_SOLVER_BASE_H
