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

    // ahb uart
    output wire ahb_uart_tx,
    input wire ahb_uart_rx,

    // CM33 DBG
    input wire SW_CLK_PIN,
    inout wire SWD_IO_PIN
  );

  // CLKS
  wire PLL1_LOCK;
  wire CLK_CM33;
  wire CLK_FPGA_SYS1;
  wire AHB_USR_CLK = CLK_CM33;
  PLL_FREQ PLL_inst1(
             .CLKI(CLK_IN_25M_PIN), // 25 IN
             .CLKOP(CLK_CM33),
             .CLKOS(CLK_FPGA_SYS1),
            //  .CLKOS2(),
            //  .CLKOS3(),
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

  // AHBlite Interconncet
  wire [ 31: 0] TARGEXP1HADDR;
  wire [ 2: 0] TARGEXP1HBURST;
  wire TARGEXP1HMASTLOCK;
  wire [ 3: 0] TARGEXP1HPROT;
  wire [ 31: 0] TARGEXP1HRDATA;
  wire TARGEXP1HREADYMUX;
  wire TARGEXP1HREADYOUT;
  wire TARGEXP1HRESP;
  wire TARGEXP1HSEL;
  wire [ 2: 0] TARGEXP1HSIZE;
  wire [ 1: 0] TARGEXP1HTRANS;
  wire [ 31: 0] TARGEXP1HWDATA;
  wire TARGEXP1HWRITE;

  wire HSEL_P0;
  wire [31: 0] HADDR_P0;
  wire [2: 0] HBURST_P0;
  wire HMASTLOCK_P0;
  wire [3: 0] HPROT_P0;
  wire [2: 0] HSIZE_P0;
  wire [1: 0] HTRANS_P0;
  wire [31: 0] HWDATA_P0;
  wire HWRITE_P0;
  wire HREADY_P0;
  wire HREADYOUT_P0;
  wire [31: 0] HRDATA_P0;
  wire HRESP_P0;

  wire HSEL_P1;
  wire [31: 0] HADDR_P1;
  wire [2: 0] HBURST_P1;
  wire HMASTLOCK_P1;
  wire [3: 0] HPROT_P1;
  wire [2: 0] HSIZE_P1;
  wire [1: 0] HTRANS_P1;
  wire [31: 0] HWDATA_P1;
  wire HWRITE_P1;
  wire HREADY_P1;
  wire HREADYOUT_P1;
  wire [31: 0] HRDATA_P1;
  wire HRESP_P1;

  wire HSEL_P2;
  wire [31: 0] HADDR_P2;
  wire [2: 0] HBURST_P2;
  wire HMASTLOCK_P2;
  wire [3: 0] HPROT_P2;
  wire [2: 0] HSIZE_P2;
  wire [1: 0] HTRANS_P2;
  wire [31: 0] HWDATA_P2;
  wire HWRITE_P2;
  wire HREADY_P2;
  wire HREADYOUT_P2;
  wire [31: 0] HRDATA_P2;
  wire HRESP_P2;

  // null 设备
  wire HSEL_P3;
  wire [31: 0] HADDR_P3;
  wire [2: 0] HBURST_P3;
  wire HMASTLOCK_P3;
  wire [3: 0] HPROT_P3;
  wire [2: 0] HSIZE_P3;
  wire [1: 0] HTRANS_P3;
  wire [31: 0] HWDATA_P3;
  wire HWRITE_P3;
  wire HREADY_P3;
  wire HREADYOUT_P3;
  wire [31: 0] HRDATA_P3;
  wire HRESP_P3;

  AHBlite_Interconnect Interconncet(
                         .HCLK (AHB_USR_CLK),
                         .HRESETn (CM33SYS_RSTN),

                         // CORE SIDE
                         .HSEL_M (TARGEXP1HSEL),
                         .HADDR (TARGEXP1HADDR),
                         .HTRANS (TARGEXP1HTRANS),
                         .HSIZE (TARGEXP1HSIZE),
                         .HBURST (TARGEXP1HBURST),
                         .HPROT (TARGEXP1HPROT),
                         .HMASTLOCK (TARGEXP1HMASTLOCK),
                         .HWRITE (TARGEXP1HWRITE),
                         .HWDATA (TARGEXP1HWDATA),
                         .HRDATA (TARGEXP1HRDATA),
                         .HREADY_master (TARGEXP1HREADYMUX),
                         .HREADY (TARGEXP1HREADYOUT),
                         .HRESP (TARGEXP1HRESP),

                         // P0
                         .HSEL_P0 (HSEL_P0),
                         .HADDR_P0 (HADDR_P0),
                         .HBURST_P0 (HBURST_P0),
                         .HMASTLOCK_P0 (HMASTLOCK_P0),
                         .HPROT_P0 (HPROT_P0),
                         .HSIZE_P0 (HSIZE_P0),
                         .HTRANS_P0 (HTRANS_P0),
                         .HWDATA_P0 (HWDATA_P0),
                         .HWRITE_P0 (HWRITE_P0),
                         .HREADY_P0 (HREADY_P0),
                         .HREADYOUT_P0 (HREADYOUT_P0),
                         .HRDATA_P0 (HRDATA_P0),
                         .HRESP_P0 (HRESP_P0),

                         // P1
                         .HSEL_P1 (HSEL_P1),
                         .HADDR_P1 (HADDR_P1),
                         .HBURST_P1 (HBURST_P1),
                         .HMASTLOCK_P1 (HMASTLOCK_P1),
                         .HPROT_P1 (HPROT_P1),
                         .HSIZE_P1 (HSIZE_P1),
                         .HTRANS_P1 (HTRANS_P1),
                         .HWDATA_P1 (HWDATA_P1),
                         .HWRITE_P1 (HWRITE_P1),
                         .HREADY_P1 (HREADY_P1),
                         .HREADYOUT_P1 (HREADYOUT_P1),
                         .HRDATA_P1 (HRDATA_P1),
                         .HRESP_P1 (HRESP_P1),

                         // P2
                         .HSEL_P2 (HSEL_P2),
                         .HADDR_P2 (HADDR_P2),
                         .HBURST_P2 (HBURST_P2),
                         .HMASTLOCK_P2 (HMASTLOCK_P2),
                         .HPROT_P2 (HPROT_P2),
                         .HSIZE_P2 (HSIZE_P2),
                         .HTRANS_P2 (HTRANS_P2),
                         .HWDATA_P2 (HWDATA_P2),
                         .HWRITE_P2 (HWRITE_P2),
                         .HREADY_P2 (HREADY_P2),
                         .HREADYOUT_P2 (HREADYOUT_P2),
                         .HRDATA_P2 (HRDATA_P2),
                         .HRESP_P2 (HRESP_P2),

                         // P3
                         .HSEL_P3 (HSEL_P3),
                         .HADDR_P3 (HADDR_P3),
                         .HBURST_P3 (HBURST_P3),
                         .HMASTLOCK_P3 (HMASTLOCK_P3),
                         .HPROT_P3 (HPROT_P3),
                         .HSIZE_P3 (HSIZE_P3),
                         .HTRANS_P3 (HTRANS_P3),
                         .HWDATA_P3 (HWDATA_P3),
                         .HWRITE_P3 (HWRITE_P3),
                         .HREADY_P3 (HREADY_P3),
                         .HREADYOUT_P3 (HREADYOUT_P3),
                         .HRDATA_P3 (HRDATA_P3),
                         .HRESP_P3 (HRESP_P3)
                       );

  // 8位数码管
  ahb_seg7x8 seg_inst(
               .HCLK ( AHB_USR_CLK ), //时钟
               .HRESETn ( CM33SYS_RSTN ), //复位

               .HSEL ( HSEL_P1 ), // AHB inputs，设备选择
               .HADDR ( HADDR_P1[ 15: 0 ] ), //地址
               .HTRANS ( HTRANS_P1 ), //传输控制信号
               .HSIZE ( HSIZE_P1 ), //传输大小
               .HWRITE ( HWRITE_P1 ), //写控制
               .HWDATA ( HWDATA_P1 ), //写数据
               .HREADY ( HREADY_P1 ), //传输完成

               .HREADYOUT ( HREADYOUT_P1 ), // AHB Outputs，设备准备信号
               .HRDATA ( HRDATA_P1 ), //读取到的数据
               .HRESP ( HRESP_P1 ), //设备响应

               .SH_CLK( seg7_SH_CP ),
               .LD_CLK( seg7_ST_CP ),
               .HC_DAT( seg7_DS)
             );

  // uart
  ahb_uart ahb_uart1(
             .HCLK ( AHB_USR_CLK ),
             .HRESETn ( CM33SYS_RSTN ),

             .HSEL ( HSEL_P2 ),
             .HADDR ( HADDR_P2[ 15: 0 ] ),
             .HTRANS ( HTRANS_P2 ),
             .HSIZE ( HSIZE_P2 ),
             .HWRITE ( HWRITE_P2 ),
             .HWDATA ( HWDATA_P2 ),
             .HREADY ( HREADY_P2 ),

             .HREADYOUT ( HREADYOUT_P2 ),
             .HRDATA ( HRDATA_P2 ),
             .HRESP ( HRESP_P2 ),

             .TX_OUT( ahb_uart_tx ),
             .RX_IN( ahb_uart_rx )
           );

  // ahb_null
  ahb_null ahb_null1(
             .HCLK ( AHB_USR_CLK ),
             .HRESETn ( CM33SYS_RSTN ),

             .HSEL ( HSEL_P3 ),
             .HADDR ( HADDR_P3[ 15: 0 ] ),
             .HTRANS ( HTRANS_P3 ),
             .HSIZE ( HSIZE_P3 ),
             .HWRITE ( HWRITE_P3 ),
             .HWDATA ( HWDATA_P3 ),
             .HREADY ( HREADY_P3 ),

             .HREADYOUT ( HREADYOUT_P3 ),
             .HRDATA ( HRDATA_P3 ),
             .HRESP ( HRESP_P3 )
           );

  ahb_null ahb_null2(
             .HCLK ( AHB_USR_CLK ),
             .HRESETn ( CM33SYS_RSTN ),

             .HSEL ( HSEL_P0 ),
             .HADDR ( HADDR_P0[ 15: 0 ] ),
             .HTRANS ( HTRANS_P0 ),
             .HSIZE ( HSIZE_P0 ),
             .HWRITE ( HWRITE_P0 ),
             .HWDATA ( HWDATA_P0 ),
             .HREADY ( HREADY_P0 ),

             .HREADYOUT ( HREADYOUT_P0 ),
             .HRDATA ( HRDATA_P0 ),
             .HRESP ( HRESP_P0 )
           );

  // CM33内核
  wire [15: 0] CM33_INT_TRIG = 0;

  xsCM33 CM33_inst(
           // ClockS
           .PLL_USRCLK         (1'b0),
           .CLK_TREE           (1'b0),
           .CLK_PAD            (1'b0),
           .PLL_OUT            (1'b0),
           .CIB_CLK            (CLK_CM33),
           .MTX_CLK            (),
           .CLKOUT             (),
           .PCLK               (),
           .PCLKEN             (),

           // interrupt
           .EXTINT             (CM33_INT_TRIG),
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
           .TARGEXP0HREADYOUT  (1'b1),
           .TARGEXP0HRESP      (1'b1),
           .TARGEXP0HSEL       (),
           .TARGEXP0HSIZE      (),
           .TARGEXP0HTRANS     (),
           .TARGEXP0HWDATA     (),
           .TARGEXP0HWRITE     (),

           .TARGEXP1HADDR      (TARGEXP1HADDR),
           .TARGEXP1HBURST     (TARGEXP1HBURST),
           .TARGEXP1HMASTLOCK  (TARGEXP1HMASTLOCK),
           .TARGEXP1HPROT      (TARGEXP1HPROT),
           .TARGEXP1HRDATA     (TARGEXP1HRDATA),
           .TARGEXP1HREADYMUX  (TARGEXP1HREADYMUX),
           .TARGEXP1HREADYOUT  (TARGEXP1HREADYOUT),
           .TARGEXP1HRESP      (TARGEXP1HRESP),
           .TARGEXP1HSEL       (TARGEXP1HSEL),
           .TARGEXP1HSIZE      (TARGEXP1HSIZE),
           .TARGEXP1HTRANS     (TARGEXP1HTRANS),
           .TARGEXP1HWDATA     (TARGEXP1HWDATA),
           .TARGEXP1HWRITE     (TARGEXP1HWRITE),

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
