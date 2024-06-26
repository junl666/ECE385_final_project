// final_project_1.sv
// Top level module for the final project.
`define WIN_WIDTH 200
`define WIN_HEIGHT 200
`define WIN_X 220
`define WIN_Y 150
`define WIN_BIAS 307200
`define LOSE_BIAS 347200


module final_project_1( 
            input               CLOCK_50,
            input        [3:0]  KEY,          //bit 0 is set up as Reset
            output logic [6:0]  HEX0, HEX1,
            // VGA Interface 
            output logic [7:0]  VGA_R,        //VGA Red
                                VGA_G,        //VGA Green
                                VGA_B,        //VGA Blue
            output logic        VGA_CLK,      //VGA Clock
                                VGA_SYNC_N,   //VGA Sync signal
                                VGA_BLANK_N,  //VGA Blank signal
                                VGA_VS,       //VGA virtical sync signal
                                VGA_HS,       //VGA horizontal sync signal
            // CY7C67200 Interface
            inout  wire  [15:0] OTG_DATA,     //CY7C67200 Data bus 16 Bits
            output logic [1:0]  OTG_ADDR,     //CY7C67200 Address 2 Bits
            output logic        OTG_CS_N,     //CY7C67200 Chip Select
                                OTG_RD_N,     //CY7C67200 Write
                                OTG_WR_N,     //CY7C67200 Read
                                OTG_RST_N,    //CY7C67200 Reset
            input               OTG_INT,      //CY7C67200 Interrupt
            // SDRAM Interface for Nios II Software
            output logic [12:0] DRAM_ADDR,    //SDRAM Address 13 Bits
            inout  wire  [31:0] DRAM_DQ,      //SDRAM Data 32 Bits
            output logic [1:0]  DRAM_BA,      //SDRAM Bank Address 2 Bits
            output logic [3:0]  DRAM_DQM,     //SDRAM Data Mast 4 Bits
            output logic        DRAM_RAS_N,   //SDRAM Row Address Strobe
                                DRAM_CAS_N,   //SDRAM Column Address Strobe
                                DRAM_CKE,     //SDRAM Clock Enable
                                DRAM_WE_N,    //SDRAM Write Enable
                                DRAM_CS_N,    //SDRAM Chip Select
                                DRAM_CLK,     //SDRAM Clock
            // SRAM Interface
            output logic CE, UB, LB, OE, WE,
            output logic [19:0] SRAM_ADDR,
            inout wire [15:0] SRAM_DQ
            );
    
    logic Reset_h, Reset_game, Clk;
    logic [15:0] keycode;
    logic [9:0] DrawX, DrawY;                   // The current pixel coordinates
    logic [3:0] red, green, blue;               // The RGB values specified by DrawX and DrawY, but need too remove glitches
    logic [7:0] color_index1, color_index2;     // The color index for the background and sprite
    logic [7:0] color_index;                    // The true color index
    logic [15:0] color;                         // The color specified by the color index
    logic [15:0] Data_from_SRAM;
    logic [1:0] game_status;

    assign Clk = CLOCK_50;
    always_ff @ (posedge Clk) begin
        Reset_h <= ~(KEY[0]);                   // The push buttons are active low
        Reset_game <= ~(KEY[1]);
    end
    
    logic [1:0] hpi_addr;
    logic [15:0] hpi_data_in, hpi_data_out;
    logic hpi_r, hpi_w, hpi_cs, hpi_reset;
    
    // Interface between NIOS II and EZ-OTG chip
    hpi_io_intf hpi_io_inst(
                            .Clk(Clk),
                            .Reset(Reset_h),
                            // signals connected to NIOS II
                            .from_sw_address(hpi_addr),
                            .from_sw_data_in(hpi_data_in),
                            .from_sw_data_out(hpi_data_out),
                            .from_sw_r(hpi_r),
                            .from_sw_w(hpi_w),
                            .from_sw_cs(hpi_cs),
                            .from_sw_reset(hpi_reset),
                            // signals connected to EZ-OTG chip
                            .OTG_DATA(OTG_DATA),    
                            .OTG_ADDR(OTG_ADDR),    
                            .OTG_RD_N(OTG_RD_N),    
                            .OTG_WR_N(OTG_WR_N),    
                            .OTG_CS_N(OTG_CS_N),
                            .OTG_RST_N(OTG_RST_N)
    );
     
     // You need to make sure that the port names here match the ports in Qsys-generated codes.
     final_project_1_soc nios_system(
                             .clk_clk(Clk),         
                             .reset_reset_n(1'b1),    // Never reset NIOS
                             .sdram_wire_addr(DRAM_ADDR), 
                             .sdram_wire_ba(DRAM_BA),   
                             .sdram_wire_cas_n(DRAM_CAS_N),
                             .sdram_wire_cke(DRAM_CKE),  
                             .sdram_wire_cs_n(DRAM_CS_N), 
                             .sdram_wire_dq(DRAM_DQ),   
                             .sdram_wire_dqm(DRAM_DQM),  
                             .sdram_wire_ras_n(DRAM_RAS_N),
                             .sdram_wire_we_n(DRAM_WE_N), 
                             .sdram_clk_clk(DRAM_CLK),
                             .keycode_export(keycode),  
                             .otg_hpi_address_export(hpi_addr),
                             .otg_hpi_data_in_port(hpi_data_in),
                             .otg_hpi_data_out_port(hpi_data_out),
                             .otg_hpi_cs_export(hpi_cs),
                             .otg_hpi_r_export(hpi_r),
                             .otg_hpi_w_export(hpi_w),
                             .otg_hpi_reset_export(hpi_reset),

                            // VGA interface
                            .vga_port_drawx (DrawX),
                            .vga_port_drawy (DrawY),
                            .vga_port_color_index (color_index2),
                            .vga_port_game_status(game_status)
    );
    
    // Use PLL to generate the 25MHZ VGA_CLK.
    // You will have to generate it on your own in simulation.
    vga_clk vga_clk_instance(.inclk0(Clk), .c0(VGA_CLK));
    
    VGA_controller vga_controller_instance( .Clk(Clk),
                                            .Reset(Reset_h),
                                            .VGA_HS(VGA_HS),
                                            .VGA_VS(VGA_VS),
                                            .VGA_CLK(VGA_CLK),
                                            .VGA_BLANK_N(VGA_BLANK_N),
                                            .VGA_SYNC_N(VGA_SYNC_N),
                                            .DrawX(DrawX),
                                            .DrawY(DrawY)
                                            );

    // -------------------------------Assign color Begin-------------------------------
    assign color_index = (color_index2 == 0) ? color_index1 : color_index2;

    always_comb begin
        red = color[11:8];
        green = color[7:4];
        blue = color[3:0];
    end
    palette_rom palette_rom_instance(    
                                        .addr(color_index), 
                                        .data_out(color));
    // -------------------------------Assign color End-------------------------------

    //-------------------------------Remove Glitches Begin-------------------------------
    always @(posedge VGA_CLK) 
    begin
        if (VGA_BLANK_N)
        begin
            VGA_R[7:4] <= red;
            VGA_G[7:4] <= green;
            VGA_B[7:4] <= blue;		
        end
        else
        begin
            VGA_R[7:4] <= 4'b0;
            VGA_G[7:4] <= 4'b0;
            VGA_B[7:4] <= 4'b0;
        end        
    end
    //-------------------------------Remove Glitches End-------------------------------
    

    //-------------------------------SRAM Conection Begin-------------------------------
    // Our SRAM and I/O controller
    logic [18:0] pixel_addr;
    logic [15:0] pixel_data;        // The pixel data(color index, from 0 to 255) specified by DrawX and DrawY  
    logic [18:0] win_bias, lose_bias;
    logic [18:0] del_x, del_y;


    assign win_bias = `WIN_BIAS;  
    assign lose_bias = `LOSE_BIAS;
    assign del_x = DrawX - `WIN_X;
    assign del_y = DrawY - `WIN_Y;

    always_comb begin
        if(is_point_in_win(DrawX, DrawY))begin
            if (game_status == 2'b11)begin      //win
                pixel_addr = win_bias + del_x + del_y * `WIN_WIDTH;
            end
            else if (game_status == 2'b10)begin // lose
                pixel_addr = lose_bias + del_x + del_y * `WIN_WIDTH;
            end
            else begin
                pixel_addr = DrawX + DrawY * 640;
            end
        end
        else begin
            pixel_addr = DrawX + DrawY * 640;
        end
    end

    assign SRAM_ADDR = {1'b0, pixel_addr};
    assign color_index1 = pixel_data[7:0];

    // These should always be active
	assign CE = 1'b0;
	assign UB = 1'b0;
	assign LB = 1'b0;
    // ????? Maybe use FSM to control?
    assign OE = 1'b0;
    assign WE = 1'b1;

    Mem2IO memory_subsystem(
        .Clk(Clk), .Reset(Reset_h), .ADDR(SRAM_ADDR),
        .CE(CE), .UB(UB), .LB(LB), .OE(OE), .WE(WE),
        .Data_from_CPU(), .Data_to_CPU(pixel_data),
        .Data_from_SRAM(Data_from_SRAM), .Data_to_SRAM()
    );

    // The tri-state buffer serves as the interface between Mem2IO and SRAM
    tristate #(.N(16)) tr0(
        .Clk(Clk), .tristate_output_enable(~WE), .Data_write(), .Data_read(Data_from_SRAM), .Data(SRAM_DQ)
    );

    function logic is_point_in_win(logic [9:0] Drawx, logic [9:0] Drawy);
        return (Drawx >= `WIN_X) && (Drawx <= (`WIN_X + `WIN_WIDTH)) && (Drawy >= `WIN_Y) && (Drawy <= (`WIN_Y + `WIN_HEIGHT));
    endfunction
    //-----------------------------SRAM Connection End-------------------------------


    // Display keycode on hex display
    HexDriver hex_inst_0 (keycode[3:0], HEX0);
    HexDriver hex_inst_1 (keycode[7:4], HEX1);
    
endmodule
