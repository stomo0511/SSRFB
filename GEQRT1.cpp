//
//  GEQRT Tuning
//
//  Created by T. Suzuki on 2016/02/23.
//  Copyright (c) 2013 T. Suzuki. All rights reserved.
//

#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <plasma.h>
#include <core_blas.h>
#include "Matrix.h"

#define MAX_DIVS 100
#define TRIES 5

using namespace std;

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: GE1 [natural number]\n";
        return EXIT_FAILURE;
    }

    int nb = atoi(argv[1]);
    int count = 0;
    int ibs[MAX_DIVS];

    // Sieve of Eratosthenes 
    for (int i=1; i<=nb; i++)
    {
        if ( nb % i == 0 )
        {
            ibs[count] = i;
            count++;
        }
    }

    #ifdef DEBUG
    cout << "Max threads = " << omp_get_max_threads() << endl;
    cout << "Timer precision = " << omp_get_wtick() << endl;
    cout << "NB = " << nb << " : ";
    for (unsigned int i=0; i<count; i++)
        cout << ibs[i] << ", ";
    cout << endl;
    #endif

    /////////////////////////////////////////

    double min_time = 1000.0;
    int min_ib = 0;

    for (int c = 0; c<TRIES; c++)
    {
        Matrix<double> A1( nb, nb );
        Matrix<double> T1( nb, nb );

        double* WORK = new double[ nb*nb ];
        double* TAU = new double[ nb ];

        A1.Set_Rnd(20160220);


    	for (int i = 0; i<count; i++)
    	{
        if ((ibs[i] >= 10) && (ibs[i] != nb))
        {
            core_dgeqrt( nb, nb, ibs[i],
                         A1.top(), nb,
                         T1.top(), nb, TAU, WORK );

            A1.Set_Rnd(20180220);

            // Timer start
            double time = omp_get_wtime();

            core_dgeqrt( nb, nb, ibs[i],
                         A1.top(), nb,
                         T1.top(), nb, TAU, WORK );

            // Timer stop
            time = omp_get_wtime() - time;
            cout << nb << "," << ibs[i] << "," << time << endl;

            #pragma omp critical
            if (time < min_time)
            {
                min_time = time;
                min_ib = ibs[i];
            }
        }
        // if (ibs[i] >= 0) END
    }
    // i-loop END

//    	A1.~Matrix();
//    	A2.~Matrix();
//    	C1.~Matrix();
//    	C2.~Matrix();
//    	T1.~Matrix();
//    	T2.~Matrix();

        delete [] WORK;
        delete [] TAU;

    }
    // c-loop END

//    cout << endl << nb << ", " << min_ib << ", " << min_time << endl;


    return EXIT_SUCCESS;
}
