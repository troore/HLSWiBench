
open_project -reset fft_no_pragmas_prj
set_top fft_nrvs_two_arrays
add_files fft_no_pragmas.cpp
add_files -tb fft_main.cpp

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

csim_design -argv {512}
csynth_design
cosim_design -trace_level none -argv {512}
#export_design 
exit
