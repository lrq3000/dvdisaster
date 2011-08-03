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

<h3>Program dvdisaster doplňuje PI/PO skeny</h3>

PI/PO skeny jsou cenným nástrojem pro měření a optimalizaci
kvality vypálených médií.<p>

Přesto <b>nejsou</b> PI/PO skeny spolehlivým prostředkem <b>předpovědi
životnosti</b> DVD média!

<ul>
<li> PI/PO skeny nepodávají žádnou informaci o tom, kolik dalších
škrábanců nebo týdnů stárnutí médium ještě vydrží, než začne vykazovat
první nečitelný sektor.<p></p></li>

<li>Kopírování média jen kvůli špatným hodnotám PI/PO se často
ukazuje jako předčasné - někdy takováto média zůstanou čitelná
mnohem déle, než se očekávalo.<p></p></li>

<li>Bez použití dodatečných nástrojů, jako je dvdisaster, ale není
možné odkládat zálohu média, až se objeví první chyby čtení - to by
rozhodně vedlo ke ztrátě dat.</li>
</ul>

<a href="index30.php">Přehled pro a proti programu dvdisaster...</a>


<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
