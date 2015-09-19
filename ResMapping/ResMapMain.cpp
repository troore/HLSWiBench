
#include <iostream>
#include <string.h>

#include "gauss.h"
#include "GeneralFunc.h"
#include "Equalizer.h"
#include "ResMapper.h"
#include "OFDM.h"

#include "check.h"

//#define SCMapper
	
LTE_PHY_PARAMS lte_phy_params;

void test_SCMapper(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Resource mapping starts" << std::endl;

	GeneRandomInput(lte_phy_params->resm_in, lte_phy_params->resm_in_buf_sz, "SubCarrierMapInputReal", "SubCarrierMapInputImag");

	SubCarrierMapping(lte_phy_params->resm_in, lte_phy_params->resm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_tx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);
	ofmodulating(lte_phy_params->resm_out, lte_phy_params->ofmod_out, lte_phy_params->N_tx_ant, lte_phy_params->N_fft_sz, lte_phy_params->N_samps_cp_l_0);
	
	WriteOutputToFiles(lte_phy_params->ofmod_out, lte_phy_params->ofmod_out_buf_sz, "testModulationOutputReal", "testModulationOutputImag");
	
	std::cout << "Resource mapping ends" << std::endl;
}

void test_SCDemapper(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Resource demapping starts" << std::endl;
	
	ReadInputFromFiles(lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_in_buf_sz, "testModulationOutputReal", "testModulationOutputImag");

	ofdemodulating(lte_phy_params->ofdemod_in, lte_phy_params->ofdemod_out, lte_phy_params->N_rx_ant, lte_phy_params->N_fft_sz, lte_phy_params->N_samps_cp_l_0);
	
	SubCarrierDemapping(lte_phy_params->ofdemod_out, lte_phy_params->resdm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_rx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);

	Equalizing(lte_phy_params->resdm_out, lte_phy_params->eq_out, lte_phy_params->N_dft_sz, lte_phy_params->N_tx_ant, lte_phy_params->N_rx_ant);
	
	WriteOutputToFiles(lte_phy_params->eq_out, lte_phy_params->eq_out_buf_sz, "testLSCELSEqOutputReal", "testLSCELSEqOutputImag");

	std::cout << "Resource demapping ends" << std::endl;
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

//#ifdef SCMapper
	
	test_SCMapper(&lte_phy_params);

//#else

	test_SCDemapper(&lte_phy_params);

//#endif

	strcpy(tx_in_fname, "SubCarrierMapInputReal");
	strcpy(rx_out_fname, "testLSCELSEqOutputReal");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	
	strcpy(tx_in_fname, "SubCarrierMapInputImag");
	strcpy(rx_out_fname, "testLSCELSEqOutputImag");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);

	
	return 0;
}




