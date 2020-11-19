// example driver
// loads the uboone photon library
// accesses the pmt visibilities for scintillation occurring at a given x,y,z position

// note: requires local copy of photon library to run

#include "library_access.h"

#include <iostream>
#include <fstream>
#include <cmath>

int main(){
	
	// load optical library
	LibraryAccess optical_library;
    //optical_library.LoadLibraryFromFile("uboone_photon_library_v5.root");
   	optical_library.LoadLibraryFromFile("uboone_photon_library_v6_70kV.root");

    // energy deposition
    double E = 25; // MeV
    double x = 100; double y = 0; double z = 450; // cm

    // uboone light properties
    double scintillation_yield = 29000;	// photons per MeV energy deposited
    double global_qe = 0.0093;	// global QE factor

    // calculate voxel
   	double position[3] {x, y, z};
	int voxel_number = optical_library.GetVoxelID(position);

	// loop over pmts
    for (int pmt = 0; pmt < 32; pmt++) {

    	if (pmt == 17) continue;	// skip broken pmt

        // get visibility
    	double visibility = optical_library.PhotonLibraryAnalyzer(pmt, voxel_number);

    	// get number photons
    	int n = std::round(E * scintillation_yield * global_qe * visibility);

    	std::cout << "PMT " << pmt << ": n = " << n << std::endl;

    }

    return 0;
}