
open_project -reset rm_prj
set_top RxRateMatching
add_files RateMatcher.cpp -cflags "-I../ -I../lib"
add_files SubblockInterleaver_lte.cpp -cflags "-I../ -I../lib"
add_files -tb RateMatcherMain.cpp -cflags "-I../ -I../lib"
#add_files -tb TxRateMatchInput
#add_files -tb RxRateMatchInput
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../ -I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../ -I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

csim_design -argv {0}
#csynth_design
#cosim_design -trace_level none -argv {0}
#export_design 
exit
