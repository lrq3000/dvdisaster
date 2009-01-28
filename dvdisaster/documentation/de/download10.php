<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2009 Carsten Gnörlich
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

<h3>Systemanforderungen</h3>

<ul>
 <li>Prozessoren: x86, PowerPC oder Sparc;<p></li>
 <li>Rechenleistung vergleichbar mit oder besser als P4 mit 2Ghz;<p>
 <li>ein aktuelles CD-/DVD-/BD-Laufwerk mit ATAPI- oder SCSI-Schnittstelle;<p></li>
 <li>ausreichend Festplattenplatz zum Abspeichern eines ISO-Abbildes der bearbeiteten Datenträger.</li>
</ul>

<h3>Betriebssysteme</h3>

<ul>
 <li><a name="#freebsd"></a><b>FreeBSD</b> ab Version <b>6.0</b><br> 
     (für ATAPI-Laufwerke muß das Kernelmodul <i>atapicam</i> geladen werden - siehe INSTALL-Dokument)<p>
 </li>
 <li><b>GNU/Linux</b> ab Kernel <b>2.6.7</b><br> 
     (Kernel >= 2.4.20 sind möglich, aber nicht empfohlen!)<p>
 </li>
 <li><b>Mac OS X</b> ab Version 10.4 (Tiger),<br> 
      auf x86 und PowerPC-Hardware.<p>
 </li>
 <li><b>NetBSD</b> ab Version 3.1.<p></li>

 <li><b>Windows</b> <b>2000</b> oder <b>XP</b><br>
     Windows 98 und ME sind möglich, aber für die Bearbeitung von DVD und größeren
     Formaten nicht empfohlen.<br>
     Die Benutzung unter Windows Vista (R) ist möglich, aber weitgehend ungetestet.
 </li>
</ul>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
