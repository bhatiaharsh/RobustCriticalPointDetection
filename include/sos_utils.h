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

#ifndef SOS_UTILS_H
#define SOS_UTILS_H

#include <vector>
#include <fstream>
#include <iostream>

extern "C" {
    #include "basic.h"
    #include "lia.h"
    #include "sos.h"
}


using namespace std;
#define MAX_DECIMALS  16
/* Maximum number of significant decimal digits of "#fix" input coordinates.
   NOTE: 64-bit IEEE 745 floating-point roughly corresponds to 16 significant
   decimal digits. */


// --------------------------------------------------------------------
// data stucture to store meta-data about the data
typedef struct SoS_metadata_struct
{
  // info about data name
  char *name, *title;
  int lines;

  // info about data size
  int data_size;    // no of points
  int data_dim;     // dim of points
  //int simp_size;    // no of simplices
  //int simp_dim;     // dim of simplices

  // info about data precision
  int decimals;
  int fix_w, fix_a;
  double scale;
  int has_weights;

} SoS_metadata;


// --------------------------------------------------------------------

// Utilities to push data to SoS Matrix
// copied from "dt.c"

class SoSUtils{

public:

    // check if the edge vj-vk intersects the halfline whose left end-point is given by vi
    static int intersect_halfline( int vi, int vj, int vk ){

        int result, s, d;

        // correct if vj vk are out of order
        if( sos_smaller( vk, 2, vj, 2 ) )
            return intersect_halfline( vi, vk, vj );

        // ---------------
        if( sos_smaller( vj, 2, vi, 2 ) && sos_smaller( vi, 2, vk, 2 ) ){

            s = basic_isort3 (&vi, &vj, &vk);
            d = sos_lambda3 (vi, vj, vk) -> signum;
            result = If (Odd (s), (d == -1), (d == 1));

            return result;
        }
        else return 0;
    }

    static int is_smaller(int va, int i, int vb, int j){

        if( va > vb )
            return is_smaller(vb, j, va, i);

        return sos_smaller(va, i, vb, j);
    }

    static double float_to_fixed(double value, int d){

        char str[50];
        sprintf (str, "%0.*f", d, value);
        return atof( str );
    }
    static vector<double> float_to_fixed( const vector<double> &sc_values, int d ){

        int sz = sc_values.size();

        vector<double> fx_sc_values;
        fx_sc_values.resize(sz);

        for( int i = 0; i < sz; i++ ){
            fx_sc_values[i] = SoSUtils::float_to_fixed(sc_values[i], d);
        }
    }

    static double get_max( const vector<double> &sc_values ){

        double max = 0.0;

        for( int i = 0; i < sc_values.size(); i++ ){

            double this_val = fabs( sc_values[i] );
            if( this_val > max )
                    max = this_val;
        }
    }
    static void get_wa(double max, int &w, int &a){

        // count no of digits in this maximum
        int n = 0;
        while( max > 1 ){

                max = max/10.0;
                n++;
        }

        if( n > 10 ){
            cout<<"SoSUtils::get_wa() - Error. n = "<<n<<endl;
            return;
        }

        if( n == 0 )
            n = 1;

        int d = 16-n;

        a = d;
        w = d+n;
    }

    static void int_param_push2 (int i, int j, int x)
    {
      Lia lx[3];  /* 32-bit int, 10 decimal digits, ceiling(10/8) + 1 == 3 */
      lia_load (lx, x);
      sos_param (i, j, lx);
      lia_push (lx);
      lia_ipower (2);
    }
    static void ffp_param_push2 (int i, int j, double x, int w, int a)
    {
      Lia lx[Lia_DIGITS(MAX_DECIMALS)];
      lia_ffpload (lx, w, a, x);
      sos_param (i, j, lx);
      lia_push (lx);
      lia_ipower (2);
    }
    static void push_zeros(int i, int w, int a){
        ffp_param_push2 (i, 1, 0.0, w, a);
        ffp_param_push2 (i, 2, 0.0, w, a);
        ffp_param_push2 (i, 3, 0.0, w, a);
    }

    static int point_in_triangle(int p, int v1, int v2, int v3 ){

        //printf(" point in triangle (%d %d %d %d)\n", p, v1, v2, v3);
        int count = 0;

        if( intersect_halfline (p, v1, v2) )    count++;
        if( intersect_halfline (p, v2, v3) )    count++;
        if( intersect_halfline (p, v3, v1) )    count++;

        if ( Odd( count) )
            return 1;

        return 0;
    }
    static int point_in_tet(int p, int v1, int v2, int v3, int v4 ){

        //printf(" point in tet (%d %d %d %d %d)\n", p, v1, v2, v3, v4);
        int D0 = sos_positive3( v1, v2, v3, v4 );

        if( D0 != sos_positive3( p, v2, v3, v4 ) )
            return 0;

        if( D0 != sos_positive3( v1, p, v3, v4 ) )
            return 0;

        if( D0 != sos_positive3( v1, v2, p, v4 ) )
            return 0;

        if( D0 != sos_positive3( v1, v2, v3, p ) )
            return 0;

        return 1;
    }


    static vector<double> convert_float_to_fixed( const vector<double> &sc_values, int &w, int &a, double max ){
        SoSUtils::get_wa(max, w, a);
        return SoSUtils::float_to_fixed(sc_values, a);
    }
    static vector<double> convert_float_to_fixed( const vector<double> &sc_values, int &w, int &a ){
        return SoSUtils::convert_float_to_fixed( sc_values, w, a, SoSUtils::get_max(sc_values) );
    }
};


#endif // SOS_UTILS_H
