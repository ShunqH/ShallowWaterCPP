// #include <vector>
#include <cmath> 
#include "sim.h" 
#include "config.h"

using namespace std; 

ShallowWater::ShallowWater(){
    g = Config::getInstance().get("g"); 
    H = Config::getInstance().get("H"); 
    bmax = Config::getInstance().get("bmax"); 
    bmin = Config::getInstance().get("bmin"); 
    NX = Config::getInstance().get("NX");
    NY = Config::getInstance().get("NY"); 
    NGhost = Config::getInstance().get("NGhost"); 
    xmin = Config::getInstance().get("xmin"); 
    xmax = Config::getInstance().get("xmax"); 
    ymin = Config::getInstance().get("ymin"); 
    ymax = Config::getInstance().get("ymax"); 
    spongeArea = Config::getInstance().get("sponge"); 
    LSponge = Config::getInstance().get("L_sponge"); 
    sigmaMax = Config::getInstance().get("sigma_max"); 
    if (spongeArea == 0){
        LSponge = 0; 
        sigmaMax = 0; 
    }
    lenx = NX + NGhost*2; 
    leny = NY + NGhost*2;  
    lenArr = lenx*leny; 
    cmax = 1; 

    x.resize(NX); 
    y.resize(NY); 
    dx = ( xmax + LSponge - xmin ) / NX; 
    dy = ( ymax - ymin ) / NY; 
    for (int i=0; i<NX; i++) { x[i] = xmin + 0.5*dx + i*dx; }
    for (int j=0; j<NY; j++) { y[j] = ymin + 0.5*dy + j*dy; }

    h.resize(lenArr, 0);
    velx.resize(lenArr, 0);
    vely.resize(lenArr, 0);
    b.resize(lenArr, 0);
    Parbx.resize(lenArr, 0);
    Parby.resize(lenArr, 0);
    FHalf1.resize((NX+1)*NY, 0); 
    FHalf2.resize((NX+1)*NY, 0); 
    FHalf3.resize((NX+1)*NY, 0); 
    GHalf1.resize(NX*(NY+1), 0); 
    GHalf2.resize(NX*(NY+1), 0); 
    GHalf3.resize(NX*(NY+1), 0); 
}
