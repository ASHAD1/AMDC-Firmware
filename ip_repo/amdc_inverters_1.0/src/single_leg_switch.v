`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:57:30 08/28/2012 
// Design Name: 
// Module Name:    single_leg_switch 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module single_leg_switch(CLK, Sin, Q1, Q2);
input CLK;		//50 MHz clock
//input RST;		//reset signal
input Sin;		//1 if leg output should be high, 0 if leg output should be low
output reg Q1;		//1 if top switch should be on, 0 if top switch should be off
output reg Q2;		//1 if bot switch should be on, 0 if top switch should be off


parameter Q1OFF_Q2OFF = 4'b0,  //switch states
			 Q1OFF_Q2ON = 4'b1,
			 Q1ON_Q2OFF = 4'd2,
			 Q1ON_Q2ON = 4'd3,		//should not be possible
			 DEADTIME_COUNT = 8'd5;	//dead time = (DEADTIME_COUNT+1)/50e6 (but note: the FPGA pin rise/fall time and delays cause this to be about 10-20ns shorter than we expect from this EQ)
			 
reg [2:0] State, nextState;
//reg curS;			//1 if the leg was previously high, 0 if leg was previously low

reg [7:0] deadtime_count;
parameter [3:0] A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7;
parameter [7:0] period=30;

initial
	begin	
		deadtime_count = 7'b0;
		State = Q1OFF_Q2OFF;
		nextState = Q1OFF_Q2OFF;
		Q1 = 1'b0;
		Q2 = 1'b0;
	end
	 
	
//update next state variable on the positive edge
always @ (posedge CLK)
begin
	case (State)
		Q1OFF_Q2OFF:
			//dead time...
			if (deadtime_count >= DEADTIME_COUNT) //ready to move on
				if (Sin == 1'b1)	//going high
					nextState <= Q1ON_Q2OFF;
				else					//going low
					nextState <= Q1OFF_Q2ON;
			else	//not ready to move on
				nextState <= Q1OFF_Q2OFF;
		
		Q1OFF_Q2ON: begin
			//curS <= 1'b0;
			if (Sin == 1'b1)		//wants to go high
				nextState <= Q1OFF_Q2OFF;
			else
				nextState <= Q1OFF_Q2ON;
			end
		
		Q1ON_Q2OFF: begin
			//curS <= 1'b1;
			if (Sin == 1'b0)		//wants to go low
				nextState <= Q1OFF_Q2OFF;
			else
				nextState <= Q1ON_Q2OFF;						
			end
		
		//Q1ON_Q2ON:	//should never get here				
	endcase
end

//update outputs on negative edge
always @(negedge CLK)
begin
	begin
		case (State)
			Q1OFF_Q2OFF: begin
				deadtime_count <= deadtime_count + 1;
				Q1 <= 1'b0;
				Q2 <= 1'b0;
				end
			
			Q1OFF_Q2ON: begin
				deadtime_count <= 7'b0;
				Q1 <= 1'b0;
				Q2 <= 1'b1;
				end
				
			Q1ON_Q2OFF: begin
				deadtime_count <= 7'b0;
				Q1 <= 1'b1;
				Q2 <= 1'b0;
				end
		endcase
		State <= nextState;	//update the state
	end
end

endmodule				
			  
