#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>

using namespace std;

#define SYSTEMTIME clock_t

void OnMult(int m_ar, int m_br)
{

	SYSTEMTIME Time1, Time2;

	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;

	pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for (i = 0; i < m_ar; i++)
		for (j = 0; j < m_ar; j++)
			pha[i * m_ar + j] = (double)1.0;

	for (i = 0; i < m_br; i++)
		for (j = 0; j < m_br; j++)
			phb[i * m_br + j] = (double)(i + 1);

	Time1 = clock();

	for (i = 0; i < m_ar; i++)
	{
		for (j = 0; j < m_br; j++)
		{
			temp = 0;
			for (k = 0; k < m_ar; k++)
			{
				temp += pha[i * m_ar + k] * phb[k * m_br + j];
			}
			phc[i * m_ar + j] = temp;
		}
	}

	Time2 = clock();
	sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
	cout << st;

	// cout << "Result matrix: " << endl;
	// for (i = 0; i < m_ar; i++)
	// {
	// 	for (j = 0; j < min(10, m_br); j++)
	// 		cout << phc[j] << " ";
	// }
	// cout << endl;

	free(pha);
	free(phb);
	free(phc);
}

void OnMultLine(int m_ar, int m_br)
{

	SYSTEMTIME Time1, Time2;

	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;

	pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for (i = 0; i < m_ar; i++)
	{
		for (j = 0; j < min(10, m_br); j++)
			phc[j] = 0;
	}

	for (i = 0; i < m_ar; i++)
		for (j = 0; j < m_ar; j++)
			pha[i * m_ar + j] = (double)1.0;

	for (i = 0; i < m_br; i++)
		for (j = 0; j < m_br; j++)
			phb[i * m_br + j] = (double)(i + 1);

	Time1 = clock();

	for (i = 0; i < m_ar; i++)
	{
		for (j = 0; j < m_br; j++)
		{
			for (k = 0; k < m_ar; k++)
			{
				phc[i * m_ar + k] += pha[i * m_ar + j] * phb[j * m_br + k];
			}
		}
	}

	Time2 = clock();
	sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
	cout << st;

	// cout << "Result matrix: " << endl;
	// for (i = 0; i < m_ar; i++)
	// {
	// 	for (j = 0; j < min(10, m_br); j++)
	// 		cout << phc[j] << " ";
	// }
	// cout << endl;

	free(pha);
	free(phb);
	free(phc);
}

void onMultLineBlockProd(int m_ar, int m_br, int b, int t, int tn, double *pha, double *phb, double *phc)
{

	int blockNo;
	int limBlocks = m_ar * m_br / b;

	for (blockNo = tn; blockNo < limBlocks; blockNo += t)
	{
		int iIni = blockNo * b / m_ar;
		int jIni = blockNo * b % m_ar;
		int start = iIni * m_ar + jIni;

		int lim = start + b;
		if (lim >= sizeof(pha))
			lim = sizeof(pha);

		// cout << "DEBUG || Thread No.: " << tn << " Block No.: " << blockNo << " Index: " << start << " " << lim << " " << iIni << " " << jIni << endl;
		for (int index = start; index < lim; index++)
		{
			int i = index / m_ar;
			int j = index % m_ar;
			for (int k = 0; k < m_ar; k++)
			{
				phc[i * m_ar + k] += pha[i * m_ar + j] * phb[j * m_br + k];
			}
		}
	}
}

void onMultLineBlock(int m_ar, int m_br, int b, int t)
{

	SYSTEMTIME Time1, Time2;

	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;

	pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for (i = 0; i < m_ar; i++)
	{
		for (j = 0; j < min(10, m_br); j++)
			phc[j] = 0;
	}

	for (i = 0; i < m_ar; i++)
		for (j = 0; j < m_ar; j++)
			pha[i * m_ar + j] = (double)1.0;

	for (i = 0; i < m_br; i++)
		for (j = 0; j < m_br; j++)
			phb[i * m_br + j] = (double)(i + 1);

	Time1 = clock();

#pragma omp parallel for
	for (i = 0; i < t; i++)
	{
		onMultLineBlockProd(m_ar, m_br, b, t, i, pha, phb, phc);
	}

	Time2 = clock();
	sprintf(st, "Time: %3.3f seconds\n", (double)(Time2 - Time1) / CLOCKS_PER_SEC);
	cout << st;

	// cout << "Result matrix: " << endl;
	// for (i = 0; i < m_br; i++)
	// {
	// 	for (j = 0; j < min(10, m_br); j++)
	// 		cout << phc[j] << " ";

	// 	cout << endl;
	// }
	// cout << endl;

	free(pha);
	free(phb);
	free(phc);
}

float produtoInterno(float *v1, float *v2, int col)
{
	int i;
	float soma = 0.0;

	for (i = 0; i < col; i++)
		soma += v1[i] * v2[i];

	return (soma);
}

void handle_error(int retval)
{
	printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
	exit(1);
}

void init_papi()
{
	int retval = PAPI_library_init(PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT && retval < 0)
	{
		printf("PAPI library version mismatch!\n");
		exit(1);
	}
	if (retval < 0)
		handle_error(retval);

	std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
			  << " MINOR: " << PAPI_VERSION_MINOR(retval)
			  << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}

int main(int argc, char *argv[])
{

	char c;
	int lin, col, block_size, thread_no, nt = 1;
	int op;

	int EventSet = PAPI_NULL;
	long long values[2];
	int ret;

	ret = PAPI_library_init(PAPI_VER_CURRENT);
	if (ret != PAPI_VER_CURRENT)
		std::cout << "FAIL" << endl;

	ret = PAPI_create_eventset(&EventSet);
	if (ret != PAPI_OK)
		cout << "ERRO: create eventset" << endl;

	ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
	if (ret != PAPI_OK)
		cout << "ERRO: PAPI_L1_DCM" << endl;

	ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
	if (ret != PAPI_OK)
		cout << "ERRO: PAPI_L2_DCM" << endl;

	op = 1;

	if (*argv[1] == '0')
		do
		{
			cout << endl
				 << "1. Multiplication" << endl;
			cout << "2. Line Multiplication" << endl;
			cout << "3. Block Multiplication" << endl;
			cout << "Selection?: ";
			cin >> op;
			if (op == 0)
				break;
			printf("Dimensions: lins cols ? ");
			cin >> lin >> col;

			// Start counting
			ret = PAPI_start(EventSet);
			if (ret != PAPI_OK)
				cout << "ERRO: Start PAPI" << endl;

			switch (op)
			{
			case 1:
				OnMult(lin, col);
				break;
			case 2:
				OnMultLine(lin, col);
				break;
			case 3:
				cout << "Variables: block_size thread_no ? ";
				cin >> block_size >> thread_no;
				onMultLineBlock(lin, col, block_size, thread_no);
			}

			ret = PAPI_stop(EventSet, values);
			if (ret != PAPI_OK)
				cout << "ERRO: Stop PAPI" << endl;
			printf("L1 DCM: %lld \n", values[0]);
			printf("L2 DCM: %lld \n", values[1]);

			ret = PAPI_reset(EventSet);
			if (ret != PAPI_OK)
				std::cout << "FAIL reset" << endl;

		} while (op != 0);
	else
	{
		int iterations = 0;
		int prod_type = atoi(argv[2]);
		int line_start = atoi(argv[3]);
		int col_start = atoi(argv[4]);
		int line_end = atoi(argv[5]);
		int col_end = atoi(argv[6]);
		int increment = atoi(argv[7]);

		switch (prod_type)
		{

		case 1:
			iterations = (line_end - line_start) / increment;
			std::cout << "Iterations: " + to_string(iterations) << endl;

			for (int i = 0; i <= iterations; i++)
			{
				OnMult(line_start + i * increment, col_start + i * increment);
			}
			break;
		case 2:
			iterations = (line_end - line_start) / increment;
			std::cout << "Iterations: " + to_string(iterations) << endl;

			for (int i = 0; i <= iterations; i++)
			{
				OnMultLine(line_start + i * increment, col_start + i * increment);
			}
			break;
		case 3:
			int block_no = *argv[8];
			int thread_no = *argv[9];
			iterations = (line_end - line_start) / increment;
			std::cout << "Iterations: " + to_string(iterations) << endl;

			for (int i = 0; i <= iterations; i++)
			{
				onMultLineBlock(line_start + i * increment, col_start + i * increment, block_no, thread_no);
			}
		}
	}

	ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
	if (ret != PAPI_OK)
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
	if (ret != PAPI_OK)
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_destroy_eventset(&EventSet);
	if (ret != PAPI_OK)
		std::cout << "FAIL destroy" << endl;
}