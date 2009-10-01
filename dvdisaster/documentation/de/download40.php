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
$show_all=$_GET["showall"];
?>

<!--- Insert actual page content below --->

<h3>Alpha-/Entwicklerversionen</h3>

<b>Helfen Sie beim Testen!</b> Hier finden Sie experimentelle 
dvdisaster-Versionen, die auf dem Weg zur nächsten "stabilen" Version
entstehen.<p>

<b>Ein Wort der Vorsicht:</b> Alpha-Versionen sind nicht so intensiv getestet
wie die stabilen Versionen. Sie können mehr Fehler enthalten und 
sollten nicht zum Bearbeiten von wichtigen Daten
verwendet werden.<p>

Verwenden Sie im Zweifelsfall die <a href="download.php">stabile Version 0.72</a>
und warten Sie auf die Veröffentlichung von Version 0.74.

<hr>

<h3>Geplante Änderungen in der neuen Version</h3>

Allgemein:

<ul>
<li> Einbauen mehrerer kleinerer Erweiterungen, die während des langen
0.72er-Entwicklungszyklus liegengeblieben sind.</li>
<li> Entfernung nicht mehr benötigter Funktionen.</li>
<li> Aufräumen der Kodebasis und Vorbereitung für Multithreading und
Mehrkernprozessoren.</li>
</ul>

Windows:

<ul>
<li> Aktualisierung des GTK+-Toolkits und der übrigen Entwicklungsumgebung.</li>
<li> Erhöhen der Systemvoraussetzungen auf Windows 2000 oder neuer (ältere
Windows-Versionen werden von der Entwicklungsumgebung nicht mehr unterstützt). 
Damit wird auch die Unterstützung von ASPI-Treibern und das Aufteilen von 
Dateien in 2G-Segmente überflüssig.</li>
</ul>

MacOS:

<ul>
<li> Aktualisierung des GTK+-Toolkits und weitere Workarounds für die
Benutzeroberfläche.</li>
</ul>

<hr>

<h3>Herunterladen</h3>
<a name="download"></a>

Die Alpha-Versionen verwenden das gleiche Paketformat wie die normalen
Versionen.<p>

<table class="download" cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.73</b></td><td align="right">08-Aug-2009</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode für alle Betriebssysteme:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.tar.bz2">dvdisaster-0.73.1.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.tar.bz2.gpg">dvdisaster-0.73.1.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Mac OS X 10.5 / x86:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.app.zip">dvdisaster-0.73.1.app.zip</a>&nbsp;--&nbsp;bitte erst den <a href="download30.php#mac">Hinweis</a> lesen</td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1.app.zip.gpg">dvdisaster-0.73.1.app.zip.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1-setup.exe">dvdisaster-0.73.1-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.1-setup.exe.gpg">dvdisaster-0.73.1-setup.exe.gpg</a></td></tr>
    <tr><td colspan="2"> </td></tr>
<?php
  if($show_all == 0) {
?>
    <tr><td colspan="2"><a href="download40.php?showall=1#download">Ältere Veröffentlichungen des 0.73er-Versionszweiges anzeigen</a></td></tr>
<?php
  }
  else {
?> 
   <tr><td colspan="2"><a href="download40.php?showall=0#download">Ältere Veröffentlichungen des 0.73er-Versionszweiges verbergen</a></td></tr>
   <tr><td colspan="2"> </td></tr>
    <tr><td align="right">&nbsp;&nbsp;Quellkode für alle Betriebssysteme:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.tar.bz2">dvdisaster-0.73.0.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.tar.bz2.gpg">dvdisaster-0.73.0.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Mac OS X 10.5 / x86:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.app.zip">dvdisaster-0.73.0.app.zip</a>&nbsp;--&nbsp;bitte erst den <a href="download30.php#mac">Hinweis</a> lesen</td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0.app.zip.gpg">dvdisaster-0.73.0.app.zip.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0-setup.exe">dvdisaster-0.73.0-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://dvdisaster.net/downloads/dvdisaster-0.73.0-setup.exe.gpg">dvdisaster-0.73.0-setup.exe.gpg</a></td></tr>
<?php
  }
?>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">

<b>0.73.1</b> noch nicht veröffentlicht (19-Sep-2009)<p>
</td></tr></table><p>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
