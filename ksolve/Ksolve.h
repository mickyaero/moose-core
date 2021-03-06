/**********************************************************************
** This program is part of 'MOOSE', the
** Messaging Object Oriented Simulation Environment.
**           Copyright (C) 2003-2014 Upinder S. Bhalla. and NCBS
** It is made available under the terms of the
** GNU Lesser General Public License version 2.1
** See the file COPYING.LIB for the full notice.
**********************************************************************/

#ifndef _KSOLVE_H
#define _KSOLVE_H

class Stoich;

class Ksolve: public ZombiePoolInterface
{
public:
    Ksolve();
    ~Ksolve();

    //////////////////////////////////////////////////////////////////
    // Field assignment stuff
    //////////////////////////////////////////////////////////////////
    /// Assigns integration method
    string getMethod() const;
    void setMethod( string method );

    /// Assigns Absolute tolerance for integration
    double getEpsAbs() const;
    void setEpsAbs( double val );

    /// Assigns Relative tolerance for integration
    double getEpsRel() const;
    void setEpsRel( double val );

    /// Assigns Stoich object to Ksolve.
    Id getStoich() const;
    void setStoich( Id stoich ); /// Inherited from ZombiePoolInterface.

    /// Assigns Dsolve object to Ksolve.
    Id getDsolve() const;
    void setDsolve( Id dsolve ); /// Inherited from ZombiePoolInterface.

    unsigned int getNumLocalVoxels() const;
    unsigned int getNumAllVoxels() const;
    /**
     * Assigns the number of voxels used in the entire reac-diff
     * system. Note that fewer than this may be used on any given node.
     */
    void setNumAllVoxels( unsigned int num );

    /// Returns the vector of pool Num at the specified voxel.
    vector< double > getNvec( unsigned int voxel) const;
    void setNvec( unsigned int voxel, vector< double > vec );

    /**
     * This does a quick and dirty estimate of the timestep suitable
     * for this sytem
     */
    double getEstimatedDt() const;

    //////////////////////////////////////////////////////////////////
    // Dest Finfos
    //////////////////////////////////////////////////////////////////
    void process( const Eref& e, ProcPtr p );
    void reinit( const Eref& e, ProcPtr p );
    void initProc( const Eref& e, ProcPtr p );
    void initReinit( const Eref& e, ProcPtr p );
    /**
     * Handles request to change volumes of voxels in this Ksolve, and
     * all cascading effects of this. At this point it won't handle
     * change in size of voxel array.
     */
    void updateVoxelVol( vector< double > vols );
    //////////////////////////////////////////////////////////////////
    // Utility for SrcFinfo
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    // Solver interface functions
    //////////////////////////////////////////////////////////////////
    unsigned int getPoolIndex( const Eref& e ) const;
    unsigned int getVoxelIndex( const Eref& e ) const;

    //////////////////////////////////////////////////////////////////
    // ZombiePoolInterface inherited functions
    //////////////////////////////////////////////////////////////////

    void setN( const Eref& e, double v );
    double getN( const Eref& e ) const;
    void setNinit( const Eref& e, double v );
    double getNinit( const Eref& e ) const;
    void setDiffConst( const Eref& e, double v );
    double getDiffConst( const Eref& e ) const;

    /**
     * Assigns number of different pools (chemical species) present in
     * each voxel.
     * Inherited.
     */
    void setNumPools( unsigned int num );
    unsigned int getNumPools() const;
    VoxelPoolsBase* pools( unsigned int i );
    double volume( unsigned int i ) const;

    void getBlock( vector< double >& values ) const;
    void setBlock( const vector< double >& values );

    void matchJunctionVols( vector< double >& vols, Id otherCompt )
    const;

    /**
     * Rescale specified voxel rate term following rate constant change
     * or volume change. If index == ~0U then does all terms.
     */
    void updateRateTerms( unsigned int index );

    //////////////////////////////////////////////////////////////////
    // Functions for cross-compartment transfer
    //////////////////////////////////////////////////////////////////
    void setupXfer( Id myKsolve, Id otherKsolve,
                    unsigned int numProxyMols,
                    const vector< VoxelJunction >& vj );

    void assignXferIndex( unsigned int numProxyMols,
                          unsigned int xferCompt,
                          const vector< vector< unsigned int > >& voxy );

    void assignXferVoxels( unsigned int xferCompt );

    unsigned int assignProxyPools( const map< Id, vector< Id > >& xr,
                                   Id myKsolve, Id otherKsolve, Id otherComptId );

    void buildCrossReacVolScaling( Id otherKsolve,
                                   const vector< VoxelJunction >& vj );
    //////////////////////////////////////////////////////////////////
    // for debugging
    void print() const;

    //////////////////////////////////////////////////////////////////
    static SrcFinfo2< Id, vector< double > >* xComptOut();
    static const Cinfo* initCinfo();

private:

    string method_;
    double epsAbs_;
    double epsRel_;
    /**
     * Each VoxelPools entry handles all the pools in a single voxel.
     * Each entry knows how to update itself in order to complete
     * the kinetic calculations for that voxel. The ksolver does
     * multinode management by indexing only the subset of entries
     * present on this node.
     */
    vector< VoxelPools > voxelPools_;

    /// First voxel indexed on the current node.
    unsigned int startVoxel_;

    /// Utility ptr used to help Pool Id lookups by the Ksolve.
    Stoich* stoichPtr_;

    /**
     * Id of diffusion solver, needed for coordinating numerics.
     */
    Id dsolve_;

    /// Pointer to diffusion solver
    ZombiePoolInterface* dsolvePtr_;
};

#endif	// _KSOLVE_H
