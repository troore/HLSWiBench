
#open_project -reset rem_base_prj
open_project -reset rem_base_prj
set_top SubCarrierMapping
add_files ResMapper_base.cpp -cflags "-I../ -I../lib"
add_files ../dmrs/dmrs.cpp -cflags "-I../ -I../lib"
#add_files -tb SubCarrierMapInputReal
#add_files -tb SubCarrierMapInputImag
add_files -tb ResMapMain.cpp -cflags "-I../ -I../lib"
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

csim_design -argv {0}
#csynth_design
#cosim_design -trace_level none -argv {0}
#export_design 
exit
