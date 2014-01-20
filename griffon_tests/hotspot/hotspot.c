#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#ifdef _GFN
#include <gfn.h>
#endif
#define STR_SIZE	256

/* maximum power density possible (say 300W for a 10mm x 10mm chip)	*/
#define MAX_PD	(3.0e6)
/* required precision in degrees	*/
#define PRECISION	0.001
#define SPEC_HEAT_SI 1.75e6
#define K_SI 100
/* capacitance fitting factor	*/
#define FACTOR_CHIP	0.5
#define OPEN

/* chip parameters	*/
double t_chip = 0.0005;
double chip_height = 0.016;
double chip_width = 0.016;
/* ambient temperature, assuming no package at all	*/
double amb_temp = 80.0;

long long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

/* Single iteration of the transient solver in the grid model.
 * advances the solution of the discretized difference equations 
 * by one time step
 */
void single_iteration(double **result, double **temp, double **power, int row, int col,
					  double Cap, double Rx, double Ry, double Rz, 
					  double step)
{
	double delta, temprc;
	int r, c;
	int rN, rS, cW, cE;

    #pragma gfn parallel_for private(rN, rS, cW, cE, delta) \
    	pcopy(temp[0:row{partition}][0:col]) in_pattern(temp:[-1,1][-1,1]) \
    	pcopyin(power[0:row{partition}][0:col]) \
    	pcopyout(result[0:row{partition}][0:col])
	for (r = 0; r < row; r++) {
		#pragma gfn loop
		for (c = 0; c < col; c++) {
			rN = (r==0)? r : r-1;
			rS = (r==row-1)? r : r+1;
			cW = (c==0)? c : c-1;
			cE = (c==col-1)? c : c+1;
			
			temprc = temp[r][c];
			
			delta = (step / Cap) * (power[r][c] + 
					(temp[rS][c] + temp[rN][c] - 2.0*temprc) / Ry + 
					(temp[r][cE] + temp[r][cW] - 2.0*temprc) / Rx + 
					(amb_temp - temprc) / Rz);
  			
			/*	Update Temperatures	*/
			result[r][c] =temprc+ delta;
		}
	}

	#pragma gfn parallel_for pcopyin(result[0:row][0:col]) \
		pcopyout(temp[0:row][0:col])
	for (r = 0; r < row; r++) {
		#pragma gfn loop
		for (c = 0; c < col; c++) {
			temp[r][c]=result[r][c];
		}
	}
}

/* Transient solver driver routine: simply converts the heat 
 * transfer differential equations to difference equations 
 * and solves the difference equations by iterating
 */
void compute_tran_temp(double **result, int num_iterations, double **temp, double **power, int row, int col) 
{
	int i = 0;

	double grid_height = chip_height / row;
	double grid_width = chip_width / col;

	double Cap = FACTOR_CHIP * SPEC_HEAT_SI * t_chip * grid_width * grid_height;
	double Rx = grid_width / (2.0 * K_SI * t_chip * grid_height);
	double Ry = grid_height / (2.0 * K_SI * t_chip * grid_width);
	double Rz = t_chip / (K_SI * grid_height * grid_width);

	double max_slope = MAX_PD / (FACTOR_CHIP * t_chip * SPEC_HEAT_SI);
	double step = PRECISION / max_slope;
	double t;
	
	long long time0, time1;

	#ifdef VERBOSE
	fprintf(stdout, "total iterations: %d s\tstep size: %g s\n", num_iterations, step);
	fprintf(stdout, "Rx: %g\tRy: %g\tRz: %g\tCap: %g\n", Rx, Ry, Rz, Cap);
	#endif

	time0 = get_time();

#pragma gfn data copy(temp[0:row{partition}][0:col]) \
    copyin(power[0:row{partition}][0:col]) \
    create(result[0:row{partition}][0:col])
{
     for (i = 0; i < num_iterations ; i++)
	{
		#ifdef VERBOSE
		fprintf(stdout, "iteration %d\n", i++);
		#endif
		single_iteration(result, temp, power, row, col, Cap, Rx, Ry, Rz, step);
	}	
}	
	
	time1 = get_time();
	printf("%.12f s : TOTAL TIME\n", (float) (time1-time0) / 1000000);

	#ifdef VERBOSE
	fprintf(stdout, "iteration %d\n", i++);
	#endif
}

void fatal(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	exit(1);
}

void read_input(double *vect, int grid_rows, int grid_cols, char *file)
{
  	int i, index;
	FILE *fp;
	char str[STR_SIZE];
	double val;

	fp = fopen (file, "r");
	if (!fp)
		fatal ("file could not be opened for reading");

	for (i=0; i < grid_rows * grid_cols; i++) {
		fgets(str, STR_SIZE, fp);
		if (feof(fp))
			fatal("not enough lines in file");
		if ((sscanf(str, "%lf", &val) != 1) )
			fatal("invalid file format");
		vect[i] = val;
	}

	fclose(fp);	
}

void usage(int argc, char **argv)
{
	fprintf(stderr, "Usage: %s <grid_rows> <grid_cols> <sim_time> <no. of threads><temp_file> <power_file>\n", argv[0]);
	fprintf(stderr, "\t<grid_rows>  - number of rows in the grid (positive integer)\n");
	fprintf(stderr, "\t<grid_cols>  - number of columns in the grid (positive integer)\n");
	fprintf(stderr, "\t<sim_time>   - number of iterations\n");
	fprintf(stderr, "\t<temp_file>  - name of the file containing the initial temperature values of each cell\n");
	fprintf(stderr, "\t<power_file> - name of the file containing the dissipated power values of each cell\n");
	exit(1);
}

int main(int argc, char **argv)
{
	int grid_rows, grid_cols, sim_time, i, j;
	double **temp, **power, **result;
	char *tfile, *pfile;
	FILE *outf;
	
	/* check validity of inputs	*/
	if (argc != 6)
		usage(argc, argv);
	if ((grid_rows = atoi(argv[1])) <= 0 ||
		(grid_cols = atoi(argv[2])) <= 0 ||
		(sim_time = atoi(argv[3])) <= 0
		)
		usage(argc, argv);

	/* allocate memory for the temperature and power arrays	*/
	temp = (double **) malloc (grid_rows * sizeof(double*));
	temp[0] = (double *) malloc (grid_rows * grid_cols * sizeof(double));
	power = (double **) malloc (grid_rows * sizeof(double*));
	power[0] = (double *) malloc (grid_rows * grid_cols * sizeof(double));
	result = (double **) malloc (grid_rows * sizeof(double*));
	result[0] = (double *) malloc (grid_rows * grid_cols * sizeof(double));
	for (i = 1; i < grid_rows; i++) {
		temp[i] = temp[i-1] + grid_cols;
		power[i] = power[i-1] + grid_cols;
		result[i] = result[i-1] + grid_cols;
	}
	
	if(!temp || !power)
		fatal("unable to allocate memory");

	/* read initial temperatures and input power	*/
	tfile = argv[4];
	pfile = argv[5];
	read_input(temp[0], grid_rows, grid_cols, tfile);
	read_input(power[0], grid_rows, grid_cols, pfile);

	printf("Start computing the transient temperature\n");
	compute_tran_temp(result,sim_time, temp, power, grid_rows, grid_cols);
	printf("Ending simulation\n");
	/* output results	*/
#ifdef VERBOSE
	fprintf(stdout, "Final Temperatures:\n");
#endif

	outf = fopen("result.txt", "w");
	for(i=0; i < grid_rows; i++)
		for(j=0; j < grid_cols; j++)
			fprintf(outf, "%d\t%g\n", i*grid_cols+j, temp[i][j]);
	fclose(outf);

	/* cleanup	*/
	free(temp);
	free(power);

	return 0;
}

