library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity address_decoder is
	port(
		a11: in std_logic;
		a10: in std_logic;
		a9: in std_logic;
		a8: in std_logic;

		ram_cs : out std_logic;
		adc_cs : out std_logic;
		oled_cs : out std_logic;
		oled_dc : out std_logic
	);

	attribute LOC : string;
	attribute LOC of ram_cs : signal is "P19";
	attribute LOC of adc_cs : signal is "P18";
	attribute LOC of oled_cs : signal is "P17";
	attribute LOC of oled_dc : signal is "P16";

	attribute LOC of a11 : signal is "P1";
	attribute LOC of a10 : signal is "P2";
	attribute LOC of a9 : signal is "P3";
--	attribute LOC of a8 : signal is "P19";

end address_decoder;

architecture behavioral of address_decoder is
begin

	ram_cs <= a11;
	adc_cs <= not(not a11 and a10); 
	oled_cs <= not a11 and not a10;
	oled_dc <= a9;

end behavioral;
