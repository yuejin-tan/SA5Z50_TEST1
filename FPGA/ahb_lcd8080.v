module ahb_lcd8080
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
    output wire CS_8080,
    output wire RS_8080, // 0:CMD 1:DATA
    output wire WR_8080,
    output wire RD_8080,

    inout wire [ 15: 0] DATA_8080
  );


  // --------------------------------------------------------------------------
  //   ahb interface logic
  // --------------------------------------------------------------------------

  // No err
  assign HRESP = 1'b0;

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
              byte_strobe_nxt = 4'b1000; //HxDATA[31:24]
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

  // 主状态
  // 0:cmd_idle
  // 1:cmd_chg2w
  // 2:cmd_wl
  // 3:cmd_wh
  // 4:xxxxxxx
  // 5:cmd_chg2r
  // 6:cmd_rl
  // 7:cmd_rh

  // 8:data_idle
  // 9:data_chg2w
  // 10:data_wl
  // 11:data_wh
  // 12:xxxxxxx
  // 13:data_chg2r
  // 14:data_rl
  // 15:data_rh
  reg [ 3: 0] sta8080 = 0;
  reg [ 3: 0] sta8080_now = 0;

  // 0x0 cmd & 0x4 data
  reg [ 15: 0] cmd_data_w_reg = 0;
  reg [ 15: 0] cmd_data_r_reg = 0;

  // 0x8
  reg ctrl8080_reg = 0;

  // 0x9
  reg [ 7: 0] tick_cmd_data_chg = 255;
  // 0xa
  reg [ 7: 0] tick_write_l = 255;
  // 0xb
  reg [ 7: 0] tick_write_h = 255;
  // 0xc
  reg [ 7: 0] tick_read_l = 255;
  // 0xd
  reg [ 7: 0] tick_read_h = 255;


  // 根据内部状态机判断读写指令完成情况
  wire status_idle = (sta8080[2:0]==0)?1:0;
  wire status_now_idle = (sta8080_now[2:0]==0)?1:0;

  assign HREADYOUT = status_now_idle;

  reg sta8080_cmd_w_req = 0;
  always @( *)
    begin
      if ( ( write_en ) & ( ctrl8080_reg != 0 ) & (status_idle) & (addr[ 15: 2 ] == 14'd0))
        begin
          sta8080_cmd_w_req = 1;
        end
      else
        begin
          sta8080_cmd_w_req = 0;
        end
    end

  reg sta8080_data_w_req = 0;
  always @( *)
    begin
      if ( ( write_en ) & ( ctrl8080_reg != 0 ) & (status_idle) & (addr[ 15: 2 ] == 14'd1))
        begin
          sta8080_data_w_req = 1;
        end
      else
        begin
          sta8080_data_w_req = 0;
        end
    end

  //写寄存器1
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          cmd_data_w_reg <= 0;
        end
      else if (  ( write_en ) & ( ctrl8080_reg != 0 ) & (status_idle))
        begin
          if( addr[ 15: 3 ] == 13'd0 )
            begin
              // cmd write
              if ( byte_strobe[ 0 ] )
                begin
                  cmd_data_w_reg[ 7: 0 ] <= HWDATA[ 7: 0 ];
                end
              if ( byte_strobe[ 1 ] )
                begin
                  cmd_data_w_reg[ 15: 8 ] <= HWDATA[ 15: 8 ];
                end
            end
        end
    end

  //写寄存器2
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          ctrl8080_reg <= 0;

          tick_cmd_data_chg <= 255;
          tick_write_l <= 255;
          tick_write_h <= 255;
          tick_read_l <= 255;
          tick_read_h <= 255;
        end
      else if ( ( addr[ 15: 2 ] == 14'd2 ) & ( write_en ) )
        begin
          if ( byte_strobe[ 0 ] )
            begin
              ctrl8080_reg <= HWDATA[ 0 ];
            end
          if ( byte_strobe[ 1 ] )
            begin
              tick_cmd_data_chg <= HWDATA[ 15: 8 ];
            end
          if ( byte_strobe[ 2 ] )
            begin
              tick_write_l[ 7: 0] <= HWDATA[ 23: 16 ];
            end
          if ( byte_strobe[ 3 ] )
            begin
              tick_write_h <= HWDATA[ 31: 24 ];
            end
        end
      else if ( ( addr[ 15: 2 ] == 14'd3 ) & ( write_en ) )
        begin
          if ( byte_strobe[ 0 ] )
            begin
              tick_read_l <= HWDATA[ 7: 0 ];
            end
          if ( byte_strobe[ 1 ] )
            begin
              tick_read_h <= HWDATA[ 15: 8 ];
            end
        end
    end

  //读寄存器
  reg [ 31: 0] HRDATA_reg = 0;

  always @( *)
    begin
      if (read_en & (addr[ 15: 3 ] == 13'd0) )
        begin
          HRDATA_reg={ 16'b0, cmd_data_r_reg};
        end
      else if(read_en & (addr[ 15: 2 ] == 14'd2) )
        begin
          HRDATA_reg={tick_write_h, tick_write_l, tick_cmd_data_chg, 7'b0, ctrl8080_reg};
        end
      else if(read_en & (addr[ 15: 2 ] == 14'd3) )
        begin
          HRDATA_reg={ 16'b0, tick_read_h, tick_read_l};
        end
      else
        begin
          HRDATA_reg = { 32{ 1'bx } };
        end
    end

  assign HRDATA = HRDATA_reg;

  reg sta8080_cmd_r_req = 0;
  always @( *)
    begin
      if ( ( read_en ) & ( ctrl8080_reg != 0 ) & (status_idle) & (addr[ 15: 2 ] == 14'd0))
        begin
          sta8080_cmd_r_req = 1;
        end
      else
        begin
          sta8080_cmd_r_req = 0;
        end
    end

  reg sta8080_data_r_req = 0;
  always @( *)
    begin
      if ( ( read_en ) & ( ctrl8080_reg != 0 ) & (status_idle) & (addr[ 15: 2 ] == 14'd1))
        begin
          sta8080_data_r_req = 1;
        end
      else
        begin
          sta8080_data_r_req = 0;
        end
    end


  // --------------------------------------------------------------------------
  //   lcd driver logic
  // --------------------------------------------------------------------------

  reg [ 7: 0] cmd_data_chg_cnt = 0;
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          cmd_data_chg_cnt <= 0;
        end
      else if ( sta8080_now[1:0]==1)
        begin
          cmd_data_chg_cnt <= cmd_data_chg_cnt+1;
        end
      else
        begin
          cmd_data_chg_cnt <= 0;
        end
    end

  reg [ 7: 0] write_l_cnt = 0;
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          write_l_cnt <= 0;
        end
      else if ( sta8080_now[2:0]==2)
        begin
          write_l_cnt <= write_l_cnt+1;
        end
      else
        begin
          write_l_cnt <= 0;
        end
    end

  reg [ 7: 0] write_h_cnt = 0;
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          write_h_cnt <= 0;
        end
      else if ( sta8080_now[2:0]==3)
        begin
          write_h_cnt <= write_h_cnt+1;
        end
      else
        begin
          write_h_cnt <= 0;
        end
    end

  reg [ 7: 0] read_l_cnt = 0;
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          read_l_cnt <= 0;
        end
      else if ( sta8080_now[2:0]==6)
        begin
          read_l_cnt <= read_l_cnt+1;
        end
      else
        begin
          read_l_cnt <= 0;
        end
    end

  reg [ 7: 0] read_h_cnt = 0;
  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          read_h_cnt <= 0;
        end
      else if ( sta8080_now[2:0]==7)
        begin
          read_h_cnt <= read_h_cnt+1;
        end
      else
        begin
          read_h_cnt <= 0;
        end
    end


  always @( *)
    begin
      case (sta8080)
        default:
          sta8080_now=0;

        0:
          begin
            if(sta8080_cmd_w_req)
              sta8080_now=2;
            else if(sta8080_cmd_r_req)
              sta8080_now=6;
            else if(sta8080_data_w_req)
              sta8080_now=9;
            else if(sta8080_data_r_req)
              sta8080_now=13;
            else
              sta8080_now=0;
          end

        1:
          begin
            if(cmd_data_chg_cnt==tick_cmd_data_chg)
              sta8080_now=2;
            else
              sta8080_now=1;
          end

        2:
          begin
            if(write_l_cnt==tick_write_l)
              sta8080_now=3;
            else
              sta8080_now=2;
          end

        3:
          begin
            if(write_h_cnt==tick_write_h)
              sta8080_now=0;
            else
              sta8080_now=3;
          end

        5:
          begin
            if(cmd_data_chg_cnt==tick_cmd_data_chg)
              sta8080_now=6;
            else
              sta8080_now=5;
          end

        6:
          begin
            if(read_l_cnt==tick_read_l)
              sta8080_now=7;
            else
              sta8080_now=6;
          end

        7:
          begin
            if(read_h_cnt==tick_read_h)
              sta8080_now=0;
            else
              sta8080_now=7;
          end

        8:
          begin
            if(sta8080_cmd_w_req)
              sta8080_now=1;
            else if(sta8080_cmd_r_req)
              sta8080_now=5;
            else if(sta8080_data_w_req)
              sta8080_now=10;
            else if(sta8080_data_r_req)
              sta8080_now=14;
            else
              sta8080_now=0;
          end

        9:
          begin
            if(cmd_data_chg_cnt==tick_cmd_data_chg)
              sta8080_now=10;
            else
              sta8080_now=9;
          end

        10:
          begin
            if(write_l_cnt==tick_write_l)
              sta8080_now=11;
            else
              sta8080_now=10;
          end

        11:
          begin
            if(write_h_cnt==tick_write_h)
              sta8080_now=8;
            else
              sta8080_now=11;
          end

        13:
          begin
            if(cmd_data_chg_cnt==tick_cmd_data_chg)
              sta8080_now=14;
            else
              sta8080_now=13;
          end

        14:
          begin
            if(read_l_cnt==tick_read_l)
              sta8080_now=15;
            else
              sta8080_now=14;
          end

        15:
          begin
            if(read_h_cnt==tick_read_h)
              sta8080_now=8;
            else
              sta8080_now=15;
          end
      endcase
    end

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          sta8080 <= 0;
        end
      else if ( !ctrl8080_reg )
        begin
          sta8080 <= 0;
        end
      else
        begin
          sta8080 <= sta8080_now;
        end
    end

  assign CS_8080 = !ctrl8080_reg;
  assign RS_8080 = sta8080[3];

  assign WR_8080 = (sta8080[2:0]==2) ? 0: 1;
  assign RD_8080 = (sta8080[2:0]==6) ? 0: 1;

  wire output_en = ((sta8080[2:0]==2)||(sta8080[2:0]==3)) ? 1: 0;

  assign DATA_8080 = output_en ? cmd_data_w_reg : 16'bzzzz_zzzz_zzzz_zzzz;

  always @( posedge HCLK or negedge HRESETn)
    begin
      if ( ~HRESETn)
        begin
          cmd_data_r_reg <= 0;
        end
      else if ( (sta8080[2:0]==6)&(sta8080_now[2:0]==7) )
        begin
          cmd_data_r_reg <= DATA_8080;
        end
    end

endmodule
