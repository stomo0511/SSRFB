//
//  SSRFB Tuning
//
//  Created by T. Suzuki on 2015/06/16.
//  Copyright (c) 2013 T. Suzuki. All rights reserved.
//

#include <omp.h>
#include <cstdlib>
#include <cassert>

#include <CoreBlasTile.hpp>
#include <TMatrix.hpp>

#define MAX_DIVS 100
#define MAX_THREADS 8
#define MAX_DIST  20
#define TRIES 5

using namespace std;

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: LA0 [natural number]\n";
        return EXIT_FAILURE;
    }

    int nb = atoi(argv[1]);
    int count = 0;
    int ibs[MAX_DIVS];

    for (int i=1; i<=nb; i++)
    {
        if ( nb % i == 0 )
        {
            ibs[count] = i;
            count++;
        }
    }

//    cout << "NB = " << nb << " : ";
//    for (unsigned int i=0; i<count; i++)
//        cout << ibs[i] << ", ";
//    cout << endl;

    /////////////////////////////////////////

    double time;

    //    cout << "b,s,time\n";

    for (int c = 0; c<TRIES; c++)
    {
        for (int i = 0; i<count; i++)
        {
            if ((ibs[i] >= 10) && (ibs[i] != nb))
            {
            	cout << nb << "," << ibs[i] << "," << flush;

                TMatrix A( 2*nb, (MAX_THREADS+1)*nb, nb, nb, ibs[i]);
                TMatrix T( 2*ibs[i], (MAX_THREADS+1)*nb, ibs[i], nb, ibs[i]);

                A.Set_Rnd( 20160616 );

				#pragma omp parallel private(time)
                {
					#pragma omp single
                	{
                		GEQRT( A(0,0), T(0,0) );
                	}

                	// Timer start
                	time = omp_get_wtime();

					#pragma omp for
                	for (int j=1; j<=MAX_THREADS; j++)
                	{
                		LARFB( PlasmaLeft, PlasmaTrans, A(0,0), T(0,0), A(0,j) );
                	}

                	// Timer stop
                	time = omp_get_wtime() - time;
//                time /= MAX_THREADS;
                	if (omp_get_thread_num() == 0)
                		cout << time << endl;
                }

//              A.~TMatrix();
//              T.~TMatrix();
            }
            // if (ibs[i] >= 0) END
        }
        // i-loop END
    }
    // c-loop END

//    cout << endl << nb << ", " << min_ib << ", " << min_time << endl;

    return EXIT_SUCCESS;
}
