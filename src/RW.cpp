/**
 Copyright (c) 2016, Lawrence Livermore National Security, LLC.
 Produced at the Lawrence Livermore National Laboratory
 Written by Harsh Bhatia (bhatia4@llnl.gov).
 CODE-701040.
 All rights reserved.

 This file is part of RobustCriticalPointDetection v1.0.

 For details, see https://github.com/bhatiaharsh/RobustCriticalPointDetection.
 For more details on the Licence, please read LICENCE file.
*/

#include "vec.h"
#include "RW.h"

using namespace std;


// -----------------------------------------------------------------------
// text files

void RW::read_text(std::vector<point> &points, vector<vec> &vfield, string filename, int vdim){

    ifstream infile(filename.c_str());
    if(!infile.is_open()){
        cerr << "Unable to open file "<<filename<<endl;
        exit(1);
    }

    // --------------------------------------
    // read file
    printf(" Read text file %s...", filename.c_str());
    fflush(stdout);

    vfield.clear();
    if(vdim == 3) {

        std::string str;
        do {

            if( ! (infile >> str) )           break;
            double x = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double y = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double z = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double vx = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double vy = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double vz = atof(str.c_str());

            points.push_back( point(x,y,z) );
            vfield.push_back( vec(vx,vy,vz) );

        } while (true);
    }
    else if(vdim == 2) {

        std::string str;
        do {

            if( ! (infile >> str) )           break;
            double x = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double y = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double vx = atof(str.c_str());

            if( ! (infile >> str) )           break;
            double vy = atof(str.c_str());

            points.push_back( point(x,y,0) );
            vfield.push_back( vec(vx,vy,0) );

        } while (true);
    }

    infile.close();
    printf(" Done! Read %'ld vectors and points\n", vfield.size());
}

void RW::read_text(vector<ivec4> &tets, string filename){

    ifstream infile(filename.c_str());
    if(!infile.is_open()){
        cerr << "Unable to open file "<<filename<<endl;
        exit(1);
    }

    // --------------------------------------
    // read file
    printf(" Read text file %s...", filename.c_str());
    fflush(stdout);

    tets.clear();

    std::string str;
    do {

        if( ! (infile >> str) )           break;
        int a = atof(str.c_str());

        if( ! (infile >> str) )           break;
        int b = atof(str.c_str());

        if( ! (infile >> str) )           break;
        int c = atof(str.c_str());

        if( ! (infile >> str) )           break;
        int d = atof(str.c_str());

        tets.push_back( ivec4(a,b,c,d) );
    } while (true);

    infile.close();
    printf(" Done! Read %'ld tets\n", tets.size());
}

void RW::read_text(vector<ivec3> &tris, string filename){

    ifstream infile(filename.c_str());
    if(!infile.is_open()){
        cerr << "Unable to open file "<<filename<<endl;
        exit(1);
    }

    // --------------------------------------
    // read file
    printf(" Read text file %s...", filename.c_str());
    fflush(stdout);

    tris.clear();

    std::string str;
    do {

        if( ! (infile >> str) )           break;
        int a = atoi(str.c_str());

        if( ! (infile >> str) )           break;
        int b = atoi(str.c_str());

        if( ! (infile >> str) )           break;
        int c = atoi(str.c_str());

        tris.push_back( ivec3(a,b,c) );
    } while (true);

    infile.close();
    printf(" Done! Read %'ld tets\n", tris.size());
}

// -----------------------------------------------------------------------
vector<vec> RW::read_plot3d_q(string filename, int &x, int &y, int &z, int &dim){

    ifstream qfile;     qfile.open(filename.c_str());
    if(!qfile.is_open()){
        cerr << "Unable to open file "<<filename<<endl;
        exit(1);
    }

    // --------------------------------------
    // read qfile
    printf(" Read plot3d file %s...", filename.c_str());
    fflush(stdout);

    string str;
    getline(qfile, str);    x = atoi(str.c_str());
    getline(qfile, str);    y = atoi(str.c_str());
    getline(qfile, str);    z = atoi(str.c_str());
    getline(qfile, str);    dim = atoi(str.c_str());

    unsigned int sz = x*y*z;

    vector<vec> vfield(sz);
    for(unsigned int i = 0; i < sz; i++){
    for(unsigned int j = 0; j < 3; j++){
        getline(qfile, str);
        vfield[i][j] = atof(str.c_str());
    }
    }

    qfile.close();
    printf(" Done! Read [%dx%dx%d] = %'ld vectors\n", x, y, z, vfield.size());
    return vfield;
}

vector<point> RW::read_plot3d_x(string filename, int &x, int &y, int &z){

    ifstream xfile;     xfile.open(filename.c_str());
    if(!xfile.is_open()){
        cerr << "Unable to open file "<<filename<<endl;
        exit(1);
    }

    // --------------------------------------
    // read xfile
    printf(" Read plot3d file %s...", filename.c_str());
    fflush(stdout);

    string str;
    getline(xfile, str);    x = atoi(str.c_str());
    getline(xfile, str);    y = atoi(str.c_str());
    getline(xfile, str);    z = atoi(str.c_str());

    unsigned int sz = x*y*z;

    vector<vec> points(sz);
    for(unsigned int j = 0; j < 3; j++){
    for(unsigned int i = 0; i < sz; i++){
        getline(xfile, str);
        points[i][j] = atof(str.c_str());
    }
    }

    xfile.close();
    printf(" Done! Read [%dx%dx%d] = %'ld points", x, y, z, points.size());
    return points;
}

point RW::get_centroid(const ivec4 &tet, const std::vector<point> &points){
    return ( points[tet[0]] + points[tet[1]] + points[tet[2]] + points[tet[3]] ) / 4.0;
}

point RW::get_centroid(const ivec3 &tri, const std::vector<point> &points){
    return ( points[tri[0]] + points[tri[1]] + points[tri[2]] ) / 3.0;
}


// -----------------------------------------------------------------------
// VTK Image file

#ifdef USE_VTK
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataReader.h>

int RW::read_vti(std::vector<size_t> &dims, std::vector<vec> &vfield, vector<point> &points, std::string filename) {

    printf(" Read vti file %s...", filename.c_str());
    fflush(stdout);

    // Read the file
    vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
    reader->SetFileName(filename.c_str());
    reader->Update();

    vtkImageData* idata = reader->GetOutput();
    vtkDataArray* field = idata->GetPointData()->GetVectors();

    int idims[3];
    idata->GetDimensions(idims);

    double origin[3], spacing[3];
    idata->GetOrigin(origin);
    idata->GetSpacing(spacing);

    dims.resize(3);
    for(size_t i=0; i < 3; i++)
        dims[i] = size_t(idims[i]);

    size_t npoints = dims[0]*dims[1]*dims[2];
    vfield.resize(npoints);
    points.resize(npoints);

    for(size_t z = 0; z < dims[2]; z++){
    for(size_t y = 0; y < dims[1]; y++){
    for(size_t x = 0; x < dims[0]; x++){

        size_t idx = x + y*dims[0] + z*dims[0]*dims[1];

        points[idx][0] = origin[0] + float(x)*spacing[0];
        points[idx][1] = origin[1] + float(y)*spacing[1];
        points[idx][2] = origin[2] + float(z)*spacing[2];

        for(uint8_t d = 0; d < 3; d++){
            vfield[idx][d] = field->GetComponent(idx, d);
        }
    }
    }
    }

    printf(" Done! Read %'ld vectors, domain = [%ld x %ld x %ld]\n", vfield.size(), dims[0], dims[1], dims[2]);
    return (dims[2] == 1 ? 2 : 3);
}

#else
int RW::read_vti(std::vector<size_t> &dims, std::vector<vec> &vfield, std::string filename) {
    printf("VTK not available. Please reinstall with VTK libraries!\n");
    exit(1);
}
#endif
// -----------------------------------------------------------------------
