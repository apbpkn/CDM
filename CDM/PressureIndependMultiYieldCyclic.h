// $Revision: 1.20 $
// $Date: 2009-01-16 19:40:36 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/soil/PressureIndependMultiYieldCyclic.h,v $
                                                                        
// Written: ZHY
// Created: August 2000

// Description: This file contains the class prototype for PressureIndependMultiYieldCyclic.
//
// What: "@(#) PressureIndependMultiYieldCyclic.h, revA"

#ifndef PressureIndependMultiYieldCyclic_h
#define PressureIndependMultiYieldCyclic_h

#include <NDMaterial.h>
#include <MultiYieldSurface.h>
#include <Matrix.h>

class PressureIndependMultiYieldCyclic : public NDMaterial
{
public:
     // Initialization constructor
     PressureIndependMultiYieldCyclic (int tag, 
		 int nd,
		 double rho,
		 double refShearModul,
		 double refBulkModul,
		 double cohesi,
		 double peakShearStra,
		 double residualCohesion,
		 double SofteningNum1,
		 double SofteningNum2,
		 double SofteningNum3,
		 double SofteningNum4,
		 double Cvalue,
		 double frictionAng = 0.,
		 double residualFriction = 0.,
		 double refPress = 100,
		 double pressDependCoe = 0.0,
		 int   numberOfYieldSurf = 20,
		 double * gredu = 0);

     // Default constructor
     PressureIndependMultiYieldCyclic ();

     // Copy constructor
     PressureIndependMultiYieldCyclic (const PressureIndependMultiYieldCyclic &);

     // Destructor: clean up memory storage space.
     virtual ~PressureIndependMultiYieldCyclic ();

     const char *getClassType(void) const {return "PressureIndependMultiYieldCyclic";};

     double getRho(void) {return rhox[matN];} ;

     // Sets the values of the trial strain tensor.
     int setTrialStrain (const Vector &strain);

     // Sets the values of the trial strain and strain rate tensors.
     int setTrialStrain(const Vector &v, const Vector &r);

     int setTrialStrainIncr(const Vector &v);
     int setTrialStrainIncr(const Vector &v, const Vector &r);

     // Calculates current tangent stiffness.
     const Matrix &getTangent (void);
     const Matrix &getInitialTangent (void);
        
     void getBackbone (Matrix &);

     // Calculates the corresponding stress increment (rate), for a given strain increment. 
     const Vector &getStress (void);
     const Vector &getStrain (void);
     const Vector &getCommittedStress (void);
     const Vector &getStressToRecord (int numOutput); // Added by Alborz Ghofrani - UW
     const Vector &getCommittedStrain (void);

     // Accepts the current trial strain values as being on the solution path, and updates 
     // all model parameters related to stress/strain states. Return 0 on success.
     int commitState (void);

     // Revert the stress/strain states to the last committed states. Return 0 on success.
     int revertToLastCommit (void);

     int revertToStart(void) {return 0;}

     // Return an exact copy of itself.
     NDMaterial *getCopy (void);

     // Return a copy of itself if "code"="PressureIndependMultiYieldCyclic", otherwise return null.
     NDMaterial *getCopy (const char *code);

     // Return the string "PressureIndependMultiYieldCyclic".
     const char *getType (void) const ;

     // Return ndm.
     int getOrder (void) const ;

     int sendSelf(int commitTag, Channel &theChannel);  
     int recvSelf(int commitTag, Channel &theChannel, 
		  FEM_ObjectBroker &theBroker);    

     Response *setResponse (const char **argv, int argc, OPS_Stream &s);
     int getResponse (int responseID, Information &matInformation);
     void Print(OPS_Stream &s, int flag =0);

     //void setCurrentStress(const Vector stress) { currentStress=T2Vector(stress); }
     int setParameter(const char **argv, int argc, Parameter &param);
     int updateParameter(int responseID, Information &eleInformation);	

    // RWB; PyLiq1 & TzLiq1 need to see the excess pore pressure and initial stresses.
    friend class PyLiq1;
    friend class TzLiq1;

protected:

private:

	static int matCount;
	static int* loadStagex;  //=0 if elastic; =1 if plastic

  // user supplied
	static int* ndmx;  //num of dimensions (2 or 3)
	static double* rhox;
	static double* frictionAnglex;
	static double* peakShearStrainx;
	static double* refPressurex;
	static double* cohesionx;
	static double* pressDependCoeffx;
	static int*    numOfSurfacesx;
	static double* residualCohesionx;
	static double* residualFrictionx;
	static double* SofteningNum1x;
	static double* SofteningNum2x;
	static double* SofteningNum3x;
	static double* SofteningNum4x;
	static double* Cvaluex;
	static T2Vector workT2V;
	

	// internal
	static double* residualPressx;
	static Matrix theTangent;  //classwise member
	int e2p;
	int matN;
	double refShearModulus;
	double refBulkModulus;
	MultiYieldSurface * theSurfaces; // NOTE: surfaces[0] is not used  
	MultiYieldSurface * committedSurfaces;  
	int    activeSurfaceNum;  
	int    committedActiveSurf;
	T2Vector currentStress;
	T2Vector trialStress;
	T2Vector currentStrain;
	T2Vector strainRate;
	static T2Vector subStrainRate;
    double * mGredu;
	T2Vector updatedTrialStress;

	double    CycD;

	double PeakOctStress;
	double refShearModulus_1;
	double refBulkModulus_1;
	double cohesiRes;
	double ru;
	double Cru;
	double Cmu;
	double mu;
	double kappaL;
	double kappa;
	double LL;
	double SL1;
	double SL2;
	double SL3;
	double SL4;
	double SL5;
	double SL6;
	static Vector    strainL;
	void elast2Plast(void);
	// Called by constructor
	void setUpSurfaces(double *);  
	void StrainSoftening(void);
	double yieldFunc(const T2Vector & stress, const MultiYieldSurface * surfaces, 
			 int surface_num);

	void deviatorScaling(T2Vector & stress, const MultiYieldSurface * surfaces, 
			     int surfaceNum, int count=0);

	void initSurfaceUpdate(void);

	void paramScaling(void);

	// Return num_strain_subincre
	int setSubStrainRate(void);

	int isLoadReversal(void);

	void getContactStress(T2Vector &contactStress);

	void getSurfaceNormal(const T2Vector & stress, Vector &surfaceNormal); 

	void setTrialStress(T2Vector & stress); 

	double getLoadingFunc(const T2Vector & contact, 
			      const Vector & surfaceNormal,int crossedSurface);

	void stressCorrection(int crossedSurface);
	
	void updateActiveSurface(void);
	
	void updateInnerSurface(void);

	// Return 1 if crossing the active surface; return 0 o/w
	int  isCrossingNextSurface(void);  

};

#endif







