#ifndef D3Q19_H
#define D3Q19_H

#include <vector>
#include "base.hpp"

//D3Q19

inline const std::vector<float> setWt()
{
    return {1.0/3.0, 1.0/18.0, 1.0/18.0, 1.0/18.0, 1.0/18.0, 1.0/18.0, 1.0/18.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0};
}

inline const std::vector<float> setCx()
{
    return {0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0};
}

inline const std::vector<float> setCy()
{
    return {0.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 1.0, -1.0};
}

inline const std::vector<float> setCz()
{
    return {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0};
}

inline int reflectQ(const int q)
{
    if(q == 0)
    {
        return 0;
    }
    else if(q == 1)
    {
        return 2;
    }
    else if(q == 2)
    {
        return 1;
    }
    else if(q == 3)
    {
        return 4;
    }
    else if(q == 4)
    {
        return 3;
    }
    else if(q == 5)
    {
        return 6;
    }
    else if(q == 6)
    {
        return 5;
    }
    else if(q == 7)
    {
        return 8;
    }
    else if(q == 8)
    {
        return 7;
    }
    else if(q == 9)
    {
        return 10;
    }
    else if(q == 10)
    {
        return 9;
    }
    else if(q == 11)
    {
        return 12;
    }
    else if(q == 12)
    {
        return 11;
    }
    else if(q == 13)
    {
        return 14;
    }
    else if(q == 14)
    {
        return 13;
    }
    else if(q == 15)
    {
        return 16;
    }
    else if(q == 16)
    {
        return 15;
    }
    else if(q == 17)
    {
        return 18;
    }
    else if(q == 18)
    {
        return 17;
    }
    std::cerr << "Error q = [0:18], q = " << q << std::endl;
    exit(EXIT_FAILURE);    
}

inline int upwindID_B(const int q, const int i, const int j, const int k, const int nx, const int ny, const int nz, const int boundary1, const int boundary2, const int boundary3)
{
    bool isnotWall1 = (boundary1 == 1 || boundary1 == 4 || boundary1 == 5 || boundary1 == 6) ? false : true;
    bool isnotWall2 = (boundary2 == 1 || boundary2 == 4 || boundary2 == 5 || boundary2 == 6) ? false : true;
    bool isnotWall3 = (boundary3 == 1 || boundary3 == 4 || boundary3 == 5 || boundary3 == 6) ? false : true;


    if(q == 0)
    {
        return index1d(i,j,k,nx,ny);
    }
    else if(q == 1)
    {
        return i != 0 ? index1d(i-1,j,k,nx,ny) : (isnotWall1 ? index1d(nx-1,j,k,nx,ny) : -1);
    }
    else if(q == 2)
    {
        return i != nx-1 ? index1d(i+1,j,k,nx,ny) : (isnotWall1 ? index1d(0,j,k,nx,ny) : -1);
    }
    else if(q == 3)
    {
        if(j != 0)
        {
            return index1d(i,j-1,k,nx,ny);
        }
        else
        {
            return isnotWall2 ? index1d(i,ny-1,k,nx,ny) : -1;
        }
    }
    else if(q == 4)
    {
        if(j != ny-1)
        {
            return index1d(i,j+1,k,nx,ny);
        }
        else
        {
            return isnotWall2 ? index1d(i,0,k,nx,ny) : -1;
        }
    }
    else if(q == 5)
    {
        if(k != 0)
        {
            return index1d(i,j,k-1,nx,ny);
        }
        else
        {
            return isnotWall3 ? index1d(i,j,nz-1,nx,ny) : -1;
        }
    }
    else if(q == 6)
    {
        if(k != nz-1)
        {
            return index1d(i,j,k+1,nx,ny);
        }
        else
        {
            return isnotWall3 ? index1d(i,j,0,nx,ny) : -1;
        }
    }
    else if(q == 7)
    {
        return (i != 0 && j != 0) ? index1d(i-1, j-1, k, nx, ny) :
               (i == 0 && j != 0) ? (isnotWall1 ? index1d(nx-1, j-1, k, nx, ny) : -1) :
               (i != 0 && j == 0) ? (isnotWall2 ? index1d(i-1, ny-1, k, nx, ny) : -1) :
               ((isnotWall1 && isnotWall2) ? index1d(nx-1, ny-1, k, nx, ny) : -1);
    }
    else if(q == 8)
    {
        return (i != nx-1 && j != ny-1) ? index1d(i+1, j+1, k, nx, ny) :
               (i == nx-1 && j != ny-1) ? (isnotWall1 ? index1d(0, j+1, k, nx, ny) : -1) :
               (i != nx-1 && j == ny-1) ? (isnotWall2 ? index1d(i+1, 0, k, nx, ny) : -1) :
               ((isnotWall1 && isnotWall2) ? index1d(0, 0, k, nx, ny) : -1);
    }
    else if(q == 9)
    {
        return (i != 0 && j != ny-1) ? index1d(i-1, j+1, k, nx, ny) :
               (i == 0 && j != ny-1) ? (isnotWall1 ? index1d(nx-1, j+1, k, nx, ny) : -1):
               (i != 0 && j == ny-1) ? (isnotWall2 ? index1d(i-1, 0, k, nx, ny) : -1) :
               ((isnotWall1 && isnotWall2) ? index1d(nx-1, 0, k, nx, ny) : -1);
    }
    else if(q == 10)
    {
        return (i != nx-1 && j != 0) ? index1d(i+1, j-1, k, nx, ny) :
               (i == nx-1 && j != 0) ? (isnotWall1 ? index1d(0, j-1, k, nx, ny) : -1) :
               (i != nx-1 && j == 0) ? (isnotWall2 ? index1d(i+1, ny-1, k, nx, ny) : -1) :
               ((isnotWall1 && isnotWall2) ? index1d(0, ny-1, k, nx, ny) : -1);
    }
    else if(q == 11)
    {
        if(i != 0 && k != 0)
        {
            return index1d(i-1, j, k-1, nx, ny);
        }
        else if(i == 0 && k != 0)
        {
            return isnotWall1 ? index1d(nx-1, j, k-1, nx, ny) : -1;
        }
        else if(i != 0 && k == 0)
        {
            return isnotWall3 ? index1d(i-1, j, nz-1, nx, ny) : -1;
        }
        else
        {
            return (isnotWall1 && isnotWall3) ? index1d(nx-1, j, nz-1, nx, ny) : -1;
        }
    }
    else if(q == 12)
    {
        if(i != nx-1 && k != nz-1)
        {
            return index1d(i+1, j, k+1, nx, ny);
        }
        else if(i == nx-1 && k != nz-1)
        {
            return isnotWall1 ? index1d(0, j, k+1, nx, ny) : -1;
        }
        else if(i != nx-1 && k == nz-1)
        {
            return isnotWall3 ? index1d(i+1, j, 0, nx, ny) : -1;
        }
        else
        {
            return (isnotWall1 && isnotWall3) ? index1d(0, j, 0, nx, ny) : -1;
        }
    }
    else if(q == 13)
    {
        if(i != 0 && k != nz-1)
        {
            return index1d(i-1, j, k+1, nx, ny);
        }
        else if(i == 0 && k != nz-1)
        {
            return isnotWall1 ? index1d(nx-1, j, k+1, nx, ny) : -1;
        }
        else if(i != 0 && k == nz-1)
        {
            return isnotWall3 ? index1d(i-1, j, 0, nx, ny) : -1;
        }
        else
        {
            return (isnotWall1 && isnotWall3) ? index1d(nx-1, j, 0, nx, ny) : -1;
        }
    }
    else if(q == 14)
    {
        if(i != nx-1 && k != 0)
        {
            return index1d(i+1, j, k-1, nx, ny);
        }
        else if(i == nx-1 && k != 0)
        {
            return isnotWall1 ? index1d(0, j, k-1, nx, ny) : -1;
        }
        else if(i != nx-1 && k == 0)
        {
            return isnotWall3 ? index1d(i+1, j, nz-1, nx, ny) : -1;
        }
        else
        {
            return (isnotWall1 && isnotWall3) ? index1d(0, j, nz-1, nx, ny) : -1;
        }
    }
    else if(q == 15)
    {
        if(j != 0 && k != 0)
        {
            return index1d(i, j-1, k-1, nx, ny);
        }
        else if(j == 0 && k != 0)
        {
            return isnotWall2 ? index1d(i, ny-1, k-1, nx, ny) : -1;
        }
        else if(j != 0 && k == 0)
        {
            return isnotWall3 ? index1d(i, j-1, nz-1, nx, ny) : -1;
        }
        else
        {
            return (isnotWall2 && isnotWall3) ? index1d(i, ny-1, nz-1, nx, ny) : -1;
        }
    }
    else if(q == 16)
    {
        if(j != ny-1 && k != nz-1)
        {
            return index1d(i, j+1, k+1, nx, ny);
        }
        else if(j == ny-1 && k != nz-1)
        {
            return isnotWall2 ? index1d(i, 0, k+1, nx, ny) : -1;
        }
        else if(j != ny-1 && k == nz-1)
        {
            return isnotWall3 ? index1d(i, j+1, 0, nx, ny) : -1;
        }
        else
        {
            return (isnotWall2 && isnotWall3) ? index1d(i, 0, 0, nx, ny) : -1;
        }
    }
    else if(q == 17)
    {
        if(j != 0 && k != nz-1)
        {
            return index1d(i, j-1, k+1, nx, ny);
        }
        else if(j == 0 && k != nz-1)
        {
            return isnotWall2 ? index1d(i, ny-1, k+1, nx, ny) : -1;
        }
        else if(j != 0 && k == nz-1)
        {
            return isnotWall3 ? index1d(i, j-1, 0, nx, ny) : -1;
        }
        else
        {
            return (isnotWall2 && isnotWall3) ? index1d(i, ny-1, 0, nx, ny) : -1;
        }
    }
    else if(q == 18)
    {
        if(j != ny-1 && k != 0)
        {
            return index1d(i, j+1, k-1, nx, ny);
        }
        else if(j == ny-1 && k != 0)
        {
            return isnotWall2 ? index1d(i, 0, k-1, nx, ny) : -1;
        }
        else if(j != ny-1 && k == 0)
        {
            return isnotWall3 ? index1d(i, j+1, nz-1, nx, ny) : -1;
        }
        else
        {
            return (isnotWall2 && isnotWall3) ? index1d(i, 0, nz-1, nx, ny) : -1;
        }
    }
    else
    {
        return 0;
    }
}

inline int upwindID(const int q, const int i, const int j, const int k, const int nx, const int ny, const int nz)
{
    if(q == 0)
    {
        return index1d(i,j,k,nx,ny);
    }
    else if(q == 1)
    {
        return i != 0 ? index1d(i-1,j,k,nx,ny) : index1d(nx-1,j,k,nx,ny);
    }
    else if(q == 2)
    {
        return i != nx-1 ? index1d(i+1,j,k,nx,ny) : index1d(0,j,k,nx,ny);
    }
    else if(q == 3)
    {
        return j != 0 ? index1d(i,j-1,k,nx,ny) : index1d(i,ny-1,k,nx,ny);
    }
    else if(q == 4)
    {
        return j != ny-1 ? index1d(i,j+1,k,nx,ny) : index1d(i,0,k,nx,ny);
    }
    else if(q == 5)
    {
        return k != 0 ? index1d(i,j,k-1,nx,ny) : index1d(i,j,nz-1,nx,ny);
    }
    else if(q == 6)
    {
        return k != nz-1 ? index1d(i,j,k+1,nx,ny) : index1d(i,j,0,nx,ny);
    }
    else if(q == 7)
    {
        return (i != 0 && j != 0) ? index1d(i-1, j-1, k, nx, ny) :
               (i == 0 && j != 0) ? index1d(nx-1, j-1, k, nx, ny) :
               (i != 0 && j == 0) ? index1d(i-1, ny-1, k, nx, ny) :
               index1d(nx-1, ny-1, k, nx, ny);
    }
    else if(q == 8)
    {
        return (i != nx-1 && j != ny-1) ? index1d(i+1, j+1, k, nx, ny) :
               (i == nx-1 && j != ny-1) ? index1d(0, j+1, k, nx, ny) :
               (i != nx-1 && j == ny-1) ? index1d(i+1, 0, k, nx, ny) :
               index1d(0, 0, k, nx, ny);
    }
    else if(q == 9)
    {
        return (i != 0 && j != ny-1) ? index1d(i-1, j+1, k, nx, ny) :
               (i == 0 && j != ny-1) ? index1d(nx-1, j+1, k, nx, ny) :
               (i != 0 && j == ny-1) ? index1d(i-1, 0, k, nx, ny) :
               index1d(nx-1, 0, k, nx, ny);
    }
    else if(q == 10)
    {
        return (i != nx-1 && j != 0) ? index1d(i+1, j-1, k, nx, ny) :
               (i == nx-1 && j != 0) ? index1d(0, j-1, k, nx, ny) :
               (i != nx-1 && j == 0) ? index1d(i+1, ny-1, k, nx, ny) :
               index1d(0, ny-1, k, nx, ny);
    }
    else if(q == 11)
    {
        return (i != 0 && k != 0) ? index1d(i-1, j, k-1, nx, ny) :
               (i == 0 && k != 0) ? index1d(nx-1, j, k-1, nx, ny) :
               (i != 0 && k == 0) ? index1d(i-1, j, nz-1, nx, ny) :
               index1d(nx-1, j, nz-1, nx, ny);
    }
    else if(q == 12)
    {
        return (i != nx-1 && k != nz-1) ? index1d(i+1, j, k+1, nx, ny) :
               (i == nx-1 && k != nz-1) ? index1d(0, j, k+1, nx, ny) :
               (i != nx-1 && k == nz-1) ? index1d(i+1, j, 0, nx, ny) :
               index1d(0, j, 0, nx, ny);
    }
    else if(q == 13)
    {
        return (i != 0 && k != nz-1) ? index1d(i-1, j, k+1, nx, ny) :
               (i == 0 && k != nz-1) ? index1d(nx-1, j, k+1, nx, ny) :
               (i != 0 && k == nz-1) ? index1d(i-1, j, 0, nx, ny) :
               index1d(nx-1, j, 0, nx, ny);
    }
    else if(q == 14)
    {
        return (i != nx-1 && k != 0) ? index1d(i+1, j, k-1, nx, ny) :
               (i == nx-1 && k != 0) ? index1d(0, j, k-1, nx, ny) :
               (i != nx-1 && k == 0) ? index1d(i+1, j, nz-1, nx, ny) :
               index1d(0, j, nz-1, nx, ny);
    }
    else if(q == 15)
    {
        return (j != 0 && k != 0) ? index1d(i, j-1, k-1, nx, ny) :
               (j == 0 && k != 0) ? index1d(i, ny-1, k-1, nx, ny) :
               (j != 0 && k == 0) ? index1d(i, j-1, nz-1, nx, ny) :
               index1d(i, ny-1, nz-1, nx, ny);
    }
    else if(q == 16)
    {
        return (j != ny-1 && k != nz-1) ? index1d(i, j+1, k+1, nx, ny) :
               (j == ny-1 && k != nz-1) ? index1d(i, 0, k+1, nx, ny) :
               (j != ny-1 && k == nz-1) ? index1d(i, j+1, 0, nx, ny) :
               index1d(i, 0, 0, nx, ny);
    }
    else if(q == 17)
    {
        return (j != 0 && k != nz-1) ? index1d(i, j-1, k+1, nx, ny) :
               (j == 0 && k != nz-1) ? index1d(i, ny-1, k+1, nx, ny) :
               (j != 0 && k == nz-1) ? index1d(i, j-1, 0, nx, ny) :
               index1d(i, ny-1, 0, nx, ny);
    }
    else if(q == 18)
    {
        return (j != ny-1 && k != 0) ? index1d(i, j+1, k-1, nx, ny) :
               (j == ny-1 && k != 0) ? index1d(i, 0, k-1, nx, ny) :
               (j != ny-1 && k == 0) ? index1d(i, j+1, nz-1, nx, ny) :
               index1d(i, 0, nz-1, nx, ny);
    }
    else
    {
        return 0;
    }
}

inline int downwindID(const int q, const int i, const int j, const int k, const int nx, const int ny, const int nz)
{
    if(q == 0)
    {
        return index1d(i,j,k,nx,ny);
    }
    else if(q == 2)
    {
        return i != 0 ? index1d(i-1,j,k,nx,ny) : index1d(nx-1,j,k,nx,ny);
    }
    else if(q == 1)
    {
        return i != nx-1 ? index1d(i+1,j,k,nx,ny) : index1d(0,j,k,nx,ny);
    }
    else if(q == 4)
    {
        return j != 0 ? index1d(i,j-1,k,nx,ny) : index1d(i,ny-1,k,nx,ny);
    }
    else if(q == 3)
    {
        return j != ny-1 ? index1d(i,j+1,k,nx,ny) : index1d(i,0,k,nx,ny);
    }
    else if(q == 6)
    {
        return k != 0 ? index1d(i,j,k-1,nx,ny) : index1d(i,j,nz-1,nx,ny);
    }
    else if(q == 5)
    {
        return k != nz-1 ? index1d(i,j,k+1,nx,ny) : index1d(i,j,0,nx,ny);
    }
    else if(q == 8)
    {
        return (i != 0 && j != 0) ? index1d(i-1, j-1, k, nx, ny) :
               (i == 0 && j != 0) ? index1d(nx-1, j-1, k, nx, ny) :
               (i != 0 && j == 0) ? index1d(i-1, ny-1, k, nx, ny) :
               index1d(nx-1, ny-1, k, nx, ny);
    }
    else if(q == 7)
    {
        return (i != nx-1 && j != ny-1) ? index1d(i+1, j+1, k, nx, ny) :
               (i == nx-1 && j != ny-1) ? index1d(0, j+1, k, nx, ny) :
               (i != nx-1 && j == ny-1) ? index1d(i+1, 0, k, nx, ny) :
               index1d(0, 0, k, nx, ny);
    }
    else if(q == 10)
    {
        return (i != 0 && j != ny-1) ? index1d(i-1, j+1, k, nx, ny) :
               (i == 0 && j != ny-1) ? index1d(nx-1, j+1, k, nx, ny) :
               (i != 0 && j == ny-1) ? index1d(i-1, 0, k, nx, ny) :
               index1d(nx-1, 0, k, nx, ny);
    }
    else if(q == 9)
    {
        return (i != nx-1 && j != 0) ? index1d(i+1, j-1, k, nx, ny) :
               (i == nx-1 && j != 0) ? index1d(0, j-1, k, nx, ny) :
               (i != nx-1 && j == 0) ? index1d(i+1, ny-1, k, nx, ny) :
               index1d(0, ny-1, k, nx, ny);
    }
    else if(q == 12)
    {
        return (i != 0 && k != 0) ? index1d(i-1, j, k-1, nx, ny) :
               (i == 0 && k != 0) ? index1d(nx-1, j, k-1, nx, ny) :
               (i != 0 && k == 0) ? index1d(i-1, j, nz-1, nx, ny) :
               index1d(nx-1, j, nz-1, nx, ny);
    }
    else if(q == 11)
    {
        return (i != nx-1 && k != nz-1) ? index1d(i+1, j, k+1, nx, ny) :
               (i == nx-1 && k != nz-1) ? index1d(0, j, k+1, nx, ny) :
               (i != nx-1 && k == nz-1) ? index1d(i+1, j, 0, nx, ny) :
               index1d(0, j, 0, nx, ny);
    }
    else if(q == 14)
    {
        return (i != 0 && k != nz-1) ? index1d(i-1, j, k+1, nx, ny) :
               (i == 0 && k != nz-1) ? index1d(nx-1, j, k+1, nx, ny) :
               (i != 0 && k == nz-1) ? index1d(i-1, j, 0, nx, ny) :
               index1d(nx-1, j, 0, nx, ny);
    }
    else if(q == 13)
    {
        return (i != nx-1 && k != 0) ? index1d(i+1, j, k-1, nx, ny) :
               (i == nx-1 && k != 0) ? index1d(0, j, k-1, nx, ny) :
               (i != nx-1 && k == 0) ? index1d(i+1, j, nz-1, nx, ny) :
               index1d(0, j, nz-1, nx, ny);
    }
    else if(q == 16)
    {
        return (j != 0 && k != 0) ? index1d(i, j-1, k-1, nx, ny) :
               (j == 0 && k != 0) ? index1d(i, ny-1, k-1, nx, ny) :
               (j != 0 && k == 0) ? index1d(i, j-1, nz-1, nx, ny) :
               index1d(i, ny-1, nz-1, nx, ny);
    }
    else if(q == 15)
    {
        return (j != ny-1 && k != nz-1) ? index1d(i, j+1, k+1, nx, ny) :
               (j == ny-1 && k != nz-1) ? index1d(i, 0, k+1, nx, ny) :
               (j != ny-1 && k == nz-1) ? index1d(i, j+1, 0, nx, ny) :
               index1d(i, 0, 0, nx, ny);
    }
    else if(q == 18)
    {
        return (j != 0 && k != nz-1) ? index1d(i, j-1, k+1, nx, ny) :
               (j == 0 && k != nz-1) ? index1d(i, ny-1, k+1, nx, ny) :
               (j != 0 && k == nz-1) ? index1d(i, j-1, 0, nx, ny) :
               index1d(i, ny-1, 0, nx, ny);
    }
    else if(q == 17)
    {
        return (j != ny-1 && k != 0) ? index1d(i, j+1, k-1, nx, ny) :
               (j == ny-1 && k != 0) ? index1d(i, 0, k-1, nx, ny) :
               (j != ny-1 && k == 0) ? index1d(i, j+1, nz-1, nx, ny) :
               index1d(i, 0, nz-1, nx, ny);
    }
    else
    {
        return 0;
    }
}

void collision(const float omega, const int ic, const int nx, const int ny, const int nz, const std::vector<float>& cx, const std::vector<float>& cy, const std::vector<float>& cz, const std::vector<float>& wt, std::vector<float>& f)
{
    float rho = 0.0;
    float u = 0.0;
    float v = 0.0;
    float w = 0.0;
    for(int q = 0; q < 19; q++)
    {
        int qic = idf(q,ic,nx,ny,nz);

        rho += f[qic];

        u += f[qic]*cx[q];
        v += f[qic]*cy[q];
        w += f[qic]*cz[q];
    }
    u /= rho;
    v /= rho;
    w /= rho;

    for(int q = 0; q < 19; q++)
    {
        float uSqr =u*u+v*v+w*w;
        float uDotC = u*cx[q]+v*cy[q]+w*cz[q];
        float feq = (1.0+3.0*uDotC +4.5*uDotC*uDotC -1.5*uSqr)*wt[q]*rho;

        int qic = idf(q,ic,nx,ny,nz);

        f[qic] += -omega *(f[qic] -feq);
    }
}

void externalForce(const float dpdx, const int ic, const int nx, const int ny, const int nz, const std::vector<float>& cx, const std::vector<float>& cy, const std::vector<float>& cz, const std::vector<float>& wt, std::vector<float>& f)
{
    float rho = 0.0;
    for(int q = 0; q < 19; q++)
    {
        int qic = idf(q,ic,nx,ny,nz);
        rho += f[qic];
    }
    for(int q = 0; q < 19; q++)
    {
        int qic = idf(q,ic,nx,ny,nz);
        f[qic] += rho*wt[q]*3.0*dpdx*cx[q];
    }
}

void cal_rhoUVW(int ic, int nx, int ny, int nz, const std::vector<float>& f, const std::vector<float>& cx, const std::vector<float>& cy, const std::vector<float>& cz, float& rho, float& u, float& v, float& w, float& dpdx)
{
    rho = 0.0;
    u = 0.0;
    v = 0.0;
    w = 0.0;

    for(int q = 0; q < 19; q++)
    {
        int qic = idf(q,ic,nx,ny,nz);
        rho += f[qic];
        u += f[qic]*cx[q];
        v += f[qic]*cy[q];
        w += f[qic]*cz[q];
    }
    u /= rho;
    v /= rho;
    w /= rho;                   
    u += 0.5f*dpdx/rho;
}

void streaming(const int ic, const int i, const int j, const int k,const int nx, const int ny, const int nz, std::vector<float>& ftmp, std::vector<float>& f)
{
    for(int q = 0; q < 19; q++)
    {
        int qic = idf(q,ic,nx,ny,nz);
        int upID = upwindID(q,i,j,k,nx,ny,nz);
        f[qic] = ftmp[idf(q,upID,nx,ny,nz)];
    }
}

void streaming(const int ic, const int i, const int j, const int k,const int nx, const int ny, const int nz, std::vector<float>& ftmp, std::vector<float>& f, const std::vector<int>& upID)
{
    for(int q = 0; q < 19; q++)
    {
        int qic = idf(q,ic,nx,ny,nz);
        f[qic] = ftmp[idf(q,upID[qic],nx,ny,nz)];
    }
}


#endif