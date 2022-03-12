#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <omp.h>
#include <chrono>
#include "LKS.hpp"
#include "input.hpp"

int main()
{
    // Improved Lattice Kinetic Scheme model

    bool restart;
    bool Fwrite;
    bool writeBinary;
    int startTimeStep;
    int endTimeStep;
    int nextOutTime;
    int outInterval;
    int nx;
    int ny;
    int nz;

    input(restart, Fwrite, writeBinary, startTimeStep, endTimeStep, nextOutTime, outInterval, nx, ny, nz);

    // D3Q19 model
    const std::vector<double> wt = setWt();
    
    const std::vector<double> cx = setCx();
    const std::vector<double> cy = setCy();
    const std::vector<double> cz = setCz();
    
    std::vector<double> p(nx*ny*nz);
    std::vector<double> u(nx*ny*nz);
    std::vector<double> v(nx*ny*nz);
    std::vector<double> w(nx*ny*nz);

    std::vector<obstructure> obst(nx*ny*nz);

    // -- For cavity flow --
    const double u0 = 0.1;
    const double rho0 = 1.0;
    const double Re = 100.0;
    double nu = std::abs(u0)*double(nx-1)/Re;
    double dpdx = 0.0;
    #include "boundaryCondition_cavityFlow.hpp"    
    // --

    //For channel flow
    // double Retau = 10;
    // double Retau = 1;
    // double utau = 0.005;
    // double nu = utau*0.5*ny/Retau;
    // double dpdx = utau*utau/(0.5*ny);
    // #include "boundaryCondition_channelFlow.hpp"    

    //For Poiseuille flow
    // double umax = 0.1;
    // double h = 1.0;
    // double nu = umax*h/Re;
    // double dpdx = umax/(h*h)*8.0*nu/(ny-1);
    // #include "boundaryCondition_poiseuilleFlow.hpp"
     
    if(restart)
    {
        #include "restart.hpp"
    }
    else
    {
        #include "initialization.hpp"        
    }

    double Au = getAu(nu);
    int nl = 5;
    double Ma = 0.08;
    double Cs = u0/Ma;
    double Ap = getAp(Cs, nl);

    std::cout << "Au: " << Au << ", Ap: " << Ap << std::endl;    

    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    start = std::chrono::system_clock::now();

    // Time marching
    for(int nt = startTimeStep; nt <= endTimeStep; nt++)
    {   
        #include "write.hpp"

        std::vector<double> uTmp = u;
        std::vector<double> vTmp = v;
        std::vector<double> wTmp = w;
        std::vector<double> pTmp = p;


        
        for(int l = 0; l < nl; l++)
        {
            #pragma omp parallel for                
            for(int ic = 0; ic < nx*ny*nz; ic++)
            {
                int i = ic2i(ic, nx, ny);
                int j = ic2j(ic, nx, ny);
                int k = ic2k(ic, nx, ny);

                p[ic] = updateP(i,j,k,nx,ny,nz,pTmp,uTmp,vTmp,wTmp,cx,cy,cz,wt,Ap);
            }
            pTmp = p;
            
            #pragma omp parallel for                
            for(int ic = 0; ic < nx*ny*nz; ic++)
            {
                int i = ic2i(ic, nx, ny);
                int j = ic2j(ic, nx, ny);
                int k = ic2k(ic, nx, ny);

                boundaryConditionsP(obst[ic], p, i, j, k, nx, ny, nz);
            }
        }

        #pragma omp parallel for
        for(int ic = 0; ic < nx*ny*nz; ic++)
        {

            int i = ic2i(ic, nx, ny);
            int j = ic2j(ic, nx, ny);
            int k = ic2k(ic, nx, ny);
            
            u[ic] = updateU(i,j,k,nx,ny,nz,p,uTmp,vTmp,wTmp,cx,cy,cz,wt,Au);
            v[ic] = updateV(i,j,k,nx,ny,nz,p,uTmp,vTmp,wTmp,cx,cy,cz,wt,Au);
            w[ic] = updateW(i,j,k,nx,ny,nz,p,uTmp,vTmp,wTmp,cx,cy,cz,wt,Au);
        }

        #pragma omp parallel for
        for(int ic = 0; ic < nx*ny*nz; ic++)                
        {
            int i = ic2i(ic, nx, ny);
            int j = ic2j(ic, nx, ny);
            int k = ic2k(ic, nx, ny);

            boundaryConditionsU(obst[ic], u, v, w, i, j, k, nx, ny, nz);
        }
    }
    end = std::chrono::system_clock::now();
    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() *1e-6);
    std::cout << "Execution time: " << time << " (s)" << std::endl;
    std::cout << "Speed: " << double(endTimeStep)*double(nx*ny*nz)/time << " (LUPS)" << std::endl;

    return EXIT_SUCCESS;
}