
open_project -reset fft_prj
set_top fft_nrvs_two_arrays
add_files fft.cpp
add_files -tb fft_main.cpp

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

csim_design -argv {2048}
csynth_design
cosim_design -trace_level none -argv {2048}
#export_design 
exit
