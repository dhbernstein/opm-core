/*
  Copyright 2012 SINTEF ICT, Applied Mathematics.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPM_TRANSPORTSOLVERTWOPHASEREORDER_HEADER_INCLUDED
#define OPM_TRANSPORTSOLVERTWOPHASEREORDER_HEADER_INCLUDED

#include <opm/core/transport/reorder/ReorderSolverInterface.hpp>
#include <opm/core/transport/TransportSolverTwophaseInterface.hpp>
#include <vector>
#include <map>
#include <ostream>
struct UnstructuredGrid;

namespace Opm
{

    class IncompPropertiesInterface;

    /// Implements a reordering transport solver for incompressible two-phase flow.
    class TransportSolverTwophaseReorder : public TransportSolverTwophaseInterface, ReorderSolverInterface
    {
    public:
        /// Construct solver.
        /// \param[in] grid      A 2d or 3d grid.
        /// \param[in] props     Rock and fluid properties.
        /// \param[in] gravity   Gravity vector (null for no gravity).
        /// \param[in] tol       Tolerance used in the solver.
        /// \param[in] maxit     Maximum number of non-linear iterations used.
        TransportSolverTwophaseReorder(const UnstructuredGrid& grid,
                                       const Opm::IncompPropertiesInterface& props,
                                       const double* gravity,
                                       const double tol,
                                       const int maxit);

        // Virtual destructor.
        virtual ~TransportSolverTwophaseReorder();

        /// Solve for saturation at next timestep.
        /// Note that this only performs advection by total velocity, and
        /// no gravity segregation.
        /// \param[in]      porevolume   Array of pore volumes.
        /// \param[in]      source       Transport source term. For interpretation see Opm::computeTransportSource().
        /// \param[in]      dt           Time step.
        /// \param[in, out] state        Reservoir state. Calling solve() will read state.faceflux() and
        ///                              read and write state.saturation().
        virtual void solve(const double* porevolume,
                           const double* source,
                           const double dt,
                           TwophaseState& state);

        /// Solve for gravity segregation.
        /// This uses a column-wise nonlinear Gauss-Seidel approach.
        /// It assumes that the grid can be divided into vertical columns
        /// that do not interact with each other (for gravity segregation).
        /// \param[in] porevolume        Array of pore volumes.
        /// \param[in] dt                Time step.
        /// \param[in, out] state        Reservoir state. Calling solveGravity() will read state.faceflux() and
        ///                              read and write state.saturation().
        void solveGravity(const double* porevolume,
                          const double dt,
                          TwophaseState& state);

        //// Return the number of iterations used by the reordering solver.
        //// \return vector of iteration per cell
        const std::vector<int>& getReorderIterations() const;

    private:
        void initGravity(const double* grav);
        void initColumns();
        virtual void solveSingleCell(const int cell);
        virtual void solveMultiCell(const int num_cells, const int* cells);

        void solveSingleCellGravity(const std::vector<int>& cells,
                                    const int pos,
                                    const double* gravflux);
        int solveGravityColumn(const std::vector<int>& cells);
    private:
        const UnstructuredGrid& grid_;
        const IncompPropertiesInterface& props_;
        const double* visc_;
        std::vector<double> smin_;
        std::vector<double> smax_;
        double tol_;
        int maxit_;

        const double* darcyflux_;   // one flux per grid face
        const double* porevolume_;  // one volume per cell
        const double* source_;      // one source per cell
        double dt_;
        std::vector<double> saturation_;        // one per cell, only water saturation!
        std::vector<double> fractionalflow_;  // = m[0]/(m[0] + m[1]) per cell
        std::vector<int> reorder_iterations_;
        //std::vector<double> reorder_fval_;
        // For gravity segregation.
        std::vector<double> gravflux_;
        std::vector<double> mob_;
        std::vector<double> s0_;
        std::vector<std::vector<int> > columns_;

        // Storing the upwind and downwind graphs for experiments.
        std::vector<int> ia_upw_;
        std::vector<int> ja_upw_;
        std::vector<int> ia_downw_;
        std::vector<int> ja_downw_;

        struct Residual;
        double fracFlow(double s, int cell) const;

        struct GravityResidual;
        void mobility(double s, int cell, double* mob) const;
    };

} // namespace Opm

#endif // OPM_TRANSPORTMODELTWOPHASE_HEADER_INCLUDED
