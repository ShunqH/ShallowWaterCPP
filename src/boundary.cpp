#include <iostream>  
#include "sim.h" 
#include "config.h"

using namespace std; 

void ShallowWater::UpdateBDOutflow(){
    for (int j = 0; j < leny; j++) {
        for (int n = 0; n < NGhost; n++) {
            h[ID(n, j)] = h[ID(NGhost, j)];
            velx[ID(n, j)] = velx[ID(NGhost, j)];
            vely[ID(n, j)] = vely[ID(NGhost, j)];
            h[ID(NX + NGhost + n, j)] = h[ID(NGhost + NX - 1, j)];
            velx[ID(NX + NGhost + n, j)] = velx[ID(NGhost + NX - 1, j)];
            vely[ID(NX + NGhost + n, j)] = vely[ID(NGhost + NX - 1, j)];
        }
    }
    for (int i = 0; i < lenx; i++) {
        for (int n = 0; n < NGhost; n++) {
            h[ID(i, n)] = h[ID(i, NGhost)];
            velx[ID(i, n)] = velx[ID(i, NGhost)];
            vely[ID(i, n)] = vely[ID(i, NGhost)];
            h[ID(i, NY + NGhost + n)] = h[ID(i, NGhost + NY - 1)];
            velx[ID(i, NY + NGhost + n)] = velx[ID(i, NGhost + NY - 1)];
            vely[ID(i, NY + NGhost + n)] = vely[ID(i, NGhost + NY - 1)];
        }
    }
    return;
}

void ShallowWater::UpdateBDPeriodic(){
    for (int j = 0; j < leny; j++) {
        for (int n = 0; n < NGhost; n++) {
            h[ID(n, j)] = h[ID(NX + n, j)];
            velx[ID(n, j)] = velx[ID(NX + n, j)];
            vely[ID(n, j)] = vely[ID(NX + n, j)];
            h[ID(NX + NGhost + n, j)] = h[ID(NGhost + n, j)];
            velx[ID(NX + NGhost + n, j)] = velx[ID(NGhost + n, j)];
            vely[ID(NX + NGhost + n, j)] = vely[ID(NGhost + n, j)];
        }
    }
    for (int i = 0; i < lenx; i++) {
        for (int n = 0; n < NGhost; n++) {
            h[ID(i, n)] = h[ID(i, NY + n)];
            velx[ID(i, n)] = velx[ID(i, NY + n)];
            vely[ID(i, n)] = vely[ID(i, NY + n)];
            h[ID(i, NY + NGhost + n)] = h[ID(i, NGhost + n)];
            velx[ID(i, NY + NGhost + n)] = velx[ID(i, NGhost + n)];
            vely[ID(i, NY + NGhost + n)] = vely[ID(i, NGhost + n)];
        }
    }
    return;
}

void ShallowWater::UpdateBDReflect(){
    for (int j=0; j<leny; j++){
        for(int n=0; n<NGhost; n++){
            h[ID(n, j)] = h[ID(NGhost, j)]; 
            velx[ID(n, j)] = -velx[ID(NGhost, j)]; 
            vely[ID(n, j)] = vely[ID(NGhost, j)]; 
            h[ID(NX+NGhost+n, j)] = h[ID(NX+NGhost-1, j)]; 
            velx[ID(NX+NGhost+n, j)] = -velx[ID(NX+NGhost-1, j)]; 
            vely[ID(NX+NGhost+n, j)] = vely[ID(NX+NGhost-1, j)]; 
        }
    }
    for (int i=0; i<lenx; i++){
        for(int n=0; n<NGhost; n++){
            h[ID(i, n)] = h[ID(i, NGhost)]; 
            velx[ID(i, n)] = velx[ID(i, NGhost)]; 
            vely[ID(i, n)] = -vely[ID(i, NGhost)]; 
            h[ID(i, NY+NGhost+n)] = h[ID(i, NY+NGhost-1)]; 
            velx[ID(i, NY+NGhost+n)] = velx[ID(i, NY+NGhost-1)]; 
            vely[ID(i, NY+NGhost+n)] = -vely[ID(i, NY+NGhost-1)]; 
        }
    }
    return; 
}

void ShallowWater::UpdateBD(){
    int boundaryType = Config::getInstance().get("boundaryType"); 
    if (boundaryType == 1){
        UpdateBDOutflow(); 
    }else if (boundaryType == 2){
        UpdateBDPeriodic(); 
    }else if (boundaryType == 3){
        UpdateBDReflect(); 
    }else if (boundaryType == 0){
        UpdateBDSelfDefine(); 
    }else { 
        cerr << "Error: unknown boundaryType = " << boundaryType << endl; 
        exit(1); 
    }
}