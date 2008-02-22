<?php
# dvdisaster: English homepage translation
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

<h3>Beschädigten Datenträger einlesen</h3>

<table width="100%" cellspacing="5" valign="top">
<tr>
<td width="20%"><b>Aufgabe</b></td>
<td>
Von einem beschädigten Datenträger werden so viele Sektoren wie möglich eingelesen.
</td>
</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td><b>Hinweis:</b></td>
<td>Nur durch wiederholtes Einlesen ist eine Wiederherstellung des 
Datenträger-Inhaltes sehr unwahrscheinlich. 
Zur vollständigen Wiederherstellung werden  Fehlerkorrektur-Daten benötigt. 
<tr><td> <pre> </pre> </td></tr>


<tr>
<td colspan="2"><b>Benötigte Datenträger:</b><p></td>
</tr>

<tr>
<td><img src="../images/bad-cd.png"></td>
<td>
Diese Funktion ist auf das Lesen von beschädigten Datenträgern spezialisiert.
</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Nutzbare Zusatzdaten:</b></td>
</tr>

<tr>
<td><img src="../images/ecc.png"></td>
<td>
dvdisaster reduziert den Leseaufwand auf das benötigte Minimum, um das Abbild
mit Hilfe der Fehlerkorrektur-Daten wiederherzustellen.
</td>
</tr>
<tr><td> <pre> </pre> </td></tr>


<tr>
<td colspan="2"><b>Durchzuführende Schritte:<p></b></td>
</tr>

<tr>
<td></td>
<td>
1. Grundeinstellungen zum Lesen beschädigter Datenträger vornehmen<br>
2. ISO-Abbild des Datenträgers erstellen (soweit möglich)<br>
</td>
</tr>
</table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
