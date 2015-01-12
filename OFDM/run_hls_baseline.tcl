
open_project -reset ofmod_baseline_prj4_two_arrays_iter
set_top ofmodulating_two_arrays
add_files OFDM_baseline.cpp -cflags "-I../"
add_files fft_baseline.cpp -cflags "-I../"
add_files -tb OFDMMain.cpp -cflags "-I../"
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

#csim_design
csynth_design
#cosim_design
export_design 
exit
