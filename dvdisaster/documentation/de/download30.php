<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2007 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3>Installieren der Binärversion (nur für Windows)</h3>

Zum Installieren der Windows-Version führen Sie bitte das
Installations-Programm (z.B. <?php echo ${pkgname}?>-setup.exe) 
aus. Es enthält einen Dialog zum Einrichten von dvdisaster.<p>

<b>Warnung:</b> Sie können dvdisaster nicht installieren, indem Sie
das setup.exe-Programm von Hand auspacken oder indem Sie Teile von
dvdisaster aus einer bereits erfolgten Installation kopieren.
Wird dvdisaster nicht durch das setup.exe-Programm installiert,
so erscheinen seltsame Fehler(-meldungen), 
die nicht immer nachvollziehbar mit einer unvollständigen Installation
zusammenhängen.

<h3>Installation der Quellkode-Pakete</h3>

dvdisaster verwendet den üblichen
<tt>./configure; make; make install</tt>-Mechanismus für die Installation
aus dem Quellkode. In dem Quellkode-Paket ist eine Datei
<tt>INSTALL</tt> mit weiteren Information enthalten.

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
