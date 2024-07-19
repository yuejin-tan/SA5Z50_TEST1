module AHBlite_Decoder
  #(
     parameter Port0_en = 1,
     parameter Port1_en = 1,
     parameter Port2_en = 1,
     parameter Port3_en = 1
   )(
     input [31: 0] HADDR,
     input wire HSEL_M,

     output wire P0_HSEL,
     output wire P1_HSEL,
     output wire P2_HSEL,
     output wire P3_HSEL
   );

  assign P0_HSEL = ((HADDR[31: 16] == 16'hA000) && HSEL_M) ? Port0_en : 1'd0;

  assign P1_HSEL = ((HADDR[31: 16] == 16'hA001) && HSEL_M) ? Port1_en : 1'd0;

  assign P2_HSEL = ((HADDR[31: 16] == 16'hA002) && HSEL_M) ? Port2_en : 1'd0;

  assign P3_HSEL = ((!P0_HSEL) && (!P1_HSEL) && (!P2_HSEL) && HSEL_M) ? Port3_en : 1'd0;

endmodule
