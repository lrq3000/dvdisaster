<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2012 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!-- Insert actual page content below -->

<h3 class="top">Zusätzliche Papiere</h3>

Die Online-Dokumentation, die Sie gerade lesen, ist auch
in den dvdisaster-Programmpaketen selbst enthalten. Sie brauchen
sich diese Seiten also nicht zusätzlich herunterzuladen.<p>

Folgende zusätzliche Papiere sind verfügbar:<p>

<b>Spezifikation für RS01 und RS03</b><p>

Dieses Dokument enthält die 
<a href="http://dvdisaster.net/papers/codecs.pdf">Spezifikation 
der RS01- und RS03-Kodierer (codecs.pdf)</a>,
um deren Eigenschaften diskutieren zu können. <p>

Die Erstellung der Spezifikation von RS02 ist in Vorbereitung. <p>

Mit RS03 wird in zukünftigen dvdisaster-Versionen ein neues
Kodierungsverfahren eingeführt, das seine Berechnungen auf mehrere
Prozessorkerne verteilen kann. Dies ist mit den momentanen 
Verfahren RS01 und RS02 aufgrund deren inneren Aufbaus nicht möglich. 
Die Spezifikation von RS03 kann sich noch in einigen Details ändern.
<p>

Das Dokument setzt Kenntnisse im Bereich Kodierungstheorie voraus
und ist nicht als Benutzerdokumentation gedacht.<p>

<hr>

<table>
<tr><td>
<a href="http://pdfreaders.org/"><img src="http://pdfreaders.org/graphics/pdfreaders-lang-neutral.png" alt="pdfreaders.org" /></a></td>
<td>Programme zum Lesen des PDF-Dokuments sind auf <a href="http://pdfreaders.org">PDFreaders.org</a> erhältlich.</td>
</tr>
</table>



<!-- do not change below -->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
