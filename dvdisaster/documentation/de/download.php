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
require("../include/footnote.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3>dvdisaster herunterladen</h3>

dvdisaster unterstützt die Betriebssysteme Darwin/Mac OS X, FreeBSD, GNU/Linux, NetBSD und Windows in den <a href="download10.php">aktuellen Versionen</a>.
Es wird Ihnen 
als <a href="http://www.germany.fsfeurope.org/documents/freesoftware.de.html">freie Software</a> 
unter der <a href="http://dvdisaster.cvs.sourceforge.net/dvdisaster/dvdisaster/COPYING?view=markup">GNU General Public License V2</a><a href="#gpl3"><sup>*)</sup></a> zur 
Verfügung gestellt.<p>

Laden Sie sich bitte entweder den Quellkode oder eine Binärversion 
aus der folgenden Liste herunter. Die Pakete können mit
einer <a href="download20.php">digitalen Unterschrift</a> auf 
ihren Ursprungszustand überprüft werden.<p>

Um die Binärversion  <a href="download30.php">für Windows zu installieren</a>, 
rufen Sie das Programm nach dem Herunterladen auf und folgen dem Dialog. <p>

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
<tr><td><b>dvdisaster-0.70</b></td><td align="right">24-Feb-2008</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode für alle Betriebssysteme:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.5.tar.bz2?download">dvdisaster-0.70.5.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.5.tar.bz2.gpg?download">dvdisaster-0.70.5.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.5-setup.exe?download">dvdisaster-0.70.5-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.5-setup.exe.gpg?download">dvdisaster-0.70.5-setup.exe.gpg</a></td></tr>
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

<b>Patches</b> (kleine Änderungen nach Version 0.70; die obigen Dateien wurden erneuert):<p>

<b>pl5</b> Behebt ein Problem mit neueren Linux-Kernen, das unter bestimmten Bedingungen zum
Einfrieren des Systems führen kann. Die Behandlung von Umlauten in Dateinamen wurde
verbessert.
Zurückportierung einiger weiterer
Verbesserungen aus 0.71.24. <i>(24-Feb-2008)</i>.<p>

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

Der Quellkode von dvdisaster kann auch direkt im 
<a href="http://sourceforge.net/cvs/?group_id=157550">CVS-Archiv</a>
betrachtet werden. Einige interessante Dateien darin sind: 
<ul>
<li><a href="http://dvdisaster.cvs.sourceforge.net/dvdisaster/dvdisaster/CHANGELOG?view=markup">CHANGELOG</a>- was hat sich in den letzten Versionen geändert (in Englisch);</li>
<li><a href="http://dvdisaster.cvs.sourceforge.net/dvdisaster/dvdisaster/CREDITS.de?view=markup">CREDITS.de</a>- wer bei dem Projekt bisher mitgemacht hat;</li>
<li><a href="http://dvdisaster.cvs.sourceforge.net/dvdisaster/dvdisaster/INSTALL?view=markup">INSTALL</a> - Installationshinweise (in Englisch);</li>
<li><a href="http://dvdisaster.cvs.sourceforge.net/dvdisaster/dvdisaster/README?view=markup">README</a> - eine Übersicht zum Quellkodearchiv (in Englisch).</li>
</ul>

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

<?php
footnote("*","gpl3","dvdisaster wird zu einem späteren Zeitpunkt zur GNU General Public License Version 3 wechseln..");
?>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
