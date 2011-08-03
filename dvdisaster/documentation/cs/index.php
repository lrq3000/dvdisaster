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

<h3>Projekt dvdisaster:</h3>

dvdisaster poskytuje dodatečné zabezpečení proti <b>ztrátě dat</b> na
CD a DVD médiích
způsobené <b>stárnutím</b> nebo <b>poškrábáním</b>.  

<ul>
<li>dvdisaster vytváří <b>data oprav chyb</b> pro kompenzaci
chyb čtení, které nejsou opravitelné mechanikou CD/DVD.</li>
<li>dvdisaster se pokouší načíst co možná nejvíce dat z poškozeného média.
Později jsou nečitelné sektory obnoveny s pomocí dříve vytvořeného
kódu pro opravu chyb. Maximální kapacita opravy chyb je uživatelem volitelná.</li>
<li>dvdisaster pracuje na úrovni obrazu
a není závislý na souborovém systému.</li>
</ul>

Pokud vytvoříte soubor oprav chyb včas a uchováte jej na bezpečném místě,
máte dobré předpoklady k obnovení obsahu média při typických chybách čtení
a přenesení obnovených dat na nové médium.

<p>
<a href="index10.php">Příklady oprav chyb...</a>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
