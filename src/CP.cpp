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

#include "CP.h"

// -----------------------------------------------------------------------
// Initialize SoS
bool CPDetector::createSoS(bool verbose){

#ifndef USE_SOS
    return true;
#else

    if(vfield == 0) {
        printf(" createSOS -- null vector field received!\n");
        return false;
    }
    if(vfield->empty()){
        printf(" createSOS -- empty vector field received!\n");
        return false;
    }
    if(dim != 2 && dim != 3){
        printf(" createSOS -- invalid dimension %d\n", dim);
       return false;
    }

    if(verbose)
        printf(" -------------- Creating SOS Matrix ........... ");

    uint vsz = vfield->size();
    static int lia_count = 1;

    if( !sos_is_down() ){
        sos_shutdown();
        lia_count++;
    }

    // ------------------------------------------
    // initialize SoS just once
    SoS_metadata sm;

    sm.name = NULL;
    sm.title = NULL;
    sm.lines = 0;

    sm.data_size = vsz+1;           // no of values + one for ZERO_IND
    sm.data_dim  = dim;            // dim of points
    //sm.simp_size = H_U.size();    // no of simplices
    //sm.simp_dim  = 1;             // dim of simplices

    sm.fix_w = 15;
    sm.fix_a = 14;

    sm.scale = 1.0;     //ie, no scaling
    sm.decimals = 10;   //ie, int coordinates
    sm.has_weights = 0; //ie, unknown

    // ------------------------------------------
    // test the metadata

    if(verbose){
        // 0 <= a < w < MAX
        if (!( (0 <= sm.fix_a) && (sm.fix_a < sm.fix_w) && (sm.fix_w <= MAX_DECIMALS) ) )
            printf("\n SoS Metadata Error: invalid #fix=%d.%d\n", sm.fix_w, sm.fix_a );

        if( sm.data_size == 0 || sm.data_dim == 0 )
            printf("\n SoS Metadata Error: invalid data. size = %d, dim = %d\n", sm.data_size, sm.data_dim );

        if (sm.fix_w != 0){

            print ("(#fix=%d.%d) ", sm.fix_w, sm.fix_a);
            sm.decimals = sm.fix_w;
            sm.scale = sm.scale / powf(10, (double) sm.fix_a);
            //sm.scale = sm.scale / exp10 ((double) sm.fix_a);
            printf("scale = %.15f\n", sm.scale);
        }
    }
    // ------------------------------------------

   //printf("initialize now!\n");
   sos_matrix (sm.data_size, sm.data_dim, sm.scale,
               Lia_DIGITS (5 * sm.decimals + 3),
               Lia_DIGITS (2 * sm.decimals + 1));

   //printf(" lia_limit now!!\n");
   lia_stack_limit( lia_count*((sm.data_size+1) * sm.data_dim));

    // --------------------
   for(uint v = 0; v < vsz; v++){
   for(uint d = 0; d < sm.data_dim; d++){
      SoSUtils::ffp_param_push2 (v+1, d+1, SoSUtils::float_to_fixed(vfield->at(v)[d], sm.fix_a), sm.fix_w, sm.fix_a);
      //printf(" adding to SoS [%d][%d] %f %f\n", v+1, d+1, vfield->at(v)[d], SoSUtils::float_to_fixed(vfield->at(v)[d], sm.fix_a));
   }
   }

    // give the last index to zero
   SOS_ZERO_IDX = sm.data_size;

   for(uint d = 0; d < sm.data_dim; d++){
       //printf(" -- adding 0 to SoS [%d][%d] \n", ZERO_IND, d+1);
      SoSUtils::ffp_param_push2 (SOS_ZERO_IDX, d+1, 0.0, sm.fix_w, sm.fix_a);
   }
   return true;
#endif
}


float CPDetector::sign (const point &p1, const point &p2, const point &p3){
    return 0;
}

#ifndef USE_SOS
bool CPDetector::point_in_triangle(const point &p, const point &a, const point &b, const point &c) {

    std::cerr << "CPDetector::point_in_triangle not implemented without USE_SOS!\n";
    return false;
#if 0
    bool b1, b2, b3;

    b1 = sign(p, a, b) < 0.0f;
    b2 = sign(p, b, c) < 0.0f;
    b3 = sign(p, v, a) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
#endif
}

bool CPDetector::point_in_tetrahedron(const point &p, const point &a, const point &b, const point &c, const point &d) {
    std::cerr << "CPDetector::point_in_tetrahedron not implemented without USE_SOS!\n";
    return false;
}
#endif

void CPDetector::compute() {

    printf("\n Detecting %dD Critical Points..", this->dim);
    fflush(stdout);

    if(dim == 3 && tets != 0) {

        for(uint t = 0; t < tets->size(); t++){

            const ivec4 &tet = tets->at(t);

#ifdef USE_SOS
            bool cp_found = SoSUtils::point_in_tet(SOS_ZERO_IDX, tet[0]+1, tet[1]+1, tet[2]+1, tet[3]+1);
#else
            bool cp_found = point_in_tetrahedron(point(0,0,0), vfield[tet[0]], vfield[tet[1]], vfield[tet[2]], vfield[tet[3]]);
#endif
            if(cp_found){
                cp.push_back(t);
            }
        }
    }

    else if(dim == 2 && tris != 0) {

        for(uint t = 0; t < tris->size(); t++){

            const ivec3 &tri = tris->at(t);

#ifdef USE_SOS
            bool cp_found = SoSUtils::point_in_triangle(SOS_ZERO_IDX, tri[0]+1, tri[1]+1, tri[2]+1);
#else
            bool cp_found = point_in_triangle(point(0,0,0), vfield[tri[0]], vfield[tri[1]], vfield[tri[2]]);
#endif
            if(cp_found){
                cp.push_back(t);
            }
        }
    }

    printf(" Detected %ld simplices with critical points!\n", cp.size());
}
