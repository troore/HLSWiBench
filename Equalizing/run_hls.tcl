
open_project -reset equalizer_prj
set_top Equalizing
add_files Equalizer.cpp -cflags "-I../ -I../lib"
add_files ../dmrs/dmrs.cpp -cflags "-I../ -I../lib"
add_files -tb EqualizerMain.cpp -cflags "-I../ -I../lib -I../ResMapping -I../OFDM"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lib/check.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"
add_files -tb ../ResMapping/ResMapper_baseline.cpp -cflags "-I../ -I../lib"
add_files -tb ../fft/fft_baseline.cpp
add_files -tb ../OFDM/OFDM_baseline_nrvs_same_array_cyclic.cpp -cflags "-I../ -I../lib"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

csim_design -argv {0}
#csynth_design
#cosim_design  -trace_level none -argv {4}
#export_design 
exit
