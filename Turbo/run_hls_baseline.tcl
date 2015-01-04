
open_project -reset turbodec_baseline_prj
set_top turbo_decoding
add_files Turbo_baseline.cpp -cflags "-I../"
add_files -tb TurboMain.cpp -cflags "-I../"
add_files -tb testTurboEncoderOutput
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

#source "./mod_prj/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
#export_design 
exit
