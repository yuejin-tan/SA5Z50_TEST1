module fpga_cm33(
    // RST
    input wire RST_N_PIN,

    // CLK
    input wire CLK_IN_25M_PIN,

    // CM33 GPIO
    inout wire [31:0] GPIO_PIN,

    //数码管接口
    output wire seg7_SH_CP,   //移位时钟 SCLK
    output wire seg7_ST_CP,   //数据锁存时钟 RCLK
    output wire seg7_DS,      //移位串行数据 DIO

    // LED X2
    output wire [1:0] LED01_PIN,

    // CM33 DBG
    input wire SW_CLK_PIN,
    inout wire SWD_IO_PIN
  );

  // CLKS
  wire PLL1_LOCK;
  wire MTX_CLK;
  wire CLK_CM33;
  wire CLK_FPGA_SYS1;
  PLL_FREQ PLL_inst1(
             .CLKI(CLK_IN_25M_PIN), // 25 IN
             .CLKOP(), // 250
             .CLKOS(CLK_CM33), // 200
             .CLKOS2(CLK_FPGA_SYS1),  // 100
             .CLKOS3(),  // 50
             .LOCK(PLL1_LOCK)
           );

  // RESET
  reg CM33SYS_RSTN_buff = 0;
  reg CM33SYS_RSTN = 0;
  always@( posedge CLK_FPGA_SYS1)
    begin
      CM33SYS_RSTN_buff <= RST_N_PIN & PLL1_LOCK;
      CM33SYS_RSTN <= CM33SYS_RSTN_buff;
    end

  // GPIO
  wire [31:0] GPIO_OUT;
  wire [31:0] GPIO_OUT_EN;
  wire [31:0] GPIO_IN;
  genvar i;
  generate
    for(i=0;i<32;i=i+1)
      begin:setb
        xsIOBB inst_xsIOBB_GPIO (
                 .I (GPIO_OUT[i]),
                 .T (~GPIO_OUT_EN[i]),
                 .O (GPIO_IN[i]),
                 .B (GPIO_PIN[i])
               );
      end
  endgenerate

  // DEBUG
  wire SWD_IO_OUT_EN;
  wire SWD_IO_OUT;
  wire SWD_IO_IN;
  xsIOBB inst_xsIOBB_DBG (
           .I (SWD_IO_OUT),
           .T (SWD_IO_OUT_EN),
           .O (SWD_IO_IN),
           .B (SWD_IO_PIN)
         );

  // fpga 流水灯
  led_wf led_wf_inst1   (
           .clk(CLK_FPGA_SYS1),
           .rst(CM33SYS_RSTN),
           .led_sig(LED01_PIN)
         );

  // 8位数码管
  assign seg7_SH_CP=1;
  assign seg7_ST_CP=1;
  assign seg7_DS=1;

  // CM33内核
  xsCM33 CM33_inst(
           // ClockS
           .MTX_CLK            (MTX_CLK),
           .CLK_PAD            (1'b0),
           .PLL_OUT            (1'b0),
           .PLL_USRCLK         (1'b0),
           .CIB_CLK            (CLK_CM33),
           .CLK_TREE           (1'b0),
           .CLKOUT             (),
           .PCLK               (),
           .PCLKEN             (),

           // interrupt
           .EXTINT             (),
           // GPIO
           .IO_EXP_PORT_I      (GPIO_IN), //{GPIO_PIN},16'b0
           .IO_EXP_PORT_O_INT  (GPIO_OUT),
           .IO_EXP_PORT_OEN_INT(GPIO_OUT_EN),
           // reset
           .FABRIC_CPU_RSTN    (CM33SYS_RSTN), //CPURSTN
           .FABRIC_MTX_RSTN    (CM33SYS_RSTN), //RST_N_PIN

           // REMAP CFG
           .REMAP              (3'b000),

           // Coprocessor
           .CPCDP              (),
           .CPENABLED          (),
           .CPERROR            (1'b0),
           .CPMCR              (),
           .CPMRC              (),
           .CPNSATTR           (),
           .CPNSPRESENT        (8'hff),
           .CPNUM              (),
           .CPOPC              (),
           .CPPRIV             (),
           .CPPWRSU            (),
           .CPRDATA            (64'b0),
           .CPREADY            (1'b0),
           .CPREGS             (),
           .CPSIZE             (),
           .CPSPRESENT         (8'hff),
           .CPVALID            (),
           .CPWDATA            (),

           // Debug and Trace
           .CS_TCK             (SW_CLK_PIN),
           .CS_TDI             (1'b0),
           .CS_TRACECLK        (),
           .CS_TRACEDATA       (),
           .CS_NSRST           (1'b1),
           .CS_NTRST           (1'b1),
           .DBGQACCEPTN        (),
           .DBGQACTIVE         (),
           .DBGQDENY           (),
           .DBGQREQN           (1'b1),
           .DBGRET             (1'b1),
           .DBG_SWDI_TMS       (SWD_IO_IN),
           .DBG_SWDO           (SWD_IO_OUT),
           .DBG_SWDO_EN        (SWD_IO_OUT_EN),
           .TDO_ENABLE         (),
           .TDO_TMS            (), //  jtagTDO

           // DMAC
           .DMACBREQ           (4'b0),
           .DMACCLR            (),
           .DMACLBREQ          (4'b0),
           .DMACLSREQ          (4'b0),
           .DMACSREQ           (4'b0),
           .DMACTC             (),

           // FPU
           .FPDZC              (),
           .FPIDC              (),
           .FPIOC              (),
           .FPIXC              (),
           .FPOFC              (),
           .FPUFC              (),
           .FPUQACCEPTN        (),
           .FPUQACTIVE         (),
           .FPUQDENY           (),
           .FPUQREQN           (1'b1),
           .FPURET             (1'b1),

           // AHBLite slaves
           .INITEXP0HADDR      (32'b0),
           .INITEXP0HBURST     (3'b0),
           .INITEXP0HMASTLOCK  (1'b0),
           .INITEXP0HPROT      (4'b0),
           .INITEXP0HRDATA     (),
           .INITEXP0HREADY     (),
           .INITEXP0HRESP      (),
           .INITEXP0HSEL       (1'b0),
           .INITEXP0HSIZE      (3'b0),
           .INITEXP0HTRANS     (2'b0),
           .INITEXP0HWDATA     (32'b0),
           .INITEXP0HWRITE     (1'b0),

           .INITEXP1HADDR      (32'b0),
           .INITEXP1HBURST     (3'b0),
           .INITEXP1HMASTLOCK  (1'b0),
           .INITEXP1HPROT      (4'b0),
           .INITEXP1HRDATA     (),
           .INITEXP1HREADY     (),
           .INITEXP1HRESP      (),
           .INITEXP1HSEL       (1'b0),
           .INITEXP1HSIZE      (3'b0),
           .INITEXP1HTRANS     (2'b0),
           .INITEXP1HWDATA     (32'b0),
           .INITEXP1HWRITE     (1'b0),

           // AHBLite masters
           .TARGEXP0HADDR      (),
           .TARGEXP0HBURST     (),
           .TARGEXP0HMASTLOCK  (),
           .TARGEXP0HPROT      (),
           .TARGEXP0HRDATA     (32'b0),
           .TARGEXP0HREADYMUX  (),
           .TARGEXP0HREADYOUT  (1'b0),
           .TARGEXP0HRESP      (1'b0),
           .TARGEXP0HSEL       (),
           .TARGEXP0HSIZE      (),
           .TARGEXP0HTRANS     (),
           .TARGEXP0HWDATA     (),
           .TARGEXP0HWRITE     (),

           .TARGEXP1HADDR      (),
           .TARGEXP1HBURST     (),
           .TARGEXP1HMASTLOCK  (),
           .TARGEXP1HPROT      (),
           .TARGEXP1HRDATA     (32'b0),
           .TARGEXP1HREADYMUX  (),
           .TARGEXP1HREADYOUT  (1'b0),
           .TARGEXP1HRESP      (1'b0),
           .TARGEXP1HSEL       (),
           .TARGEXP1HSIZE      (),
           .TARGEXP1HTRANS     (),
           .TARGEXP1HWDATA     (),
           .TARGEXP1HWRITE     (),

           // QSPI
           .QSPICSN            (),
           .QSPIDI             (4'b0),
           .QSPIDO             (),
           .QSPIDOEN           (),
           .QSPISCK            (),

           // CORE State
           .CORECLKEN          (),
           .TCMCLKEN           (),
           .CURRNS             (),

           // Q-channel interface for use in power management
           .COREQACCEPTN       (),
           .COREQACTIVE        (),
           .COREQDENY          (),
           .COREQREQN          (1'b1),
           .CORERET            (1'b1),
           .MTBQACCEPTN        (),
           .MTBQACTIVE         (),
           .MTBQDENY           (),
           .MTBQREQN           (1'b1),
           .RAMSQACCEPTN       (),
           .RAMSQACTIVE        (),
           .RAMSQDENY          (),
           .RAMSQREQN          (1'b1),
           .RAMSRET            (1'b1)
         );

  // CLK FUSE
  defparam CM33_inst.CORECLK_SEL = 7; //0 = "PLL_USRCLK"; 1 = "CLK_TREE"; 3 = "CLK_PAD"; 5 = "PLL_OUT"; 7 = "CIB_CLK"
  defparam CM33_inst.MTXCLK_SEL = "FALSE"; //"FALSE" = "CORECLK" ;"TRUE" = "SOURCECLK"
  defparam CM33_inst.CORECLK_EN = "TRUE";
  defparam CM33_inst.CORE_SET = "TRUE";
  defparam CM33_inst.PCLK_DIV = 0; // PCLK 不分频
  defparam CM33_inst.RSTN_ENABLE = "TRUE";

  // QSPI FUSE
  defparam CM33_inst.INITQSPIADDRSEL = "FALSE";
  defparam CM33_inst.INITQSPISCKMODE = "FALSE";
  defparam CM33_inst.INITQSPIXPRALBSIZE = "FALSE";
  defparam CM33_inst.INITQSPIXPRALTBYTES = 8'h00;
  defparam CM33_inst.INITQSPIXPRDDRMODE = "FALSE";
  defparam CM33_inst.INITQSPIXPREN = "FALSE";
  defparam CM33_inst.INITQSPIXPRNUMDC = 5'h0f;
  defparam CM33_inst.INITQSPIXPRXFERMODE = "FALSE";

  // CORE FUSE
  defparam CM33_inst.CFGNOCDECP = 8'b11111111;
  defparam CM33_inst.SSTCLKEN = "FALSE";
  defparam CM33_inst.NSTCLKEN = "FALSE";
  defparam CM33_inst.SSTCALIB = 26'h10F423F;
  defparam CM33_inst.NSSTCALIB = 26'h10F423F;
  defparam CM33_inst.MPUSDISABLE = "FALSE";
  defparam CM33_inst.MPUNSDISABLE = "FALSE";
  defparam CM33_inst.SAUDISABLE = "FALSE";
  defparam CM33_inst.INITSVTOR = 0;
  defparam CM33_inst.INITNSVTOR = 0;
  defparam CM33_inst.INITTCMEN = 0;
  defparam CM33_inst.INITL1RSTDIS = "FALSE";
  defparam CM33_inst.LOCKITGU = "FALSE";
  defparam CM33_inst.LOCKDTGU = "FALSE";
  defparam CM33_inst.LOCKTCM = "FALSE";
  defparam CM33_inst.CFGITCMSZ = 0;
  defparam CM33_inst.CFGDTCMSZ = 0;
  defparam CM33_inst.CFGMEMALIAS = 0;
  defparam CM33_inst.CFGFMADDR = 12'b0000_0000_0001; //input, 1 : boot from ahbc  0:from flash
  defparam CM33_inst.CPU0ECOREVNUM = 0;


endmodule
