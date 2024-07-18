// D:\fpga\AC208-SA5Z30-CM3\hq_xist_2.14.4_021824_win64\build\ipcreator\sup_files\ipdepot\pll\pll_freq_30k\..\_ipgen_seal_\pll_seal.exe -meta_xml D:\fpga\AC208-SA5Z30-CM3\hq_xist_2.14.4_021824_win64\build\ipcreator\sup_files\ipdepot\pll\pll_freq_30k\pll_freq_25k.xml -ini_file D:/fpga/AC201-SA5Z50-CM33/TEST1/FPGA/ipcore_dir/PLL_FREQ/xsIP_PLL_FREQ.hqip -lang chs


`timescale 1 ns / 1 ps
module PLL_FREQ (CLKI, CLKOP, CLKOS, CLKOS2, CLKOS3, LOCK)/* synthesis NGD_DRC_MASK=1 */;
    input wire CLKI;
    output wire CLKOP;
    output wire CLKOS;
    output wire CLKOS2;
    output wire CLKOS3;
    output wire LOCK;

    supply0 RST;
    supply0 RESETM;
    supply0 RESETC;
    supply0 RESETD;
    supply0 STDBY;
    supply0 [1:0] PHASESEL;
    supply0 PHASEDIR;
    supply0 PHASESTEP;
    wire DPHSRC;
    wire CLKO5_t;
    wire CLKOS3_t;
    wire CLKOS2_t;
    wire CLKOP_t;
    wire CLKOS_t;

    supply0 ipgen_GND;

    wire CLKINTFB_t;
    wire CLKI_t;
    assign CLKI_t = CLKI;

    defparam PLLInst_0.CLKO5_SEL = "CLKO5" ;
    defparam PLLInst_0.CLKOPD_DLY = 0 ;
    defparam PLLInst_0.DDRST_ENA = "DISABLED" ;
    defparam PLLInst_0.DCRST_ENA = "DISABLED" ;
    defparam PLLInst_0.MRST_ENA = "DISABLED" ;
    defparam PLLInst_0.PLLRST_ENA = "DISABLED" ;
    defparam PLLInst_0.INTFB_WAKE = "DISABLED" ;
    defparam PLLInst_0.STDBY_ENABLE = "DISABLED" ;
    defparam PLLInst_0.DPHASE_SOURCE = "DISABLED" ;
    defparam PLLInst_0.PLL_USE_WB = "DISABLED" ;
    defparam PLLInst_0.PLL_LOCK_MODE = 0 ;
    defparam PLLInst_0.CLKOS_TRIM_DELAY = 0 ;
    defparam PLLInst_0.CLKOS_TRIM_POL = "RISING" ;
    defparam PLLInst_0.CLKOP_TRIM_DELAY = 0 ;
    defparam PLLInst_0.CLKOP_TRIM_POL = "RISING" ;
    defparam PLLInst_0.FRACN_DIV = 0 ;
    defparam PLLInst_0.FRACN_ENABLE = "DISABLED" ;
    defparam PLLInst_0.CLKO5_FPHASE = 0 ;
    defparam PLLInst_0.CLKO5_CPHASE = 0 ;
    defparam PLLInst_0.OUTDIVIDER_MUXE2 = "DIVE" ;
    //defparam PLLInst_0.PREDIVIDER_MUXE1 = 0 ;
    defparam PLLInst_0.VCO_BYPASS_E0 = "DISABLED" ;
    defparam PLLInst_0.CLKO5_ENABLE = "DISABLED" ;
    defparam PLLInst_0.CLKO5_DIV = 0 ;
    defparam PLLInst_0.CLKOS3_FPHASE = 0 ;
    defparam PLLInst_0.CLKOS3_CPHASE = 19 ;
    defparam PLLInst_0.CLKOS2_FPHASE = 0 ;
    defparam PLLInst_0.CLKOS2_CPHASE = 9 ;
    defparam PLLInst_0.CLKOS_FPHASE = 0 ;
    defparam PLLInst_0.CLKOS_CPHASE = 4 ;
    defparam PLLInst_0.CLKOP_FPHASE = 0 ;
    defparam PLLInst_0.CLKOP_CPHASE = 3 ;
    defparam PLLInst_0.OUTDIVIDER_MUXD2 = "DIVD" ;
    defparam PLLInst_0.PREDIVIDER_MUXD1 = 0 ;
    defparam PLLInst_0.VCO_BYPASS_D0 = "DISABLED" ;
    defparam PLLInst_0.CLKOS3_ENABLE = "ENABLED" ;
    defparam PLLInst_0.OUTDIVIDER_MUXC2 = "DIVC" ;
    defparam PLLInst_0.PREDIVIDER_MUXC1 = 0 ;
    defparam PLLInst_0.VCO_BYPASS_C0 = "DISABLED" ;
    defparam PLLInst_0.CLKOS2_ENABLE = "ENABLED" ;
    defparam PLLInst_0.OUTDIVIDER_MUXB2 = "DIVB" ;
    defparam PLLInst_0.PREDIVIDER_MUXB1 = 0 ;
    defparam PLLInst_0.VCO_BYPASS_B0 = "DISABLED" ;
    defparam PLLInst_0.CLKOS_ENABLE = "ENABLED" ;
    defparam PLLInst_0.OUTDIVIDER_MUXA2 = "DIVA" ;
    defparam PLLInst_0.PREDIVIDER_MUXA1 = 0 ;
    defparam PLLInst_0.VCO_BYPASS_A0 = "DISABLED" ;
    defparam PLLInst_0.CLKOP_ENABLE = "ENABLED" ;
    defparam PLLInst_0.CLKOS3_DIV = 20 ;
    defparam PLLInst_0.CLKOS2_DIV = 10 ;
    defparam PLLInst_0.CLKOS_DIV = 5 ;
    defparam PLLInst_0.CLKOP_DIV = 4 ;
    defparam PLLInst_0.CLKFB_DIV = 10 ;
    defparam PLLInst_0.CLKI_DIV = 1 ;
    defparam PLLInst_0.FEEDBK_PATH = "INT_DIVA" ;
    defparam PLLInst_0.EN_PHI = "FALSE" ;
    xsPLLSA PLLInst_0 (.CLKI(CLKI_t), .CLKFB(CLKINTFB_t), .PHASESEL1(PHASESEL[1]), 
        .PHASESEL0(PHASESEL[0]), .PHASEDIR(PHASEDIR), .PHASESTEP(PHASESTEP), 
        .LOADREG(ipgen_GND), .STDBY(STDBY), .PLLWAKESYNC(ipgen_GND), 
        .RST(RST), .RESETM(RESETM), .RESETC(RESETC), .RESETD(RESETD), 
        .ENCLKOP(ipgen_GND), .ENCLKOS(ipgen_GND), .ENCLKOS2(ipgen_GND), 
        .ENCLKOS3(ipgen_GND), .PLLCLK(ipgen_GND), .PLLRST(ipgen_GND), .PLLSTB(ipgen_GND), 
        .PLLWE(ipgen_GND), .PLLADDR4(ipgen_GND), .PLLADDR3(ipgen_GND), .PLLADDR2(ipgen_GND), 
        .PLLADDR1(ipgen_GND), .PLLADDR0(ipgen_GND), .PLLDATI7(ipgen_GND), 
        .PLLDATI6(ipgen_GND), .PLLDATI5(ipgen_GND), .PLLDATI4(ipgen_GND), 
        .PLLDATI3(ipgen_GND), .PLLDATI2(ipgen_GND), .PLLDATI1(ipgen_GND), 
        .PLLDATI0(ipgen_GND), .CLKOP(CLKOP_t), .CLKOS(CLKOS_t), .CLKOS2(CLKOS2_t), 
        .CLKOS3(CLKOS3_t), .LOCK(LOCK), .INTLOCK(), .REFCLK(), .CLKINTFB(CLKINTFB_t), 
        .DPHSRC(DPHSRC), .PLLACK(), .PLLDATO7(), .PLLDATO6(), .PLLDATO5(), .PLLDATO4(), 
        .PLLDATO3(), .PLLDATO2(), .PLLDATO1(), .PLLDATO0(), .CLKO5(CLKO5_t))
             /* synthesis FREQUENCY_PIN_CLKOS3="50.000000" */
             /* synthesis FREQUENCY_PIN_CLKOS2="100.000000" */
             /* synthesis FREQUENCY_PIN_CLKOS="200.000000" */
             /* synthesis FREQUENCY_PIN_CLKOP="250.000000" */
             /* synthesis FREQUENCY_PIN_CLKI="25.000000" */
             /* synthesis ICP_CURRENT="10" */
             /* synthesis LPF_RESISTOR="48" */
             /* synthesis FREQ_LOCK_ACCURACY="2" */;

    assign CLKOS3 = CLKOS3_t;
    assign CLKOS2 = CLKOS2_t;
    assign CLKOS = CLKOS_t;
    assign CLKOP = CLKOP_t;


    // exemplar begin
    // exemplar attribute PLLInst_0 FREQUENCY_PIN_CLKOS3 50.000000
    // exemplar attribute PLLInst_0 FREQUENCY_PIN_CLKOS2 100.000000
    // exemplar attribute PLLInst_0 FREQUENCY_PIN_CLKOS 200.000000
    // exemplar attribute PLLInst_0 FREQUENCY_PIN_CLKOP 250.000000
    // exemplar attribute PLLInst_0 FREQUENCY_PIN_CLKI 25.000000
    // exemplar attribute PLLInst_0 ICP_CURRENT 10
    // exemplar attribute PLLInst_0 LPF_RESISTOR 48
    // exemplar attribute PLLInst_0 FREQ_LOCK_ACCURACY 2
    // exemplar end

    // phase clkos3 0.000000
    // phase clkos2 0.000000
    // phase clkos  0.000000
    // phase clkop  0.000000

endmodule
