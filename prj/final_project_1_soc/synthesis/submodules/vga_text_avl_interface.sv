/************************************************************************
Avalon-MM Interface VGA Text mode display

Modified for DE2-115 board

Register Map:
0x000-0x0257 : VRAM, 80x30 (2400 byte, 600 word) raster order (first column then row)
0x258        : control register

VRAM Format:
X->
[ 31  30-24][ 23  22-16][ 15  14-8 ][ 7    6-0 ]
[IV3][CODE3][IV2][CODE2][IV1][CODE1][IV0][CODE0]

IVn = Draw inverse glyph
CODEn = Glyph code from IBM codepage 437

Control Register Format:
[[31-25][24-21][20-17][16-13][ 12-9][ 8-5 ][ 4-1 ][   0    ] 
[[RSVD ][FGD_R][FGD_G][FGD_B][BKG_R][BKG_G][BKG_B][RESERVED]

VSYNC signal = bit which flips on every Vsync (time for new frame), used to synchronize software
BKG_R/G/B = Background color, flipped with foreground when IVn bit is set
FGD_R/G/B = Foreground color, flipped with background when Inv bit is set

************************************************************************/
module vga_text_avl_interface (
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
	input  logic [11:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data
	
	// Exported Conduit (mapped to VGA port - make sure you export in Platform Designer)
	output logic [3:0]  red, green, blue,	// VGA color channels (mapped to output pins in top-level)
	output logic hs, vs,					// VGA HS/VS
	output logic sync, blank, pixel_clk		// Required by DE2-115 video encoder
);

// put other local variables here
logic [9:0]  DrawX, DrawY;					// VGA Draw X and Y coordinates
logic [3:0] FGD_R, FGD_G, FGD_B, 
			BKG_R, BKG_G, BKG_B;     		// Foreground and background colors
logic is_font;								// Whether a pixel is font or not
logic inverse;								// Inverse the color or not
logic [10:0] sprite_addr;					// Sprite address
logic [7:0]  sprite_data;					// Sprite data
logic [31:0] vram_data_s;					// VRAM data(32-bit word), connect vram_data[0:1] to this signal
logic [15:0] vram_data[0:1];				// VRAM data(32-bit word)
logic [15:0]  bytes_data;					// 2-Byte data(16-bit)
logic [3:0] FGD_index, BKG_index;			// The index of the color bytes in the palette registers(from 0 to 15)
logic [11:0] FGD_vals [1:0];				// The values of the 2 RGB color in one 32-bit word
logic [11:0] BKG_vals [1:0];				// The values of the 2 RGB color in one 32-bit word
logic [11:0] FGD_val, BKG_val;				// The value of the RGB color 
logic [6:0] ascii_code;						// The ASCII code of the character
logic [31:0] rgb_reg [8];					// The RGB color register


integer DrawX_i, DrawY_i;					// Intergel of DrawX and DrawY
integer shape_x, shape_y;					// The x and y coordinate(left above) of the byte
integer word_addr;							// The address of the 4-byte word in LOCAL_REG(VRAM)
integer bytes_index;						// The index of the 2-byte data in the 4-byte word(0, 1)

// integer assignment
always_comb 
begin
	DrawX_i = DrawX;
	DrawY_i = DrawY;
	shape_x = (DrawX_i >> 3) * 8;		
	shape_y = (DrawY_i >> 4) * 16;		
	word_addr = (DrawX_i >> 4) + (DrawY_i >> 4)*40;		
	bytes_index = (shape_x >> 3) % 2;	
end

// Assign vram_data based on the word address(32-bit word)
assign vram_data[0] = vram_data_s[15:0];
assign vram_data[1] = vram_data_s[31:16];

// Assign byte_data based on the byte index(8-bit)
assign bytes_data = vram_data[bytes_index];

// Assign FGD_index and BKG_index based on bytes_data
assign FGD_index = bytes_data[7:4];
assign BKG_index = bytes_data[3:0];

// Assign FGD_val and BKG_val based on FGD_index and BKG_index
assign FGD_vals[0] = rgb_reg[(FGD_index >> 1)][12:1];
assign FGD_vals[1] = rgb_reg[(FGD_index >> 1)][24:13];
assign BKG_vals[0] = rgb_reg[(BKG_index >> 1)][12:1];
assign BKG_vals[1] = rgb_reg[(BKG_index >> 1)][24:13];
assign FGD_val = FGD_vals[FGD_index % 2];
assign BKG_val = BKG_vals[BKG_index % 2];

// Assign FGD_R, FGD_G, FGD_B, BKG_R, BKG_G, BKG_B based on FGD_val and BKG_val
assign FGD_R = FGD_val[11:8];
assign FGD_G = FGD_val[7:4];
assign FGD_B = FGD_val[3:0];
assign BKG_R = BKG_val[11:8];
assign BKG_G = BKG_val[7:4];
assign BKG_B = BKG_val[3:0];

// Calculate sprite address
assign ascii_code = bytes_data[14:8];
assign sprite_addr = DrawY - shape_y + 16*ascii_code;

// Check if the pixel is font or not
assign is_font = sprite_data[7 - (DrawX - shape_x)];
// Check if the color should be inversed
assign inverse = bytes_data[15];

// Declare submodules..e.g. VGA controller, ROMS, etc
vga_controller vga_controller_inst (
	// Inputs
	.Clk(CLK),
	.Reset(RESET),
	// Outputs
	.hs(hs),
	.vs(vs),
	.pixel_clk(pixel_clk),
	.blank(blank),
	.sync(sync),
	.DrawX(DrawX),
	.DrawY(DrawY)
);

font_rom font_rom_inst (
	// Inputs
	.addr(sprite_addr),
	// Outputs
	.data(sprite_data)
);

lab9_soc_onchip_memory2_0 lab9_soc_onchip_memory2_0_inst (
	// Inputs
	.address(AVL_ADDR),
	.address2(word_addr),
	.byteenable(AVL_BYTE_EN),
	.byteenable2(4'b1111),
	.chipselect(AVL_CS),
	.chipselect2(1'b1),
	.clk(CLK),
	.clken(1'b1),
	.clken2(1'b1),
	.freeze(1'b0),
	.reset(RESET),
	.reset_req(1'b0),
	.write(AVL_WRITE),
	.write2(1'b0),
	.writedata(AVL_WRITEDATA),
	.writedata2(),
	// Outputs
	.readdata(AVL_READDATA),
	.readdata2(vram_data_s)
);

// Get FGD_vals and BKG_vals
always_ff @(posedge CLK) begin
	if (AVL_ADDR >= 12'h800 && AVL_ADDR <= 12'h807 && AVL_WRITE && AVL_CS)
	begin
		rgb_reg[AVL_ADDR - 12'h800] <= AVL_WRITEDATA;
	end
end

//handle drawing (may either be combinational or sequential - or both).
logic [3:0] red_reg, green_reg, blue_reg;
color_mapper my_color_mapper (
	// Inputs
	.is_font(is_font),
	.inverse(inverse),
	.FGD_R(FGD_R),
	.FGD_G(FGD_G),
	.FGD_B(FGD_B),
	.BKG_R(BKG_R),
	.BKG_G(BKG_G),
	.BKG_B(BKG_B),
	.DrawX(DrawX),
	.DrawY(DrawY),
	// Outputs
	.VGA_R(red_reg),
	.VGA_G(green_reg),
	.VGA_B(blue_reg)
);
///////////////////////////// Remove Glitches Begin /////////////////////////////
always @(posedge pixel_clk) 
begin
	if (blank)
	begin
		red <= red_reg;
		green <= green_reg;
		blue <= blue_reg;		
	end
	else
	begin
		red <= 4'b0;
		green <= 4'b0;
		blue <= 4'b0;
	end
		
end
///////////////////////////// Remove Glitches End /////////////////////////////

endmodule