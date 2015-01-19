
#open_project -reset tp_base_prj
open_project -reset tp_prj
set_top TransformPrecoding
#set_top TransformDecoding
add_files TransformPrecoder.cpp -cflags "-I../ -I../lib"
add_files ../dft/dft.cpp -cflags "-I../ -I../lib"
add_files -tb TransformPreMain.cpp -cflags "-I../ -I../lib"
#add_files -tb TransformPrecoderInputReal
#add_files -tb TransformPrecoderInputImag
add_files -tb ../lib/GeneralFunc.cpp -cflags "-I../lib"
add_files -tb ../lib/gauss.cpp -cflags "-I../lib"
add_files -tb ../lte_phy.cpp -cflags "-I../"

open_solution -reset "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default

#csim_design -argv {0}
csynth_design
cosim_design -trace_level none -argv {0}
export_design 
exit
