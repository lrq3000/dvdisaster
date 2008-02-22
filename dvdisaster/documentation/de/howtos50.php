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

<h3>Informationen über ISO-Abbilder und Fehlerkorrektur-Daten anzeigen</h3>

<table width="100%" cellspacing="5">
<tr>
<td width="20%"><b>Aufgabe</b></td>
<td>
Es werden Informationen über den Zustand 
von ISO-Abbildern und den zugehörigen Fehlerkorrektur-Daten angezeigt.
</td>
</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Benötigte Daten:</b><p></td>
</tr>

<tr>
<td>
<img src="../images/good-image.png" align="top">
<img src="../images/bad-image.png" align="top">
<img src="../images/ecc.png" align="top">
</td>
<td>
Diese Funktion verarbeitet Abbilder in beliebigem Zustand und die zugehörigen
Fehlerkorrektur-Daten. Es ist auch möglich, nur Informationen über Abbilder
oder Fehlerkorrektur-Dateien alleine anzuzeigen.

</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Durchzuführende Schritte:<p></b></td>
</tr>

<tr>
<td></td>
<td>
1. Informationen über ISO-Abbilder und Fehlerkorrektur-Daten anzeigen
</td>
</tr>
</table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
