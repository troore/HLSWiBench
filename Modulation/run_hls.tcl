
open_project -reset demod_prj
set_top Demodulating
add_files Modulation.cpp -cflags "-I../ -I../lib"
add_files -tb ModMain.cpp -cflags "-I../ -I../lib"
#add_files -tb ModulationInput
#add_files -tb testModulationOutputReal
#add_files -tb testModulationOutputImag
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

#source "./mod_prj/solution1/directives.tcl"
csim_design -argv {0}
#csynth_design
#cosim_design -trace_level none -argv {0}
#export_design 
exit
