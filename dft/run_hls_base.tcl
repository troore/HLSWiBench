
#set N 8

open_project -reset dft_base_prj
set_top dft
#add_files dft_base.cpp -cflags "-DN=$N"
#add_files -tb dft_main.cpp -cflags "-DN=$N"
#add_files dft_base.cpp -cflags "-DN=75"
#add_files -tb dft_main.cpp -cflags "-DN=75"
add_files dft_base.cpp
add_files -tb dft_main.cpp

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

#csim_design -argv {$N}
csim_design -argv {150}
csynth_design
cosim_design -trace_level none -argv {150}
#export_design 
exit
