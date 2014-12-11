
#open_project -reset scrb_base_prj
open_project -reset scrb_prj
set_top Scrambling
add_files Scrambler.cpp -cflags "-I../"
add_files -tb ScrambMain.cpp -cflags "-I../"
add_files -tb ScrambleInput
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

#source "./mod_prj/solution1/directives.tcl"
csim_design
#csynth_design
#cosim_design
#export_design 
exit
