/***
 *       Filename:  CudaKsolve.cu
 *
 *    Description:  CUDA version of Ksolve.
 *
 *        Version:  0.0.1
 *        Created:  2017-06-25
 *       Revision:  none
 *
 *         Author:  Micky Droch <mickydroch@gmail.com>
 *   Organization:  IIT Bombay
 *
 *        License:  GNU GPL3
 *
 *
 *        TODO: Needs to figure out a better location for this function.
 */

#ifdef USE_CUDA

#include <stdio.h>
#include "CudaKsolve.h"

#include "../basecode/header.h"
#include "../ksolve/VoxelPools.h"
#include "../ksolve/RateTerm.h"
#include "../ksolve/BoostSys.h"

inline void callMe( )
{
    printf( "Calling me\n" );
}

inline void cuda_ksolve( double* dy, double* y, const double currentTime, const double time, size_t n )
{

}

void voxelPoolToCudaOdeSystem( VoxelPools & pool, CudaOdeSystem* pOde )
{
    // Get the Stoich first. It contains matrices we need.
    vector< double > yvec = pool.SInitVec( );

    pOde->dimension = yvec.size( );

    cout << "Volumne " << pool.getVolume( ) << endl;
    pOde->f = pool.varS( );

    BoostSys bs = pool.sys_;

#if 0
    vector<RateTerm*> vecRates = pool.getRateTerms( );
    for( auto r : vecRates )
        cout << r->getR1( ) << " " << r->getR2( ) << endl;
#endif

    //cout << "Total rate terms " << vecRates.size( ) << endl;
}


#endif
