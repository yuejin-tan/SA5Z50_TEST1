module ahb_seg7x8
  #(
     // 2M 驱动时钟，持续刷新
     parameter HC595_DRV_CLK_DIV = 99
   )
   (
     input wire HCLK,    // Clock
     input wire HRESETn, // Reset
     input wire HSEL,    // Device select
     input wire [ 15: 0] HADDR,   // Address
     input wire [ 1: 0] HTRANS,  // Transfer control
     input wire [ 2: 0] HSIZE,   // Transfer size
     input wire HWRITE,  // Write control
     input wire [ 31: 0] HWDATA,  // Write data
     input wire HREADY,  // Transfer phase done

     output wire HREADYOUT, // Device ready
     output wire [ 31: 0] HRDATA,  // Read data output
     output wire HRESP,   // Device response (always OKAY)

     // HC595 SIG
     output reg SH_CLK = 0,
     output reg LD_CLK = 0,
     output reg HC_DAT = 0
   );

  // bit4:dig bit0-3 nums
  reg [ 7: 0] segx_reg[ 7: 0 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  // --------------------------------------------------------------------------
  //   ahb interface logic
  // --------------------------------------------------------------------------

  // No err
  assign HRESP = 1'b0;
  // slave always ready
  assign HREADYOUT = 1'b1;

  wire trans_en;
  assign trans_en = HSEL & HTRANS[1];

  wire write_en;
  assign write_en = trans_en & HWRITE;

  wire read_en;
  assign read_en = trans_en & (~HWRITE);

  reg [3: 0] size_dec;
  always@( * )
    begin
      case ({HADDR[1: 0], HSIZE[1: 0]})
        4'h0 :
          size_dec = 4'h1;
        4'h1 :
          size_dec = 4'h3;
        4'h2 :
          size_dec = 4'hf;
        4'h4 :
          size_dec = 4'h2;
        4'h8 :
          size_dec = 4'h4;
        4'h9 :
          size_dec = 4'hc;
        4'hc :
          size_dec = 4'h8;
        default :
          size_dec = 4'h0;
      endcase
    end

  reg [3: 0]size_reg; // 数据选择
  always@(posedge HCLK or negedge HRESETn)
    begin
      if (~HRESETn)
        size_reg <= 0;
      else if (write_en & HREADY)
        size_reg <= size_dec;
    end

  // 写逻辑
  reg [15: 0] addr_reg;
  always@(posedge HCLK or negedge HRESETn)
    begin
      if (~HRESETn)
        addr_reg <= 0;
      else if (trans_en & HREADY)
        addr_reg <= {HADDR[15: 2], 2'd0};
    end

  reg wr_en_reg;
  always@(posedge HCLK or negedge HRESETn)
    begin
      if (~HRESETn)
        wr_en_reg <= 1'b0;
      else if (HREADY)
        wr_en_reg <= write_en;
      else
        wr_en_reg <= 1'b0;
    end

  always@(posedge HCLK or negedge HRESETn)
    begin
      if (~HRESETn)
        begin
          segx_reg[ 0] <= 0;
          segx_reg[ 1] <= 0;
          segx_reg[ 2] <= 0;
          segx_reg[ 3] <= 0;
        end
      else
        if ((addr_reg[ 15: 2 ] == 14'd0 ) & wr_en_reg)
          begin
            if (size_reg[0])
              begin
                segx_reg[ 0] <= HWDATA[ 7: 0 ];
              end
            if (size_reg[1])
              begin
                segx_reg[ 1] <= HWDATA[ 15: 8 ];
              end
            if (size_reg[2])
              begin
                segx_reg[ 2] <= HWDATA[ 23: 16 ];
              end
            if (size_reg[3])
              begin
                segx_reg[ 3] <= HWDATA[ 31: 24 ];
              end
          end
    end

  always@(posedge HCLK or negedge HRESETn)
    begin
      if (~HRESETn)
        begin
          segx_reg[ 4] <= 0;
          segx_reg[ 5] <= 0;
          segx_reg[ 6] <= 0;
          segx_reg[ 7] <= 0;
        end
      else
        if ((addr_reg[ 15: 2 ] == 14'd1 ) & wr_en_reg)
          begin
            if (size_reg[0])
              begin
                segx_reg[ 4] <= HWDATA[ 7: 0 ];
              end
            if (size_reg[1])
              begin
                segx_reg[ 5] <= HWDATA[ 15: 8 ];
              end
            if (size_reg[2])
              begin
                segx_reg[ 6] <= HWDATA[ 23: 16 ];
              end
            if (size_reg[3])
              begin
                segx_reg[ 7] <= HWDATA[ 31: 24 ];
              end
          end
    end

  reg [ 31: 0] HRDATA_reg = 0;

  //register read
  always @ ( *)
    begin
      if ( read_en)
        begin
          case ( addr_reg[ 15: 2 ] )
            14'd0:
              // HRDATA_reg = { segx_reg[ 3 ], segx_reg[ 2 ], segx_reg[ 1 ], segx_reg[ 0 ] };
              HRDATA_reg = 32'h12345678;
            14'd1:
              // HRDATA_reg = { segx_reg[ 7 ], segx_reg[ 6 ], segx_reg[ 5 ], segx_reg[ 4 ] };
              HRDATA_reg = 32'h9abcdef0;
            default:
              HRDATA_reg = 0;
          endcase
        end
      else
        begin
          HRDATA_reg = { 32{ 1'bx } };
        end
    end

  assign HRDATA = HRDATA_reg;


  // --------------------------------------------------------------------------
  //   seg driver logic
  // --------------------------------------------------------------------------

  //   产生工作时钟
  reg [ 7: 0 ] clk_1M_cnt = 0;
  reg clk_DRV = 0;

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          clk_1M_cnt <= 0;
          clk_DRV <= 0;
        end
      else if ( clk_1M_cnt == HC595_DRV_CLK_DIV )
        begin
          clk_1M_cnt <= 0;
          clk_DRV <= ~clk_DRV;
        end
      else
        begin
          clk_1M_cnt <= clk_1M_cnt + 1;
          clk_DRV <= clk_DRV;
        end
    end

  reg [ 3: 0] refresh_segnum_reg = 0;
  wire [ 3: 0] refresh_segnum_add1 = refresh_segnum_reg + 1;

  reg [ 7: 0] HC595_CLK_CNT = 0;
  wire[ 7: 0] HC595_CLK_CNT_add1 = HC595_CLK_CNT + 1;

  reg [ 15: 0] seg_data_shifer_reg = 0;

  localparam HC595_CLK_CNT_MAX = 16;
  localparam SEG_NUM_CNT_MAX = 7;

  reg [ 7: 0 ] seg_2_dec = 0;
  reg [ 7: 0 ] seg_dec8bit = 0;
  reg [ 7: 0 ] seg_pos8bit = 0;

  //   信号解码
  always @(* )
    begin
      case ( refresh_segnum_reg)
        default:
          seg_2_dec = segx_reg[ 0 ];
        1:
          seg_2_dec = segx_reg[ 1 ];
        2:
          seg_2_dec = segx_reg[ 2 ];
        3:
          seg_2_dec = segx_reg[ 3 ];
        4:
          seg_2_dec = segx_reg[ 4 ];
        5:
          seg_2_dec = segx_reg[ 5 ];
        6:
          seg_2_dec = segx_reg[ 6 ];
        7:
          seg_2_dec = segx_reg[ 7 ];
      endcase
    end

  always @(* )
    begin
      case ( seg_2_dec[ 4: 0 ] )
        default:
          seg_dec8bit = 8'b11000000;
        1:
          seg_dec8bit = 8'b11111001;
        2:
          seg_dec8bit = 8'b10100100;
        3:
          seg_dec8bit = 8'b10110000;
        4:
          seg_dec8bit = 8'b10011001;
        5:
          seg_dec8bit = 8'b10010010;
        6:
          seg_dec8bit = 8'b10000010;
        7:
          seg_dec8bit = 8'b11111000;
        8:
          seg_dec8bit = 8'b10000000;
        9:
          seg_dec8bit = 8'b10010000;
        10:
          seg_dec8bit = 8'b10001000;
        11:
          seg_dec8bit = 8'b10000011;
        12:
          seg_dec8bit = 8'b11000110;
        13:
          seg_dec8bit = 8'b10100001;
        14:
          seg_dec8bit = 8'b10000110;
        15:
          seg_dec8bit = 8'b10001110;
        16:
          seg_dec8bit = 8'b01000000;
        17:
          seg_dec8bit = 8'b01111001;
        18:
          seg_dec8bit = 8'b00100100;
        19:
          seg_dec8bit = 8'b00110000;
        20:
          seg_dec8bit = 8'b00011001;
        21:
          seg_dec8bit = 8'b00010010;
        22:
          seg_dec8bit = 8'b00000010;
        23:
          seg_dec8bit = 8'b01111000;
        24:
          seg_dec8bit = 8'b00000000;
        25:
          seg_dec8bit = 8'b00010000;
        26:
          seg_dec8bit = 8'b00001000;
        27:
          seg_dec8bit = 8'b00000011;
        28:
          seg_dec8bit = 8'b01000110;
        29:
          seg_dec8bit = 8'b00100001;
        30:
          seg_dec8bit = 8'b00000110;
        31:
          seg_dec8bit = 8'b00001110;
      endcase
    end

  always @(* )
    begin
      case ( refresh_segnum_reg )
        default:
          seg_pos8bit = 8'b00000001;
        1:
          seg_pos8bit = 8'b00000010;
        2:
          seg_pos8bit = 8'b00000100;
        3:
          seg_pos8bit = 8'b00001000;
        4:
          seg_pos8bit = 8'b00010000;
        5:
          seg_pos8bit = 8'b00100000;
        6:
          seg_pos8bit = 8'b01000000;
        7:
          seg_pos8bit = 8'b10000000;
      endcase
    end

  always @( posedge clk_DRV or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          HC595_CLK_CNT <= 0;
          SH_CLK <= 0;
          LD_CLK <= 0;
          HC_DAT <= 0;
          refresh_segnum_reg <= 0;
          seg_data_shifer_reg <= 0;
        end
      else if ( ( HC595_CLK_CNT[ 7: 1] == HC595_CLK_CNT_MAX ) )
        begin
          // 刷新脉冲
          if ( HC595_CLK_CNT[ 0 ] )
            begin
              HC595_CLK_CNT <= 0;
              SH_CLK <= 0;
              LD_CLK <= 1;

              HC_DAT <= 0;
              refresh_segnum_reg <= refresh_segnum_reg;
              seg_data_shifer_reg <= { seg_dec8bit, seg_pos8bit };
            end
          else
            begin
              HC595_CLK_CNT <= HC595_CLK_CNT_add1;
              SH_CLK <= 0;
              LD_CLK <= 0;

              HC_DAT <= 0;
              if ( refresh_segnum_reg == SEG_NUM_CNT_MAX)
                begin
                  refresh_segnum_reg <= 0;
                end
              else
                begin
                  refresh_segnum_reg <= refresh_segnum_add1;
                end
              seg_data_shifer_reg <= seg_data_shifer_reg;
            end
        end
      else
        begin
          begin
            // 寄存脉冲
            if ( HC595_CLK_CNT[ 0 ] )
              begin
                HC595_CLK_CNT <= HC595_CLK_CNT_add1;
                SH_CLK <= 1;
                LD_CLK <= 0;

                HC_DAT <= HC_DAT;
                refresh_segnum_reg <= refresh_segnum_reg;
                seg_data_shifer_reg <= ( seg_data_shifer_reg << 1 );
              end
            else
              begin
                HC595_CLK_CNT <= HC595_CLK_CNT_add1;
                SH_CLK <= 0;
                LD_CLK <= 0;

                HC_DAT <= seg_data_shifer_reg[ 15 ];
                refresh_segnum_reg <= refresh_segnum_reg;
                seg_data_shifer_reg <= seg_data_shifer_reg;
              end
          end
        end
    end

endmodule
