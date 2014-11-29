
#include "ModMain.h"

//#define Mod

//int RANDOMSEED;

LTE_PHY_PARAMS lte_phy_params;


void test_mod(LTE_PHY_PARAMS *lte_phy_params, int mod_type)
{
	std::cout << "Modulation starts" << std::endl;
	
//	GeneRandomInput(lte_phy_params->mod_in, lte_phy_params->mod_in_buf_sz, "MoodulationRandomInput");
	ReadInputFromFiles(lte_phy_params->mod_in, lte_phy_params->mod_in_buf_sz, "ModulationInput");

	Modulating(lte_phy_params->mod_in, lte_phy_params->mod_out, lte_phy_params->mod_in_buf_sz, mod_type);
	
	WriteOutputToFiles(lte_phy_params->mod_out, lte_phy_params->mod_out_buf_sz, "testModulationOutputReal", "testModulationOutputImag");

	std::cout << "Modulation ends" << std::endl;

}

void test_demod(LTE_PHY_PARAMS *lte_phy_params, int mod_type)
{
		
	std::cout << "Demodulation starts" << std::endl;

	float awgn_sigma = 0.193649; //this value is for the standard input  see "AWGNSigma"
	
	ReadInputFromFiles(lte_phy_params->demod_in, lte_phy_params->demod_in_buf_sz, "testModulationOutputReal", "testModulationOutputImag");

	Demodulating(lte_phy_params->demod_in, lte_phy_params->demod_LLR, lte_phy_params->demod_in_buf_sz, mod_type, awgn_sigma);

	int tmp[N_MOD_IN_MAX];
	for (int i = 0; i < lte_phy_params->demod_out_buf_sz; i++)
		tmp[i] = (lte_phy_params->demod_LLR[i] > 0.0 ? 1 : 0);

//	WriteOutputToFiles(lte_phy_params->demod_LLR, lte_phy_params->demod_out_buf_sz, "testDemodulationOutput");
	WriteOutputToFiles(tmp, lte_phy_params->demod_out_buf_sz, "testDemodulationOutput");

	std::cout << "Demodulation ends" << std::endl;
}

int main(int argc, char *argv[])
{

	int enum_fs;
	int n_tx_ant, n_rx_ant;
	int mod_type;

	/*
	if (argc != 5)
	{
		printf("Usage: ./a.out enum_fs mod_type n_tx_ant n_rx_ant\n");
		
		return 1;
	}
	
	enum_fs = atoi(argv[1]);
	mod_type = atoi(argv[2]);
	n_tx_ant = atoi(argv[3]);
	n_rx_ant = atoi(argv[4]);
	*/	
	enum_fs = 0;
	mod_type = 2;
	n_tx_ant = 2;
	n_rx_ant = 2;
	
	lte_phy_init(&lte_phy_params, enum_fs, mod_type, n_tx_ant, n_rx_ant);

#ifdef Mod

	test_mod(&lte_phy_params, mod_type);
	
	#else

//	for (int i = 0; i < 100; i++)
		test_demod(&lte_phy_params, mod_type);

	#endif
	
	return 0;
}

