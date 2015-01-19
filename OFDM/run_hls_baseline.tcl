
open_project -reset ofmod_baseline_fix_prj
set_top ofmodulating
add_files OFDM_baseline.cpp -cflags "-I../ -I../lib"
add_files ../fft/fft_base.cpp -cflags "-I../ -I../lib"
add_files -tb OFDMMain.cpp -cflags "-I../ -I../lib"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

csim_design -argv {4}
#csynth_design
#cosim_design -trace_level none -argv {0}
#export_design 
exit
