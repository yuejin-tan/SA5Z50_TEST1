module ahb_null
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
    output wire HRESP   // Device response (always OKAY)

  );

  // err
  assign HRESP = 1'b1;
  // slave always ready
  assign HREADYOUT = 1'b1;
  // no data
  assign HRDATA = 0;

endmodule
