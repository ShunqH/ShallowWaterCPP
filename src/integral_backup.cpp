#include <cmath>
// #include <vector>
#include <iostream>  
#include "sim.h"

using namespace std; 

void ShallowWater::UpdateU(double dt){
    double dtdx = dt/dx; 
    double dtdy = dt/dy; 
    UpdateBD(); 
    UpdateF(); 
    UpdateG(); 
    cmax = 0.0;
    #pragma omp parallel for reduction(max:cmax) schedule(dynamic)
    for (int i=NGhost; i<NX+NGhost; i++){
        for (int j=NGhost; j<NY+NGhost; j++){
            int idnow = ID(i, j); 
            double U1 = h[idnow]; 
            double U2 = h[idnow] * velx[idnow]; 
            double U3 = h[idnow] * vely[idnow]; 
            flux3 QBed = CalQBed(idnow) ;
            flux3 QSpo = CalQSponge(i, idnow, U1, U2, U3) ;
            U1 = U1 - (  dtdx*( FHalf1[FHalfID(i, j, 1)] - FHalf1[FHalfID(i, j, 0)] ) 
                       + dtdy*( GHalf1[GHalfID(i, j, 1)] - GHalf1[GHalfID(i, j, 0)] ) )
                    +  dt * QBed.f1 - dt * QSpo.f1; 
            U2 = U2 - (  dtdx*( FHalf2[FHalfID(i, j, 1)] - FHalf2[FHalfID(i, j, 0)] ) 
                       + dtdy*( GHalf2[GHalfID(i, j, 1)] - GHalf2[GHalfID(i, j, 0)] ) )
                    +  dt * QBed.f2 - dt * QSpo.f2; 
            U3 = U3 - (  dtdx*( FHalf3[FHalfID(i, j, 1)] - FHalf3[FHalfID(i, j, 0)] ) 
                       + dtdy*( GHalf3[GHalfID(i, j, 1)] - GHalf3[GHalfID(i, j, 0)] ) )
                    +  dt * QBed.f3 - dt * QSpo.f3; 
            if (U1 > hmin){
                h[idnow] = U1; 
                velx[idnow] = U2/h[idnow]; 
                vely[idnow] = U3/h[idnow]; 
            }else{
                h[idnow] = hmin; 
                velx[idnow] = 0.; 
                vely[idnow] = 0.; 
            }
            double vmag = sqrt( g*h[idnow] ) 
                        + sqrt(velx[idnow]*velx[idnow] + vely[idnow]*vely[idnow]);
            if (vmag > cmax){
                cout<<"here b is "<<b[idnow]
                    <<", h is "<<h[idnow]
                    <<", and vmag is "<<vmag
                    <<endl; 
                cout<<"position is ("<<x[i]
                    <<", "<<y[j]<<")"
                    <<endl; 
                cmax = vmag;
            }
        }
    }
    t += dt; 
    return; 
}