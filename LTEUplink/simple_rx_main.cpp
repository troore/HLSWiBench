
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_rx.h"

#include "lte_phy.h"
#include "GeneralFunc.h"

//#include "check.h"

LTE_PHY_PARAMS lte_phy_params;

void test_equalizer_chain(LTE_PHY_PARAMS *lte_phy_params)
{
//	GeneRandomInput(lte_phy_params->resm_in, lte_phy_params->resm_in_buf_sz, "SubCarrierMapInputReal", "SubCarrierMapInputImag");
	GeneRandomInput(lte_phy_params->ofmod_out, lte_phy_params->ofmod_out_buf_sz, "ModulationInputReal", "ModulationInputImag");
	equalizer_chain(lte_phy_params);
	WriteOutputToFiles(lte_phy_params->eq_out, lte_phy_params->eq_out_buf_sz, "testLSCELSEqOutputReal", "testLSCELSEqOutputImag");
}

void test_equalizer_indep(LTE_PHY_PARAMS *lte_phy_params)
{}

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

	test_equalizer_chain(&lte_phy_params);

	/*
	strcpy(tx_in_fname, "SubCarrierMapInputReal");
	strcpy(rx_out_fname, "testLSCELSEqOutputReal");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	
	strcpy(tx_in_fname, "SubCarrierMapInputImag");
	strcpy(rx_out_fname, "testLSCELSEqOutputImag");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	*/

	return 0;
}
