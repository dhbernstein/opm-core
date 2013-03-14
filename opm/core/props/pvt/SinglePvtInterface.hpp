/*
  Copyright 2010, 2011, 2012 SINTEF ICT, Applied Mathematics.

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

#ifndef OPM_SINGLEPVTINTERFACE_HEADER_INCLUDED
#define OPM_SINGLEPVTINTERFACE_HEADER_INCLUDED


#include <opm/core/props/BlackoilPhases.hpp>


namespace Opm
{

    class SinglePvtInterface : public BlackoilPhases
    {
    public:
        SinglePvtInterface();

        virtual ~SinglePvtInterface();

        /// \param[in]  num_phases   The number of active phases.
        /// \param[in]  phase_pos    Array of BlackpoilPhases::MaxNumPhases
        ///                          integers, giving the relative
        ///                          positions of the three canonical
        ///                          phases A, L, V in order to handle
        ///                          arbitrary two-phase and three-phase situations.
        void setPhaseConfiguration(const int num_phases, const int* phase_pos);

        /// For all the virtual methods, the following apply: p and z
        /// are expected to be of size n and n*num_phases, respectively.
        /// Output arrays shall be of size n, and must be valid before
        /// calling the method.

        /// Viscosity as a function of p and z.
        virtual void mu(const int n,
                        const double* p,
                        const double* z,
                        double* output_mu) const = 0;

        /// Formation volume factor as a function of p and z.
        virtual void B(const int n,
                       const double* p,
                       const double* z,
                       double* output_B) const = 0;

        /// Formation volume factor and p-derivative as functions of p and z.
        virtual void dBdp(const int n,
                          const double* p,
                          const double* z,
                          double* output_B,
                          double* output_dBdp) const = 0;

        /// Solution factor as a function of p and z.
        virtual void R(const int n,
                       const double* p,
                       const double* z,
                       double* output_R) const = 0;

        /// Solution factor and p-derivative as functions of p and z.
        virtual void dRdp(const int n,
                          const double* p,
                          const double* z,
                          double* output_R,
                          double* output_dRdp) const = 0;
    protected:
        int num_phases_;
        int phase_pos_[MaxNumPhases];
    };

} // namespace Opm

#endif // OPM_SINGLEPVTINTERFACE_HEADER_INCLUDED

