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

<h3>dvdisaster herunterladen</h3>

dvdisaster unterstützt die Betriebssysteme Darwin/Mac OS X, FreeBSD, Linux, NetBSD und Windows
in den <a href="download10.php">aktuellen Versionen</a>.<p>

Laden Sie sich bitte entweder den Quellkode oder eine Binärversion 
aus der folgenden Liste herunter. Die Pakete können mit
einer <a href="download20.php">digitalen Unterschrift</a> auf 
ihren Ursprungszustand überprüft werden.<p>

Um die Binärversion für Windows zu <a href="download30.php">installieren</a>, 
rufen Sie das Programm nach dem Herunterladen auf und folgen dem Dialog. 
Die Quellkode-Version enthält eine Datei <tt>INSTALL</tt>
mit Installationshinweisen.

<pre> </pre>

<!---
<b>Entwickler-Versionen</b> - neu und experimentell für erfahrene Benutzer!<p> 
<b>Stabile Versionen</b> - zum Einstieg empfohlen<p> 
--->

<b>Alpha-/Entwickler-Versionen</b> - neu und experimentell für erfahrene Benutzer!<p> 

Sie sind eingeladen, die nächste dvdisaster-Version auszuprobieren, 
aber beachten Sie bitte, daß diese Version noch Fehler und 
Kompatibilitätsprobleme enthalten kann. Die neueste experimentelle Version 
ist <a href="download40.php"><?php echo $cooked_version ?></a>.

<pre> </pre>

<b>Stabile Version</b> - zum Einstieg empfohlen<p> 

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.70</b></td><td align="right">20-Jan-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode für alle Betriebssysteme:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4.tar.bz2?download">dvdisaster-0.70.4.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4.tar.bz2.gpg?download">dvdisaster-0.70.4.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4-setup.exe?download">dvdisaster-0.70.4-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4-setup.exe.gpg?download">dvdisaster-0.70.4-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Das RS02-Fehlerkorrektur-Verfahren
wird in der graphischen Benutzeroberfläche vollständig unterstützt.
Damit erzeugte Abbilder können nun auch
mit dem angepaßten Leseverfahren verarbeitet werden.<p>

Julian Einwag hat damit begonnen, dvdisaster 
für Mac OS X / Darwin anzupassen.<p>

Daniel Nylander hat die Bildschirmtexte ins Schwedische übersetzt.<p>

<b>Patches</b> (kleine Änderungen nach Version 0.70; die obigen Dateien wurden erneuert):<br>

<b>pl4</b> ist besser kompatibel zu zweischichtigen DVDs 
(DVD-R DL und DVD+R DL).<br> 
Einige kleinere Fehler wurden behoben. <i>(20-Jan-2007)</i>.<p>

<b>pl3</b> behebt einen Fehler bei der Erkennung von nicht unterstützten CDs,
der unter Umständen einen blauen Bildschirm unter Windows erzeugte.
Abbruchmöglichkeit während der RS02-Erkennung für DVD RW hinzugefügt.
<i>(10-Dez-2006)</i>.<p>

<b>pl2</b> behebt eine fehlerhafte Speicherfreigabe beim Schließen des Programms.
Das Auspacken der Bildschirmfotos für die Dokumentation auf PPC-Plattformen wurde
korrigiert. Nur die Quellkode-Archive wurden erneuert. 
<i>(03-Okt-2006)</i>.<p>

<b>pl1</b> behebt einen Fehler im angepaßten Lesen für RS02, durch den unter bestimmten
Bedingungen nicht genügend Daten für eine erfolgreiche Wiederherstellung gelesen wurden.
Ein paar kleine Verbesserungen an der Dokumentation und der Benutzbarkeit sind auch dabei.
<i>(30-Jul-2006)</i>
</td></tr></table><p>

Der dvdisaster-Quellkode wird auch in einem
<a href="http://sourceforge.net/cvs/?group_id=157550">CVS-Archiv</a>
bereitgestellt.

<pre> </pre>

<b>Vorangegangene Version</b> - eine Aktualisierung auf Version 0.70.4 wird empfohlen.<p> 

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.66</b></td><td align="right">25-Mär-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode für alle Betriebssysteme:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2?download">dvdisaster-0.66.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2.gpg?download">dvdisaster-0.66.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe?download">dvdisaster-0.66.3-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe.gpg?download">dvdisaster-0.66.3-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
</table><p>


<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
