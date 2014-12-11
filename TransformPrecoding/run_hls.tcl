
#open_project -reset tp_base_prj
open_project -reset tp_prj
set_top TransformPrecoding
#set_top TransformDecoding
add_files TransformPrecoder.cpp -cflags "-I../"
add_files dft.cpp -cflags "-I../"
add_files -tb TransformPreMain.cpp -cflags "-I../"
add_files -tb TransformPrecoderInputReal
add_files -tb TransformPrecoderInputImag
add_files -tb ../GeneralFunc.cpp -cflags "-I../"
add_files -tb ../gauss.cpp -cflags "-I../"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7vx980tffg1930-2}
create_clock -period 10 -name default

csim_design
csynth_design
#cosim_design
#export_design 
exit
