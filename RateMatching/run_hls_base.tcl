
open_project -reset ratematch_rx_base_prj
set_top RxRateMatching
add_files RateMatcher_base.cpp -cflags "-I../"
add_files SubblockInterleaver_lte_base.cpp -cflags "-I../"
add_files -tb RateMatcherMain.cpp -cflags "-I../"
add_files -tb TxRateMatchInput
add_files -tb RxRateMatchInput
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

#csim_design
csynth_design
cosim_design
export_design 
exit
