
open_project -reset fft_no_pragmas_prj
set_top fft_nrvs_two_arrays
add_files fft_no_pragmas.cpp -cflags "-I../"
#add_files ../fft.cpp -cflags "-I../"
#add_files -tb OFDMMain.cpp -cflags "-I../"
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

csim_design
csynth_design
cosim_design
#export_design 
exit
