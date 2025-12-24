#include <iostream>  
#include <cmath>
#include "sim.h" 
#include "utils.h"
#include "config.h"

using namespace std; 

void Setup(ShallowWater& sim){
    sim.t = 0; 
    RandomGenerator ran(0, 1); 
    sim.spectrum.Amax = 0.2; 
    sim.spectrum.NSpectrum = 20;
    sim.spectrum.kmin = 2*pi / 2.0;
    sim.spectrum.kmax = 2*pi / 1.0; 
    sim.spectrum.thetaMax = 3.0 * pi / 180.0;

    for (int n=0; n<sim.spectrum.NSpectrum; n++){
        double k = sim.spectrum.kmin + ran() * (sim.spectrum.kmax - sim.spectrum.kmin);
        double theta = (ran()*2 - 1) * sim.spectrum.thetaMax;

        double kx = k * cos(theta);   // always > 0
        double ky = k * sin(theta);

        sim.spectrum.k.push_back(k);
        sim.spectrum.kx.push_back(kx);
        sim.spectrum.ky.push_back(ky);
        sim.spectrum.omega.push_back( sqrt(sim.g * sim.H) * k );
        sim.spectrum.A.push_back( sim.spectrum.Amax * sim.H * (2*ran() - 1) ); 
        sim.spectrum.phi.push_back( ran()*2*pi );
    }

    for (int i=sim.NGhost; i<sim.NX+sim.NGhost; i++){
        for (int j=sim.NGhost; j<sim.NY+sim.NGhost; j++){
            double xnow = sim.x[i-sim.NGhost]; 
            double ynow = sim.y[j-sim.NGhost]; 
            sim.b[sim.ID(i, j)] = ( sim.bmax - sim.bmin ) * ( xnow - sim.xmin ) 
                                  / ( sim.xmax + sim.LSponge - sim.xmin ) + sim.bmin; 
            sim.Parbx[sim.ID(i, j)] = ( sim.bmax - sim.bmin ) / ( sim.xmax + sim.LSponge - sim.xmin ); 
            sim.Parby[sim.ID(i, j)] = 0; 
            sim.h[sim.ID(i, j)] = sim.H - sim.b[sim.ID(i, j)]; 
            sim.velx[sim.ID(i, j)] = 0; 
            sim.vely[sim.ID(i, j)] = 0; 
            sim.cmax = max(sim.cmax, 
                                sqrt(sim.h[sim.ID(i, j)]*sim.g) 
                                + max(sim.velx[sim.ID(i, j)], sim.vely[sim.ID(i, j)])); 
        }
    }
}

void ShallowWater::UpdateBDSelfDefine(){
    double xnow = xmin - 0.5*dx; 
    for (int j = NGhost; j < NY+NGhost; j++) {
        double ynow = y[j-NGhost]; 
        for (int i = 0; i < NGhost; i++) {
            double H0 = H - b[ID(NGhost, j)];
            h[ID(i, j)] = H0; 
            velx[ID(i, j)] = 0;
            vely[ID(i, j)] = 0;
            for (int n=0; n<spectrum.NSpectrum; n++){
                double phase = spectrum.kx[n]*xnow + spectrum.ky[n]*ynow + spectrum.phi[n] - spectrum.omega[n]*t;
                double eta_n = spectrum.A[n] * cos(phase); 
                h[ID(i, j)] += eta_n; 
                velx[ID(i, j)] += g * spectrum.kx[n] * eta_n / spectrum.omega[n]; 
                vely[ID(i, j)] += g * spectrum.ky[n] * eta_n / spectrum.omega[n]; 
            }
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

