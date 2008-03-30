<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2008 Carsten Gnörlich
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

<h3>Platzhalter-Seite</h3>

Diese Seite wurde noch nicht ausformuliert.
<pre> </pre>


<font size="-1">
<i>Warum enthält diese Dokumentation leere Seiten?</i><p>
Wir brauchen die Platzhalter-Seiten damit an anderen Stellen der
Dokumentation bereits die passenden Querverweise gesetzt werden können. 

<?php /*
<h3>Datenträger-Abbild rekonstruieren</h3>

<table width="100%" cellspacing="5">
<tr>
<td width="20%"><b>Aufgabe</b></td>
<td>
Die fehlenden Teile eines Datenträger-Abbild werden durch Fehlerkorrektur-Daten
wiederhergestellt.
</td>
</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Benötigte Daten:</b><p></td>
</tr>

<tr>
<td><img src="../images/bad-image.png" align="top">
<img src="../images/ecc.png" align="top">
</td>
<td>
Diese Funktion benötigt das ISO-Abbild eines beschädigten Datenträgers
sowie die zugehörigen Fehlerkorrektur-Daten. Je nach verwendetem 
Fehlerkorrektur-Verfahren befinden sich die Fehlerkorrektur-Daten in einer
eigenen Datei oder bereits im Abbild selbst.

</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Durchzuführende Schritte:<p></b></td>
</tr>

<tr>
<td></td>
<td>
1. Abbild wiederherstellen
</td>
</tr>
</table><p>
   */ ?>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
