#include <cmath>
#include "sim.h" 

using namespace std; 

void ShallowWater::UpdateF(){
    #pragma omp parallel for schedule(dynamic)
    for (int ii=0; ii<NX+1; ii++){
        for (int jj=0; jj<NY; jj++){
            int idnow = ii*(NY) + jj; 
            double hl = h[IDFromFHalf(ii, jj, 0)]; 
            double hr = h[IDFromFHalf(ii, jj, 1)]; 
            double ul = velx[IDFromFHalf(ii, jj, 0)]; 
            double ur = velx[IDFromFHalf(ii, jj, 1)]; 
            double vl = vely[IDFromFHalf(ii, jj, 0)]; 
            double vr = vely[IDFromFHalf(ii, jj, 1)]; 
            double cl = sqrt( g * hl ); 
            double cr = sqrt( g * hr ); 
            double hs = ( 1/g ) * ( 0.5*(cl+cr) + 0.25*(ul-ur) ) * ( 0.5*(cl+cr) + 0.25*(ul-ur) ); 
            double ql = (hs <= hl) ? 1 : sqrt( 0.5*(hs+hl)*hs/(hl*hl) ); 
            double sl = ul - cl*ql; 
            double qr = (hs <= hr) ? 1 : sqrt( 0.5*(hs+hr)*hs/(hr*hr) ); 
            double sr = ur + cr*qr; 
            flux3 FL = CalF(hl, ul, vl);
            if ( sl >= 0 ){
                FHalf1[idnow] = FL.f1; 
                FHalf2[idnow] = FL.f2; 
                FHalf3[idnow] = FL.f3; 
                continue; 
            } 
            flux3 FR = CalF(hr, ur, vr);
            if ( sr <= 0 ){
                FHalf1[idnow] = FR.f1; 
                FHalf2[idnow] = FR.f2; 
                FHalf3[idnow] = FR.f3; 
                continue; 
            } 
            double ss = ( sl*hr*(ur-sr) - sr*hl*(ul-sl) ) / ( hr * (ur - sr) - hl * (ul - sl) ); 
            double hsl = hl * (sl-ul) / (sl-ss);  
            if ( (sl <= 0) && (ss >= 0) ){
                FHalf1[idnow] = FL.f1 + sl*( hsl - hl ); 
                FHalf2[idnow] = FL.f2 + sl*( hsl*ss - hl*ul ); 
                FHalf3[idnow] = FL.f3 + sl*( hsl*vl - hl*vl ); 
                continue; 
            }
            double hsr = hr * (sr-ur) / (sr-ss); 
            if ( (ss <= 0) && (sr >= 0) ){
                FHalf1[idnow] = FR.f1 + sr*( hsr - hr ); 
                FHalf2[idnow] = FR.f2 + sr*( hsr*ss - hr*ur ); 
                FHalf3[idnow] = FR.f3 + sr*( hsr*vr - hr*vr ); 
                continue; 
            }
        }
    }
    return; 
}

void ShallowWater::UpdateG(){
    #pragma omp parallel for schedule(dynamic)
    for (int ii=0; ii<NX; ii++){
        for (int jj=0; jj<NY+1; jj++){
            int idnow = ii*(NY+1) + jj;  
            double hl = h[IDFromGHalf(ii, jj, 0)]; 
            double hr = h[IDFromGHalf(ii, jj, 1)]; 
            double ul = velx[IDFromGHalf(ii, jj, 0)]; 
            double ur = velx[IDFromGHalf(ii, jj, 1)]; 
            double vl = vely[IDFromGHalf(ii, jj, 0)]; 
            double vr = vely[IDFromGHalf(ii, jj, 1)]; 
            double cl = sqrt( g * hl ); 
            double cr = sqrt( g * hr ); 
            double hs = ( 1/g ) * ( 0.5*(cl+cr) + 0.25*(vl-vr) ) * ( 0.5*(cl+cr) + 0.25*(vl-vr) ); 
            double ql = (hs <= hl) ? 1 : sqrt( 0.5*(hs+hl)*hs/(hl*hl) ); 
            double sl = vl - cl*ql; 
            double qr = (hs <= hr) ? 1 : sqrt( 0.5*(hs+hr)*hs/(hr*hr) ); 
            double sr = vr + cr*qr; 
            flux3 GL = CalG(hl, ul, vl);
            if ( sl >= 0 ){
                GHalf1[idnow] = GL.f1; 
                GHalf2[idnow] = GL.f2; 
                GHalf3[idnow] = GL.f3; 
                continue; 
            } 
            flux3 GR = CalG(hr, ur, vr);
            if ( sr <= 0 ){
                GHalf1[idnow] = GR.f1; 
                GHalf2[idnow] = GR.f2; 
                GHalf3[idnow] = GR.f3; 
                continue; 
            } 
            double ss = ( sl*hr*(vr-sr) - sr*hl*(vl-sl) ) / ( hr * (vr - sr) - hl * (vl - sl) ); 
            double hsl = hl * (sl-vl) / (sl-ss);  
            if ( (sl <= 0) && (ss >= 0) ){
                GHalf1[idnow] = GL.f1 + sl*( hsl - hl ); 
                GHalf2[idnow] = GL.f2 + sl*( hsl*ul - hl*ul ); 
                GHalf3[idnow] = GL.f3 + sl*( hsl*ss - hl*vl ); 
                continue; 
            }
            double hsr = hr * (sr-vr) / (sr-ss); 
            if ( (ss <= 0) && (sr >= 0) ){
                GHalf1[idnow] = GR.f1 + sr*( hsr - hr ); 
                GHalf2[idnow] = GR.f2 + sr*( hsr*ur - hr*ur ); 
                GHalf3[idnow] = GR.f3 + sr*( hsr*ss - hr*vr ); 
                continue; 
            }
        }
    }
    return; 
}