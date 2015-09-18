
open_project -reset scrb_prj
set_top Scrambling
add_files Scrambler.cpp -cflags "-I../ -I../lib"
add_files -tb ScrambMain.cpp -cflags "-I../ -I../lib"
#add_files -tb ScrambleInput
#add_files -tb testDescrambleOutput
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lib/check.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

#source "./mod_prj/solution1/directives.tcl"
csim_design -argv {0}
csynth_design
#cosim_design -trace_level none -argv {0}
export_design 
exit
