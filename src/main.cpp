#include <iostream>  
#include <iomanip>
#include <cmath> 
#include <chrono>
#include <ctime>

#include "config.h"
#include "sim.h"
#include "setup.h"

using namespace std;

int main(int argc, char* argv[]){
    auto time0 = chrono::high_resolution_clock::now();
    clock_t cputime0 = clock();

    if (argc < 3 || std::string(argv[1]) != "-i") {
        cerr << "Usage: " << argv[0] << " -i input.in" << endl;
        return 1;
    }
    Config::getInstance().loadFromFile(argv[2]);

    double dtoutput = Config::getInstance().get("dtoutput"); 
    double tmax = Config::getInstance().get("tmax"); 
    double CFL = Config::getInstance().get("CFL"); 
    int progressInfo = Config::getInstance().get("progressInfo"); 

    ShallowWater sim; 
    Setup(sim); 
    double drmin = min(sim.dx, sim.dy); 

    double tNextOutput = 0; 
    int outputID = 0; 
    int doOutput = 0; 

    sim.WriteFrame(outputID); 
    tNextOutput = sim.t + dtoutput; 
    outputID++; 

    double dt = 0; 
    int step = 0; 
    auto time1 = chrono::high_resolution_clock::now();
    auto time2 = chrono::high_resolution_clock::now();
    clock_t cputime1 = clock();
    clock_t cputime2 = clock();
    double walltime  = 0; 
    double cputime = 0; 
    while(sim.t<=tmax){
        step ++; 
        dt = CFL*drmin/sim.cmax; 
        if (dt<1e-8){ 
            cout << right
                 << "error: dt is too small" 
                 << " , dt = "     << setw(10) << fixed << setprecision(6) << dt
                 <<" , and vmag is "<<sim.cmax 
                 << " program quit. "
                 << "\n-----------------------------------------------------------"
                 << endl;
            doOutput = 0; 
            break; 
        }
        if (sim.t+dt >= tNextOutput) {
            dt = tNextOutput - sim.t; 
            doOutput = 1; 
        }
        sim.UpdateU(dt); 

        if (progressInfo == 1){
            cout    << right
                    << "Progress: "
                    << "total steps = " << setw(6) << step
                    << " , t = "     << setw(8) << fixed << setprecision(3) << sim.t
                    << " , dt = "     << setw(10) << fixed << setprecision(6) << dt
                    << " , cmax = " << setw(10) << setprecision(6) << sim.cmax
                    << " , CFL = "  << setw(10) << setprecision(6) << sim.cmax * dt / drmin
                    << endl;
        }

        if (doOutput == 1){
            sim.WriteFrame(outputID); 
            tNextOutput += dtoutput; 
            outputID++; 
            time2 = chrono::high_resolution_clock::now();
            cputime2 = clock(); 
            walltime = chrono::duration<double>(time2 - time1).count();
            cputime = double(cputime2 - cputime1) / CLOCKS_PER_SEC;
            time1 = time2; 
            cputime1 = cputime2; 
            cout << right
                 << "Output frame: " << setw(8) << outputID
                 << "\n        t = "     << setw(8) << fixed << setprecision(3) << sim.t
                 << " , total steps = " << setw(6) << step
                 << "\n        step walltime = " << setw(8) << fixed << setprecision(3) << walltime
                 << " s,  cputime = "     << setw(8) << fixed << setprecision(3) << cputime << " s"
                 << "\n-----------------------------------------------------------"
                 << endl;
            doOutput = 0; 
        }
    }
    walltime = chrono::duration<double>(time2 - time0).count(); 
    cputime = double(cputime2 - cputime0) / CLOCKS_PER_SEC; 
    cout << right
            << "Finish! " 
            << "\n    total steps = " << setw(6) << step
            << " , \n    walltime = " << setw(8) << fixed << setprecision(3) << walltime
            << " s, \n    cputime = " << setw(8) << fixed << setprecision(3) << cputime << " s"
            << "\n-----------------------------------------------------------"
            << endl;
}