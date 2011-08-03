<?php
# dvdisaster: Czech homepage translation
# Copyright (C) 2006 Luboš Staněk
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3>Výhody používání programu dvdisaster:</h3>

<ul>
<li><b>Chrání</b> před neúmyslným poškozením médií.<p></p></li>
<li>Testy chyb čtení běží <b>rychleji</b>, než probíhají skeny PI/PO;<br>
při rychlosti až 16x podle použité mechaniky DVD.<p></p></li>
<li><b>Efektivita nákladů:</b> Média se musí nahradit novou kopií jen v případě,
že jsou skutečně poškozená.</li>
</ul>

<h3>Omezení při používání programu dvdisaster:</h3>
<ul>
<li>Soubor oprav chyb <b>musí být vytvořen dříve, než médium selže</b>.<p></p></li>
<li>Soubory oprav chyb vyžadují <b>další úložný prostor</b> a je nutné
je uchovávat na spolehlivém médiu.
Při použití standardního nastavení
dosahuje dodatečný úložný prostor velikosti 15% původního datového rozsahu
(přibližně 700 MB pro plné 4.7 GB DVD).<p></p></li>
<li>proti ztrátě dat.</li>
</ul>

V kolekci základních informací
zjistíte více o tom, jak program dvdisaster funguje.

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
