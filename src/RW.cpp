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
    printf(" Done! Read [%dx%dx%d] = %'d points", x, y, z, points.size());
    return points;
}
