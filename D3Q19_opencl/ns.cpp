#include <boost/algorithm/string.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <chrono>

#include <omp.h>

#include "D3Q19.hpp"
#include "input.hpp"

// For OpenCL
#ifdef __APPLE__
#define CL_SILENCE_DEPRECATION
#endif
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#include <CL/cl2.hpp>

#include "util.hpp"

#include "err_code.h"

// pick up device type from compiler command line or from the default type
#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif
// 

int main()
{
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
    float uMax;
    float rho0;
    float Re;

    input(restart, Fwrite, writeBinary, startTimeStep, endTimeStep, nextOutTime, outInterval, nx, ny, nz, uMax, rho0, Re);

    // Single Relaxation Time model
    // // -- For cavity flow --
    const float a = 1.0; //(m)
    const float L = a/float(nx-1);

    const float U0 = 0.1;
    const float c = uMax/U0;

    const float deltaT = L/c;

    float nu = uMax*a/Re;
    nu = nu/(L*c);
    float dpdx = 0.0;


    //For channel flow
    // float Retau = 10;
    // float Retau = 1;
    // float utau = 0.005;
    // float nu = utau*0.5*ny/Retau;
    // float dpdx = utau*utau/(0.5*ny);

    //For Poiseuille flow
    // float umax = 0.1;
    // float h = 1.0;
    // float nu = umax*h/Re;
    // float dpdx = umax/(h*h)*8.0*nu/(ny-1);
    // float dpdx = 0.00001;

    // std::cout << "dpdx = " << dpdx << std::endl;

    // std::cout << "nu = " << nu << std::endl;
    
    // nu = nu*(ny-1);

    float omega = 1.0/(3.0*nu +0.5);
    // float omega = 1.0/0.56;

    std::cout << "tau = " << 1.0/omega << std::endl;

    const std::vector<float> wt = setWt();

    // D3Q19 model
    const std::vector<float> cx = setCx();
    const std::vector<float> cy = setCy();
    const std::vector<float> cz = setCz();

    std::vector<float> f(19*nx*ny*nz);
    std::vector<float> fTmp(19*nx*ny*nz);
    
    std::vector<float> rho(nx*ny*nz);
    std::vector<float> u(nx*ny*nz);
    std::vector<float> v(nx*ny*nz);
    std::vector<float> w(nx*ny*nz);

    std::vector<obstructure> obst(nx*ny*nz);
    std::vector<float> u0(nx*ny*nz);
    std::vector<float> v0(nx*ny*nz);
    std::vector<float> w0(nx*ny*nz);
    std::vector<int> normal(nx*ny*nz,0);

    const unsigned elements = nx*ny*nz;
    const unsigned qElements = 19*elements;

    // Setting conditions
    #include "boundaryCondition_cavityFlow2d.hpp"
    // #include "boundaryCondition_cavityFlow3dDiagonal.hpp"
    // #include "boundaryCondition_channelFlow.hpp"
    if(restart)
    {
        #include "restart.hpp"
    }
    else
    {
        #include "initialization.hpp"        
    }

    for(int ic = 0; ic < nx*ny*nz; ic++)
    {
        u0[ic] = obst[ic].u0;
        v0[ic] = obst[ic].v0;
        w0[ic] = obst[ic].w0;

        const std::vector<int> normal_in = {-1,0,0};
        const std::vector<int> normal_ip = {1,0,0};
        const std::vector<int> normal_jn = {0,-1,0};
        const std::vector<int> normal_jp = {0,1,0};
        const std::vector<int> normal_kn = {0,0,-1};
        const std::vector<int> normal_kp = {0,0,1};

        if(obst[ic].normal == normal_in)
        {
            normal[ic] = -1;
        }
        else if(obst[ic].normal == normal_ip)
        {
            normal[ic] = 1;
        }
        else if(obst[ic].normal == normal_jn)
        {
            normal[ic] = -2;
        }
        else if(obst[ic].normal == normal_jp)
        {
            normal[ic] = 2;
        }
        else if(obst[ic].normal == normal_kn)
        {
            normal[ic] = -3;
        }
        else if(obst[ic].normal == normal_kp)
        {
            normal[ic] = 3;
        }
    }

    std::vector<int> upID(19*nx*ny*nz);
    std::vector<unsigned> upQID(19*nx*ny*nz);
    for(int ic = 0; ic < nx*ny*nz; ic++)
    {
        int i = ic2i(ic,nx,ny);
        int j = ic2j(ic,nx,ny);
        int k = ic2k(ic,nx,ny);
        for(int q = 0; q < 19; q++)
        {
            int qic = idf(q,ic,nx,ny,nz);
            upID[qic] = upwindID(q,i,j,k,nx,ny,nz);
            upQID[qic] = idf(q, upID[qic], nx, ny, nz);
        }
    }

    std::vector<int> internalID;
    std::vector<int> boundaryID;
    for(int ic = 0; ic < nx*ny*nz; ic++)
    {
        if(obst[ic].boundary == 0)
        {
            internalID.push_back(ic);
        }
        else
        {
            boundaryID.push_back(ic);
        }
    }

    std::vector<int> BBID;
    std::vector<int> BBmovingWallID;
    for(int bID = 0; bID < boundaryID.size(); bID++)
    {
        int ic = boundaryID[bID];
        if(obst[ic].boundary == 1)
        {
            BBID.push_back(ic);
        }
        else if(obst[ic].boundary == 2)
        {
            BBmovingWallID.push_back(ic);
        }
    }
    const unsigned nBB = BBID.size();
    const unsigned nBBMW = BBmovingWallID.size();

    std::vector<int> BBQID(19*BBID.size());
    for(int bID = 0; bID < BBID.size(); bID++)
    {
        int ic = BBID[bID];

        int i = ic2i(ic,nx,ny);
        int j = ic2j(ic,nx,ny);
        int k = ic2k(ic,nx,ny);
        for(int q = 0; q < 19; q++)
        {
            int qic = q*BBID.size()+bID;
            BBQID[qic] = bounceBackQID(obst[ic],q,i,j,k,nx,ny,nz);
        }
    }


    // Create a context
    cl::Context context(DEVICE);

    cl::Device device = context.getInfo<CL_CONTEXT_DEVICES>()[0];

    std::cout << std::endl << "Using OpenCL device: "
                << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    

    // Load in kernel source, creating and building a program object for the context
    cl::Program program(context, util::loadProgram("kernel.cl"), true);

    
    cl::CommandQueue queue(context);

    cl::Buffer f_d(context, f.begin(), f.end(), true);
    // cl::Buffer u_d(context, u.begin(), u.end(), true);
    // cl::Buffer v_d(context, v.begin(), v.end(), true);
    // cl::Buffer w_d(context, w.begin(), w.end(), true);

    cl::Buffer fTmp_d(context, fTmp.begin(), fTmp.end(), true);
    // cl::Buffer uTmp_d(context, u.begin(), u.end(), true);
    // cl::Buffer vTmp_d(context, v.begin(), v.end(), true);
    // cl::Buffer wTmp_d(context, w.begin(), w.end(), true);

    cl::Buffer upQID_d(context, upQID.begin(), upQID.end(), true);
    cl::Buffer BBID_d(context, BBID.begin(), BBID.end(), true);
    cl::Buffer BBQID_d(context, BBQID.begin(), BBQID.end(), true);
    cl::Buffer BBmovingWallID_d(context, BBmovingWallID.begin(), BBmovingWallID.end(), true);
    cl::Buffer u0_d(context, u0.begin(), u0.end(), true);
    cl::Buffer v0_d(context, v0.begin(), v0.end(), true);
    cl::Buffer w0_d(context, w0.begin(), w0.end(), true);
    cl::Buffer normal_d(context, normal.begin(), normal.end(), true);
    

    // Create the kernel functor of collision
    cl::KernelFunctor
    <
        cl::Buffer, cl::Buffer,
        const unsigned,
        const float
    > k_collision(program, "k_collision");

    // Create the kernel functor of externalForce
    cl::KernelFunctor
    <
        cl::Buffer, cl::Buffer,
        const unsigned,
        const float
    > k_externalForce(program, "k_externalForce");

    // Create the kernel functor of streaming
    cl::KernelFunctor
    <
        cl::Buffer, cl::Buffer,
        cl::Buffer
    > k_streaming(program, "k_streaming");

    // Create the kernel functor of bounceBack
    cl::KernelFunctor
    <
        cl::Buffer,
        cl::Buffer, cl::Buffer,
        const unsigned, const unsigned
    > k_bounceBack(program, "k_bounceBack");

    // Create the kernel functor of bounceBackMovingWall
    cl::KernelFunctor
    <
        cl::Buffer,
        cl::Buffer,
        cl::Buffer, cl::Buffer, cl::Buffer,
        cl::Buffer,
        const unsigned
    > k_bounceBackMovingWall(program, "k_bounceBackMovingWall");

    // // {
    // // cl::Kernel ko_BC_P(program,"BC_P");
    // // size_t work_group_size = ko_BC_P.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(cl::Device::getDefault());
    // // std::cout << "work_group_size: " << work_group_size << std::endl;
    // // }



    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    start = std::chrono::system_clock::now();

    // Time marching
    for(int nt = startTimeStep; nt <= endTimeStep; nt++)
    {
        #include "write.hpp"

        {
        util::Timer timer;
        k_collision
        (
            cl::EnqueueArgs(queue,cl::NDRange(elements)),
            f_d, fTmp_d,
            elements,
            omega
        );
        k_externalForce
        (
            cl::EnqueueArgs(queue,cl::NDRange(elements)),
            f_d, fTmp_d,
            elements,
            dpdx
        );
        queue.finish();
        double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // printf("\nThe kernels of collision and externalForce ran in %lf m seconds\n", rtime);
        }

        // {
        // util::Timer timer;
        // #pragma omp parallel for
        // for(int ic = 0; ic < nx*ny*nz; ic++)
        // {
        //     collision(omega,ic,nx,ny,nz,cx,cy,cz,wt,f);
        //     externalForce(dpdx,ic,nx,ny,nz,cx,cy,cz,wt,f);
        //     for(int q = 0; q < 19; q++)
        //     {
        //         int qic = idf(q,ic,nx,ny,nz);
        //         fTmp[qic] = f[qic];
        //     }
        // }
        // double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // // printf("\nThe functions of collision and externalForce ran in %lf m seconds\n", rtime);
        // }


        {
        util::Timer timer;
        k_streaming
        (
            cl::EnqueueArgs(queue,cl::NDRange(qElements)),
            f_d, fTmp_d,
            upQID_d
        );
        queue.finish();
        double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // printf("\nThe kernel of streaming ran in %lf m seconds\n", rtime);
        }

        // {
        // util::Timer timer;
        // #pragma omp parallel for
        // for(int ic = 0; ic < nx*ny*nz; ic++)
        // {
        //     int i = ic2i(ic,nx,ny);
        //     int j = ic2j(ic,nx,ny);
        //     int k = ic2k(ic,nx,ny);

        //     streaming(ic,i,j,k,nx,ny,nz,fTmp,f,upID);
        // }
        // double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // // printf("\nThe function of streaming ran in %lf m seconds\n", rtime);
        // }

        {
        util::Timer timer;
        k_bounceBack
        (
            cl::EnqueueArgs(queue,cl::NDRange(nBB)),
            f_d,
            BBID_d, BBQID_d,
            elements, nBB
        );
        // queue.finish();
        double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // printf("\nThe kernel of bounceBack ran in %lf m seconds\n", rtime);
        }

        {
        util::Timer timer;
        k_bounceBackMovingWall
        (
            cl::EnqueueArgs(queue,cl::NDRange(nBBMW)),
            f_d,
            BBmovingWallID_d,
            u0_d, v0_d, w0_d,
            normal_d,
            elements
        );
        queue.finish();
        double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // printf("\nThe kernel of bounceBackMovingWall ran in %lf m seconds\n", rtime);
        }


        // cl::copy(queue, f_d, f.begin(), f.end());
        // {
        // util::Timer timer;
        // #pragma omp parallel for
        // for(int bID = 0; bID < boundaryID.size(); bID++)
        // {
        //     int ic = boundaryID[bID];
        //     boundaryConditions(obst[ic],f,ic,nx,ny,nz);
        // }
        // double rtime = static_cast<double>(timer.getTimeMicroseconds()) / 1000.0;
        // // printf("\nThe function of boundaryConditions ran in %lf m seconds\n", rtime);
        // }
        // cl::copy(queue, f.begin(), f.end(), f_d);
    }
    end = std::chrono::system_clock::now();
    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() *1e-6);
    std::cout << "Execution time: " << time << " (s)" << std::endl;
    std::cout << "Speed: " << double(endTimeStep)*double(nx*ny*nz)/time*1e-6 << " (MLUPS)" << std::endl;

    return EXIT_SUCCESS;
}
