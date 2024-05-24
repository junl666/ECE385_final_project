module palette_rom(
    // input Clk,
    input logic [7: 0] addr,
    output logic [15: 0] data_out
);

logic [15: 0] palette [256];

initial begin
    $readmemh("palette.txt", palette);
end

assign data_out = palette[addr];
    
endmodule