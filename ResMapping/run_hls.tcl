
#open_project -reset rem_base_prj
open_project -reset rem_prj
set_top SubCarrierMapping
add_files ResMapper.cpp -cflags "-I../dmrs -I../"
add_files ../dmrs/dmrs.cpp
add_files -tb SubCarrierMapInputReal
add_files -tb SubCarrierMapInputImag
add_files -tb ResMapMain.cpp -cflags "-I../"
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

csim_design
#csynth_design
#cosim_design
#export_design 
exit
