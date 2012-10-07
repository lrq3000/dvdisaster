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

<h3 class="top">Systemanforderungen</h3>

<ul>
 <li>x86-, PowerPC- oder Sparc-Prozessor;</li>
 <li>ein aktuelles CD-/DVD-/BD-Laufwerk mit ATAPI-, SATA- oder SCSI-Schnittstelle;</li>
 <li>ausreichend Festplattenplatz zum Abspeichern eines ISO-Abbildes der bearbeiteten Datenträger.</li>
</ul>

<h3 class="top">Unterstützte Betriebssysteme</h3>
Die folgende Tabelle enthält Informationen zu den unterstützten
Betriebssystemen. Die angegebenen Versionen wurde zum Entwickeln
und Testen der momentanen dvdisaster-Version verwendet. 
Es ist wahrscheinlich, daß etwas ältere oder neuere Betriebssystem-Versionen
ebenfalls verwendet werden können.<p>

<table border="1">
<tr>
<td>Betriebssystem</td>
<td>Version</td>
</tr>
<tr>
<td>GNU/Linux</td>
<td>Debian Squeeze (6.0.4) <br> Kernel 2.6.32</td>
</tr>
<tr>
<td>FreeBSD<sup>1)</sup></td>
<td>9.0</td>
</tr>
<tr>
<td>NetBSD</td>
<td>5.1.2</td>
</tr>
</table><p>

<sup>1)</sup>FreeBSD: für ATAPI-Laufwerke muß das Kernelmodul <i>atapicam</i> geladen sein -- siehe das INSTALL-Dokument<br>

<h3 class="top">Nicht unterstützte Betriebssysteme</h3>

<ul>
<li> Die Unterstützung von Mac OS X wurde eingestellt und ist auch
zukünftig nicht mehr geplant.</li>
<li> Windows wird nur noch im Versionszweig 0.72.x unterstützt.
Eine Unterstützung in 0.79.x und zukünftigen Versionen ist nicht mehr geplant.</li>
</ul>

Siehe auch den <a href="qa10.php#unsupported">entsprechenden Eintrag in den Fragen und Antworten</a>.

<!-- do not change below -->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
