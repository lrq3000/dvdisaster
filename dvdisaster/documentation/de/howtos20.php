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

<h3>Fehlerkorrektur-Daten erzeugen</h3>

<table width="100%" cellspacing="5">
<tr>
<td width="20%"><b>Aufgabe</b></td>
<td>
Ein Datenträger wird mit Fehlerkorrektur-Daten versehen.
</td>
</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td><b>Hinweis:</b></td>
<td>Das Anlegen von Fehlerkorrektur-Daten ist vergleichbar zum Anlegen einer 
Datensicherung. Von einem bereits beschädigten Datenträger können
nachträglich keine Fehlerkorrektur-Daten mehr erstellt werden.
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Benötigte Datenträger:</b><p></td>
</tr>

<tr>
<td><img src="../images/good-cd.png"></td>
<td>
Zur Erstellung von Fehlerkorrektur-Daten wird ein guter, fehlerfreier Datenträger
benötigt.
</tr>
<tr><td> <pre> </pre> </td></tr>

<tr>
<td colspan="2"><b>Nutzbare Zusatzdaten:</b></td>
</tr>

<tr>
<td><img src="../images/good-image.png"></td>
<td>Wenn Sie bereits ein ISO-Abbild des Datenträgers (zum Beispiel vom Brennvorgang)
   haben, können Sie dieses direkt verwenden und sparen das erneute Einlesen.
</td>
</tr>
<tr><td> <pre> </pre> </td></tr>


<tr>
<td colspan="2"><b>Durchzuführende Schritte:<p></b></td>
</tr>

<tr>
<td></td>
<td>
1. Grundeinstellungen zum Lesen vornehmen<br>
2. ISO-Abbild des Datenträgers erstellen<br>
3. Fehlerkorrektur-Daten erstellen
</td>
</tr>
</table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
