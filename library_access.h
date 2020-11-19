#ifndef LIBRARY_ACCESS_H
#define LIBRARY_ACCESS_H

// this class is designed to access the visibility parameters from the
// optical libraries, that can then be used to calculate the number of
// photons incident on each PMT

#include <vector>
#include <string>

class LibraryAccess{

  public:

    void LoadLibraryFromFile(std::string libraryfile);
    
    const float* GetCounts(size_t Voxel, int no_pmt);
    const float* GetLibraryEntries(int VoxID, int no_pmt);
    
    int GetVoxelID(double* Position);
    
    float PhotonLibraryAnalyzer(int _pmt_number, int _voxel);
    
    LibraryAccess();

  private:
    
    // table to store library visibilities
    std::vector<std::vector<float> > table_;

    // microboone voxelisation scheme
    const double gLowerCorner[3] = {-64.825, -193, -127.24};
    const double gUpperCorner[3] = {321.175, 193, 1164.24};

    const int gxSteps = 75;
    const int gySteps = 75;
    const int gzSteps = 400;

};

#endif
