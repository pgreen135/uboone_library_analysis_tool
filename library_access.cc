#include "library_access.h"

#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"

// constructor
LibraryAccess::LibraryAccess()
	: table_(std::vector<std::vector<float> >())
{

}

// function to load optical library
void LibraryAccess::LoadLibraryFromFile(std::string libraryfile)
{
	std::cout << "Reading photon library from input file: " << libraryfile << std::endl;

	TFile *f = nullptr;
	TTree *tt = nullptr;

	try
	{
		f  =  TFile::Open(libraryfile.c_str());
		tt =  (TTree*)f->Get("PhotonLibraryData");

		if (!tt) {

			TKey *key = f->FindKeyAny("PhotonLibraryData");
			if (key)
				tt = (TTree*)key->ReadObj();
			else {
				std::cout << "PhotonLibraryData not found in file: " << libraryfile << std::endl;
			}
		}
	}
	catch(...)
	{
		std::cout << "Error in ttree load, reading photon library: " << libraryfile << std::endl;
	}

	int voxel;
	int opChannel;
	float visibility;
	int maxvoxel = tt->GetMaximum("Voxel")+1;
	int maxopChannel = tt->GetMaximum("OpChannel")+2;

	table_.resize(maxvoxel, std::vector<float>(maxopChannel, 0));
	
	tt->SetBranchAddress("Voxel",      &voxel);
	tt->SetBranchAddress("OpChannel",  &opChannel);
	tt->SetBranchAddress("Visibility", &visibility);

	size_t nentries = tt->GetEntries();

	for(size_t i=0; i!=nentries; ++i)
	{
		tt->GetEntry(i);
		if((voxel<0)||(voxel>= maxvoxel)||(opChannel<0)||(opChannel>= maxopChannel))
		{}
		else
		{
			table_.at(voxel).at(opChannel) = visibility;
		}
	}

	try
	{
		f->Close();
	}
	catch(...)
	{
		std::cout << "Error in closing file : " << libraryfile << std::endl;
	}
}

const float* LibraryAccess::GetCounts(size_t voxel, int no_pmt)
{
	return &table_.at(voxel).at(no_pmt);
}

const float* LibraryAccess::GetLibraryEntries(int voxID, int no_pmt)
{
	return GetCounts(voxID, no_pmt);
}

// GetVoxelID
int LibraryAccess::GetVoxelID(double* Position) //const
{
  // figure out how many steps this point is in the x,y,z directions                                                                                                              
  int xStep = int ((Position[0]-gLowerCorner[0]) / (gUpperCorner[0]-gLowerCorner[0]) * gxSteps );
  int yStep = int ((Position[1]-gLowerCorner[1]) / (gUpperCorner[1]-gLowerCorner[1]) * gySteps );
  int zStep = int ((Position[2]-gLowerCorner[2]) / (gUpperCorner[2]-gLowerCorner[2]) * gzSteps );
 
  int ID;
 
  // check if point lies within the voxelized region                                                                                                                              
  if((0 <= xStep) && (xStep < gxSteps) &&
     (0 <= yStep) && (yStep < gySteps) &&
     (0 <= zStep) && (zStep < gzSteps) )
    {
      // if within bounds, generate the voxel ID                                                                                                                                  
      ID = xStep
    + yStep * (gxSteps)
    + zStep * (gxSteps * gySteps);
    }
  else
    {
      // if out of bounds, print warning and return -1                                                                                                                            
      ID = -1;
    }
 
  return ID;
 
}

// function to return visibility of voxel-pmt combination
float LibraryAccess::PhotonLibraryAnalyzer(int _pmt_number, int _voxel)
{
  	//Look up visibility parameter by comparing the optical channel (PMT Number)
  	//and the detector location (voxel, i)
	const float *Visibilities = GetLibraryEntries(_voxel, _pmt_number);
	const float vis = *Visibilities;

	return vis;
}
