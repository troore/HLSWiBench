
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "GeneralFunc.h"

#include "Scrambler.h"
#include "check.h"

#define Scramb

LTE_PHY_PARAMS lte_phy_params;

void test_scrambling(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Tx scrambling starts" << std::endl;

//	ReadInputFromFiles(lte_phy_params->scramb_in, lte_phy_params->scramb_in_buf_sz, "../testsuite/ScrambleInput");
	GeneRandomInput(lte_phy_params->scramb_in, lte_phy_params->scramb_in_buf_sz, "ScrambleInput");

	Scrambling(lte_phy_params->scramb_in, lte_phy_params->scramb_out, lte_phy_params->scramb_in_buf_sz);

	WriteOutputToFiles(lte_phy_params->scramb_out, lte_phy_params->scramb_out_buf_sz, "testScrambleOutput");

	std::cout << "Tx scrambling ends" << std::endl;
}

void test_descrambling(LTE_PHY_PARAMS *lte_phy_params)
{
	
	std::cout << "Rx descrambling starts" << std::endl;

	float *rx_scramb_in = (float *)malloc(lte_phy_params->descramb_in_buf_sz * sizeof(float));
	float *rx_scramb_out = (float *)malloc(lte_phy_params->descramb_out_buf_sz * sizeof(float));
	int i;

	ReadInputFromFiles(lte_phy_params->descramb_in, lte_phy_params->descramb_in_buf_sz, "testScrambleOutput");
//	GeneRandomInput(lte_phy_params->descramb_in, lte_phy_params->descramb_in_buf_sz, "ScrambleOutput");
//	ReadInputFromFiles(rx_scramb_in, in_buf_sz, "testScrambleOutput");
	
	for (i = 0; i < lte_phy_params->descramb_in_buf_sz; i++)
	{
		if (lte_phy_params->descramb_in[i] == 0)
			rx_scramb_in[i] = -1.0;
		else
			rx_scramb_in[i] = 1.0;
	 }
	
//	Descrambling(lte_phy_params, lte_phy_params->descramb_in, lte_phy_params->descramb_out);
	Descrambling(rx_scramb_in, rx_scramb_out, lte_phy_params->descramb_in_buf_sz);
	
	for (i = 0; i < lte_phy_params->descramb_out_buf_sz; i++)
	{
		if (rx_scramb_out[i] > 0)
			lte_phy_params->descramb_out[i] = 1.0;
		else
			lte_phy_params->descramb_out[i] = 0.0;
	}

	WriteOutputToFiles(lte_phy_params->descramb_out, lte_phy_params->descramb_out_buf_sz, "testDescrambleOutput");
	
	cout << "Rx descrambling ends" << endl;

}

int main(int argc, char *argv[])
{
	int fs_id;
	int n_tx_ant, n_rx_ant;
	int mod_type;

	char tx_in_fname[50];
	char rx_out_fname[50];
	int err_n;
	
	if (argc != 5 && argc != 2)
	{
		lte_phy_usage_info(argv[0]);
		
		return 1;
	}

	if (argc == 5)
	{
		fs_id = atoi(argv[1]);
		mod_type = atoi(argv[2]);
		n_tx_ant = atoi(argv[3]);
		n_rx_ant = atoi(argv[4]);

		lte_phy_init(&lte_phy_params, fs_id, mod_type, n_tx_ant, n_rx_ant);
	}
	else
	{
		fs_id = atoi(argv[1]);

		lte_phy_init(&lte_phy_params, fs_id);
	}

//#ifdef Scramb
	test_scrambling(&lte_phy_params);
//#else
	test_descrambling(&lte_phy_params);
	strcpy(tx_in_fname, "ScrambleInput");
	strcpy(rx_out_fname, "testDescrambleOutput");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
//#endif

	return 0;
}
