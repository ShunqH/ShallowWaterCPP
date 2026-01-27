#include <iostream>  
#include <vector>
#include <cmath>
#include "sim.h" 
#include "utils.h"
#include "config.h"

using namespace std; 

void Setup(ShallowWater& sim){
    sim.t = 0; 
    sim.spectrum.NSpectrum = 1; 
    sim.spectrum.A.push_back( 0.5 ); 
    sim.spectrum.omega.push_back( 0.1 * pi );
    // double x0 = Config::getInstance().get("x0"); 
    // double y0 = Config::getInstance().get("y0"); 
    // vector<double> x0 = {2.0, 4.0, -1}; 
    // vector<double> y0 = {-3.0, 2.0, 0.}; 
    vector<double> x0 = {1.}; 
    vector<double> y0 = {0.}; 
    double bSigma = Config::getInstance().get("b_sigma"); 
    for (int i=sim.NGhost; i<sim.NX+sim.NGhost; i++){
        for (int j=sim.NGhost; j<sim.NY+sim.NGhost; j++){
            sim.b[sim.ID(i, j)] = 0; 
            sim.Parbx[sim.ID(i, j)] = 0; 
            sim.Parby[sim.ID(i, j)] = 0; 
            for (int n=0; n<x0.size(); n++){
                double delx = sim.x[i-sim.NGhost] - x0[n]; 
                double dely = sim.y[j-sim.NGhost] - y0[n]; 
                double r = sqrt( delx*delx + dely*dely ); 
                double r2 = r*r; 
                double gaussian = exp( - r2 / (2*bSigma*bSigma)); 
                sim.b[sim.ID(i, j)] += sim.bmin + sim.bmax * gaussian; 
                sim.Parbx[sim.ID(i, j)] += - sim.bmax * gaussian * delx / (bSigma*bSigma);
                sim.Parby[sim.ID(i, j)] += - sim.bmax * gaussian * dely / (bSigma*bSigma);
            }
            sim.h[sim.ID(i, j)] = max(sim.H - sim.b[sim.ID(i, j)], sim.hmin); 
            sim.velx[sim.ID(i, j)] = 0; 
            sim.vely[sim.ID(i, j)] = 0; 
            sim.cmax = max(sim.cmax, 
                                sqrt(sim.h[sim.ID(i, j)]*sim.g) 
                                + max(sim.velx[sim.ID(i, j)], sim.vely[sim.ID(i, j)])); 
        }
    }
}

void ShallowWater::UpdateBDSelfDefine(){
    for (int j = NGhost; j < NY+NGhost; j++) {
        for (int i = 0; i < NGhost; i++) {
            double H0 = H - bmin;
            double c = sqrt(g*H0); 
            h[ID(i, j)] = H0; 
            for (int n=0; n<spectrum.NSpectrum; n++){
                double phase = spectrum.omega[n]*t;
                double eta_n = spectrum.A[n] * cos(phase); 
                h[ID(i, j)] += eta_n; 
            }
            velx[ID(i, j)] = (h[ID(i, j)] - H0) * c / H0;
            vely[ID(i, j)] = 0;
            h[ID(NX + NGhost + i, j)] = h[ID(NGhost + NX - 1, j)];
            velx[ID(NX + NGhost + i, j)] = velx[ID(NGhost + NX - 1, j)];
            vely[ID(NX + NGhost + i, j)] = vely[ID(NGhost + NX - 1, j)];
        }
    }
    for (int i = 0; i < lenx; i++) {
        for (int j = 0; j < NGhost; j++) {
            h[ID(i, j)] = h[ID(i, NGhost)];
            velx[ID(i, j)] = velx[ID(i, NGhost)];
            vely[ID(i, j)] = vely[ID(i, NGhost)];
            h[ID(i, NY + NGhost + j)] = h[ID(i, NGhost + NY - 1)];
            velx[ID(i, NY + NGhost + j)] = velx[ID(i, NGhost + NY - 1)];
            vely[ID(i, NY + NGhost + j)] = vely[ID(i, NGhost + NY - 1)];
        }
    }
    return;
}

