
open_project -reset equalizer_baseline_prj
set_top Equalizing
add_files Equalizer_baseline.cpp -cflags "-I../"
add_files dmrs.cpp -cflags "-I../"
add_files -tb EqualizerMain.cpp -cflags "-I../"
add_files -tb LSCELSEqInputReal
add_files -tb LSCELSEqInputImag
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
