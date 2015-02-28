
open_project -reset turbodec_2_prj
set_top turbo_decoding
add_files Turbo.cpp -cflags "-I../ -I../lib"
add_files -tb TurboMain.cpp -cflags "-I../ -I../lib"
#add_files -tb testTurboEncoderOutput
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lib/check.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

#source "./mod_prj/solution1/directives.tcl"
csim_design -argv {2}
csynth_design
cosim_design -trace_level none -argv {2}
#export_design 
exit
