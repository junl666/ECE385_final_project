// This module is the interface to write the data in sprite ROM to the on-chip memory.
`define SPRITE_NUM 8
`define SPRITE_WIDTH 30
`define SPRITE_HEIGHT 50

module spriteRam_avl_interface (
	// Avalon Clock Input, note this clock is also used for VGA, so this must be 50Mhz
	// We can put a clock divider here in the future to make this IP more generalizable
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,					// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,			// Avalon-MM Byte Enable
	input  logic [2:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data

	// Imported Conduit
	input logic [9:0]  drawx, drawy,		// VGA Draw X and Y coordinates
	// Exported Conduit
	output logic [7:0] color_index,			// VGA Color Index
	output logic [1:0] game_status			// The status of the game, 11 - game over: win, 10 - game over: lose
);


logic [31:0] LOCAL_REG [`SPRITE_NUM];		// Registers to store the sprite data(the position and the pose)
											// [31: 22] - the x coordinate of the sprite
											// [21: 12] - the y coordinate of the sprite
											// [6: 0] - the pose index of the sprite

logic is_point_in_rect_arr [0: `SPRITE_NUM - 1]; // An array of functions to check the point is in a rectangle

logic [9:0]  shapex, shapey;				// The x and y coordinates of the sprite in the sprite ROM	
logic [9:0]  del_x, del_y;	
logic [15: 0] sprite_rom_addr;
logic [7:0]  sprite_rom_color_index;		// The color index specified by sprite_rom_addr
logic [9:0]  sprite1_x, sprite1_y;

assign game_status = LOCAL_REG[0][8: 7];
				
// Register operations
always_ff @(posedge CLK) begin
	if (RESET)						// Reset
	begin
		for(integer i = 0; i < `SPRITE_NUM; i = i + 1)
		begin
			LOCAL_REG[i] <= 32'b0;
		end		
	end

	else if (AVL_WRITE && AVL_CS) 	// Write
	begin
		case (AVL_BYTE_EN)
			4'b1111: LOCAL_REG[AVL_ADDR] 		<= AVL_WRITEDATA;
			4'b1100: LOCAL_REG[AVL_ADDR][31:16] <= AVL_WRITEDATA[31:16];
			4'b0011: LOCAL_REG[AVL_ADDR][15:0]  <= AVL_WRITEDATA[15:0];
			4'b1000: LOCAL_REG[AVL_ADDR][31:24] <= AVL_WRITEDATA[31:24];
			4'b0100: LOCAL_REG[AVL_ADDR][23:16] <= AVL_WRITEDATA[23:16];
			4'b0010: LOCAL_REG[AVL_ADDR][15:8]  <= AVL_WRITEDATA[15:8];
			4'b0001: LOCAL_REG[AVL_ADDR][7:0] 	<= AVL_WRITEDATA[7:0];
			default: LOCAL_REG[AVL_ADDR] 		<= 32'b0;
		endcase
	end

	else if (AVL_READ && AVL_CS) 	// Read
	begin
		AVL_READDATA = LOCAL_REG[AVL_ADDR];
	end

	else							// No operation
		;
end

// Check if the point is in a rectangle
always_ff @(posedge CLK) begin
    begin
        for (integer i = 0; i < `SPRITE_NUM; i = i + 1) begin
            is_point_in_rect_arr[i] = is_point_in_rect(drawx, drawy, LOCAL_REG[i][31:22], LOCAL_REG[i][21:12], LOCAL_REG[i][31:22] + `SPRITE_WIDTH, LOCAL_REG[i][21:12] + `SPRITE_HEIGHT);
        end
    end
end

integer first_1_index;
logic  found;
// Find the first 1 in the array and calculate the color index
always_ff @(posedge CLK) begin  // always_ff????????
	if (is_point_in_rect_arr[0] ==1'b1)begin
		first_1_index = 0;
		found = 1'b1;
	end
	else if (is_point_in_rect_arr[1] == 1'b1)begin
		first_1_index = 1;
		found = 1'b1;		
	end
	else if (is_point_in_rect_arr[2] == 1'b1)begin
		first_1_index = 2;
		found = 1'b1;		
	end
	else if (is_point_in_rect_arr[3] == 1'b1)begin
		first_1_index = 3;
		found = 1'b1;		
	end
	else if (is_point_in_rect_arr[4] == 1'b1)begin
		first_1_index = 4;
		found = 1'b1;		
	end
	else if (is_point_in_rect_arr[5] ==1'b1)begin
		first_1_index = 5;
		found = 1'b1;		
	end
	else if (is_point_in_rect_arr[6] == 1'b1)begin
		first_1_index = 6;
		found = 1'b1;		
	end
	else if (is_point_in_rect_arr[7] == 1'b1)begin
		first_1_index = 7;
		found = 1'b1;		
	end
	else 
		found = 1'b0;		
end

assign color_index = sprite_rom_color_index;
always_ff @(posedge CLK) begin
	if (found)begin
		shapex = (LOCAL_REG[first_1_index][6:0] % 10) * `SPRITE_WIDTH;
		shapey = (LOCAL_REG[first_1_index][6:0] / 10) * `SPRITE_HEIGHT;
		del_x = drawx - LOCAL_REG[first_1_index][31:22];
		del_y = drawy - LOCAL_REG[first_1_index][21:12];
		sprite_rom_addr = (shapey + del_y) * 300 + (shapex + del_x);
	end

	else begin
		sprite_rom_addr = 16'h0;  // let the first sprite be "White"(index 0)!!!!!need to be write in sprite.txt
    end
end

final_project_1_soc_onchip_memory2_0 sprite_rom_inst(
		.clk         (CLK), 
		.address     (sprite_rom_addr),
		.debugaccess (), 
		.clken       (1'b1), 
		.chipselect  (1'b1),  
		.write       (1'b0),  
		.readdata    (sprite_rom_color_index), 
		.writedata   (), 
		.reset		 (RESET),
		.reset_req   (1'b0),           
		.freeze      (1'b0) 

);


function logic is_point_in_rect(
    logic [31:0] point_x, logic [31:0] point_y, // The coordinates of the point
    logic [31:0] rect_x1, logic [31:0] rect_y1, // The coordinates of the top-left corner of the rectangle
	logic [31:0] rect_x2, logic [31:0] rect_y2  // The coordinates of the bottom-right corner of the rectangle
);
    return point_x >= rect_x1 && point_x <= rect_x2 && point_y >= rect_y1 && point_y <= rect_y2;
endfunction

endmodule