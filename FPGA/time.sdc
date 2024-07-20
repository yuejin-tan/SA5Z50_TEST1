create_clock -period 40 -waveform {0 20} -name {CLK_IN_25M_PIN} {CLK_IN_25M_PIN}

# create_generated_clock {PLL_inst1/PLLInst_0/CLKOP} -source {PLL_inst1/PLLInst_0/CLKI} -multiply_by 7 -name {cm33_clk}
# create_generated_clock {PLL_inst1/PLLInst_0/CLKOS2} -source {PLL_inst1/PLLInst_0/CLKI} -multiply_by 4 -name {fpga_clk}

# create_clock -period 6.6 -waveform {0 3.3} -name {ahb_clk} {AHB_USR_CLK}

# create_generated_clock {CM33_inst/MTX_CLK} -source {CM33_inst/CLK_TREE} -divide_by 1 -name {ahb_clk}
# create_generated_clock {CM33_inst/CLKOUT} -source {CM33_inst/CLK_TREE} -divide_by 1 -name {ahb_clk2}

# set_false_path -from [get_clocks fpga_clk] -to [get_clocks cm33_clk]
# set_false_path -from [get_clocks ahb_clk] -to [get_clocks cm33_clk]

# set_false_path -from [get_clocks cm33_clk] -to [get_clocks ahb_clk]
# set_false_path -from [get_clocks fpga_clk] -to [get_clocks ahb_clk]

# set_false_path -from [get_clocks cm33_clk] -to [get_clocks fpga_clk]
# set_false_path -from [get_clocks ahb_clk] -to [get_clocks fpga_clk]

