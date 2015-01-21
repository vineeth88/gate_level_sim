/***********************************************************************
        Copyright (C) 2012,
        Virginia Polytechnic Institute & State University
        
        This verilog file is transformed from ITC 99 benchmark
        which is available from
        http://www.cad.polito.it/downloads/tools/itc99.html This
        verilog file was originally coverted by Mr. Min Li under
        the supervision of Dr. Michael S. Hsiao, in the Bradley
        Department of Electrical Engineering, VPI&SU, in 2012. We
        used a commertial Tool: VHDL2verilog 17.01g for initial
        convertion and made necessary changes manually. The
        verilog file is verified by random simulation.

        This verilog file is released for research use only. This
        verilog file, or any derivative thereof, may not be
        reproduced nor used for any commercial product without the
        written permission of the authors.

        Mr. Min Li
        Research Assistant
        Bradley Department of Electrical Engineering
        Virginia Polytechnic Institute & State University
        Blacksburg, VA 24061

        Ph.: (540) 808-8129
        Fax: (540) 231-3362
        E-Mail: min.li@vt.edu
        Web: http://www.ece.vt.edu/mhsiao

***********************************************************************/

`define false 1'b 0
`define FALSE 1'b 0
`define true 1'b 1
`define TRUE 1'b 1

`timescale 1 ns / 1 ns // timescale for following modules


module b14 (
   clock,
   reset,
   addr,
   datai,
   datao,
   rd,
   wr);
 

input   clock; 
input   reset; 
output   [31:0] addr; 
input   [31:0] datai; 
output   [31:0] datao; 
output   rd; 
output   wr; 

reg     [31:0] addr; 
reg     [31:0] datao; 
reg     rd; 
reg     wr; 
parameter FETCH = 0; 
parameter EXEC = 1; 
integer  reg0; 
integer  reg1; 
integer  reg2; 
integer  reg3; 
reg      B; 
reg     [31:0]  MAR; 
integer  MBR; 
reg     [1:0]  mf; 
reg     [2:0]  df; 
reg      cf; 
reg     [3:0]  ff; 
reg     [31:0]  tail; 
integer  IR; 
reg      state; 
integer  r; 
integer  m; 
integer  t; 
integer  d; 
integer  temp; 
reg     [1:0]  s; 


always @ (posedge clock)
   begin : process_1
   if (reset == 1'b 1)
      begin
      MAR = 0;   
      MBR = 0;   
      IR = 0;   
      d = 0;   
      r = 0;   
      m = 0;   
      s = 0;   
      temp = 0;   
      mf = 0;   
      df = 0;   
      ff = 0;   
      cf = 0;   
      tail = 0;   
      B = 1'b 0;   
      reg0 = 0;   
      reg1 = 0;   
      reg2 = 0;   
      reg3 = 0;   
      addr <= 0;   
      rd <= 1'b0;   
      wr <= 1'b0;   
      datao <= 0;   
      state = FETCH;   
      end
   else
      begin
      rd <= 1'b0;   
      wr <= 1'b0;   
      case (state)
      FETCH:
         begin
         MAR = reg3 %  /* VHDL ** operator */ 2**20;   
         addr <= MAR;   
         rd <= 1'b1;   
         MBR = datai;   
         IR = MBR;   
         state = EXEC;   
         end
      EXEC:
         begin
         if (IR < 0)
            begin
            IR = -IR;   
            end
		/* verilator lint_off WIDTH	*/
         mf = IR / (2**27) % 4;   
         df = IR / (2**24) % (2**3);
         ff = IR / (2**19) % (2**4);   
         cf = IR / (2**23) % 2;   
         tail = IR % 2**20;   
         reg3 = reg3 % (2**29) + 8;   
         s = IR / (2**29) % 4;   
		 /* verilator lint_off WIDTH */
         case (s)
         0:
            begin
            r = reg0;   
            end
         1:
            begin
            r = reg1;   
            end
         2:
            begin
            r = reg2;   
            end
         3:
            begin
            r = reg3;   
            end
         endcase
         case (cf)
         1:
            begin
            case (mf)
            0:
               begin
               m = tail;   
               end
            1:
               begin
               m = datai;   
               addr <= tail;   
               rd <= 1'b1;   
               end
            2:
               begin
               addr <= (tail + reg1) % 2**20;   
               rd <= 1'b1;   
               m = datai;   
               end
            3:
               begin
               addr <= (tail + reg2) % 2**20;   
               rd <= 1'b1;   
               m = datai;   
               end
            endcase
            case (ff)
            0:
               begin
               if (r < m)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            1:
               begin
               if (~(r < m))
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            2:
               begin
               if (r === m)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            3:
               begin
               if (~(r === m))
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            4:
               begin
               if (~(r > m))
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            5:
               begin
               if (r > m)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            6:
               begin
               if (r > (2**30) - 1)
                  begin
                  r = r - (2**30);   
                  end
               if (r < m)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            7:
               begin
               if (r > (2**30) - 1)
                  begin
                  r = r - (2**30);   
                  end
               if (~(r < m))
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            8:
               begin
               if (r < m | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            9:
               begin
               if (~(r < m) | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            10:
               begin
               if (r === m | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            11:
               begin
               if (~(r === m) | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            12:
               begin
               if (~(r > m) | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            13:
               begin
               if (r > m | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            14:
               begin
               if (r > (2**30) - 1)
                  begin
                  r = r - (2**30);   
                  end
               if (r < m | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            15:
               begin
               if (r > (2**30) - 1)
                  begin
                  r = r - (2**30);   
                  end
               if (~(r < m) | B === 1'b 1)
                  begin
                  B = 1'b 1;   
                  end
               else
                  begin
                  B = 1'b 0;   
                  end
               end
            endcase
            end
         0:
            begin
            if (~(df === 7))
               begin
               if (df === 5)
                  begin
                  if (~B === 1'b 1)
                     begin
                     d = 3;   
                     end
                  end
               else if (df === 4 )
                  begin
                  if (B === 1'b 1)
                     begin
                     d = 3;   
                     end
                  end
               else if (df === 3 )
                  begin
                  d = 3;   
                  end
               else if (df === 2 )
                  begin
                  d = 2;   
                  end
               else if (df === 1 )
                  begin
                  d = 1;   
                  end
               else if (df === 0 )
                  begin
                  d = 0;   
                  end
               case (ff)
               0:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  t = 0;   
                  case (d)
                  0:
                     begin
                     reg0 = t - m;   
                     end
                  1:
                     begin
                     reg1 = t - m;   
                     end
                  2:
                     begin
                     reg2 = t - m;   
                     end
                  3:
                     begin
                     reg3 = t - m;   
                     end
                  default:
                     ;
                  endcase
                  end
               1:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  reg2 = reg3;   
                  reg3 = m;   
                  end
               2:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = m;   
                     end
                  1:
                     begin
                     reg1 = m;   
                     end
                  2:
                     begin
                     reg2 = m;   
                     end
                  3:
                     begin
                     reg3 = m;   
                     end
                  default:
                     ;
                  endcase
                  end
               3:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = m;   
                     end
                  1:
                     begin
                     reg1 = m;   
                     end
                  2:
                     begin
                     reg2 = m;   
                     end
                  3:
                     begin
                     reg3 = m;   
                     end
                  default:
                     ;
                  endcase
                  end
               4:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r + m) % (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r + m) % (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r + m) % (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r + m) % (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               5:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) % 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r + m) % (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r + m) % (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               6:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               7:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               8:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               9:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               10:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r + m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               11:
                  begin
                  case (mf)
                  0:
                     begin
                     m = tail;   
                     end
                  1:
                     begin
                     m = datai;   
                     addr <= tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (tail + reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  3:
                     begin
                     addr <= (tail + reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     m = datai;   
                     end
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     reg1 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     reg2 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     reg3 = (r - m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               12:
                  begin
                  case (mf)
                  0:
                     begin
                     t = r / 2;   
                     end
                  1:
                     begin
                     t = r / 2;   
                     if (B === 1'b 1)
                        begin
                        t = t %  /* VHDL ** operator */ (2**29);   
                        end
                     end
                  2:
                     begin
                     t = r %  /* VHDL ** operator */ (2**29) * 2;   
                     end
                  3:
                     begin
                     t = r %  /* VHDL ** operator */ (2**29) * 2;   
                     if (t >  /* VHDL ** operator */ (2**30) - 1)
                        begin
                        B = 1'b 1;   
                        end
                     else
                        begin
                        B = 1'b 0;   
                        end
                     end
                  default:
                     ;
                  endcase
                  case (d)
                  0:
                     begin
                     reg0 = t;   
                     end
                  1:
                     begin
                     reg1 = t;   
                     end
                  2:
                     begin
                     reg2 = t;   
                     end
                  3:
                     begin
                     reg3 = t;   
                     end
                  default:
                     ;
                  endcase
                  end
               13,
               14,
               15:
                  ;
               endcase
               end
            else if (df === 7 )
               begin
               case (mf)
               0:
                  begin
                  m = tail;   
                  end
               1:
                  begin
                  m = tail;   
                  end
               2:
                  begin
                  m = (reg1 % 2**20) + (tail % 2**20);   
                  end
               3:
                  begin
                  m = (reg2 % 2**20) + (tail % 2**20);   
                  end
               endcase
               addr <= m % 2**20;   
               wr <= 1'b 1;   
               datao <= r;   
               end
            end
         endcase
         state = FETCH;   
         end
      endcase
      end
   end

endmodule // module b14

