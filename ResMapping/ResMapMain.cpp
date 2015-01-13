
#include <iostream>

#include "gauss.h"
#include "GeneralFunc.h"
#include "check.h"

#include "ResMapper.h"

#define SCMapper
	
LTE_PHY_PARAMS lte_phy_params;

void test_SCMapper(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Resource mapping starts" << std::endl;

//	ReadInputFromFiles(lte_phy_params->resm_in, lte_phy_params->resm_in_buf_sz, "../testsuite/SubCarrierMapInputReal", "../testsuite/SubCarrierMapInputImag");
//	GeneRandomInput(lte_phy_params->resm_in_real, lte_phy_params->resm_in_imag, lte_phy_params->resm_in_buf_sz, "../testsuite/SubCarrierMapInputReal", "../testsuite/SubCarrierMapInputImag");
	GeneRandomInput(lte_phy_params->resm_in, lte_phy_params->resm_in_buf_sz, "SubCarrierMapInputReal", "SubCarrierMapInputImag");

	SubCarrierMapping(lte_phy_params->resm_in, lte_phy_params->resm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_tx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);
	
//	SubCarrierMapping(lte_phy_params, lte_phy_params->resm_in_real, lte_phy_params->resm_in_imag, lte_phy_params->resm_out_real, lte_phy_params->resm_out_imag);

	WriteOutputToFiles(lte_phy_params->resm_out, lte_phy_params->resm_out_buf_sz, "testSubCarrierMapOutputReal", "testSubCarrierMapOutputImag");
//	WriteOutputToFiles(lte_phy_params->resm_out_real, lte_phy_params->resm_out_imag, lte_phy_params->resm_out_buf_sz, "../testsuite/testSubCarrierMapOutputReal", "../testsuite/testSubCarrierMapOutputImag");
	
	std::cout << "Resource mapping ends" << std::endl;
}

void test_SCDemapper(LTE_PHY_PARAMS *lte_phy_params)
{
	std::cout << "Resource demapping starts" << std::endl;
	
//	int in_buf_sz, out_buf_sz;

//	in_buf_sz = lte_phy_params->N_tx_ant * lte_phy_params->N_fft_sz * lte_phy_params->N_symb_per_subfr;
	
//	ReadInputFromFiles(lte_phy_params->resdm_in, lte_phy_params->resdm_in_buf_sz, "SubCarrierDemapInputReal", "SubCarrierDemapInputImag");
	GeneRandomInput(lte_phy_params->resdm_in, lte_phy_params->resdm_in_buf_sz, "SubCarrierDemapInputReal", "SubCarrierDemapInputImag");
	
	SubCarrierDemapping(lte_phy_params->resdm_in, lte_phy_params->resdm_out, lte_phy_params->dmrs_symb_pos, lte_phy_params->N_rx_ant, lte_phy_params->sc_loc, lte_phy_params->N_fft_sz, lte_phy_params->N_dft_sz);

	WriteOutputToFiles(lte_phy_params->resdm_out, lte_phy_params->resdm_out_buf_sz, "testSubCarrierDemapOutputReal", "testSubCarrierDemapOutputImag");

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

#ifdef SCMapper
	
	test_SCMapper(&lte_phy_params);

#else

	test_SCDemapper(&lte_phy_params);

	/*
	strcpy(tx_in_fname, "../testsuite/transformprecoderinputreal");
	strcpy(rx_out_fname, "../testsuite/testtransformdecoderoutputreal");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	
	strcpy(tx_in_fname, "../testsuite/TransformPrecoderInputImag");
	strcpy(rx_out_fname, "../testsuite/testTransformDecoderOutputImag");
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
	*/

#endif
	
	return 0;
}




