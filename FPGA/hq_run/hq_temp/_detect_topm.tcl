
lappend argv "D:/fpga/AC201-SA5Z50-CM33/TEST1/FPGA/fpga_cm33.v" 
lappend argv "D:/fpga/AC201-SA5Z50-CM33/TEST1/FPGA/led_wf.v" 
lappend argv "D:/fpga/AC201-SA5Z50-CM33/TEST1/FPGA/ipcore_dir/PLL_FREQ/xsIP_PLL_FREQ.v" 
#################################################################################
#
# Copyright (c) uptops, Inc.
# All rights reserved
#
# Filename			:	u_detectTopModule.tcl
# Class				:	detectTopModule
# Description		:	to detect top module
#
# Current Version	:	1.2
# Revision times	:	none
# Authors			:	Lansong, Daniel
# Date				:	07/06/2015
#
# Original Version	:	1
# Original Authors	:	Aijun, shi
# Original Date		:	04/24/2008
#
# Original Version	:	2
# Original Authors	:	Aijun, shi
# Original Date		:	11/14/2013
#################################################################################
set outf [open "tmp_topmodule.v" w]
# unset module, instance, and topmodule list
if [info exists module_list] {
	unset module_list
}
set module_list ""
if [info exists instance_list] {
	unset instance_list
}
set instance_list ""
if [info exists topmodule_list] {
	unset topmodule_list
}
set topmodule_list ""

proc chk_ignore {line} {
	# space line
	if [regexp {^\s*$} $line] {
		return 1
	}
	# `timescale
	if [regexp {^\s*`} $line] {
		return 1
	}
	# //
	if [regexp {^\s*//} $line] {
		return 1
	}
	return 0
}
# get module, instance, list
foreach arg $argv {
	if [file exists $arg] {
		#puts "00000 $arg"
		set inf [open $arg r]
		set tmp ""; set ignore false
		set syn_off 0
		set protect 0
		while {[gets $inf line] >= 0} {
			#
			# process protected region
			if {$protect == 1} {
				if {[string first {`pragma protect end_protected} $line] >= 0} {
					set protect 0
				}
				continue
			}
			if {[string first {`pragma protect begin_protected} $line] >=0} {
				set protect 1
			}
			#
			# process synthesis on/off
			#
			if {$syn_off == 1} {
				if [regexp {^(.*)//[ ]*syn.+translate_on} $line] {
					set syn_off 0
				}
				continue
			}
			if [regexp {^(.*)//[ ]*syn.+translate_off} $line] {
				set syn_off 1
				continue
			}
			if [chk_ignore $line] {
				continue
			}
			#----------------------------------
			#strip contents between  /* ... */
			#----------------------------------
			while {1} {
				if [regexp {(.*)/\*.*\*/(.*)} $line a p1 p2] {
					set line $p1$p2
				} else {
					break
				}
			}
			# strip content between (* *)
			while {1} {
				if [regexp {(.*)\(\*.+\*\)(.*)} $line a p1 p2] {
					set line $p1$p2
				} else {
					break
				}
			}
			# strip content after //
			while {1} {
				# note abc//dfs//;
				if [regexp {(.+)//.*$} $line a p] {
					set line $p
				} else {
					break
				}
			}

			# ignore current line
			#{
				if [chk_ignore $line] {
					continue
				}
			#}

			# ignore area which between /* ... */
			#{
				# start area ignore
				if [regexp {^\s*/\*} $line] {
					set ignore true
					if [regexp {\*/\s*$} $line] {
						set ignore false
					}
					continue
				}
				# end area ignore
				if [regexp {\*/(.*)$} $line a tail] {
					set ignore false
					# always ignore the line
					if [chk_ignore $tail] {
						continue
					} else {
						set line $tail
					}
				}
				if [string equal $ignore true] {
					continue
				}
			#}
			# take care the end of ';'
			#{
				set tmp "$tmp $line"
				if {![regexp {;} $tmp] && ![regexp {endcase$} $tmp] && ![regexp {end$} $tmp] && ![regexp {endmodule} $tmp] } {
					continue
				}
			#}
			#puts $outf $tmp
			if [regexp {^\s*--} $tmp] {
				set tmp "";continue
			}
			#puts " >>> $tmp"
			#
			# elseif (COMPANY=="XILINX")
			# 	begin : myblock
			#		xilinx_ram XRAM #(..) foo(.RCLK(..)
			#	end
			#
			if [regexp {begin(?:\s*)\:(?:\s*)(?:\w+)(?:\s*)(\w+)(?:\s*)(?:#)} $tmp match instance] {
				lappend instance_list $instance
				set tmp ""
				continue
			}
			# elseif (COMPANY=="XILINX")
			# 	begin
			#		xilinx_ram XRAM #(..) foo(.RCLK(..)
			#	end
			if [regexp {begin(?:\s*)(\w+)(?:\s*)(?:#)} $tmp match instance] {
				lappend instance_list $instance
				set tmp ""
				continue
			}

			# parameter
			if [regexp {^\s*(parameter|input|output|assign|endmodule|reg|always|posedge|negedge|wire|if|else|case|endcase|end|endmodule)[\s|$]} $tmp] {
				set tmp "";continue
			}
			if [regexp -nocase {^\s*(library|use|package|component|generic|port|end|architectrue|signal|constant|process|begin|if|elsif|error|case)[\s|$]} $tmp] {
				set tmp "";continue
			}
			if [regexp -nocase {^(?:\s*)(?:entity)(?:\s+)(\w+)(?:\s*)(?:is)} $tmp match entity] {
				lappend module_list $entity
				set tmp "";continue
			}
			if [regexp {^(?:\s*)(?:module)(?:\s+)(\w+)(?:\s*)(?:\()} $tmp match module] {
				#	puts $module
				lappend module_list $module
				set tmp "";continue
			}
			if [regexp {^(?:\s*)(?:module)(?:\s+)(\w+)(?:\s*)\#(?:\s*)\(.+\)(?:\s*)(?:\()} $tmp match module] {
				# module foo #(parameter a=1, b=2) (input i1, )
				#	puts $module
				lappend module_list $module
				set tmp "";continue
			}

            if [regexp {^(?:\s*)(?:macromodule)(?:\s+)(\w+)(?:\s*)(?:\()} $tmp match macromodule] {
            	#   puts $macromodule
                lappend module_list $macromodule
                set tmp "";continue
            }
            #puts " <<<<"
			if [regexp -nocase {^(?:\s*)(?:\w+)(?:\s*)(?::)(?:\s*)(\w+)(?:\s+)(?:port map)} $tmp match component] {
				lappend instance_list $component
				#puts "1111 $component"
				set tmp "";continue
			}

			# xxxx xxxx (
			if [regexp {^(?:\s*)(\w+)(?:\s+)(?:\w+)(?:\s*)(?:\()} $tmp match instance] {
				#	puts $instance
				lappend instance_list $instance
				#puts "222 $instance"

				set tmp "";continue
			}
			# xxxx #sfjsl xxxxx(
			if [regexp {^(?:\s*)(\w+)(?:\s+)(?:#)} $tmp match instance] {
				#	puts $instance
				lappend instance_list $instance
				#puts "3333 $instance"
				set tmp "";continue
			}
			#puts "444 $tmp"
			set tmp ""
		}
	}
}
#puts $outf "-module_list [set module_list]"
#puts $outf "-instance_list [set instance_list]"
# handle module and instance list to get topmodule list
foreach module $module_list {
    puts $outf "-module [set module]"
	if {[lsearch $instance_list $module] != -1} {
		continue
	}
	lappend topmodule_list $module
}

# outputs the topmodule to file tmp_topmodule.v'
puts $outf "-topmoduleNum [llength $topmodule_list]"
foreach topmodule $topmodule_list {
	puts $outf "-topmodule [set topmodule]"
}
close $outf
return 0
