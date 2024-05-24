module sprite_rom (
    input Clk,
    input logic [18: 0] addr,
    output logic [7: 0] data_out
);

 logic [7: 0] sprite [307200];   // 640 * 480
//   logic [7: 0] sprite [3000];

// for test
//logic [7: 0] sprite [4096];

initial begin
    $readmemh("sprite.txt", sprite);
end

always_ff @(posedge Clk) begin
    data_out <= sprite[addr];
end
// assign data_out = sprite[addr];
    
endmodule