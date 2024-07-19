module ahb_uart
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

    // UART SIG
    output wire TX_OUT,
    input wire RX_IN
  );


  // --------------------------------------------------------------------------
  //   ahb interface logic
  // --------------------------------------------------------------------------

  // No err
  assign HRESP = 1'b0;
  // slave always ready
  assign HREADYOUT = 1'b1;

  // Register interface
  wire [ 15: 0] addr;
  wire read_en;
  wire write_en;
  wire [ 3: 0] byte_strobe;

  // transfer request issued only in SEQ and NONSEQ status and slave is
  // selected and last transfer finish
  wire trans_req= HREADY & HSEL & HTRANS[ 1 ]; //传输请求
  wire ahb_read_req = trans_req & ( ~HWRITE ); // AHB read request；为低电平的时候，表示读
  wire ahb_write_req = trans_req & HWRITE;  // AHB write request
  wire update_read_req;    // To update the read enable register
  wire update_write_req;   // To update the write enable register

  reg [ 15: 0] addr_reg = 0;     // address signal, registered
  reg read_en_reg = 0;  // read enable signal, registered
  reg write_en_reg = 0; // write enable signal, registered

  reg [ 3: 0] byte_strobe_reg = 0; // registered output for byte strobe
  reg [ 3: 0] byte_strobe_nxt = 0; // next state for byte_strobe_reg

  // Address signal registering, to make the address and data active at the same cycle
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          addr_reg <= { ( 16 ) { 1'b0 } };
        end  //default address 0 is selected
      else if ( trans_req)
        begin
          addr_reg <= HADDR[ 15: 0 ];
        end //地址信号设定
    end


  // register read signal generation
  assign update_read_req = ahb_read_req | ( read_en_reg & HREADY ); // Update read enable control if
  //  1. When there is a valid read request
  //  2. When there is an active read, update it at the end of transfer (HREADY=1)

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          read_en_reg <= 1'b0;
        end
      else if ( update_read_req)
        begin
          read_en_reg <= ahb_read_req;
        end
    end

  // register write signal generation
  assign update_write_req = ahb_write_req | ( write_en_reg & HREADY );  // Update write enable control if
  //  1. When there is a valid write request
  //  2. When there is an active write, update it at the end of transfer (HREADY=1)

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          write_en_reg <= 1'b0;
        end
      else if ( update_write_req)
        begin
          write_en_reg <= ahb_write_req; //总线写请求
        end
    end

  // byte strobe signal
  always @( *)
    begin
      if ( HSIZE == 3'b000)    //byte，传输数据位数000：8 bits
        begin
          case ( HADDR[ 1: 0 ] )
            2'b00:
              byte_strobe_nxt = 4'b0001; //HxDATA[0:7]
            2'b01:
              byte_strobe_nxt = 4'b0010; //HxDATA[15:8]
            2'b10:
              byte_strobe_nxt = 4'b0100; //HxDATA[23:16]
            2'b11:
              byte_strobe_nxt = 4'b1000; ////HxDATA[31:24]
            default:
              byte_strobe_nxt = 4'bxxxx;
          endcase
        end
      else if ( HSIZE == 3'b001) //half word 16bits
        begin
          if ( HADDR[ 1 ] == 1'b1)
            begin
              byte_strobe_nxt = 4'b1100;
            end //HXDATA[0:15]
          else
            begin
              byte_strobe_nxt = 4'b0011;
            end //HXDATA[16:31]
        end
      else // default 32 bits, word
        begin
          byte_strobe_nxt = 4'b1111; //HXDATA[31:0]
        end
    end

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          byte_strobe_reg <= { 4{ 1'b0 } };
        end
      else if ( update_read_req | update_write_req) //产生读请求或者写请求
        // Update byte strobe registers if
        // 1. if there is a valid read/write transfer request
        // 2. if there is an on going transfer
        begin
          byte_strobe_reg <= byte_strobe_nxt;
        end
    end

  // out
  assign addr = addr_reg[ 15: 0 ];
  assign read_en = read_en_reg;
  assign write_en = write_en_reg;
  assign byte_strobe = byte_strobe_reg;

  // --------------------------------------------------------------------------
  //   reg wr logic
  // --------------------------------------------------------------------------

  // bit0 enble & RESETn
  // bit1 txfifo full
  // bit2 txfifo empty
  // bit3 rxfifo full
  // bit4 rxfifo empty
  reg uart_en = 0;
  reg [ 15: 0] baud_div = 0;

  // fifo
  // TX
  reg [ 7: 0] TX_fifo_buff[ 15: 0 ];
  reg [ 4: 0 ] TX_fifo_wr_ptr = 0;
  reg [ 4: 0 ] TX_fifo_rd_ptr = 0;

  wire TX_fifo_full = ( ( TX_fifo_rd_ptr[ 4 ] != TX_fifo_wr_ptr[ 4 ] ) &&
                        ( TX_fifo_rd_ptr[ 3: 0 ] == TX_fifo_wr_ptr[ 3: 0 ] ) );
  wire TX_fifo_empty = ( TX_fifo_rd_ptr == TX_fifo_wr_ptr );

  // RX
  reg [ 7: 0] RX_fifo_buff[ 15: 0 ];
  reg [ 4: 0 ] RX_fifo_wr_ptr = 0;
  reg [ 4: 0 ] RX_fifo_rd_ptr = 0;

  wire RX_fifo_full = ( ( RX_fifo_rd_ptr[ 4 ] != RX_fifo_wr_ptr[ 4 ] ) &&
                        ( RX_fifo_rd_ptr[ 3: 0 ] == RX_fifo_wr_ptr[ 3: 0 ] ) );
  wire RX_fifo_empty = ( RX_fifo_rd_ptr == RX_fifo_wr_ptr );

  //写寄存器1
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          uart_en <= 0;
          baud_div <= 0;
          TX_fifo_wr_ptr <= 0;

          // md，不支持generate for
          TX_fifo_buff[ 0 ] <= 0;
          TX_fifo_buff[ 1 ] <= 0;
          TX_fifo_buff[ 2 ] <= 0;
          TX_fifo_buff[ 3 ] <= 0;
          TX_fifo_buff[ 4 ] <= 0;
          TX_fifo_buff[ 5 ] <= 0;
          TX_fifo_buff[ 6 ] <= 0;
          TX_fifo_buff[ 7 ] <= 0;
          TX_fifo_buff[ 8 ] <= 0;
          TX_fifo_buff[ 9 ] <= 0;
          TX_fifo_buff[ 10 ] <= 0;
          TX_fifo_buff[ 11 ] <= 0;
          TX_fifo_buff[ 12 ] <= 0;
          TX_fifo_buff[ 13 ] <= 0;
          TX_fifo_buff[ 14 ] <= 0;
          TX_fifo_buff[ 15 ] <= 0;
        end
      else if ( ( addr[ 15: 2 ] == 14'd0 ) & ( write_en ) )
        begin
          if ( byte_strobe[ 0 ] )
            begin
              uart_en <= HWDATA[ 0 ];
            end
          if ( byte_strobe[ 1 ] && ( !TX_fifo_full ) )
            begin
              TX_fifo_buff[ TX_fifo_wr_ptr[ 3: 0 ] ] <= HWDATA[ 15: 8 ];
              TX_fifo_wr_ptr <= TX_fifo_wr_ptr + 1;
            end
          if ( byte_strobe[ 2 ] )
            begin
              baud_div[ 7: 0] <= HWDATA[ 23: 16 ];
            end
          if ( byte_strobe[ 3 ] )
            begin
              baud_div[ 15: 8] <= HWDATA[ 31: 24 ];
            end
        end
      else if ( !uart_en)
        begin
          TX_fifo_wr_ptr <= 0;

          // md，不支持generate for
          TX_fifo_buff[ 0 ] <= 0;
          TX_fifo_buff[ 1 ] <= 0;
          TX_fifo_buff[ 2 ] <= 0;
          TX_fifo_buff[ 3 ] <= 0;
          TX_fifo_buff[ 4 ] <= 0;
          TX_fifo_buff[ 5 ] <= 0;
          TX_fifo_buff[ 6 ] <= 0;
          TX_fifo_buff[ 7 ] <= 0;
          TX_fifo_buff[ 8 ] <= 0;
          TX_fifo_buff[ 9 ] <= 0;
          TX_fifo_buff[ 10 ] <= 0;
          TX_fifo_buff[ 11 ] <= 0;
          TX_fifo_buff[ 12 ] <= 0;
          TX_fifo_buff[ 13 ] <= 0;
          TX_fifo_buff[ 14 ] <= 0;
          TX_fifo_buff[ 15 ] <= 0;
        end
    end

  reg [ 31: 0] HRDATA_reg = 0;

  always @( *)
    begin
      if ( ( addr[ 15: 2 ] == 14'd0 ) & ( read_en ) )
        begin
          HRDATA_reg = { baud_div, RX_fifo_buff[ RX_fifo_rd_ptr[ 3: 0 ] ],
                         3'b0, RX_fifo_empty, RX_fifo_full, TX_fifo_empty, TX_fifo_full, uart_en };
        end
      else
        begin
          HRDATA_reg = { 32{ 1'bx } };
        end
    end

  assign HRDATA = HRDATA_reg;

  // RX FIFO READ
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          RX_fifo_rd_ptr <= 0;
        end
      else if ( !uart_en)
        begin
          RX_fifo_rd_ptr <= 0;
        end
      else if ( ( addr[ 15: 2 ] == 14'd0 ) & ( read_en ) & ( byte_strobe[ 1 ] ) & ( !RX_fifo_empty ) )
        begin
          RX_fifo_rd_ptr <= RX_fifo_rd_ptr + 1;
        end
    end


  // --------------------------------------------------------------------------
  //   uart driver logic
  // --------------------------------------------------------------------------

  //   产生TX时钟
  reg [ 15: 0 ] TX_DIV_cnt = 0;
  reg CLK_TX_pulse = 0;

  reg TX_DIV_EN = 0;

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          TX_DIV_cnt <= 0;
          CLK_TX_pulse <= 0;
        end
      else if ( !TX_DIV_EN )
        begin
          TX_DIV_cnt <= 0;
          CLK_TX_pulse <= 0;
        end
      else if ( TX_DIV_cnt == baud_div )
        begin
          TX_DIV_cnt <= 0;
          CLK_TX_pulse <= 1;
        end
      else
        begin
          TX_DIV_cnt <= TX_DIV_cnt + 1;
          CLK_TX_pulse <= 0;
        end
    end

  // TX控制逻辑
  reg [ 10: 0] TX_shift_reg = 11'b1_1111_1111_01;
  reg [ 4: 0 ] TX_shiftOUT_cnt = 0;
  reg TX_ready = 1;

  assign TX_OUT = TX_shift_reg[ 0 ];

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          TX_shift_reg <= 11'b1_1111_1111_01;
          TX_shiftOUT_cnt <= 0;
          TX_ready <= 1;
          TX_DIV_EN <= 0;

          TX_fifo_rd_ptr <= 0;
        end
      else if ( !uart_en )
        begin
          TX_shift_reg <= 11'b1_1111_1111_01;
          TX_shiftOUT_cnt <= 0;
          TX_ready <= 1;
          TX_DIV_EN <= 0;

          TX_fifo_rd_ptr <= 0;
        end
      else if ( ( !TX_fifo_empty) & TX_ready )
        begin
          TX_shift_reg <= { 2'b11, TX_fifo_buff[ TX_fifo_rd_ptr[ 3: 0 ] ], 1'b0 };
          TX_shiftOUT_cnt <= 0;
          TX_ready <= 0;
          TX_DIV_EN <= 1;

          TX_fifo_rd_ptr <= TX_fifo_rd_ptr + 1;
        end
      else if ( ( !TX_ready ) & CLK_TX_pulse )
        begin
          if ( TX_shiftOUT_cnt == 19)
            begin
              TX_shift_reg <= 11'b1_1111_1111_01;
              TX_shiftOUT_cnt <= 0;
              TX_ready <= 1;
              TX_DIV_EN <= 0;
            end
          else
            begin
              if ( TX_shiftOUT_cnt[ 0 ] )
                begin
                  TX_shift_reg <= { 1'b1, TX_shift_reg[ 10: 1 ] };
                end
              TX_shiftOUT_cnt <= TX_shiftOUT_cnt + 1;
            end
        end
    end


  //   产生RX时钟
  reg [ 15: 0 ] RX_DIV_cnt = 0;
  reg CLK_RX_pulse = 0;

  reg RX_DIV_EN = 0;

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          RX_DIV_cnt <= 0;
          CLK_RX_pulse <= 0;
        end
      else if ( !RX_DIV_EN )
        begin
          RX_DIV_cnt <= 0;
          CLK_RX_pulse <= 0;
        end
      else if ( RX_DIV_cnt == baud_div )
        begin
          RX_DIV_cnt <= 0;
          CLK_RX_pulse <= 1;
        end
      else
        begin
          RX_DIV_cnt <= RX_DIV_cnt + 1;
          CLK_RX_pulse <= 0;
        end
    end

  // RX控制逻辑
  reg [ 5: 0 ] RX_samp_regs = 0;

  // 采样和判定
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          RX_samp_regs <= 0;
        end
      else
        begin
          RX_samp_regs <= { RX_IN, RX_samp_regs[ 5: 1 ] };
        end
    end

  reg regard_RX_samp_low = 1;
  reg regard_RX_samp_high = 0;
  always @(* )
    begin
      case ( RX_samp_regs)
        6'o00, 6'o10, 6'o20, 6'o40, 6'o01, 6'o02, 6'o04,
        6'o03, 6'o05, 6'o06, 6'o11, 6'o12, 6'o14, 6'o21, 6'o22, 6'o24, 6'o41, 6'o42, 6'o44, 6'o30, 6'o50, 6'o60 :
          begin
            regard_RX_samp_low = 1;
            regard_RX_samp_high = 0;
          end
        6'o77, 6'o76, 6'o75, 6'o73, 6'o67, 6'o57, 6'o37,
        6'o71, 6'o72, 6'o74, 6'o66, 6'o65, 6'o63, 6'o56, 6'o55, 6'o53, 6'o36, 6'o35, 6'o33, 6'o17, 6'o27, 6'o47 :
          begin
            regard_RX_samp_low = 0;
            regard_RX_samp_high = 1;
          end
        default:
          // 20种55开的情况
          begin
            regard_RX_samp_low = 0;
            regard_RX_samp_high = 0;
          end
      endcase
    end

  // 可能的起始位判定
  reg RX_samp_low_d = 1;
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          RX_samp_low_d <= 1;
        end
      else
        begin
          RX_samp_low_d <= regard_RX_samp_low;
        end
    end

  wire RX_start_bit_req = ( ~RX_samp_low_d ) & regard_RX_samp_low;

  reg [ 8: 0 ] RX_shift_reg = 0;
  reg [ 4: 0 ] RX_shiftIN_cnt = 0;
  reg RX_recving = 0;

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          RX_shift_reg <= 0;
          RX_shiftIN_cnt <= 0;
          RX_recving <= 0;
          RX_DIV_EN <= 0;

          // md，不支持generate for
          RX_fifo_buff[ 0 ] <= 0;
          RX_fifo_buff[ 1 ] <= 0;
          RX_fifo_buff[ 2 ] <= 0;
          RX_fifo_buff[ 3 ] <= 0;
          RX_fifo_buff[ 4 ] <= 0;
          RX_fifo_buff[ 5 ] <= 0;
          RX_fifo_buff[ 6 ] <= 0;
          RX_fifo_buff[ 7 ] <= 0;
          RX_fifo_buff[ 8 ] <= 0;
          RX_fifo_buff[ 9 ] <= 0;
          RX_fifo_buff[ 10 ] <= 0;
          RX_fifo_buff[ 11 ] <= 0;
          RX_fifo_buff[ 12 ] <= 0;
          RX_fifo_buff[ 13 ] <= 0;
          RX_fifo_buff[ 14 ] <= 0;
          RX_fifo_buff[ 15 ] <= 0;
          RX_fifo_wr_ptr <= 0;
        end
      else if ( !uart_en )
        begin
          RX_shift_reg <= 0;
          RX_shiftIN_cnt <= 0;
          RX_recving <= 0;
          RX_DIV_EN <= 0;

          // md，不支持generate for
          RX_fifo_buff[ 0 ] <= 0;
          RX_fifo_buff[ 1 ] <= 0;
          RX_fifo_buff[ 2 ] <= 0;
          RX_fifo_buff[ 3 ] <= 0;
          RX_fifo_buff[ 4 ] <= 0;
          RX_fifo_buff[ 5 ] <= 0;
          RX_fifo_buff[ 6 ] <= 0;
          RX_fifo_buff[ 7 ] <= 0;
          RX_fifo_buff[ 8 ] <= 0;
          RX_fifo_buff[ 9 ] <= 0;
          RX_fifo_buff[ 10 ] <= 0;
          RX_fifo_buff[ 11 ] <= 0;
          RX_fifo_buff[ 12 ] <= 0;
          RX_fifo_buff[ 13 ] <= 0;
          RX_fifo_buff[ 14 ] <= 0;
          RX_fifo_buff[ 15 ] <= 0;
          RX_fifo_wr_ptr <= 0;
        end
      else if ( ( !RX_recving ) & RX_start_bit_req)
        begin
          RX_shift_reg <= 0;
          RX_shiftIN_cnt <= 0;
          RX_recving <= 1;
          RX_DIV_EN <= 1;
        end
      else if ( RX_recving & CLK_RX_pulse)
        begin
          if ( RX_shiftIN_cnt == 18)
            // 提前结束，预留时钟偏差余量
            begin
              if ( !RX_fifo_full)
                begin
                  RX_fifo_buff[ RX_fifo_wr_ptr[ 3: 0 ] ] <= RX_shift_reg[ 8: 1 ];
                  RX_fifo_wr_ptr <= RX_fifo_wr_ptr + 1;
                end

              RX_shift_reg <= 0;
              RX_shiftIN_cnt <= 0;
              RX_recving <= 0;
              RX_DIV_EN <= 0;
            end
          else
            begin
              if ( ~RX_shiftIN_cnt[ 0 ] )
                begin
                  if ( regard_RX_samp_high)
                    begin
                      RX_shift_reg <= { 1'b1, RX_shift_reg[ 8: 1 ] };
                    end
                  else if ( regard_RX_samp_low)
                    begin
                      RX_shift_reg <= { 1'b0, RX_shift_reg[ 8: 1 ] };
                    end
                  else
                    begin
                      // 若采样疑似出错，认为是边沿不够陡峭，尝试挽救
                      RX_shift_reg <= { ~RX_shift_reg[ 8 ], RX_shift_reg[ 8: 1 ] };
                    end
                end

              RX_shiftIN_cnt <= RX_shiftIN_cnt + 1;
            end
        end
    end

endmodule
