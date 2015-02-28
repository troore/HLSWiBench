
open_project -reset dft_prj
set_top	dft
add_files dft.cpp
add_files -tb dft_main.cpp

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

csim_design -argv {600}
csynth_design
cosim_design -trace_level none -argv {600}
#export_design 
exit
