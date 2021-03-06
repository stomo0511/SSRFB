//
//  SSRFB Tuning
//
//  Created by T. Suzuki on 2015/06/16.
//  Copyright (c) 2013 T. Suzuki. All rights reserved.
//

#include <omp.h>
#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <cassert>
#include <sys/time.h>

#include <CoreBlasTile.hpp>
#include <TMatrix.hpp>

#define MAX_DIVS 100     // Maximum number of candidates of inner block size
#define MAX_THREADS 8
#define TRIES 5          // # of measurement

using namespace std;

//#define DEBUG

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: SS [tile size]\n";
        return EXIT_FAILURE;
    }
    int nb = atoi(argv[1]);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Make the candidates of inner block size
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    int count = 0;       // # of candidates of inner block size
    int ibs[MAX_DIVS];   // array for candidates of inner block size

    for (int i=1; i<=nb; i++)  // tile size is divisible by inner block size
    {
        if ( nb % i == 0 )
        {
            ibs[count] = i;
            count++;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
	int mt = omp_get_max_threads();
	#ifdef DEBUG
    cout << "omp_max_t = " << mt << endl;
	#endif
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // measure timer precision
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    double wtick = omp_get_wtick();
	#ifdef DEBUG
    cout << "omp_wtick = " << wtick << endl;
	#endif
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    double time;
    static double ttime;
	#pragma omp threadprivate(ttime)

    for (int c = 0; c<TRIES+1; c++)
    {
        for (int i = 0; i<count; i++)
        {
            if ((ibs[i] >= 10) && (ibs[i] != nb))
            {
				#pragma omp master
            	time = 0.0;

                TMatrix A( 2*nb, (MAX_THREADS+1)*nb, nb, nb, ibs[i]);
                TMatrix T( 2*ibs[i], (MAX_THREADS+1)*nb, ibs[i], nb, ibs[i]);

                A.Set_Rnd(20180423);

				#pragma omp parallel
                {
					#pragma omp single
                	{
                		GEQRT( A(0,0), T(0,0) );
                	}

					#pragma omp for
                	for (int j=1; j<=MAX_THREADS; j++)
                	{
                		LARFB( PlasmaLeft, PlasmaTrans, A(0,0), T(0,0), A(0,j) );
                	}

					#pragma omp single
                	{
                		TSQRT( A(0,0), A(1,0), T(1,0) );
                	}

					#pragma omp barrier

                	// Timer start
                	ttime = omp_get_wtime();

					#pragma omp for
                	for (int j=1; j<=MAX_THREADS; j++)
                	{
                		SSRFB( PlasmaLeft, PlasmaTrans, A(1,0), T(1,0), A(0,j), A(1,j) );
                	}

                	// Timer stop
                	ttime = omp_get_wtime() - ttime;

					#pragma omp barrier
					#pragma omp critical
                	{
                		if (time < ttime)
                			time = ttime;
                	}
                }
                if (c != 0)
                	cout << nb << "," << ibs[i] << "," << time << endl;
            }  // if (ibs[i] >= 0) END
        }  // i-loop END
    }   // c-loop END

    return EXIT_SUCCESS;
}
