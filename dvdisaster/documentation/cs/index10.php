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

<h3>Příklady oprav chyb</h3>

<table><tr><td valign="top">
<b><a name="scratches">Silně poškrábané médium.</a></b> &nbsp;

Toto médium bylo přenášeno bez ochranného obalu;
kruhové stopy poškrábání byly vytvořeny lahví vody. Škrábance jsou
nadměrně zvýrazněny díky odrazům fotografického blesku.

<p>Sken povrchu média odhalí asi 135000 nečitelných sektorů
(z celkového počtu 2200000, což představuje asi 6% poškozených sektorů).
V průběhu opravy obrazu média
dosáhne počet chyb maximální hodnoty 22 chyb na blok ECC
což představuje 69% využití opravy chyb při použitém nastavení.<p>

<a href="index20.php">Proč není sken PI/PO dostatečný...</a>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
