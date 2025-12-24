#pragma once

#include <vector> 

struct WaveSpectrum {
    std::vector<double> A, kx, ky, k, omega, phi;
    int NSpectrum; 
    double Amax, kmin, kmax, thetaMax; 
};

class ShallowWater{
public: 
    double t, g, H, bmax, bmin; 
    int NX, NY, NGhost; 
    double xmin, xmax, ymin, ymax, dx, dy; 
    int spongeArea; 
    double LSponge, sigmaMax; 
    double lenx, leny, lenArr; 
    double cmax; 
    
    struct flux3{double f1, f2, f3; }; 

    std::vector<double> x, y; 
    std::vector<double> h, velx, vely, b; 
    std::vector<double> FHalf1, FHalf2, FHalf3; 
    std::vector<double> GHalf1, GHalf2, GHalf3; 
    std::vector<double> Parbx, Parby; 

    WaveSpectrum spectrum;

    ShallowWater(); 

    inline int ID(int i, int j){return i*leny+j; }
    inline int IDFromFHalf(int ii, int jj, int side){
        return ID(ii + NGhost + side - 1, jj + NGhost); 
    }
    inline int IDFromGHalf(int ii, int jj, int side){
        return ID(ii + NGhost, jj + NGhost + side - 1); 
    }
    inline int FHalfID(int i, int j, int side){
        return (i - NGhost + side)*NY + (j - NGhost); 
    }
    inline int GHalfID(int i, int j, int side){
        return (i - NGhost)*(NY+1) + (j - NGhost + side);
    }

    inline flux3 CalU(double h_, double velx_, double vely_){
        return {h_, h_*velx_, h_*vely_}; 
    }

    inline flux3 CalF(double h_, double velx_, double vely_){
        return{
            h_*velx_, 
            h_*velx_*velx_ + 0.5*g*h_*h_, 
            h_*velx_*vely_, 
        }; 
    }
    inline flux3 CalG(double h_, double velx_, double vely_){
        return{
            h_*vely_, 
            h_*velx_*vely_, 
            h_*vely_*vely_ + 0.5*g*h_*h_, 
        };
    }
    inline flux3 CalQBed(int id){
        return{
            0, 
            - g * h[id] * Parbx[id], 
            - g * h[id] * Parby[id], 
        }; 
    }
    inline flux3 CalQSponge(int i, int id, double U1, double U2, double U3){
        double sigma = 0;
        if (x[i]>xmax){
            double ds = (x[i] - xmax)/LSponge; 
            sigma = sigmaMax * ds * ds; 
        }
        return {
            sigma * (U1 - (H - b[id])), 
            sigma * (U2 - 0), 
            sigma * (U3 - 0), 
        }; 
    }

    void UpdateBD(); 
    void UpdateBDOutflow(); 
    void UpdateBDPeriodic(); 
    void UpdateBDReflect();
    void UpdateBDSelfDefine(); 
    void UpdateF();
    void UpdateG(); 
    void UpdateU(double dt); 

    void WriteFrame(int outputID); 
}; 

