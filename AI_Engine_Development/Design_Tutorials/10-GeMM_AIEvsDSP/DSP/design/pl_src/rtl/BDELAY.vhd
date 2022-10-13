library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

--use work.TYPES_PKG.all;

entity BDELAY is
  generic(SIZE:NATURAL:=1;           -- SIZE has a default value of 1 and cannot be negative, this would require traveling back in time
          DDR:BOOLEAN:=FALSE;        -- DDR implementation for Versal -3 clock rates over 872MHz
          STYLE:STRING:="srl_reg");  -- acceptable values are "reg", "srl", "srl_reg", "reg_srl", "reg_srl_reg" or "block"
  port(CLK:in STD_LOGIC:='0';        -- an input port with a default value can be left unconnected, this could be useful when SIZE=0 (a wire)
       CLKH:in STD_LOGIC:='0';       -- half rate clock, an input port with a default value can be left unconnected, this could be useful when SIZE=0 (a wire)
       CE:in STD_LOGIC:='1';         -- clock enable, if not used can be left unconnected
       I:in BOOLEAN;
       O:out BOOLEAN:=FALSE);
  attribute secure_config:STRING;
  attribute secure_config of BDELAY:entity is "PROTECT";
end BDELAY;

architecture TEST of BDELAY is
  attribute syn_hier:STRING;
  attribute syn_hier of all:architecture is "hard";
  attribute srl_style:STRING;

  function LOG2(N:INTEGER) return INTEGER is
    variable TEMP:INTEGER;
    variable RESULT:INTEGER;
  begin
    TEMP:=N;
    RESULT:=0;
    while TEMP>1 loop
      RESULT:=RESULT+1;
      TEMP:=(TEMP+1)/2;
    end loop;  
    return RESULT; 
  end; 
begin
--  l2n:if (SIZE>=66 and SIZE<=65+33) or (SIZE>=130 and SIZE<=129+65+33) or (SIZE>=258 and SIZE<=257+129+65+33) or (SIZE>513) generate
  l2n:if DDR and (SIZE>3) generate
        signal ID:BOOLEAN:=FALSE;
        signal T,TD,SEL:STD_LOGIC:='0';
        type TBV is array(0 to SIZE/2-1) of BOOLEAN;
        signal OE,OO:TBV:=(others=>FALSE); -- delay line signal is SIZE/2 in length
        attribute srl_style of OE:signal is STYLE;
        attribute srl_style of OO:signal is STYLE;
      begin
        i0:if SIZE mod 2=0 generate
             ID<=I;
           end;
           else generate
             process(CLK)
             begin
               if rising_edge(CLK) then
                 ID<=I after 1 ps;
               end if;
             end process;
           end generate;
        process(CLKH)
        begin
          if rising_edge(CLKH) then
            OE<=ID&OE(OE'low to OE'high-1) after 1 ps;
          end if;
        end process;
        
        process(CLKH)
        begin
          if falling_edge(CLKH) then
            OO<=ID&OO(OO'low to OO'high-1) after 1 ps;
          end if;
        end process;
  
        process(CLKH)
        begin
          if rising_edge(CLKH) then
            T<=not T after 1 ps;
          end if;
        end process;
        
        process(CLK)
        begin
          if rising_edge(CLK) then
            TD<=T after 1 ps;
            SEL<=TD xor T after 1 ps;
            if SEL='0' then
              O<=OE(OE'high);
            else
              O<=OO(OO'high);
            end if;
          end if;
        end process;
      end;
      elsif (SIZE>=66 and SIZE<=65+33) or (SIZE>=130 and SIZE<=129+65+33) or (SIZE>=258 and SIZE<=257+129+65+33) generate
        signal IO:BOOLEAN:=FALSE;
        constant HSIZE:INTEGER:=2**(LOG2(SIZE)-1)+1;
      begin
        d2n:entity work.BDELAY generic map(SIZE=>HSIZE, -- 65/129/257 
                                           STYLE=>STYLE)
                               port map(CLK=>CLK,
                                        CLKH=>CLKH,
                                        CE=>CE,
                                        I=>I,
                                        O=>IO);
        d1:entity work.BDELAY generic map(SIZE=>SIZE-HSIZE, -- 2..34/2..66/2..130
                                          STYLE=>STYLE)
                              port map(CLK=>CLK,
                                       CLKH=>CLKH,
                                       CE=>CE,
                                       I=>IO,
                                       O=>O);
      end;
      elsif l513: (SIZE>17 and SIZE<=513) generate -- infer a counter plus distributed RAM implementation
        signal A:UNSIGNED(LOG2(SIZE-1)-1 downto 0):=(others=>'0');
        type TMEM is array(0 to 2**A'length-1) of BOOLEAN;
        signal MEM:TMEM:=(others=>FALSE);
        attribute ram_style:STRING;
        attribute ram_style of MEM:signal is "distributed";
      begin
        process(CLK)
        begin
          if rising_edge(CLK) then
            if CE='1' then
              if A=SIZE-2 then
                A<=(others=>'0');
              else
                A<=A+1;
              end if;
              MEM(TO_INTEGER(A))<=I;
              O<=MEM(TO_INTEGER(A));
            end if;
          end if;
        end process;
      end;
      elsif (SIZE>513) generate
        signal IO:BOOLEAN:=FALSE;
        constant HSIZE:INTEGER:=513;
      begin
        d2n:entity work.BDELAY generic map(SIZE=>HSIZE, -- 513 
                                           STYLE=>STYLE)
                               port map(CLK=>CLK,
                                        CLKH=>CLKH,
                                        CE=>CE,
                                        I=>I,
                                        O=>IO);
        d1:entity work.BDELAY generic map(SIZE=>SIZE-HSIZE, -- SIZE-513
                                          STYLE=>STYLE)
                              port map(CLK=>CLK,
                                       CLKH=>CLKH,
                                       CE=>CE,
                                       I=>IO,
                                       O=>O);
      end;
      else generate -- otherwise use the behavioral implementation and the STYLE generic
        type TD is array(0 to SIZE) of BOOLEAN;
        signal D:TD:=(others=>FALSE); -- delay line signal is SIZE+1 in length
        attribute srl_style of D:signal is STYLE;
      begin
        D(D'low)<=I;
        lk:for K in 1 to SIZE generate
           begin
             process(CLK)
             begin
               if rising_edge(CLK) then
                 if CE='1' then
                   D(K)<=D(K-1);
                 end if;
               end if;
             end process;
           end generate;
        O<=D(D'high);
      end;
      end generate;
end TEST;