#include <fstream>
#include <sstream>
#include <iomanip> 
#include "sim.h"
#include "utils.h"

using namespace std;

void ShallowWater::WriteFrame(int outputID){
    // string Path = "./output/"; 
    ostringstream filename_stream;
    filename_stream << "output_" << setw(5) << setfill('0') << outputID;  
    string filename = filename_stream.str(); 

    ofstream outFile(filename, ios::binary);

    // write frame information
    outFile.write(reinterpret_cast<char*>(&t), sizeof(t));
    outFile.write(reinterpret_cast<char*>(&g), sizeof(g));

    // write size of x, y, and ghost
    outFile.write(reinterpret_cast<char*>(&NX), sizeof(NX));
    outFile.write(reinterpret_cast<char*>(&NY), sizeof(NY));
    outFile.write(reinterpret_cast<char*>(&NGhost), sizeof(NGhost));
    
    // write mesh
    outFile.write(reinterpret_cast<char*>(x.data()), NX*sizeof(double));
    outFile.write(reinterpret_cast<char*>(y.data()), NY*sizeof(double));

    // write data
    outFile.write(reinterpret_cast<char*>(h.data()), lenArr*sizeof(double));
    outFile.write(reinterpret_cast<char*>(b.data()), lenArr*sizeof(double));
    outFile.write(reinterpret_cast<char*>(velx.data()), lenArr*sizeof(double));
    outFile.write(reinterpret_cast<char*>(vely.data()), lenArr*sizeof(double));

    outFile.close();
    return; 
}

RandomGenerator::RandomGenerator(double low, double high):
    gen(random_device{}()), dis(low, high) {}

double RandomGenerator::operator()() {
    return dis(gen);
}