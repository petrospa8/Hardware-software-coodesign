library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

entity tb_practica1 is
end tb_practica1;

architecture test of tb_practica1 is

	component practica1 port (

-- completar aquí la información de las entradas y salidas del componente "practica1"
		a,b,c,d,e,f,g,h : in signed(3 downto 0);
		sel1, sel2 : in STD_LOGIC_VECTOR(1 downto 0);
		res1, res2 : out signed(3 downto 0)
	);
	end component;
	

signal a, b, c, d, e, f, g, h :  signed (3 downto 0);
signal sel1, sel2 :  STD_LOGIC_VECTOR (1 downto 0);
signal res1, res2 :  signed (3 downto 0);	
	
begin
	
	dut: practica1 port map(
		a=>a, b=>b, c=>c, d=>d, e=>e, f=>f, g=>g, h=>h, sel1=>sel1, sel2=>sel2, res1=>res1, res2=>res2);

	simulacion: process	
	begin
	
		-- puedes jugar a poner entradas adicionales
	
		wait for 1 ns;
		
		a <= "0100";
		b <= "0101";
		c <= "0110";
		d <= "0111";
		sel1 <= "00";
		
		e <= "1100";
		f <= "1101";
		g <= "1110";
		h <= "1111";
		sel2 <= "11";		
	
		wait for 1 ns;

		sel1 <= "01";
		sel2 <= "10";
		wait for 1 ns;
		
		sel1 <= "10";
		sel2 <= "01";
		wait for 1 ns;
		
		sel1 <= "11";
		sel2 <= "00";
		wait for 1 ns;		
		
		assert false report "se ha terminado";
		wait;
	
	end process;
	
	
end test;
	