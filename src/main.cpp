#include <iostream>  
#include <iomanip>
#include <cmath> 

#include "config.h"
#include "sim.h"
#include "setup.h"

using namespace std;

int main(int argc, char* argv[]){
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
    // cout<<"check1"<<endl; 
    Setup(sim); 
    // cout<<"check2"<<endl; 
    double drmin = min(sim.dx, sim.dy); 
    // cout<<"check3"<<endl; 

    double tNextOutput = 0; 
    int outputID = 0; 
    int doOutput = 0; 

    sim.WriteFrame(outputID); 
    tNextOutput = sim.t + dtoutput; 
    outputID++; 

    double dt = 0; 
    int step = 0; 
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
            cout << right
                 << "Output frame: " << setw(8) << outputID
                 << " , t = "     << setw(8) << fixed << setprecision(3) << sim.t
                 << " , total steps = " << setw(6) << step
                 << "\n-----------------------------------------------------------"
                 << endl;
            doOutput = 0; 
        }
    }
}