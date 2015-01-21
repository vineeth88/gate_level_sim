module b13(
	input clock,
	input reset,
	input eoc,
	input[7:0] data_in,
	input dsr,
	output reg soc,
	output reg load_dato,
	output reg add_mpx2,
	output reg[3:0] canale,
	output reg mux_en,
	output reg error,
	output reg data_out
	);
	
parameter [2:0] 	//synopsys enum state_info
			GP001 	= 3'b000, 
			GP010 	= 3'b001,
			GP011 	= 3'b010,
			GP100 	= 3'b011,
			GP100w 	= 3'b100,
			GP101 	= 3'b101,
			GP110 	= 3'b110,
			GP111 	= 3'b111;
			
reg[2:0] /*synopsys enum state_info*/ S1; //synopsys state_vector S1			

`define GP01 0
`define GP10 1
`define GP11 2
`define GP11w 3

`define START_BIT 0
`define STOP_BIT 1
`define BIT0 2
`define BIT1 3
`define BIT2 4
`define BIT3 5
`define BIT4 6
`define BIT5 7
`define BIT6 8
`define BIT7 9

`define G_IDLE 0
`define G_LOAD 1
`define G_SEND 2
`define G_WAIT_END 3


reg[1:0] S2;

reg mpx, rdy, send_data;

reg confirm;
reg shot;

reg send_en;
reg tre;
reg[7:0] out_reg;
reg[3:0] next_bit;
reg tx_end;
reg[1:0] itfc_state;
reg send, load;

reg[9:0] tx_conta;
reg[3:0] conta_tmp;

parameter DelayTime = 104;

always @ (posedge clock)
begin
	if (reset)
	begin
		S1 <= GP001;
		soc <= 0;
		canale <= 0;
		conta_tmp = 0;
		send_data <= 0;
		load_dato <= 0;
		mux_en <= 0;
	end
	else
	begin
		case (S1)
			GP001:
			begin
				mux_en <= 1;
				S1 <= GP010;
			end
			GP010:
			begin
				S1 <= GP011;
			end
			GP011:
			begin
				soc <= 1;
				S1 <= GP101;
			end
			GP101:
			begin
				if (eoc)
					S1 <= GP101;
				else
				begin
					load_dato <= 1;
					S1 <= GP110;
					mux_en <= 0;
				end
			end
			GP110:
			begin
				load_dato <= 0;
				soc <= 0;
				conta_tmp = conta_tmp + 1;
				if (conta_tmp == 8)
					conta_tmp = 0;
				canale <= conta_tmp;
				S1 <= GP111;
			end
			GP111:
			begin
				send_data <= 1;
				S1 <= GP100w;
			end
			GP100w:
			begin
				S1 <= GP100;
			end
			GP100:
			begin
				if (~rdy)
					S1 <= GP100;
				else
				begin
					S1 <= GP001;
					send_data <= 0;
				end
			end
			default:
				S1 <= S1;
		endcase
	end
end

always @ (posedge clock)
begin
	if (reset)
	begin
		S2 <= `GP01;
		rdy <= 0;
		add_mpx2 <= 0;
		mpx <= 0;
		shot <= 0;
	end
	else
	begin
		case (S2)
			`GP01:
			begin
				if (send_data)
				begin
					rdy <= 1;
					S2 <= `GP10;
				end
				else
					S2 <= `GP01;
			end
			`GP10:
			begin
				shot <= 1;
				S2 <= `GP11;
			end
			`GP11:
			begin
				if (~confirm)
				begin
					shot <= 0;
					S2 <= `GP11;
				end
				else
				begin
					if (~mpx)
					begin
						add_mpx2 <= 1;
						mpx <= 1;
						S2 <= `GP10;
					end
					else
					begin
						 mpx <= 0;
						 rdy <= 0;
						 S2 <= `GP11w;
					end
				end
			end
			`GP11w:
			begin
				S2 <= `GP01;
			end
			default:
				S2 <= S2;
		endcase
	end
end

always @ (posedge clock )
begin
	if (reset)
	begin
		load <= 0;
		send <= 0;
		confirm <= 0;
		itfc_state <= `G_IDLE;
	end
	else
	begin
		case (itfc_state)
			`G_IDLE:
			begin
				if (shot)
				begin
					load <= 1;
					confirm <= 0;
					itfc_state <= `G_LOAD;
				end
				else
				begin
					confirm <= 0;
					itfc_state <= `G_IDLE;
				end
			end
			`G_LOAD:
			begin
				load <= 0;
				send <= 1;
				itfc_state <= `G_SEND;
			end
			`G_SEND:
			begin
				send <= 0;
				itfc_state <= `G_WAIT_END;
			end
			`G_WAIT_END:
			begin
				if (tx_end)
				begin
					confirm <= 1;
					itfc_state <= `G_IDLE;
				end
			end
			default:
				itfc_state <= itfc_state;
		endcase
	end
end

always @ (posedge clock )
begin
	if (reset)
	begin
		send_en <= 0;
		out_reg <= 8'h00;
		tre <= 0;
		error <= 0;
	end
	else
	begin
		if (tx_end)
		begin
			send_en <= 0;
			tre <= 1;
		end
		if (load)
		begin
			if (~tre)
			begin
				out_reg <= data_in;
				tre <= 1;
				error <= 0;
			end
			else
				error <= 1;
		end
		
		if (send)
		begin
			if ((~tre) || (~dsr))
				error <= 1;
			else
			begin
				error <= 0;
				send_en <= 1;
			end
		end
	end
end

always @ (posedge clock)
begin
	if (reset)
	begin
		tx_end <= 0;
		data_out <= 0;
		next_bit <= `START_BIT;
		tx_conta <= 0;
	end
	else
	begin
		tx_end <= 0;
		data_out <= 1;
		if (send_en)
		begin
			if (tx_conta > DelayTime)
			begin
				case (next_bit)
					`START_BIT:
					begin
						data_out <= 0;
						next_bit <= `BIT0;
					end
					`BIT0:
					begin
						data_out <= out_reg[7];
						next_bit <= `BIT1;
					end
					`BIT1:
					begin
						data_out <= out_reg[6];
						next_bit <= `BIT2;
					end
					`BIT2:
					begin
						data_out <= out_reg[5];
						next_bit <= `BIT3;
					end
					`BIT3:
					begin
						data_out <= out_reg[4];
						next_bit <= `BIT4;
					end
					`BIT4:
					begin
						data_out <= out_reg[3];
						next_bit <= `BIT5;
					end
					`BIT5:
					begin
						data_out <= out_reg[2];
						next_bit <= `BIT6;
					end
					`BIT6:
					begin
						data_out <= out_reg[1];
						next_bit <= `BIT7;
					end
					`BIT7:
					begin
						data_out <= out_reg[0];
						next_bit <= `STOP_BIT;
					end
					`STOP_BIT:
					begin
						data_out <= 1;
						next_bit <= `START_BIT;
						tx_end <= 1;
					end
					default:
						next_bit <= next_bit;
				endcase
				tx_conta <= 0;
			end
		end
	end
end

endmodule
