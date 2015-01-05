create_clock -name ap_clk -period 10 [get_ports ap_clk]

# Since this sample design is not meant to be stand alone, but exist in a larger
# design, there are no input/output delays specified. Timing will not be done 
# on these interfaces by default.
