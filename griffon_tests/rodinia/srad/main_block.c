//====================================================================================================100
//		UPDATE
//====================================================================================================100

//    2006.03   Rob Janiczek
//        --creation of prototype version
//    2006.03   Drew Gilliam
//        --rewriting of prototype version into current version
//        --got rid of multiple function calls, all code in a  
//         single function (for speed)
//        --code cleanup & commenting
//        --code optimization efforts   
//    2006.04   Drew Gilliam
//        --added diffusion coefficent saturation on [0,1]
//		2009.12 Lukasz G. Szafaryn
//		-- reading from image, command line inputs
//		2010.01 Lukasz G. Szafaryn
//		--comments

//====================================================================================================100
//	DEFINE / INCLUDE
//====================================================================================================100

#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifdef _GFN
#include <gfn.h>
#endif

#include "define.c"
#include "graphics.c"
#include "resize.c"
#include "timer.c"

//====================================================================================================100
//====================================================================================================100
//	MAIN FUNCTION
//====================================================================================================100
//====================================================================================================100

#define MIN(A,B) (((A)>(B))?(B):(A))

int main(int argc, char *argv []){

	//================================================================================80
	// 	VARIABLES
	//================================================================================80

	// time
	long long time0;
	long long time1;
#ifdef _DEBUG
	long long time6;
	long long time7;
	long long time8;
	long long time9;
	long long time10;
	long long time11;
#endif

    // inputs image, input paramenters
    fp* image_ori;																// originalinput image
	int image_ori_rows;
	int image_ori_cols;
	long image_ori_elem;

    // inputs image, input paramenters
    fp** image;															// input image
    long Nr,Nc;													// IMAGE nbr of rows/cols/elements
	long Ne;

	// algorithm parameters
    int niter;																// nbr of iterations
    fp lambda;															// update step size

    // size of IMAGE
	int r1,r2,c1,c2;												// row/col coordinates of uniform ROI
	long NeROI;														// ROI nbr of elements
    
    // ROI statistics
    fp meanROI, varROI, q0sqr;											//local region statistics

    // center pixel value
    fp Jc;

	// directional derivatives
	fp **dN,**dS,**dW,**dE;
    
    // calculation variables
    fp tmp,sum,sum2;
    fp G2,L,num,den,qsqr,D;
       
    // diffusion coefficient
    fp **c; 
	fp cN,cS,cW,cE;
    
    // counters
    int iter;   // primary loop
    long i,j;    // image row/col
    long k;      // image single index    
    int BLOCK_SIZE;
    long i0, j0;

	// number of threads
	char *filename;
	char fileinname[100];
	char fileoutname[100];

	int iN, iS, jW, jE;

	//================================================================================80
	// 	GET INPUT PARAMETERS
	//================================================================================80

	if(argc != 6){
		printf("ERROR: wrong number of arguments\n");
		return 0;
	}
	else{
		niter = atoi(argv[1]);
		lambda = atof(argv[2]);
		Nr = atoi(argv[3]);						// it is 502 in the original image
		Nc = atoi(argv[4]);						// it is 458 in the original image
		filename = argv[5];
		strcpy(fileinname, filename);
		//strcpy(fileoutname, "out_");
		//strcat(fileoutname, filename);
        BLOCK_SIZE = atoi(argv[6]);
	}

	//================================================================================80
	// 	READ IMAGE (SIZE OF IMAGE HAS TO BE KNOWN)
	//================================================================================80

    // read image
	image_ori_rows = Nr;
	image_ori_cols = Nc;
	image_ori_elem = image_ori_rows * image_ori_cols;

	image_ori = (fp*)malloc(sizeof(fp) * image_ori_elem);

	read_graphics(	fileinname,
								image_ori,
								image_ori_rows,
								image_ori_cols,
								1);

	//================================================================================80
	// 	RESIZE IMAGE (ASSUMING COLUMN MAJOR STORAGE OF image_orig)
	//================================================================================80

	Ne = Nr*Nc;

	image = (fp**)malloc(sizeof(fp*) * Nr);
	image[0] = (fp*)malloc(sizeof(fp) * Ne);
	for (i = 1; i < Nr; ++i)
		image[i] = image[i-1] + Nc;

	resize(	image_ori,
				image_ori_rows,
				image_ori_cols,
				image[0],
				Nr,
				Nc,
				1);

	//================================================================================80
	// 	SETUP
	//================================================================================80

    r1     = 0;											// top row index of ROI
    r2     = Nr - 1;									// bottom row index of ROI
    c1     = 0;											// left column index of ROI
    c2     = Nc - 1;									// right column index of ROI

    // ROI image size    
    NeROI = (r2-r1+1)*(c2-c1+1);											// number of elements in ROI, ROI size
    
	// allocate variables for directional derivatives
	dN = (fp**)malloc(sizeof(fp*)*Nr) ;
    dN[0] = (fp*)malloc(sizeof(fp)*Ne);
    dS = (fp**)malloc(sizeof(fp*)*Nr) ;
    dS[0] = (fp*)malloc(sizeof(fp)*Ne);
    dW = (fp**)malloc(sizeof(fp*)*Nr) ;
    dW[0] = (fp*)malloc(sizeof(fp)*Ne);
    dE = (fp**)malloc(sizeof(fp*)*Nr) ;
    dE[0] = (fp*)malloc(sizeof(fp)*Ne);
    for (i = 1; i < Nr; ++i) {
    	dN[i] = dN[i-1] + Nc;
    	dS[i] = dS[i-1] + Nc;
    	dW[i] = dW[i-1] + Nc;
    	dE[i] = dE[i-1] + Nc;
    }

	// allocate variable for diffusion coefficient
    c  = (fp**)malloc(sizeof(fp*)*Nr) ;											// diffusion coefficient
    c[0] = (fp*)malloc(sizeof(fp)*Ne);
    for (i = 1; i < Nr; ++i)
    	c[i] = c[i-1] + Nc;

	time0 = get_time();

{
#ifdef _DEBUG
	time6 = get_time();
#endif

	//================================================================================80
	// 	SCALE IMAGE DOWN FROM 0-255 TO 0-1 AND EXTRACT
	//================================================================================80

    for (i0 = 0; i0 < Nr; i0 += BLOCK_SIZE)
    for (j0 = 0; j0 < Nc; j0 += BLOCK_SIZE)
    {
        for (i = i0; i < MIN(Nr, i0+BLOCK_SIZE-1); i++)
            for (j = j0; j < MIN(Nc, j0+BLOCK_SIZE-1); j++)
                image[i][j] = exp(image[i][j]/255);											// exponentiate input IMAGE and copy to output image
    }

#ifdef _DEBUG
    time7 = get_time();
#endif

	//================================================================================80
	// 	COMPUTATION
	//================================================================================80

	// printf("iterations: ");

    // primary loop
    for (iter=0; iter<niter; iter++){										// do for the number of iterations input parameter

		// printf("%d ", iter);
		// fflush(NULL);

        // ROI statistics for entire ROI (single number for ROI)
        sum=0; 
		sum2=0;
        for (i0 = 0; i0 < Nr; i0 += BLOCK_SIZE)
        for (j0 = 0; j0 < Nc; j0 += BLOCK_SIZE)
        {
            for (i = i0; i < MIN(Nr, i0+BLOCK_SIZE-1); i++) {
                for (j = j0; j < MIN(Nc, j0+BLOCK_SIZE-1); j++) {
                    tmp   = image[i][j];										// get coresponding value in IMAGE
                    sum  += tmp ;												// take corresponding value and add to sum
                    sum2 += tmp*tmp;											// exponentiate input IMAGE and copy to output image
                }
            }
        }
        meanROI = sum / NeROI;												// gets mean (average) value of element in ROI
        varROI  = (sum2 / NeROI) - meanROI*meanROI;							// gets variance of ROI
        q0sqr   = varROI / (meanROI*meanROI);								// gets standard deviation of ROI

#ifdef _DEBUG
        time8 = get_time();
#endif

        // directional derivatives, ICOV, diffusion coefficent
        for (i0 = 0; i0 < Nr; i0 += BLOCK_SIZE)
        for (j0 = 0; j0 < Nc; j0 += BLOCK_SIZE)
        {
            for (i = i0; i < MIN(Nr, i0+BLOCK_SIZE-1); i++) {
                for (j = j0; j < MIN(Nc, j0+BLOCK_SIZE-1); j++) {

                    Jc = image[i][j];													// get value of the current element
                    
                    iN = (i==0)? 0 : i-1;
                    iS = (i==Nr-1)? Nr-1 : i+1;
                    jW = (j==0)? 0 : j-1;
                    jE = (j==Nc-1)? Nc-1 : j+1;
                    
                    fp tdN, tdS, tdW, tdE, cij;

                    // directional derivates (every element of IMAGE)
                    tdN = image[iN][j] - Jc;								// north direction derivative
                    tdS = image[iS][j] - Jc;								// south direction derivative
                    tdW = image[i][jW] - Jc;								// west direction derivative
                    tdE = image[i][jE] - Jc;								// east direction derivative

                    // normalized discrete gradient mag squared (equ 52,53)
                    G2 = (tdN*tdN + tdS*tdS								// gradient (based on derivatives)
                        + tdW*tdW + tdE*tdE) / (Jc*Jc);

                    // normalized discrete laplacian (equ 54)
                    L = (tdN + tdS + tdW + tdE) / Jc;					// laplacian (based on derivatives)
                    
                    dN[i][j] = tdN;
                    dS[i][j] = tdS;
                    dW[i][j] = tdW;
                    dE[i][j] = tdE;

                    // ICOV (equ 31/35)
                    num  = (0.5*G2) - ((1.0/16.0)*(L*L)) ;						// num (based on gradient and laplacian)
                    den  = 1 + (.25*L);											// den (based on laplacian)
                    qsqr = num/(den*den);										// qsqr (based on num and den)
    
                    // diffusion coefficent (equ 33) (every element of IMAGE)
                    den = (qsqr-q0sqr) / (q0sqr * (1+q0sqr)) ;					// den (based on qsqr and q0sqr)
                    cij = 1.0 / (1.0+den) ;									// diffusion coefficient (based on den)

                    // saturate diffusion coefficent to 0-1 range
                    if (cij < 0)												// if diffusion coefficient < 0
                        {c[i][j] = 0;}												// ... set to 0
                    else if (cij > 1)													// if diffusion coefficient > 1
                        {c[i][j] = 1;}												// ... set to 1
                    else
                        {c[i][j] = cij;}
                }
            }
        }

#ifdef _DEBUG
        time9 = get_time();
#endif

        // divergence & image update
        for (i0 = 0; i0 < Nr; i0 += BLOCK_SIZE)
        for (j0 = 0; j0 < Nc; j0 += BLOCK_SIZE)
        {
            for (i = i0; i < MIN(Nr, i0+BLOCK_SIZE-1); i++) {
                for (j = j0; j < MIN(Nc, j0+BLOCK_SIZE-1); j++) {

                    iS = (i==Nr-1)? Nr-1 : i+1;
                    jE = (j==Nc-1)? Nc-1 : j+1;

                    // diffusion coefficent
                    cN = c[i][j];													// north diffusion coefficient
                    cS = c[iS][j];										// south diffusion coefficient
                    cW = cN;													// west diffusion coefficient
                    cE = c[i][jE];										// east diffusion coefficient

                    // divergence (equ 58)
                    D = cN*dN[i][j] + cS*dS[i][j] + cW*dW[i][j] + cE*dE[i][j];				// divergence

                    // image update (equ 61) (every element of IMAGE)
                    image[i][j] = image[i][j] + 0.25*lambda*D;								// updates image (based on input time step and divergence)
                }
            }
        }

#ifdef _DEBUG
        time10 = get_time();
#endif

	}

	// printf("\n");

	//================================================================================80
	// 	SCALE IMAGE UP FROM 0-1 TO 0-255 AND COMPRESS
	//================================================================================80

    for (i0 = 0; i0 < Nr; i0 += BLOCK_SIZE)
    for (j0 = 0; j0 < Nc; j0 += BLOCK_SIZE)
    {
        for (i = i0; i < MIN(Nr, i0+BLOCK_SIZE-1); i++) 
            for (j = j0; j < MIN(Nc, j0+BLOCK_SIZE-1); j++) 
			image[i][j] = log(image[i][j])*255;													// take logarithm of image, log compress
	}

#ifdef _DEBUG
	time11 = get_time();
#endif

} /* end acc data */

	time1 = get_time();

	//================================================================================80
	// 	WRITE IMAGE AFTER PROCESSING
	//================================================================================80

	/*write_graphics(	fileoutname,
								image[0],
								Nr,
								Nc,
								1,
								255);*/

	//================================================================================80
	// 	DEALLOCATE
	//================================================================================80

	sum = 0.0;
	for (i=0;i<Nr;i++) 
		for (j=0;j<Nc;j++)
			sum += image[i][j];

	free(image_ori);
	free(image[0]);
	free(image);

	free(dN[0]); free(dS[0]); free(dW[0]); free(dE[0]);
    free(dN); free(dS); free(dW); free(dE);									// deallocate directional derivative memory
    free(c[0]);
    free(c);																// deallocate diffusion coefficient memory

	//================================================================================80
	//		DISPLAY TIMING
	//================================================================================80

	printf("final mean : %3.3lf\n", sum/Ne);
	printf("iteration : %d\n", niter);
	printf("input size : %ld x %ld\n", Nr, Nc);
	printf("compute time : %.12f s\n", (float)(time1-time0)/1000000);

#ifdef _DEBUG
	printf("\n");
	printf("copyin time : %.12f s\n", (float)(time6-time0)/1000000);
	printf("exp time : %.12f s\n", (float)(time7-time6)/1000000);
	printf("reduce time : %.12f s\n", (float)(time8-time7)/1000000);
	printf("kernel 1 time : %.12f s\n", (float)(time9-time8)/1000000);
	printf("kernel 2 time : %.12f s\n", (float)(time10-time9)/1000000);
	printf("log time : %.12f s\n", (float)(time11-time10)/1000000);
	printf("copyout time : %.12f s\n", (float)(time1-time11)/1000000);
#endif

//====================================================================================================100
//	END OF FILE
//====================================================================================================100

	return 0;
}

