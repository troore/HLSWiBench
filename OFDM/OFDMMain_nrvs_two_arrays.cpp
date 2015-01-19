
#include <iostream>
#include <string.h>

#include "gauss.h"
#include "GeneralFunc.h"

#include "OFDM.h"
#include "lte_phy.h"
//#include "check.h"

#define MOD

LTE_PHY_PARAMS lte_phy_params;

void test_mod(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "OFDM modulation starts" << std::endl;

	//GeneRandomInput(lte_phy_params->ofmod_in, lte_phy_params->ofmod_in_buf_sz, "RandomModulationInputReal", "RandomModulationInputImag");
	GeneRandomInput(lte_phy_params->ofmod_in_real, lte_phy_params->ofmod_in_imag, lte_phy_params->ofmod_in_buf_sz, "RandomModulationInputReal", "RandomModulationInputImag");

	printf("%d\n", lte_phy_params->ofmod_in_buf_sz);

	//ofmodulating(lte_phy_params, lte_phy_params->ofmod_in, lte_phy_params->ofmod_out);
	ofmodulating_two_arrays(lte_phy_params, lte_phy_params->ofmod_in_real, lte_phy_params->ofmod_in_imag, lte_phy_params->ofmod_out_real, lte_phy_params->ofmod_out_imag);
	
	//WriteOutputToFiles(lte_phy_params->ofmod_out, lte_phy_params->ofmod_out_buf_sz, "testModulationOutputReal", "testModulationOutputImag");
	WriteOutputToFiles(lte_phy_params->ofmod_out_real, lte_phy_params->ofmod_out_imag, lte_phy_params->ofmod_out_buf_sz, "testModulationOutputReal", "testModulationOutputImag");

	std::cout << "OFDM modulation ends" << std::endl;
}

void test_demod(LTE_PHY_PARAMS *lte_phy_params)
{
	
	std::cout <<"OFDM demodulation starts"<< std::endl;

	GeneRandomInput(lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_in_buf_sz, "RandomDemodulationInputReal", "RandomDemodulationInputImag");
//	ReadInputFromFiles(lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_in_buf_sz, "testModulationOutputReal", "testModulationOutputImag");
//	ReadInputFromFiles(lte_phy_params->ofdemod_in_real, lte_phy_params->ofdemod_in_imag, lte_phy_params->ofdemod_in_buf_sz, "testModulationOutputReal", "testModulationOutputImag");

	ofdemodulating(lte_phy_params, lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out);
//	ofdemodulating(lte_phy_params, lte_phy_params->ofdemod_in_real, lte_phy_params->ofdemod_in_imag, lte_phy_params->ofdemod_out_real, lte_phy_params->ofdemod_out_imag);

	WriteOutputToFiles(lte_phy_params->ofdemod_out, lte_phy_params->ofdemod_out_buf_sz, "testDemodulationOutputReal", "testDemodulationOutputImag");
//	WriteOutputToFiles(lte_phy_params->ofdemod_out_real, lte_phy_params->ofdemod_out_imag, lte_phy_params->ofdemod_out_buf_sz, "testDemodulationOutputReal", "testDemodulationOutputImag");
	
	std::cout << "OFDM demodulation ends" << std::endl;
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

#ifdef MOD
	
	test_mod(&lte_phy_params);
	
#else

	test_demod(&lte_phy_params);

	/*
	strcpy(tx_in_fname, "RandomModulationInputReal");
	strcpy(rx_out_fname, "testDemodulationOutputReal");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	
	strcpy(tx_in_fname, "RandomModulationInputImag");
	strcpy(rx_out_fname, "testDemodulationOutputImag");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	*/

#endif
	
	return 0;
}


