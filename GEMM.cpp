//
//  GEMM Performance check
//
//  Created by T. Suzuki on 2019/02/22.
//

#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <cassert>

#include <core_blas.h>

#include "Tile.h"

#define MAX_DIVS 100
#define MAX_THREADS 4
#define TRIES 5

using namespace std;

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: GM [natural number]\n";
        return EXIT_FAILURE;
    }

    const int nb = atoi(argv[1]);
    int count = 0;

//    cout << "NB = " << nb << " : ";
    /////////////////////////////////////////

    double time;
    double min_time = 1000.0;

//	cout << "b,time\n";

    for (int c = 0; c<TRIES; c++)
    {
//    	cout << nb << "," << flush;

    	Tile<double> A( nb, nb );
    	Tile<double> B( nb, nb );
    	Tile<double> C( nb, nb );

    	const int M = C.m();
    	const int N = C.n();
    	const int K = A.n();

    	const double alp = 1.0;
    	const double beta = 1.0;

    	A.Set_Rnd( 2019022201 );
    	B.Set_Rnd( 2019022202 );
    	C.Set_Rnd( 2019022203 );

    	core_dgemm( PlasmaNoTrans, PlasmaNoTrans,
    			M, N, K,
				alp, A.top(), A.m(), B.top(), B.m(),
				beta, C.top(), C.m() );

    	// Timer start
    	time = omp_get_wtime();

    	core_dgemm( PlasmaNoTrans, PlasmaNoTrans,
    	    		M, N, K,
    				alp, A.top(), A.m(), B.top(), B.m(),
    				beta, C.top(), C.m() );

    	// Timer stop
    	time = omp_get_wtime() - time;
//    	time /= MAX_THREADS;
//    	cout << time << endl;
    	if (time < min_time)
    	{
    		min_time = time;
    	}

//    	A.~Matrix();
//    	B.~Matrix();
//    	C.~Matrix();
    }
    // c-loop END

    cout << nb << "," << min_time << endl;

    return EXIT_SUCCESS;
}
