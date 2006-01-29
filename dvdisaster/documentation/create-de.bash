#! /bin/bash

#  dvdisaster: German homepage translation
#  Copyright (C) 2004-2006 Carsten Gnörlich
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA,
#  or direct your browser at http://www.gnu.org.

# ----- Translations for phrases used in create.bash

function dictionary_de()
{  trans_encoding="ISO-8859-1"

   trans_version="Version"

   trans_to_internet="Zur Internet-Version"
   trans_to_hoster="Übersicht (auf BerliOS)"

   trans_back="Zurück"

   trans_contents="Inhalt"
   trans_hosting="Diese Seiten liegen auf"

   trans_fdl="Die unveränderte Wiedergabe und Verteilung dieses gesamten Textes in beliebiger Form ist gestattet, sofern dieser Hinweis erhalten bleibt."
   trans_copyright="Copyright 2004-2006 Carsten Gnörlich."
}


# ----- These are the "inlined" html pages.

function inline_contents_de()
{  title="Abbildung(en)"
}

function inline1de()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Abbildung: Stark verkratzter Datenträger</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-img.jpg"></center>
<br clear="all">
</td></tr>
<tr><td>
Dieser Datenträger wurde ohne Hülle in einem Rucksack transportiert;
die runden Abdrücke stammen von einer Flasche Mineralwasser. Die Kratzer
erscheinen in dem Foto allerdings durch Reflektionen
des Blitzlichtes etwas überzeichnet.</tr></td>
EOF
}

function inline2de()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Abbildungen: Fehleranalyse</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-scan-de.png"></center>
<br clear="all">
</td></tr>
<tr><td>
Beim Einlesen von Datenträgern zeigt dvdisaster die Lesegeschwindigkeit (blaue Kurve) 
und die Anzahl der Lesefehler an. Mit Hilfe der Spirale kann man die ungefähre Verteilung
der Lesefehler von innen nach außen abschätzen. Die Maßstäblichkeit der
Darstellung darf aber nicht überinterpretiert werden, da der exakte Winkel zwischen zwei Segmenten
aus technischen Gründen nicht den wirklichen Verhältnissen auf dem Datenträger entspricht.
<pre> 

</pre>
</tr></td>
<tr align="center"><td>
<center><img src="../images/scratch-corr-de.png"></center>
<br clear="all">
</td></tr>
<tr><td>
Dieses Diagramm zeigt die zu korrigierenden Fehler pro Fehlerkorrekturbereich und vermittelt
damit einen Eindruck von der Auslastung der <a href="background10.html">Fehlerkorrektur</a> von dvdisaster.

Die Fehlerkurve entspricht <i>nicht</i> der räumlichen Anordnung der Lesefehler
auf dem Datenträger, denn das Verfahren basiert gerade darauf, 
die Lesefehler gleichmäßig über alle Fehlerkorrekturbereiche zu verteilen.<p>

Pro Fehlerkorrekturbereich können bei Verwendung der
<a href="example93.html#redundancy">Grundeinstellungen</a>
maximal 32 Lesefehler repariert werden (grüne Linie).
Wenn diese Linie überschritten wird, ist der Inhalt des Datenträgers 
nicht mehr vollständig wiederherstellbar.

</tr></td>
EOF
}

# ----- Individual page contents 

# ----- Default home page

function index_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Überblick" ;;

   link)    link_title="Überblick" ;;
   link0)   link_title="Überblick" ;;
   link10)   link_title="Beispiele für die Fehlerkorrektur" ;;   
   link20)   link_title="dvdisaster ergänzt PI/PO-Analysen" ;;
   link30)   link_title="Vor- und  Nachteile von dvdisaster" ;;

   content*) eval "index$page$lang $file" ;;
   esac 
}

function index0de()
{  cat >> $1 <<EOF
<h3>Das dvdisaster-Projekt:</h3>

dvdisaster erzeugt einen Sicherheitspuffer gegen  <b>Datenverluste</b>, die
auf <a href="qa10.html#media">CD</a>- und  <a href="qa10.html#media">DVD</a>-Datenträgern 
durch <b>Alterung</b> oder <b>Verkratzen</b> entstehen:

<ul>
<li>dvdisaster legt einen <b>Fehlerkorrekturkode</b> für die Fälle an,
in denen vom Laufwerk nicht mehr korrigierbare Lesefehler auftreten.<p></li>

<li>dvdisaster versucht so viele Daten wie möglich von einem defekten Datenträger zu lesen.
Danach werden unlesbare Datenabschnitte durch den vorher erzeugten Fehlerkorrekturkode 
rekonstruiert. 
Die maximale Kapazität der Fehlerkorrektur ist einstellbar.</li>
</ul>

Wenn Sie die Fehlerkorrekturkode-Dateien rechtzeitig erzeugen und gut aufheben,
haben Sie bei typischen Lesefehlern eine gute Aussicht, Ihre Daten noch
vollständig auf einen neuen Datenträger zu retten.

<p>
<a href="index10.html">Beispiele für die Fehlerkorrektur...</a>
EOF
}

function index10de()
{ 
  create_inline inline de 1 index10.html "zu den Beispielen für die Fehlerkorrektur"
  create_inline inline de 2 index10.html "zu den Beispielen für die Fehlerkorrektur"
 
  cat >> $1 <<EOF
<h3>Beispiele für die Fehlerkorrektur</h3>

<table><tr><td valign="top">
<b><a name="scratches">Stark verkratzter Datenträger.</a></b> &nbsp;

Dieser Datenträger wurde ohne Hülle in einem Rucksack transportiert;
die runden Abdrücke stammen von einer Flasche Mineralwasser. Die Kratzer
erscheinen in dem Foto allerdings durch Reflektionen
des Blitzlichtes etwas überzeichnet.<p>

Eine Oberflächenanalyse des Datenträgers ergibt rund 135.000 unlesbare
Sektoren (von insgesamt ca. 2.200.000, also 6% defekte Sektoren).
Bei der <a href="example50.html">Wiederherstellung</a> des Datenträger-Abbildes
(mit <a href="example93.html#redundancy">Grundeinstellungen</a>)
treten im schlechtesten <a href="background10.html">ECC-Bereich</a> 22 Fehler auf.
Dies entspricht einer maximal 69%-igen Auslastung der Fehlerkorrektur
unter den gewählten Einstellungen.<p>


</td><td width=110>
<a href="inline1.html">
<img src="../images/scratch-img.jpg" width=100 height=97></a>
<br clear="all">
<a href="inline2.html">
<img src="../images/scratch-scan-de.png" width=100 height=74></a>
<br clear="all">
<a href="inline2.html">
<img src="../images/scratch-corr-de.png" width=100 height=74></a>
</td></tr></table>

<a href="index20.html">Warum PI/PO-Analysen alleine nicht ausreichen...</a>
EOF
}

function index20de()
{  cat >> $1 <<EOF
<h3>dvdisaster als Ergänzung zu PI/PO-Analysen</h3>

<a href="qa.html#pipo">PI/PO-Analysen</a> sind ein wertvolles Werkzeug 
zur Überprüfung und Optimierung der Brennqualität.
<p>

Aber: Die <b>Lebensdauer</b> von DVD-Datenträgern läßt sich durch 
PI/PO-Analysen <b>nicht zuverlässig voraussagen</b>!

<ul>
<li> PI/PO-Analysen erlauben keine Aussage darüber,
wie viele Kratzer oder Wochen Alterung ein Datenträger noch
verkraftet, bevor erste unlesbare Sektoren auftreten.<p></li>

<li>Das Umkopieren eines Datenträgers aufgrund schlechter
PI/PO-Werte erweist sich häufig als zu voreilig - 
manchmal "leben" solche DVDs noch viel länger als erwartet.<p></li>

<li>Andererseits kann man (ohne ein Werkzeug wie dvdisaster) mit dem Umkopieren 
natürlich nicht warten, bis Lesefehler auftreten - 
dann wären die ersten Daten schließlich schon verloren.</li>
</ul>

<a href="index30.html">Vor- und Nachteile von dvdisaster zusammengefaßt...</a>
EOF
}


function index30de()
{  cat >> $1 <<EOF
<h3>Vorteile von dvdisaster:</h3>

<ul>
<li><b>Schutz</b> vor unvorhergesehener Beschädigung (in <a href="background10.html">bestimmten Grenzen</a>).<p></li>
<li>Überprüfen auf Lesefehler geht <b>schneller</b> als eine PI/PO-Analyse.<br>
Je nach Laufwerk mit bis zu 16-facher Lesegeschwindigkeit.<p></li>
<li><b>Kostensparend:</b> Datenträger brauchen erst dann umkopiert zu werden,
wenn sie wirklich defekt sind.</li>
</ul>

<h3>Nachteile von dvdisaster:</h3>

<ul>
<li>Die Fehlerkorrektur-Datei <b>muß erzeugt werden, bevor</b> der 
Datenträger kaputt geht.<p></li>
<li>Die Fehlerkorrektur-Datei benötigt <b>zusätzlichen Speicherplatz</b> und
muß auf einem zuverlässigen Speichermedium aufbewahrt werden.
In der <a href="example93.html#redundancy">Grundeinstellung</a> beträgt der
Speicherplatzbedarf 15% der Originaldaten (ca. 700MB für eine volle einschichtige DVD).<p></li>
<li><a href="background10.html">kein garantierter Schutz</a> vor Datenverlust.</li>
</ul>

<p>

Schauen Sie auch in die  <a href="background.html">Hintergrundinformationen</a>, um mehr über die
Arbeitsweise von dvdisaster zu erfahren.
EOF
}


# ----- Examples

function example_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Beispiele" ;;

   link)    link_title="Beispiele und Bildschirmfotos" ;;
   link10)   link_title="Graphische Benutzeroberfläche verwenden" ;;
   link20)   link_title="Datenträger-Abbild erstellen" ;;
   link30)   link_title="Fehlerkorrektur-Datei erzeugen" ;;
   link40)   link_title="Datenträger auf Lesefehler prüfen" ;;
   link50)   link_title="Datenträger-Abbild rekonstruieren" ;;
   link60)   link_title="Abbild mit Fehlerkorrektur-Daten vergleichen" ;;

   link90)   link_title="Einstellmöglichkeiten" ;;
   link91)   link_title="Allgemein" ;;
   link92)   link_title="Lesen und Prüfen" ;;
   link93)   link_title="Fehlerkorrektur" ;;

   content*) eval "example$page$lang $file" ;;
   esac 
}

function example0de()
{  cat >> $1 <<EOF
<h3>Beispiele und Bildschirmfotos</h3>
Die folgenden Unterseiten führen durch die Anwendung von dvdisaster:

<ol>
<li><a href="example10.html">Aufrufen der graphischen Benutzeroberfläche</a><p></li>
<li><a href="example20.html">Erstellen eines Datenträger-Abbildes</a><p></li>
<li><a href="example30.html">Erzeugen einer Fehlerkorrektur-Datei</a><p></li>
<li><a href="example40.html">Prüfen eines Datenträgers</a><p></li>
<li><a href="example50.html">Rekonstruieren eines Datenträger-Abbildes</a><p></li>
<li><a href="example60.html">Abbild mit Fehlerkorrekturdaten vergleichen</a><p></li>
</ol>
<pre> </pre>

Weitere Informationen:

<ul>
<li><a href="example90.html">Einstellmöglichkeiten</a><p></li>

<ul>
<li><a href="example91.html">Allgemeine Einstellungen</a><p></li>
<li><a href="example92.html">Einstellungen zum Lesen und Prüfen</a><p></li>
<li><a href="example93.html">Einstellungen zur Fehlerkorrektur</a><p></li>
</ul>

<li><a href="background60.html">Tips zum Aufbewahren der Fehlerkorrektur-Datei</a></li>
</ul>
EOF
}


function example10de()
{  
cat >> $1 <<EOF
<h3>Aufrufen der graphischen Benutzeroberfläche</h3>

dvdisaster öffnet automatisch eine graphische Benutzeroberfläche,<br>
wenn Sie in Ihrer Fensterumgebung auf das dvdisaster-Programmsymbol klicken.<p>

<p>Nach einer kurzen Ladezeit erscheint das dvdisaster-Fenster im Anfangszustand:<p>

<img src="../images/ex-start-de.png" alt="Anfangsbildschirm"><p>

<b>Allgemeine Hinweise:</b>

<ul>
<li>Die optische Ausgestaltung der Fensterelemente wird durch das verwendete Betriebssystem
und die persönlichen Einstellungen für die Fensterumgebung beeinflußt.<p></li>
<li>dvdisaster verfügt auch über eine <a href="syntax10.html">Kommandozeilen-Betriebsart</a>.<p></li>
<li>Beim ersten Aufruf in der graphischen Betriebsart wird eine Hilfsdatei
<i>.dvdisaster</i> angelegt, in der die Auswahlen aus dem 
<a href="example90.html">"Einstellungen"-Dialog</a> gespeichert werden. Sobald diese Datei
existiert, haben Einstellungen über die Kommandozeile keine Auswirkung innerhalb
der graphischen Betriebsart.<p>
</ul>

<b>Hinweise für die Windows-Version:</b>

<ul>
<li>Zum Zugriff auf das CD/DVD-Laufwerk werden in Windows 2000 und XP entweder 
<b>Systemverwalter-Rechte</b> oder ein <a href="qa10.html#aspi">ASPI</a>-Treiber benötigt.<p></li>
<li>Aus technischen Gründen gibt es unter Windows zwei Programmversionen:
<table>
<tr><td><i>dvdisaster<b>-win</b>.exe</i></td><td>--</td><td>für die graphische Benutzeroberfläche</td></tr>
<tr><td><i>dvdisaster.exe</i></td><td>--</td><td>für die Kommandozeilen-Betriebsart</td></tr>
</table>
</li>
</ul>

<a href="example20.html">Datenträger-Abbild erstellen...</a>
EOF
}


function example20de()
{  cat >> $1 <<EOF
<h3>Datenträger-Abbild erstellen</h3>

Die meisten Funktionen von dvdisaster arbeiten mit einem Abbild
des Datenträgers auf der Festplatte. Das geht schneller und schont das CD/DVD-Laufwerk.<p>

So erzeugen Sie ein Abbild (die einzelnen Schritte sind in dem nachfolgenden 
Bildschirmfoto markiert):<p>

<b>1. Wählen Sie das Laufwerk aus <font color="red">(1)</font>, 
in das Sie die CD oder DVD eingelegt haben.</b><p>

Unter Linux sind momentan nur diejenigen Laufwerke verfügbar,
die unter /dev aufgeführt sind.
Eine flexiblere Auswahl wird in einer der nächsten Versionen eingeführt.
<p>

<b>2. Wählen Sie eine Abbild-Datei aus.</b><p>

Ein Klick auf das 
<img src="../images/open-img.png" alt="Abbild-Dateiauswahl" align="middle">-Symbol 
<font color="red">(2a)</font> öffnet ein Fenster zur Auswahl der Abbild-Datei.<br>
Sie können den Dateinamen aber auch direkt in das Textfeld <font color="red">(2b)</font> eingeben.<p>

Achten Sie darauf, genügend Speicherplatz für das Abbild zu haben. Aktivieren Sie das
<a href="example91.html#localfiles">Aufteilen von Dateien</a> wenn Ihr 
Dateisystem keine Dateigrößen von mehr als 2GB unterstützt. <p>

<b>3. Lesen Sie das Abbild ein.</b><p>

Klicken Sie auf den "Lesen"-Knopf <font color="red">(3)</font>, 
um mit dem Einlesen des Abbildes zu beginnen.<p>

<hr>
Bildschirmfoto: Abbild einlesen
<hr>
<img src="../images/ex-read-de.png" alt="Abbild einlesen"><p>
<hr>

Verfolgen Sie den Fortschritt des Einlesens anhand der graphischen Darstellung.<p>

Durch Anklicken des 
<img src="../images/btn-protocol-de.png" alt="Protokoll" align="middle">--Symbols
erhalten Sie zusätzliche Informationen über den Lesevorgang.<p>

<a href="example30.html">Erzeugen der Fehlerkorrektur-Datei...</a>
EOF
}


function example30de()
{  cat >> $1 <<EOF
<h3>Fehlerkorrektur-Datei erzeugen</h3>
Die Fehlerkorrektur-Datei spielt bei dvdisaster eine zentrale Rolle:<br>
Sie wird zur Rekonstruktion von unlesbaren Sektoren benötigt.<p>

Nachdem Sie ein <a href="example20.html">Abbild des Datenträgers</a> auf der Festplatte
erzeugt haben, führen Sie die folgenden Schritte aus:<p>

<b>1. Wählen Sie die Abbild-Datei <font color="red">(1)</font> aus.</b><p>

Die zuletzt eingelesene Abbild-Datei ist bereits voreingestellt.<p>

<b>2. Wählen Sie eine Fehlerkorrektur-Datei aus.</b><p>

Ein Klick auf das 
<img src="../images/open-ecc.png" alt="Fehlerkorrektur-Datei-Auswahl" align="middle">-Symbol 
<font color="red">(2a)</font>
öffnet ein Fenster zur Auswahl der Fehlerkorrektur-Datei.<br>
Sie können den Dateinamen auch direkt in das Textfeld <font color="red">(2b)</font> eingeben.<p>
<p>

<b>3. Erzeugen Sie die Fehlerkorrekturdatei.</b><p>

Klicken Sie auf den "Erzeugen"-Knopf <font color="red">(3)</font>, 
um die Fehlerkorrektur-Datei zu erstellen.<p>

<hr>
Bildschirmfoto: Fehlerkorrektur-Datei erzeugen
<hr>
<img src="../images/ex-create-de.png" alt="Fehlerkorrektur-Datei erzeugen"><p>
<hr>

Die Prozentwerte zeigen den Fortschritt des Vorgangs.
Auf einem durchschnittlichen 2Ghz-Rechner werden mit der 
voreingestellten <a href="example93.html#redundancy">Redundanz</a>
etwa 10min für eine volle einschichtige DVD benötigt.<p>

<b>Beachten Sie bitte</b>:

<ul>
<li>Die Fehlerkorrektur-Datei kann nicht erstellt werden, wenn das Abbild unvollständig
ist oder Lesefehler aufweist.<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Erstellen Sie die Fehlerkorrektur-Datei deshalb sofort
nach dem Brennen des Datenträgers.</td></tr></table>
</li>

<li>Nach dem erfolgreichen Erstellen der Fehlerkorrekturdatei wird die Abbild-Datei nicht mehr
benötigt.<br>
Wenn Sie einen anderen Datenträger einlesen, wird dvdisaster
die bereits bestehende Abbild-Datei automatisch überschreiben.<p></li>

<li>
Verwahren Sie die Fehlerkorrektur-Datei auf 
einem <a href="background60.html">zuverlässigen Speichermedium</a>!
</li>
</ul>

<a href="example40.html">Überprüfen Sie danach den Datenträger regelmäßig auf Lesefehler...</a>
EOF
}


function example40de()
{  cat >> $1 <<EOF
<h3>Datenträger auf Lesefehler überprüfen</h3>

Um festzustellen, ob ein Datenträger Lesefehler enthält, 
legen Sie ihn in das Laufwerk und gehen Sie wie folgt vor:<p>

<b>1. Wählen Sie das Laufwerk aus <font color="red">(1)</font>, 
in das Sie die CD oder DVD eingelegt haben.</b><p>

Unter Linux sind momentan nur diejenigen Laufwerke verfügbar,
die unter /dev aufgeführt sind.
Eine flexiblere Auswahl wird in der nächsten Version eingeführt.<p>

Der Prüfvorgang benötigt nicht zwingend eine 
Fehlerkorrektur-Datei, kann aber die darin
enthaltenen Informationen nutzen. 
Wenn Sie für den Datenträger eine passende Fehlerkorrektur-Datei besitzen,
geben Sie ihren Namen vor dem Prüfen ein.<p>

<b>2. Beginnen Sie mit der Überprüfung.</b><p>

Klicken Sie auf den "Prüfen"-Knopf <font color="red">(2)</font>, 
um mit dem Überprüfen des Datenträgers zu beginnen.<p>

<hr>
Bildschirmfoto: Datenträger überprüfen
<hr>
<img src="../images/ex-scan-de.png" alt="Datenträger überprüfen"><p>
<hr>

Die graphische Darstellung zeigt den Fortschritt der Überprüfung:<p>

<ul>
<li>Die <b>Geschwindigkeitskurve</b> liefert eine grobe Übersicht über den Datenträger-Zustand,
weil das Laufwerk beim Lesen abbremst, wenn die Qualität des Datenträgers schlechter wird.<p>

Um auszuschließen, daß andere Systemaktivitäten die Lesegeschwindigkeit beeinflussen,
sollten Sie während der Überprüfung keine anderen Programme laufen lassen und auch das
dvdisaster-Fenster nicht bedienen.<p></li>

<li>Die <b>Spirale</b> gibt eine (nicht maßstabsgetreue) 
Übersicht über den <b>Datenträger-Zustand</b>.
Rote Markierungen weisen auf Lesefehler hin!
<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Sobald Lesefehler auf dem Datenträger auftreten, müssen Sie
schnellstmöglich versuchen, das <a href="example50.html">Datenträger-Abbild zu rekonstruieren</a> 
und auf einen neuen Datenträger übertragen. </td></tr></table><p>
</li>

<li>Durch Anklicken des 
<img src="../images/btn-protocol-de.png" alt="Protokoll" align="middle">--Symbols
erhalten Sie zusätzliche Informationen über den Lesevorgang.<p></li>
</ul>

<b>Wie oft sollte man überprüfen?</b><br>
 Das hängt von der Qualität der Rohlinge 
und den Lagerungsbedingungen ab. Als Richtlinie für einmal beschreibbare CD- und DVD-Datenträger
mit unbekannter Qualität kann man nehmen:

<ol>
<li> 3 Tage nach dem Brennen (bis dahin das Abbild auf der Festplatte aufheben!), </li>
<li> 14 Tage nach dem Brennen, </li>
<li> 3 Monate nach dem Brennen, </li>
<li> dann alle weiteren 6 Monate überprüfen.</li>
</ol>

Wenn Sie viele gleichartige Rohlinge aus der Produktion eines Markenherstellers verwenden, 
reicht gegebenenfalls auch ein stichprobenhaftes Überprüfen von 2-3 Datenträgern pro Monat und Produktionsserie.<p>

<a href="example50.html">Abbild eines Datenträgers rekonstruieren...</a>
EOF
}


function example50de()
{  cat >> $1 <<EOF
<h3>Datenträger-Abbild rekonstruieren</h3>

Zur Rekonstruktion der Daten eines defekten Datenträgers
benötigen Sie die <a href="example30.html">zugehörige Fehlerkorrektur-Datei</a>,
die Sie hoffentlich zu einem Zeitpunkt angelegt haben, als der Datenträger noch
lesbar war. Führen Sie die folgenden Schritte aus:<p>

<b>1. Lese-Verfahren umschalten.</b><p>

Wählen Sie im Einstellungs-Dialog zum Lesen und Prüfen das
<a href="example92.html#read">angepaßte Lese-Verfahren</a> aus.<p>

<b>2. Erzeugen Sie zunächst ein Abbild der noch lesbaren Sektoren.</b><p>

Wählen Sie das Laufwerk, die Abbild-Datei und die Fehlerkorrektur-Datei 
aus <font color="red">(2a)</font>. <br>Die Fehlerkorrektur-Datei wird jetzt schon benötigt,
damit dvdisaster ermitteln kann, wann genügend Informationen zur Wiederherstellung des
Abbildes eingelesen worden sind.<br>
Starten Sie dann den Lesevorgang durch Klicken auf den 
"Lesen"-Knopf <font color="red">(2b)</font>.<br>
<br> 
Berücksichtigen Sie, daß das Einlesen eines beschädigten Datenträgers 
mehrere Stunden dauern kann.<p>

<hr>
Bildschirmfoto: Abbild einlesen
<hr>
<img src="../images/ex-read-a-de.png" alt="Abbild einlesen"><p>
<hr>

<pre> </pre>

<a name="repair"></a>
<b>3. Führen Sie die Rekonstruktion des Datenträgers durch.</b><p>

Klicken Sie auf den "Reparieren"-Knopf. Während der Fehlerkorrektur ist eine zeitweilig heftige
Festplatten-Aktivität normal.<p>

<hr>
Bildschirmfoto: Datenträger-Inhalt rekonstruieren
<hr>
<img src="../images/ex-fix-de.png" alt="Datenträger-Inhalt rekonstruieren"><p>
<hr>

Der Fortschritt der Rekonstruktion wird in Prozent angezeigt.<p>

<b>Ergänzende Hinweise:</b><p>

<ul>
<li>Die Meldung "Gut! Alle Sektoren wurden repariert." bedeutet, daß das Abbild
des Datenträgers vollständig wiederhergestellt werden konnte. Das reparierte 
Abbild läßt sich wie ein normales ". i s o"-Abbild mit einer geeigneten Brennsoftware
auf einen neuen Datenträger schreiben. 
<p></li>

<li>Wenn unkorrigierbare Sektoren übrigbleiben, 
lesen Sie das Abbild erneut 
ein <a href="background50.html#reading-tips">(ein paar Tips dazu)</a>.
In diesem Fall versucht dvdisaster nur die fehlenden Sektoren noch einmal zu lesen.
Dadurch erhalten Sie gegebenenfalls genügend Daten für einen zweiten erfolgreichen 
Fehlerkorrektur-Durchlauf.<p></li>

<li>Die rote Fehlerkurve entspricht <i>nicht</i> der räumlichen Anordnung der Lesefehler
auf dem Datenträger, denn das Verfahren basiert gerade darauf, 
Lesefehler gleichmäßig über alle Fehlerkorrekturbereiche zu verteilen.<br>
Die grüne Linie kennzeichnet die Leistungsgrenze der Fehlerkorrektur. Wenn sie
überschritten wird, kann der Datenträger-Inhalt mit den zu diesem Zeitpunkt vorhandenen
Daten nicht vollständig wiederhergestellt werden.
Falls die Linie nur knapp überschritten wird,
hilft manchmal ein <a href="background50.html#reading-tips">weiterer Leseversuch</a>.
</li>

<li>Wenn Sie das Abbild mit dem "angepaßten Lese-Verfahren" einlesen,
wird die Fehlerkorrektur immer maximal ausgelastet.</li>
</ul>
EOF
}


function example60de()
{  cat >> $1 <<EOF
<h3>Datenträger-Abbild mit Fehlerkorrektur-Daten vergleichen</h3>

Diese Funktion liefert Informationen über Abbild- und Fehlerkorrektur-Dateien,
die sich bereits auf der Festplatte befinden:<p>

<b>1. Wählen Sie die zu vergleichenden Dateien aus <font color="red">(1)</font>.</b><p>

Geben Sie den Namen einer Abbild-Datei und der zugehörigen Fehlerkorrektur-Datei 
an (wie in den Schritten 2a/2b beim Erzeugen der <a href="example20.html">Abbild-Datei</a> und der <a href="example30.html">Fehlerkorrektur-Datei</a>).<p>

<b>2. Führen Sie den Vergleich durch <font color="red">(2)</font>.</b><p>

Klicken Sie auf den "Vergleichen"-Knopf. Dieser Vorgang kann eine Weile dauern, da beide 
Dateien komplett gelesen (aber nicht verändert) werden.<p>

<hr>
Bildschirmfoto: Abbild- und Fehlerkorrektur-Dateien vergleichen 
<hr>
<img src="../images/ex-compare-de.png" alt="Datenträger-Inhalt rekonstruieren"><p>
<hr>

Nach dem Vergleich erhalten Sie die folgenden Informationen:<p>

<b>Felder "Abbild-Datei" und "Datenträger-Zustand".</b> 
Diese Felder geben Aufschluß darüber, ob das Abbild eine Fehlerkorrektur
benötigt. Bei noch unkorrigierten Lesefehlern ist die Anzahl der fehlenden
Sektoren größer als Null. Prüfsummenfehler deuten hingegen auf
<a href="qa20.html#crc">Bedienungs-Fehler oder Hardware-Probleme</a> hin.<p>

<b>Feld "Fehlerkorrektur-Datei"</b>. Hier können Sie sehen, 
mit welchen Einstellungen die Fehlerkorrektur-Datei erstellt worden 
ist und ob sie mit der verwendeten
Version von dvdisaster verarbeitet werden kann.<br>
Falls die Fehlerkorrektur-Datei zu einem anderen Abbild gehört oder 
beschädigt ist, erscheinen Warnungen im Bereich der letzten vier Einträge.<p>

<b>Ergänzende Hinweise:</b>

<ul>
<li>Wenn das Abbild mit einer anderen Software als dvdisaster eingelesen
worden ist, wird das Ergebnis des Vergleichs unzuverlässig.
Insbesondere erscheinen in diesem Fall alle fehlenden Sektoren als Prüfsummenfehler.</li>
</ul>
EOF
}



function example90de()
{  cat >> $1 <<EOF
<h3>Einstellmöglichkeiten</h3>

Drücken Sie den 
<img src="../images/open-preferences.png" align="middle" alt="Preferences button">-Knopf rechts oben
in der Werkzeugleiste, um den Dialog zum Ändern der Grundeinstellungen zu öffnen.
Der Dialog enthält die folgenden Reiterkarten:<p>

<center>
<a href="example91.html">
<img src="../images/prefs-general-de.png"><br>
Allgemeine Einstellungen
</a>
</center>
<br clear="all">

<center>
<a href="example92.html">
<img src="../images/prefs-read-de.png"><br>
Einstellungen zum Lesen und Prüfen
</a>
</center>
<br clear="all">

<center>
<a href="example93.html">
<img src="../images/prefs-ecc-de.png"><br>
Einstellungen zur Fehlerkorrektur
</a>
</center>

<br clear="all">
EOF
}


function example91de()
{  cat >> $1 <<EOF
<h3>Allgemeine Einstellungen</h3>

Mehr Informationen über die Reiterkarte mit allgemeinen Einstellungen befinden sich
weiter unten.<p>

<center><img src="../images/prefs-general-de.png"></center>
<br clear="all">

<p><hr><p>

<a name="iso"></a>
<b>Dateisystem-Einstellungen für Datenträger und Abbilder</b><p>

<center><img src="../images/prefs-general-1-de.png"></center>
<br clear="all">

Aktivieren Sie diese Option, 
damit dvdisaster die Größenangaben für das Abbild aus dem
ISO- bzw. UDF-Dateisystem ermittelt. 
Dies ist hilfreich bei <a href="qa20.html#plusrw">Problemen mit Laufwerken</a>, 
die bei DVD-RW/+RW falsche Abbild-Größen zurückmelden.

<p><hr><p>

<a name="localfiles"></a>
<b>Einstellungen für lokale Dateien (auf der Festplatte)</b><p>

<center><img src="../images/prefs-general-2-de.png"></center>
<br clear="all">

<b>Dateien automatisch mit Endungen versehen <font color="red">(1)</font>:</b> 
Dieser Schalter bewirkt, daß Dateien automatisch
mit den Endungen ".img" oder ".ecc" versehen werden, sofern nicht bereits eine andere Endung
angegeben wurde.<p>

<b>Dateien in Segmente aufteilen <font color="red">(2)</font>:</b> 
Ermöglicht das Arbeiten mit Dateisystemen, 
die nur Dateien mit einer Maximalgröße von 2GB zulassen 
(z.B. FAT unter Windows). <br>
Anstelle einer einzigen Datei "abbild.img" werden bis zu 100
Segmente "abbild00.img", "abbild01.img" usw. verwendet; dies kostet
etwas Geschwindigkeit.<p> 

<p><hr><p>

<a name="auto"> </a>
<b>Automatisches Erzeugen und Löschen von Dateien</b><p>

<center><img src="../images/prefs-general-3-de.png"></center>
<br clear="all">

<b>Fehlerkorrektur-Datei erzeugen <font color="red">(1)</font>:</b> Nach dem erfolgreichen 
Einlesen eines Abbilds wird automatisch eine Fehlerkorrektur-Datei erzeugt. Zusammen mit
der "Abbild löschen"-Funktion beschleunigt dies das Erstellen von Fehlerkorrektur-Dateien
für mehrere Datenträger.<p>

<b>Abbild löschen <font color="red">(2)</font>:</b> Nach dem erfolgreichen 
Erstellen einer Fehlerkorrektur-Datei wird
die zugehörige Abbild-Datei gelöscht, wenn dieser Schalter aktiviert ist.
EOF
}


function example92de()
{  cat >> $1 <<EOF
<h3>Einstellungen zum Lesen und Prüfen</h3>

Mehr Informationen über die Reiterkarte zum Lesen und Prüfen befinden sich
weiter unten.<p>

<center><img src="../images/prefs-read-de.png" alt="Einstellungen zum Lesen und Prüfen"></center><br clear="all">

<p><hr><p>

<a name="read"></a>
<b>Einstellungen zum Lesen</b><p>

<center><img src="../images/prefs-read-1-de.png"></center>
<br clear="all">

<b>Leseverfahren <font color="red">(1)</font>:</b> Wählen Sie zwischen dem
<a href="background30.html">linearen Leseverfahren</a> und dem
<a href="background40.html">angepaßten Leseverfahren</a>.
Das lineare Verfahren eignet sich für unbeschädigte Datenträger 
und das angepaßte Verfahren ist besser für Datenträger mit Lesefehlern.<p>

<b>Lesebereich <font color="red">(2)</font>:</b> Anwählen dieses Schalters
beschränkt die einzulesenden Sektoren auf den angegebenen Bereich
(inklusiv; 0-100 liest also 101 Sektoren ein). Diese Einstellungen gelten nur für
die aktuelle Sitzung, d.h. sie werden nicht gespeichert.<p>

<b>Überspringe x Sektoren nach einem Lesefehler /<br>
Lesen beenden wenn unlesbare Bereiche < x <font color="red">(3)</font></b><br>
Mit dem Schieberegler wird die Behandlung von Lesefehlern beeinflußt. Seine
Auswirkungen sind leicht unterschiedlich für 
das <a href="background30.html#configure">lineare</a> und 
das <a href="background40.html#configure">angepaßte</a> Leseverfahren.
Größere Werte verringern die Bearbeitungszeit und die mechanische Beanspruchung 
des Laufwerkes, lassen aber größere Lücken beim Einlesen defekter Bereiche.

<p><hr><p>

<a name="image"></a>
<b>Abbild-Eigenschaften</b><p>
<center><img src="../images/prefs-read-2-de.png"></center>
<br clear="all">

<b>DAO-Abbild <font color="red">(1)</font>:</b> 
Verhindert ein <a href="qa20.html#tao">sehr seltenes Problem</a> beim
Erstellen von Abbildern von "DAO" ("disc at once") - CD-R.<p>

<b>Unlesbare Sektoren auffüllen <font color="red">(2)</font>:</b> 
Stellt die Kompatibilität mit anderen Datenrettungsprogrammen her.
Momentan bekannte Einstellungen und Programme sind:<p>
<i>0xb0 (dezimal 176)</i> -- h2cdimage/dares (c't-Magazin)<p>
Schalten Sie dieses Feld aus, wenn Sie Ihre Abbilder ausschließlich
mit dvdisaster verarbeiten.

<p><hr><p>

<b>Laufwerk vorbereiten</b><p>

<center><img src="../images/prefs-read-3-de.png"></center>
<br clear="all">

Gibt dem Laufwerk die angegebene Anzahl Sekunden Zeit zum Hochdrehen bevor
der Lesevorgang beginnt. Dadurch erscheinen keine abrupten 
Geschwindigkeitsänderungen am Anfang der Lesekurve.
EOF
}


function example93de()
{  cat >> $1 <<EOF
<h3>Einstellungen zur Fehlerkorrektur</h3>

Mehr Informationen über die Reiterkarte zur Fehlerkorrektur befinden sich
weiter unten.<p>

<center><img src="../images/prefs-ecc-de.png" alt="Fehlerkorrektur-Einstellungen"></center><br clear="all">

<p><hr><p>


<a name="redundancy"></a>
<b>Redundanz für neu erstellte Fehlerkorrekturdateien</b> <p>

<center><img src="../images/prefs-ecc-1-de.png" alt="Fehlerkorrektur-Einstellungen"></center><br clear="all">

Die Redundanz gibt an, wieviel Prozent der 
Originaldaten <a href="background10.html">im günstigsten Fall</a>
korrigiert werden können. Da der Idealfall natürlich selten eintritt, sollten
Sie die Redundanz großzügig mit einer der folgenden Möglichkeiten auswählen:<p>

<font color="red">(1)</font> / <font color="red">(2)</font> Die Voreinstellungen
<b>normal</b> und <b>hoch</b> liefern eine Redundanz von 14.3% bzw. 33.5%. 
Mit diesen beiden Einstellungen werden Fehlerkorrektur-Dateien durch optimierten Programmcode
besonders schnell erzeugt.<p>


<font color="red">(3)</font> Einstellung der Redundanz in <b>Prozentwerten</b>.
Beachten Sie dabei:
<ul>
<li> Eine Fehlerkorrekturdatei mit x% Redundanz benötigt 
auch etwa x% der Größe des zugehörigen Abbilds an Speicherplatz.</li>
<li> Die Leistung der Fehlerkorrektur hängt von der Verteilung der Lesefehler ab.
Erst Änderungen der Redundanz um etwa 5 Prozentpunkte 
haben einen spürbaren Einfluß auf die Fehlerkorrektur.</li>
</ul><p>

<font color="red">(4)</font> Größe der Fehlerkorrektur-Datei (in MB) vorgeben.
In diesem Fall wählt dvdisaster eine geeignete Redundanz, damit die
Fehlerkorrektur-Datei nicht größer als angegeben wird.<p>

Vorsicht: Wenn man unterschiedlich große Abbilder 
mit der gleichen Einstellung bearbeitet, erhalten die kleineren Abbilder 
mehr Fehlerkorrekturinformationen als die großen Abbilder.<p>

<p><hr><p>

<b>Speichernutzung.</b><p>

<center><img src="../images/prefs-ecc-2-de.png" alt="Fehlerkorrektur-Einstellungen"></center><br clear="all">

Zugriffe auf die Abbild- und Fehlerkorrektur-Datei werden durch einen eigenen
Zwischenspeicher optimiert. Die Voreinstellung von 32MB ist für die meisten Systeme passend.<p>
EOF
}


# ----- Downloads

function download_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Herunterladen" ;;

   link)    link_title="Herunterladen" ;;
   link10)   link_title="Systemanforderungen" ;;
   link20)   link_title="Installation" ;;

   content*) eval "download$page$lang $file" ;;
   esac 
}

function download0de()
{  cat >> $1 <<EOF
<h3>dvdisaster herunterladen</h3>

dvdisaster unterstützt die Betriebssysteme FreeBSD, Linux und Windows
in den <a href="download10.html">aktuellen Versionen</a>.<p>

Laden Sie sich bitte entweder den Quellkode oder eine Binärversion 
aus der folgenden Liste herunter. Die Pakete können mit
einer <a href="#signature">digitalen Unterschrift</a> auf 
ihren Ursprungszustand überprüft werden.<p>

Um die Binärversion für Windows zu installieren, rufen Sie das Programm nach dem
Herunterladen auf und folgen dem Dialog. Für die Installation
der Quellkode-Version gibt es <a href="download20.html">Installationshinweise</a>.

<pre> </pre>

<b>Entwickler-Versionen</b> - neu und experimentell für erfahrene Benutzer!<p> 

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.65</b></td><td align="right">14-Jan-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode für alle Betriebssysteme:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65.tar.bz2">dvdisaster-0.65.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65.tar.bz2.gpg">dvdisaster-0.65.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65-setup.exe">dvdisaster-0.65-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65-setup.exe.gpg">dvdisaster-0.65-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
<i>Alle Betriebssysteme:</i>
<ul>
<li>Das Prüfen des Abbildes entfällt bei der Erstellung der Fehlerkorrekturdatei,<br>
wenn das Abbild unmittelbar vorher eingelesen wurde.</li>
<li>Fehlerkorrektur-Dateien können in 2GB große Segmente aufgeteilt werden.</li>
<li>Tschechische Übersetzung von Lubo&#353; Stan&#283;k.</li>
</ul>
<i>FreeBSD:</i>
<ul>
<li>wird mit dieser Version erstmalig unterstützt.</li>
</ul>
<i>Windows:</i>
<ul>
<li>komplette Unterstützung für FAT32-Dateisysteme (s.o.)</li>
<li>Binärversion wird jetzt mit Gtk+2.6-Bibliotheken ausgeliefert.</li>
</ul>
</td></tr></table><p>

<pre> </pre>

<b>Stabile Versionen</b> - zum Einstieg empfohlen<p> 

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.64 (pl2)</b></td><td align="right">01-Nov-2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2.tar.bz2">dvdisaster-0.64.2.tar.bz2</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2.tar.bz2.gpg">dvdisaster-0.64.2.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2-setup.exe">dvdisaster-0.64.2-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2-setup.exe.gpg">dvdisaster-0.64.2-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Diese Version kann die <a href="example91.html#iso">Abbild-Größe aus dem
UDF/ISO-Dateisystem</a> bestimmen, um 
die <a href="qa20.html#plusrw">Größenerkennung von -RW/+RW-Medien</a> zu verbessern.
Das Einlesen von Abbildern und das Erzeugen der zugehörigen Fehlerkorrektur-Datei
kann jetzt zusammen
<a href="example91.html#auto">mit einem Mausklick</a> durchgeführt werden.
Außerdem gibt es eine Reihe von kleinen Verbesserungen in den Bereichen
Laufwerksunterstützung, Abrundung der Funktionalität 
sowie einige kleinere Bugs weniger ;-)<p>
<b>Patches</b> (kleine Änderungen nach Version 0.64; die obigen Dateien wurden erneuert):<br>
<b>pl1</b> verhindert Absturz sofort nach dem Programmaufruf  bei Verwendung von gtk+-2.6; 
betrifft nur die Linux-Version.<br>
<b>pl2</b> repariert die -u-Kommandozeilenoption und
paßt Dateinamen-Berechtigungen unter Linux an Unix-Konventionen an.

</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.63</b></td><td align="right">01-Aug-2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63.tbz">dvdisaster-0.63.tbz</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63.tbz.gpg">dvdisaster-0.63.tbz.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63-setup.exe">dvdisaster-0.63-setup.exe</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63-setup.exe.gpg">dvdisaster-0.63-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Enthält ein <a href="example50.html">verbessertes Verfahren</a> zum
<a href="syntax90.html#adaptiveread">Lesen defekter Datenträger</a>, 
das durch einen Artikel im c't-Magazin 16/2005 motiviert wurde.
</td></tr></table><p>

<pre> </pre>

<b>Historische Versionen</b><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.55</b></td><td align="right">20-Feb-2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Quellkode (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.tgz">dvdisaster-0.55.tgz</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.tgz.gpg">dvdisaster-0.55.tgz.gpg</a></td></tr>
    <tr><td align="right">Binärversion für Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.zip">dvdisaster-0.55.zip</a></td></tr>
    <tr><td align="right">Digitale Unterschrift:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.zip.gpg">dvdisaster-0.55.zip.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Dies ist die letzte reine Kommandozeilen-Version von dvdisaster.
</td></tr></table><p>

<pre> </pre>

<a name="signature"><b>Was ist die digitale Unterschrift?</b></a><p>

Die herunterladbaren Quellkode-Pakete enthalten
eine mit <a href="http://www.gnupg.org/gnupg.html">GnuPG</a> erstellte 
digitale Unterschrift, damit Sie nachprüfen können,
ob sich die Software in ihrem ursprünglichen Zustand befindet.<p>

Zur Überprüfung der Echtheit des <a href="../pubkey.asc">öffentlichen Schlüssels</a> 
vergleichen Sie seinen "Fingerabdruck" am besten mit demjenigen, der in der Fußzeile
meiner EMails angegeben ist. Schicken Sie mir bei Bedarf eine EMail mit dem
Betreff "GPG Fingerabdruck".
EOF
}


function download10de()
{  cat >> $1 <<EOF
<h3>Systemanforderungen</h3>

<ul>
 <li><b>x86-kompatible</b> Hardware in der <b>32bit</b>-Betriebsart
  (64-bit Unterstützung siehe unten),<p></li>
 <li>ein aktuelles CD- oder DVD-Laufwerk mit ATAPI- oder SCSI-Schnittstelle,<p></li>
 <li>mindestens P4 mit 2Ghz oder vergleichbare Rechenleistung<p></b>
</ul>

<h3>Betriebssysteme</h3>

<ul>
 <li><b>FreeBSD</b> ab Version <b>6.0</b><br> 
     (für ATAPI-Laufwerke muß der Kernel neu übersetzt werden)<p>
 </li>
 <li><b>Linux</b> ab Kernel <b>2.6.7</b>, 64bit möglich<br> 
     (Kernel >= 2.4.20 sind möglich, aber nicht empfohlen!)<p>
 </li>
 <li><b>Windows</b> <b>2000</b> oder <b>XP</b><br>
     (Windows 98 und ME sind möglich, aber für die Bearbeitung von DVD nicht empfohlen)
 </li>
</ul>

EOF
}


function download20de()
{  cat >> $1 <<EOF
<h3>Installation der Quellkode-Version</h3>

<b>Benötigte Werkzeuge und Bibliotheken</b><p>

dvdisaster benötigt die üblichen <a href="http://www.gnu.org/">GNU</a>-Entwicklungswerkzeuge 
zum Übersetzen von C-Programmen. Bei FreeBSD und Linux sind diese Werkzeuge auf den
mitgelieferten Datenträgern normalerweise vorhanden.<p>
Für Windows sind die entsprechenden Werkzeuge in der
 <a href="http://www.mingw.org">Mingw</a>-Umgebung enthalten.
Cygwin wird nicht unterstützt.<p>

Abhängig vom Betriebssystem brauchen Sie außerdem:

<ul>
<li>Die <a href="http://www.gnu.org/software/gettext/">gettext</a>-Bibliothek</li>
<li>Die <a href="http://www.gnu.org/software/libiconv/">iconv</a>-Bibliothek (nur für Windows)</li>
<li>Das Werkzeug <a href="http://www.freedesktop.org/software/pkgconfig/releases">pkgconfig</a></li>
<li>Die <a href="http://www.gtk.org">GTK+-Bibliothek</a> ab Version 2.2 einschließlich aller Hilfsbibliotheken</li>
</ul>

Diese Komponenten sind auf den FreeBSD- und Linux-Installations-Datenträgern
typischerweise vorhanden.
Das <a href="http://www.gtk.org">Gtk+</a>-Projekt bietet
spezielle Versionen dieser Bibliotheken 
<a href="http://www.gimp.org/~tml/gimp/win32/downloads.html">für Windows</a> an.
Sie benötigen sowohl die normalen Bibliotheken als auch die "-devel"-Versionen.<p> 

<a name="freebsd"></a>
<b>Hinweise für FreeBSD</b><p>

FreeBSD hat keinen einheitlichen CD-ROM-Treiber vorinstalliert, der sowohl SCSI- als
auch ATAPI-Laufwerke unterstützt (wie dies unter Linux und Windows der Fall ist).
Dadurch kann dvdisaster auf einem frisch installierten FreeBSD-System keine 
ATAPI-Laufwerke ansprechen. Um einen passenden Treiber nachzuinstallieren,
müssen Sie einen Kernel mit dem zusätzlichen Modul<p>

<tt>device atapicam</tt><p>

erzeugen. Die Module <i>ata</i>, <i>scbus</i>, <i>cd</i>
und <i>pass</i> werden ebenfalls benötigt, sind aber bei FreeBSD 6.0 
schon voreingestellt. Zum Übersetzen der dvdisaster-Quellen benötigen Sie
außerdem GNU make (gmake) und die bash-Kommandozeile.<p>

<b>Übersetzen des Quellkodes</b><p>

Um den Quellkode zu übersetzen, führen Sie bitte die folgenden Schritte
in einer Eingabeaufforderung durch.<p>

Packen Sie das Quellkode-Archiv aus:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>tar xjf $project_package.tar.bz2</b><br>
</td></tr></table><p>

Wechseln Sie in das dadurch erzeugte Verzeichnis:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>cd $project_package</b><br>
</td></tr></table><p>

Rufen Sie das Konfigurations-Skript auf:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>bash configure</b><br>
<i>[... viele Ausgaben des Konfigurationsskriptes ...]</i>
</td></tr></table><p>

Das Skript wird Ihnen gegebenenfalls Hinweise über fehlende 
Bibliotheken geben. "bash configure --help" liefert Informationen darüber,
wie man den Pfad zu einer bereits installierten Bibliothek setzen kann.<p>

Wenn das Konfigurations-Skript ohne Fehlermeldungen abgearbeitet ist,
übersetzen Sie den Quellkode in das ausführbare Programm:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake</b><br>
<i>[... viele Ausgaben des Übersetzungsprozesses ...]</i>
</td></tr></table><p>

Rufen Sie das Programm testweise auf:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>./dvdisaster</b><br>
<i>[... nach einer Weile öffnet sich das <a href="example10.html">Hauptfenster</a> ...]</i>
</td></tr></table><p>

Sie können das Programm direkt aus dem aktuellen Verzeichnis
aufrufen und verwenden. Eine Kopie der dvdisaster-Internetseiten
befindet sich übrigens im Unterverzeichnis <i>documentation/de.</i><p>

Optional können Sie das Programm und die Dokumentation auch
in Ihrem System installieren lassen:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake install</b><br>
<i>[... Ausgaben des Installationsvorganges ...]</i>
</td></tr></table><p>

Beachten Sie vorher die Ausgabe des "<tt>configure</tt>"-Skriptes oder geben 
Sie "<tt>gmake show</tt>" ein um zu erfahren, in welche Unterverzeichnisse
die Programmkomponenten installiert werden. <p>
Rufen Sie "<tt>bash configure --help</tt>" auf, um
Informationen darüber zu erhalten, wie Sie diese Pfade an Ihr System
anpassen können.

<pre> </pre>

<font size="+1">Viel Erfolg bei der Benutzung von dvdisaster!</font>
EOF
}


# ----- Command line syntex

function syntax_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Kommandozeilen-Betrieb" ;;

   link)    link_title="Kommandozeilen-Betrieb" ;;
   link10)   link_title="Kommandozeile verwenden" ;;
   link20)   link_title="Datenträger-Abbild erstellen" ;;
   link30)   link_title="Fehlerkorrektur-Datei erzeugen" ;;
   link40)   link_title="Datenträger auf Lesefehler prüfen" ;;
   link50)   link_title="Datenträger-Abbild rekonstruieren" ;;

   link60)   link_title="Laufwerk und Dateinamen wählen" ;;
   link70)   link_title="Mehrere Aktionen pro Aufruf" ;;

   link90)   link_title="Aufrufmöglichkeiten" ;;

   content*) eval "syntax$page$lang $file" ;;
   esac 
}

function syntax0de()
{  cat >> $1 <<EOF
<h3>dvdisaster als reine Kommandozeilen-Anwendung</h3>

Einführung in die grundlegenden Funktionen:

<ol>
<li><a href="syntax10.html">Arbeiten mit der Kommandozeile</a><p></li>
<li><a href="syntax20.html">Erstellen eines Datenträger-Abbildes</a><p></li>
<li><a href="syntax30.html">Erzeugen einer Fehlerkorrektur-Datei</a><p></li>
<li><a href="syntax40.html">Prüfen eines Datenträgers</a><p></li>
<li><a href="syntax50.html">Rekonstruieren eines Datenträger-Abbildes</a><p></li>
</ol>
<pre> </pre>

Einige Tips zur Arbeitserleichterung:

<ul>
<li><a href="syntax60.html">Laufwerk und Dateinamen wählen</a><p></li>
<li><a href="syntax70.html">Mehrere Aktionen pro Aufruf durchführen</a><p></li>
<li><a href="background60.html">Tips zum Aufbewahren der Fehlerkorrektur-Datei</a></li>
</ul>

<pre> </pre>

Vollständige Liste aller Kommandozeilen-Aufrufmöglichkeiten:
<ul>
<li><a href="syntax90.html">Aufrufmöglichkeiten</a><p></li>
</ul>

EOF
}


function syntax10de()
{  cat >> $1 <<EOF
<h3>Verwenden der Kommandozeile</h3>

dvdisaster verwendet die Kommandozeilen-Betriebsart,
wenn Sie beim Aufruf einen der 
<a href="syntax90.html#modes">Aktionsparameter</a> 
(read, create, scan, fix, test) angeben. Anderenfalls öffnet es ein Fenster
für die <a href="example10.html">graphische Betriebsart</a>.<p>

In der Kommandozeilenbetriebsart werden Einstellungen aus der graphischen Oberfläche
bzw. der <i>.dvdisaster</i>-Datei ignoriert.<p>

<b>Hinweise für die Windows-Version:</b>

<ul>
<li>Zum Zugriff auf das CD/DVD-Laufwerk werden in Windows 2000 und XP entweder 
<b>Systemverwalter-Rechte</b> oder ein <a href="qa10.html#aspi">ASPI</a>-Treiber benötigt.<p></li>
<li>Aus technischen Gründen gibt es unter Windows zwei Programmversionen:
<table>
<tr><td><i><b>dvdisaster</b>.exe</i></td><td>--</td><td>für die Kommandozeilen-Betriebsart</td></tr>
<tr><td><i>dvdisaster-win.exe</i></td><td>--</td><td>für die graphische Benutzeroberfläche</td></tr>
</table>
</li>
</ul>

<a href="syntax20.html">Datenträger-Abbild erstellen...</a>

EOF
}


function syntax20de()
{  cat >> $1 <<EOF
<h3>Datenträger-Abbild erstellen</h3>

Die meisten Funktionen von dvdisaster arbeiten mit einem Abbild
des Datenträgers auf der Festplatte. Das geht schneller und schont das CD/DVD-Laufwerk.<p>

So erzeugen Sie ein Abbild:
<ul>
<li>Wechseln Sie in ein Verzeichnis Ihrer Festplatte mit genügend freiem Speicherplatz.</li>
<li>Legen Sie den Datenträger in das Laufwerk ein.</li>
<li>Lesen Sie das Abbild ein:</li>
</ul>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
Dies ist freie Software; es gelten die Bedingungen der<br>
GNU GENERAL PUBLIC LICENSE aus dem Quelltext.<p>

Laufwerk: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Datenträger: DVD+R, 2224288 Sektoren, 1 Schicht(en)<p>

Erzeuge neues Abbild medium.img.<br>
Warte 5 Sekunden auf das Hochdrehen des Laufwerks...<br>
Leseposition: 100.0% ( 7.7x)<br>
Alle Sektoren erfolgreich eingelesen.
</td></tr></table><p>

Während des Einlesens werden der Fortschritt in Prozent und die momentane
Lesegeschwindigkeit des Laufwerks angezeigt. <p>

Anschließend finden Sie im aktuellen Verzeichnis die Abbild-Datei <i>medium.img</i>:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.img<br>
</td></tr></table><p>

In der Grundeinstellung liest dvdisaster vom Laufwerk <i>/dev/cdrom</i> 
und erzeugt Dateien mit dem Anfang <i>medium</i>.
Natürlich können Sie auch andere <a href="syntax60.html">Laufwerke</a>
und <a href="syntax60.html">Dateinamen</a> verwenden.<p>

<a href="syntax30.html">Erzeugen der Fehlerkorrektur-Datei...</a>
EOF
}


function syntax30de()
{  cat >> $1 <<EOF
<h3>Fehlerkorrektur-Datei erzeugen</h3>
Die Fehlerkorrektur-Datei spielt bei dvdisaster eine zentrale Rolle:<br>
Sie wird zur Rekonstruktion von unlesbaren Sektoren benötigt.<p>

Nachdem Sie ein <a href="syntax20.html">Abbild des Datenträgers</a> auf der Festplatte
erzeugt haben, geben Sie ein:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -c</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
Dies ist freie Software; es gelten die Bedingungen der<br>
GNU GENERAL PUBLIC LICENSE aus dem Quelltext.<p>

Öffne medium.img: 2224288 Datenträger-Sektoren.<br>
Prüfe Abbild-Sektoren : 100%<br>
Kodiere mit Methode RS01: 32 Nullstellen, 14.3% Redundanz.<br>
Kodierungs-Fortschritt: 100.0%<br>
Fehlerkorrektur-Datei "medium.ecc" wurde erzeugt.<br>
Verwahren Sie die Datei auf einem zuverlässigen Datenträger.
</td></tr></table><p>

dvdisaster überprüft zunächst die Vollständigkeit des Abbildes,
<i>denn zu Datenträgern, die bereits Lesefehler aufweisen, kann keine Fehlerkorrektur-Datei
mehr erstellt werden!</i> <br>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Erstellen Sie die Fehlerkorrektur-Datei deshalb sofort
nach dem Brennen des Datenträgers.</td></tr></table>

Anhand der Prozentanzeige können Sie den Fortschritt des Vorgangs beobachten.
Auf einem durchschnittlichen 2Ghz-Rechner werden mit der 
voreingestellten <a href="syntax90.html#redundancy">Redundanz</a>
etwa 10min für eine volle einschichtige DVD benötigt.<p>

Danach finden Sie die Fehlerkorrektur-Datei <i>medium.ecc</i> im aktuellen 
Verzeichnis:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    653721680 2004-07-02 22:45 medium.ecc<br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.img<br>
</td></tr></table><p>

Die Abbild-Datei können Sie jetzt löschen, <p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>rm -f medium.img</b><br>
</td></tr></table><p>

aber die Fehlerkorrektur-Datei müssen Sie auf 
einem <a href="background60.html">zuverlässigen Speichermedium</a>
aufbewahren!<p>

<a href="syntax40.html">Überprüfen Sie danach den Datenträger regelmäßig auf Lesefehler...</a>
EOF
}


function syntax40de()
{  cat >> $1 <<EOF
<h3>Datenträger auf Lesefehler überprüfen</h3>

Um festzustellen, ob ein Datenträger Lesefehler enthält, 
legen Sie ihn in das Laufwerk und geben Sie ein:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -s</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
Dies ist freie Software; es gelten die Bedingungen der<br>
GNU GENERAL PUBLIC LICENSE aus dem Quelltext.<p>

Laufwerk: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Datenträger: DVD+R, 2224288 Sektoren, 1 Schicht(en)<p>

Untersuche Datenträger auf Lesefehler.<br>
Warte 5 Sekunden auf das Hochdrehen des Laufwerks...<br>
Sektor 57664: Medium Error; Unrecovered read error. Überspringe 15 Sektoren.<br>
Sektor 57728: Medium Error; Unrecovered read error. Überspringe 15 Sektoren.<br>
[... weitere Berichte über Lesefehler ...] <br>
Sektor 716640: Medium Error; Unrecovered read error. Überspringe 15 Sektoren.<br>
Leseposition: 100.0% ( 2.4x)<br>
33840 unlesbare Sektoren.
</td></tr></table><p>

Die Beschreibung des Lesefehlers (hier: "Medium Error; ...") kann 
je nach verwendeter Laufwerks-Firmware auch anders lauten.<p>

Falls eine passende Fehlerkorrektur-Datei für den Datenträger verfügbar ist,
geben Sie ihren Namen mit der <a href="syntax90.html#image">-e - Option</a>
zusammen mit -s an. Dies bewirkt einen zusätzlichen Vergleich des Abbildes
mit den Prüfsummen aus der Fehlerkorrektur-Datei.<p>


<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Sobald Lesefehler auf dem Datenträger auftreten, müssen Sie
schnellstmöglich versuchen, das <a href="syntax50.html">Datenträger-Abbild zu rekonstruieren</a> 
und auf einen neuen Datenträger übertragen. </td></tr></table><p>

<b>Wie oft sollte man überprüfen?</b> Das hängt von der Qualität der Rohlinge
und den Lagerungsbedingungen ab. Als Richtlinie für einmal beschreibbare CD- und DVD-Datenträger
mit unbekannter Qualität kann man nehmen:

<ol>
<li> 3 Tage nach dem Brennen (bis dahin das Abbild auf der Festplatte aufheben!), </li>
<li> 14 Tage nach dem Brennen, </li>
<li> 3 Monate nach dem Brennen, </li>
<li> dann alle weiteren 6 Monate überprüfen.</li>
</ol>

Wenn Sie viele gleichartige Rohlinge aus der Produktion eines Markenherstellers verwenden, 
reicht gegebenenfalls auch ein stichprobenhaftes Überprüfen von 2-3 Datenträgern pro Monat und Produktionsserie.<p>

<a href="syntax50.html">Abbild eines Datenträgers rekonstruieren...</a>
EOF
}


function syntax50de()
{  cat >> $1 <<EOF
<h3>Datenträger-Abbild rekonstruieren</h3>

Um die Daten eines defekten Datenträgers zu rekonstruieren,
erzeugen Sie zunächst ein Abbild der noch lesbaren Sektoren.<br>
Wenn Sie das <a href="background40.html">angepaßte Leseverfahren</a> verwenden 
und dvdisaster jetzt schon den Namen
der Fehlerkorrektur-Datei mitteilen, werden nur soviele Daten eingelesen wie zur 
Wiederherstellung notwendig sind - das spart viel Zeit!<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r --adaptive-read -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
Dies ist freie Software; es gelten die Bedingungen der<br>
GNU GENERAL PUBLIC LICENSE aus dem Quelltext.<p>

Laufwerk: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Datenträger: DVD+R, 2224288 Sektoren, 1 Schicht(en)<p>

Angepaßtes Leseverfahren: Versuche genügend Daten für die Fehlerkorrektur zu sammeln.<br>
Erzeuge neues Abbild medium.img.<br>
Reparierbar:  2.6% (korrigierbar: 0; lese in [0..2224288], Größe 2224288)<br>
Sektoren 57264-57279: Medium Error; Unrecovered read error.<br>
Auffüllen des Abbild-Bereichs [57280..1083504]<br>
[... weitere Berichte über den Lesevorgang ...]<br>
Reparierbar: 100.0% (korrigierbar: 319200; lese in [320304..327065], Größe 6762)<br>
Genügend Daten zur Wiederherstellung des Abbildes vorhanden.
</td></tr></table><p>

Danach benötigen Sie die <a href="syntax30.html">zugehörige Fehlerkorrektur-Datei</a>,
die Sie hoffentlich zu einem Zeitpunkt angelegt haben, als der Datenträger noch
lesbar war. Geben Sie ein:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -f</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
Dies ist freie Software; es gelten die Bedingungen der<br>
GNU GENERAL PUBLIC LICENSE aus dem Quelltext.<p>

Öffne medium.img: 2224288 Datenträger-Sektoren.<p>

"Fix"-Modus: Reparierbare Sektoren werden im Abbild wiederhergestellt.<br>
&nbsp;&nbsp;&nbsp;    5 reparierte Sektoren: 708225 59850 69825 79800 199500 <br>
&nbsp;&nbsp;&nbsp;    6 reparierte Sektoren: 708226 59851 69826 79801 89776 199501<br>
[... weitere Berichte über reparierte Sektoren ...] <br>
&nbsp;&nbsp;&nbsp;    5 reparierte Sektoren: 708224 59849 69824 79799 199499<br>
Reparatur-Fortschritt: 100.0% <br>
Reparierte Sektoren: 33264 <br>  
Gut! Alle Sektoren wurden repariert.<br>
Auslöschungen pro Ecc-Bereich: Mittelwert =  3.3; Schlechtester = 7.
</td></tr></table><p>

Die Meldung "Gut! Alle Sektoren wurden repariert." bedeutet, daß das Abbild
des Datenträgers ("medium.img") vollständig wiederhergestellt werden konnte. 
Das reparierte Abbild läßt sich wie ein 
normales ". i s o"-Abbild mit einer geeigneten Brennsoftware
auf einen neuen Datenträger schreiben.<p>

Wenn unkorrigierbare Sektoren übrigbleiben, lesen Sie das Abbild erneut 
ein <a href="background50.html#reading-tips">(ein paar Tips dazu)</a>.
In diesem Fall versucht dvdisaster nur die fehlenden Sektoren noch einmal zu lesen.
Dadurch erhalten Sie gegebenenfalls genügend Daten für einen zweiten erfolgreichen 
Fehlerkorrektur-Durchlauf.
EOF
}


function syntax60de()
{  cat >> $1 <<EOF
<h3>Laufwerk auswählen</h3>

Verwenden Sie die <a href="syntax90.html#device">"-d" - Option</a>,
um Abbilder von einem bestimmten Laufwerk einzulesen:<p>

<table class="example-bsd" width=100%><tr><td>
user@freebsd&gt; dvdisaster <b>-d /dev/pass0</b> -r
</td></tr></table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-d /dev/hdc</b> -r
</td></tr></table><p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-d E:</b> -r
</td></tr></table><p>

Die FreeBSD-Version greift mit Hilfe des "passthrough"-Treibers <tt>/dev/pass*</tt>
auf die Laufwerke zu.
Bei der Verwendung von Linux lauten die möglichen symbolischen Gerätenamen typischerweise
<tt>/dev/hd*</tt> für ATAPI-Laufwerke sowie <tt>/dev/scd*</tt>
für SCSI-Laufwerke. Die Windows-Version verwendet die üblichen
Laufwerksbuchstaben.

<pre> </pre>

<h3>Namen für Abbild- und Fehlerkorrektur-Datei festlegen</h3>

Sie können die Namen für die Abbild- und Fehlerkorrektur-Dateien
mit Hilfe der <a href="syntax90.html#image">"-i bzw. -e" - Optionen</a>
festlegen:<p> 

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-i abbild.img -e korrektur.ecc</b> -c
</td></tr></table><p>

Die Dateiendungen ".img" bzw. ".ecc" können auch entfallen oder
durch etwas anderes ersetzt werden. 
EOF
}


function syntax70de()
{  cat >> $1 <<EOF
<h3>Mehrere Aktionen pro Aufruf durchführen</h3>

dvdisaster erlaubt es, mehrere <a href="syntax90.html#modes">Aktionen</a>
in einem Aufruf zusammenzufassen. <br>Um beispielsweise ein Abbild von <tt>/dev/hdc</tt>
einzulesen, dazu eine Fehlerkorrektur-Datei "korrektur42.ecc" zu erzeugen und zum Schluß
die temporäre Abbild-Datei "medium.img" wieder zu löschen, geben Sie ein:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-r -c -u</b> -d /dev/hdc -e korrektur42.ecc
</td></tr></table><p>

Die Aktionen werden stets in der Reihenfolge ausgeführt, wie sie in der
<a href="syntax90.html#modes">Liste der Aufrufmöglichkeiten</a> angeordnet sind,
auch wenn sie beim Aufruf in einer anderen Abfolge angegeben wurden.
EOF
}


function syntax90de()
{  cat >> $1 <<EOF
<h3>Aufrufmöglichkeiten</h3>

<b><a name="modes">Aktionen.</a>&nbsp;</b>dvdisaster benötigt beim Aufruf die Angabe einer Aktion entweder
in Langform (--read) oder mit dem Anfangsbuchstaben in Kurzform (-r):<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#scan">--scan</a></td><td>Datenträger auf Lesefehler überprüfen</tr>
<tr valign=top><td></td><td><a href="#read">--read</a></td><td>Abbild des Datenträgers einlesen</td></tr>
<tr valign=top><td></td><td><a href="#create">--create</a> &nbsp; &nbsp;</td><td>Fehlerkorrektur-Datei erzeugen</td></tr>
<tr valign=top><td></td><td><a href="#fix">--fix</a></td><td>Versucht, ein Abbild mit Hilfe der Fehlerkorrektur-Datei zu reparieren</td></tr>
<tr valign=top><td></td><td><a href="#test">--test</a></td><td>Überprüft, ob Abbild- und  Fehlerkorrektur-Datei zusammengehören</td></tr>
<tr valign=top><td></td><td><a href="#unlink">--unlink</a></td><td>Löscht die Abbild-Datei beim Beenden des Programms</td></tr>
</table>
<pre> </pre> 

<b><a name="drive">Laufwerks- und Dateinamen.</a></b> Diese Optionen haben ebenfalls eine Langform (--device) und eine
Kurzform (-d). Sie dienen dazu, andere als die voreingestellten Laufwerke und Dateinamen zu verwenden.<p>

<table>
<tr><td>&nbsp; &nbsp;</td><td><a href="#device">--device</a>  &nbsp; &nbsp;</td><td>Laufwerk auswählen</td></tr>
<tr><td></td><td><a href="#prefix">--prefix</a></td><td>Präfix für Abbild- und Fehlerkode-Dateien</td></tr>
<tr><td></td><td><a href="#image">--image</a></td><td>Name der Abbild-Datei</td></tr>
<tr valign=top><td></td><td><a href="#eccfile">--eccfile</a></td><td>Name der Fehlerkode-Datei<p></td></tr>
<tr><td></td><td><a href="#list">--list</a></td><td>Zeigt verfügbare Laufwerke unter ASPI &nbsp; (nur in der Windows-Version)</td></tr>
</table><pre> </pre> 

<b><a name="options">Weitere Optionen.</a></b> Diese weniger häufig benutzten Optionen verwenden nur die Langform,
sofern nicht anders angegeben.<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#adaptiveread">--adaptive-read</a></td><td>Angepaßtes Leseverfahren für defekte Datenträger verwenden</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#autosuffix">--auto-suffix</a></td><td>Automatisches Anfügen der .img- und .ecc-Dateiendungen</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#cache">--cache-size</a></td><td>Zwischenspeicher-Größe während der Fehlerkode-Berechnung</td></tr>
<tr valign=top><td></td><td><a href="#dao">--dao</a></td><td>Behandelt Datenträger als "disk at once"</td></tr>
<tr valign=top><td></td><td><a href="#fillunreadable">--fill-unreadable [n]</a></td><td>fülle unlesbare Sektoren mit Byte n auf</td></tr>
<tr valign=top><td></td><td><a href="#jump">-j / --jump</a></td><td>Überspringe Sektoren nach einem Lesefehler</td></tr>
<tr valign=top><td></td><td><a href="#parse-udf">--parse-udf</a>&nbsp; &nbsp;</td><td>Informationen aus dem ISO/UDF-Dateisystem auswerten</td></tr>
<tr valign=top><td></td><td><a href="#redundancy">-n / --redundancy</a>&nbsp; &nbsp;</td><td>Redundanz des Fehlerkorrekturkodes festlegen</td></tr>
<tr valign=top><td></td><td><a href="#speedwarn">--speed-warning [n]</a>&nbsp; &nbsp;</td><td>Warnung bei Einbrüchen der Lesegeschwindigkeit</td></tr>
<tr valign=top><td></td><td><a href="#spinup">--spinup-delay [n]</a>&nbsp; &nbsp;</td><td>Gibt dem Laufwerk Zeit zum Hochdrehen</td></tr>
<tr valign=top><td></td><td><a href="#split">--split-files</a>&nbsp; &nbsp;</td><td>Teilt Dateien in Segmente <=2GB auf</td></tr>
</table>

<h3>Aktionen.</h3>

<a name="scan"><b>--scan [n-m]: Datenträger auf Lesefehler überprüfen</b></a><p>

Liest jeden Sektor des Datenträgers 
und gibt am Ende die Anzahl der nicht lesbaren Sektoren aus.<p>

Die einzulesenden Sektoren können auf einen bestimmten Bereich eingeschränkt
werden. Die Sektornumerierung beginnt bei 0; "end" bezeichnet den letzten Sektor.
Das angegebene Intervall wird inklusive der Grenzen eingelesen; 0-100 liest also 101 Sektoren ein.<p>

In der Kurzform sind zwischen dem "-s" und dem Intervall keine Leerzeichen zulässig:
<pre>
dvdisaster -s0-100            # liest Sektoren 0 bis 101
dvdisaster --scan 0-100       # Langform
dvdisaster --scan 3000-end    # liest von Sektor 3000 bis zum Datenträger-Ende

</pre>
<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="read"><b>--read [n-m]: Abbild des Datenträgers erzeugen</b></a><p>

Erzeugt ein Abbild des Datenträgers auf der Festplatte.<p>

Falls die Abbild-Datei bereits existiert, werden nur diejenigen Sektoren
eingelesen, die noch nicht in der Abbild-Datei vorhanden sind.
Dies ermöglicht es, ein Abbild durch mehrere Leseversuche zu ergänzen
und dabei verschiedene Laufwerke einzusetzen.<p>

Ohne weitere Angaben setzt --read 
das <a href="background30.html">lineare Leseverfahren</a> ein. 
Verwenden Sie zusätzlich die <a href="syntax90.html#adaptiveread">--adaptive-read</a>--Option,
um das <a href="background40.html">angepaßte Leseverfahren</a> einzusetzen.<p>

Die einzulesenden Sektoren können wie bei 
der <a href="syntax90.html#scan">--scan</a>-Option auf einen 
bestimmten Bereich eingeschränkt werden. 

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="create"><b>--create: Fehlerkorrektur-Datei erzeugen</b></a><p>

Erzeugt eine Fehlerkorrekturdatei zu einem Abbild.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="fix"><b>--fix: Abbild reparieren</b></a><p>

Versucht, ein Abbild mit Hilfe der Fehlerkorrektur-Datei zu reparieren.<p>

Falls es im ersten Anlauf nicht gelingt, alle Sektoren zu reparieren,
können Sie einen weiteren Durchlauf mit <a href="#read">--read</a> versuchen
<a href="background50.html#reading-tips">(ein paar Tips dazu)</a>.
Dabei werden gegebenenfalls genügend fehlende Sektoren eingelesen
um eine erfolgreiche Reparatur des Abbildes durchzuführen.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="test"><b>--test: Abbild- und Fehlerkorrektur-Datei überprüfen</b></a><p>

Überprüft, ob die Abbild- und  Fehlerkorrektur-Dateien zu dem gleichen Datenträger
gehören und ob ihre internen Prüfsummen korrekt sind.<p>
Siehe auch <a href="qa20.html#crc">Punkt 3.3</a> in den <a href="qa20.html">Fragen und
Antworten</a>.

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="unlink"><b>--unlink: Löscht die Abbild-Datei beim Beenden des Programms</b></a><p>

Diese Funktion löscht die Abbild-Datei, wenn alle Aktionen mit Erfolg durchgeführt
worden sind. 

<div align=right><a href="#modes">&uarr;</a></div><p>



<h3>Laufwerks- und Dateinamen.</h3>

<a name="device"><b>--device &lt;Laufwerk&gt;: Laufwerk auswählen</b></a><p>

Voreingestellt sind "/dev/cdrom" unter Linux bzw. das erste CD/DVD-Laufwerk
(nach Laufwerksbuchstabe) unter Windows.<p>

<b>FreeBSD:</b> &nbsp; dvdisaster unterstützt Laufwerke, 
die über den "passthrough"-Treiber <tt>/dev/pass*</tt> eingebunden sind
und den MMC-3 Standard implementieren.
ATAPI-Laufwerke werden möglicherweise
erst nach einem <a href="download20.html#freebsd">Neuübersetzen des Kernels</a>
verfügbar.<p>

<b>Linux:</b> &nbsp; dvdisaster unterstützt Laufwerke, die über den einheitlichen
CD-ROM-Treiber von Linux angesteuert werden und den MMC3-Standard implementieren.
Dies sind insbesondere ATAPI-Laufwerke
mit den symbolischen Geräten <tt>/dev/hd*</tt> sowie SCSI-Laufwerke mit den 
symbolischen Geräten <tt>/dev/scd*</tt>. Externe Laufwerke mit anderen Bussystemen
(z.B. USB, IEEE 1394) erscheinen typischerweise ebenfalls als SCSI-Geräte.<p>

Das Einbinden von ATAPI-Geräten über das <i>ide-scsi</i>-Modul wird nicht empfohlen; 
diese Geräte sind aber normalerweise über <tt>/dev/scd*</tt> problemlos nutzbar.<p>

Der allgemeine SCSI-Treiber <i>sg</i> (<tt>/dev/sg*</tt>) wird nicht
unterstützt. Dies sollte kein Problem darstellen, da die jeweiligen Geräte
normalerweise auch über <tt>/dev/scd*</tt> oder <tt>/dev/sr*</tt> verfügbar sind.<p>

<b>Windows:</b> &nbsp; dvdisaster unterstützt lokale Laufwerke, die den MMC3-Standard
implementieren und mit einem Laufwerksbuchstaben im System erscheinen.


<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="prefix"><b>--prefix &lt;Präfix&gt;: Präfix für Abbild- und Fehlerkorrektur-Datei</b></a><p>

Voreingestellt ist "medium"; die Abbild- und Fehlerkorrekturdateien erhalten automatisch
die Endungen ".img" bzw. ".ecc".

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="image"><b>--image &lt;Dateiname&gt;: Name der Abbild-Datei</b></a><p>

Voreingestellt ist "medium.img"; der angegebene Dateiname wird genau (ohne automatisches
Anfügen einer Endung) für die Abbild-Datei übernommen, 
sofern die <a href="#autosuffix">--auto-suffix</a>--Option nicht ebenfalls verwendet wird.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="eccfile"><b>--eccfile &lt;Dateiname&gt;: Name der Fehlerkorrektur-Datei</b></a><p>

Voreingestellt ist "medium.ecc"; der angegebene Dateiname wird genau (ohne automatisches
Anfügen einer Endung) für die Fehlerkorrektur-Datei übernommen,
sofern die <a href="#autosuffix">--auto-suffix</a>--Option nicht ebenfalls verwendet wird.


<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="list"><b>--list: Zeigt verfügbare Laufwerke unter ASPI</b></a><p>

Diese Funktion zeigt alle CD/DVD-Laufwerke an, die unter Windows durch
die <a href="qa10.html#aspi">ASPI</a>-Schicht ansprechbar sind:<p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-l</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... der Rest des GPL-Begrüßungstextes ...]</i><p>

List of ASPI CD/DVD drives:<p>
 1: (H:) ATAPI DVD+RW 8X4X12 B2K7<br>
 2: (I:) MYDRIVE CD-R MY-401610X 1.05<p>

Um die Benutzung von ASPI anstelle von SPTI zu erzwingen,<br>
verwenden Sie die obigen Laufwerksnummern,<br>
also 1:, 2:,... anstelle von C:, D:,...
</td></tr></table><p>

dvdisaster verwendet standardmäßig nur dann ASPI, wenn ein Zugriff
auf die Laufwerke über SPTI nicht möglich ist.
Die Benutzung von ASPI läßt sich durch
Angabe der Laufwerksnummer anstelle des Laufwerksbuchstabens
erzwingen. Um einen Datenträger per ASPI in dem "MYDRIVE"-Laufwerk 
des obigen Beispiels einzulesen, geben Sie also ein:<p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-d 2:</b> -r<br>
</table>

<div align=right><a href="#drive">&uarr;</a></div><p>


<h3>Weitere Optionen.</h3>

<a name="adaptiveread"><b>--adaptive-read: Angepaßtes Leseverfahren für defekte Datenträger verwenden</b></a><p>

Dieser Schalter aktiviert das <a href="background40.html">angepaßte Leseverfahren</a>,
das besonders gut für das Einlesen von beschädigten Datenträgern geeignet ist.
Verwenden Sie diesen Schalter in Verbindung mit <a href="#read">-r/--read</a> sowie mit
<a href="#eccfile">-e/--eccfile</a>, damit das Verfahren anhand der Fehlerkorrektur-Datei
entscheiden kann, wann genug Informationen für eine erfolgreiche Wiederherstellung des
Abbildes eingelesen worden sind. 
<p>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="autosuffix"><b>--auto-suffix: Automatisches Anfügen der .img- und .ecc-Dateiendungen</b></a><p>

Diese Option bewirkt, daß an die mit den <a href="#image">-i/--image</a>- bzw. <a href="#eccfile">-e/--eccfile</a>-Optionen angegebenen Dateinamen die Endungen ".img" bzw. ".ecc" angehängt werden,
sofern nicht bereits eine andere Endung vorhanden ist.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="cache"><b>--cache-size &lt;Größe in MB&gt;: Zwischenspeicher-Größe einstellen</b></a><p>

dvdisaster optimiert den Zugriff auf die Abbild- und
Fehlerkorrektur-Dateien durch einen eigenen Zwischenspeicher,
dessen Größe zwischen 1 und 2048 MB betragen kann. Voreingestellt sind 32MB,
was für die meisten Systeme passend ist. <p>

Diese Option hat nur bei der <a href="#create">--create</a>--Aktion eine Wirkung.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="dao"><b>--dao: Behandelt Datenträger als "disc at once"</b></a><p>

Datenträger, die im "TAO"-Modus ("track at once") gebrannt wurden,
können am Ende zwei Sektoren 
mit <a href="qa20.html#tao">Pseudo-Lesefehlern</a> enthalten. 
Daher ignoriert dvdisaster in der Grundeinstellung bis zu 2 Lesefehler
am Ende des Datenträgers. <p>

Wenn Sie nun das ausgesprochene Pech haben,
einen "DAO" ("disc at once") - Datenträger mit genau ein oder zwei echten
Lesefehlern am Ende zu besitzen, erreichen Sie durch Angabe
der  "--dao"-Option eine korrekte Behandlung des Datenträger-Endes.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="fillunreadable"><b>--fill-unreadable &lt;Füll-Byte&gt;: Unlesbare Sektoren mit angegebenen Byte auffüllen </b></a><p>

dvdisaster kennzeichnet unlesbare Sektoren mit einem Fülltext, dessen Vorkommen auf einem
unbeschädigten Datenträger sehr unwahrscheinlich ist.<br>
Einige andere Datenrettungsprogramme erwarten hingegen, daß unlesbare Sektoren mit einem
bestimmten Byte aufgefüllt sind. Um den Austausch von Abbildern mit anderen Programmen zu
ermöglichen, müssen Sie das "richtige" Byte angeben:<p>

<ul>
<li><b>0xb0 (dezimal 176)</b>: bewirkt Kompatibilität mit dem Programm <i>h2cdimage</i> 
des c't-Magazins.
</li>
</ul>

Hinweis: Das Auffüllen mit Nullen (0x00, dezimal 0) ist <b>nicht empfehlenswert</b>.
Die meisten Datenträger enthalten auch im unbeschädigten Zustand Sektoren, die mit Null
aufgefüllt sind. Diese Sektoren können später nicht von unlesbaren Sektoren unterschieden werden.
</li>
</ul>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="jump"><b>-j / --jump &lt;Anzahl Sektoren&gt;: Überspringe Sektoren nach einem Lesefehler </b></a><p>

Die Auswirkungen dieser Option hängen von dem verwendeten Leseverfahren ab:
<ul>
<li>bei dem <a href="background30.html#configure">linearen Lese-Verfahren</a> 
(Grundeinstellung) wird die angegebene Anzahl von nachfolgenden Sektoren 
nach einem Lesefehler übersprungen.</li>
<li>bei dem <a href="background40.html#configure">angepaßten Lese-Verfahren</a> 
(<a href="#adaptiveread">--adaptive--read</a>--Option)
wird das Einlesen abgebrochen,
sobald keine unlesbaren Bereiche mehr existieren, die länger als der angebene Wert sind.</li>
</ul> 
Größere Werte verringern die Bearbeitungszeit und die mechanische Beanspruchung des Laufwerkes,
lassen aber größere Lücken beim Einlesen defekter Bereiche.
Die Anzahl der zu überspringenden Sektoren muß ein Vielfaches von 16 sein.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="parse-udf"><b>--parse-udf: Informationen aus dem ISO/UDF-Dateisystem auswerten</b></a><p>
Aktivieren Sie diese Option, damit dvdisaster die Größenangaben für das Abbild aus dem
ISO- bzw. UDF-Dateisystem ermittelt. 
Dies ist hilfreich bei <a href="qa20.html#plusrw">Problemen mit Laufwerken</a>, 
die bei DVD-RW/+RW falsche Abbild-Größen zurückmelden.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="redundancy"><b>-n / --redundancy: Redundanz des Fehlerkorrekturcodes festlegen</b></a><p>

Die Redundanz gibt an, wieviel Prozent der 
Originaldaten <a href="background10.html">im günstigsten Fall</a>
korrigiert werden können. Daher sollten Sie die Redundanz großzügig wählen
und dabei auch beachten:<p>

<ul>
<li> Eine Fehlerkorrekturdatei mit x% Redundanz benötigt 
auch etwa x% der Größe des zugehörigen Abbilds an Speicherplatz.</li>
<li> Die Leistung der Fehlerkorrektur hängt von der Verteilung der Lesefehler ab.
Erst Änderungen der Redundanz um etwa 5 Prozentpunkte 
haben einen spürbaren Einfluß auf die Fehlerkorrektur.</li>
</ul>

Zur Einstellung der Redundanz gibt es mehrere Möglichkeiten:<p>

<ol>
<li> <b>"normal" oder "hoch"</b><p>

Geben Sie "normal" oder "high" an, um die folgenden Redundanzen zu erhalten:<p>

<table border="1" cellpadding="3">
<tr><td align=center>Wert</td><td>Redundanz</td></tr>
<tr><td><tt>-n normal</tt></td><td align=center>14.3%</td></tr>
<tr><td><tt>-n high</tt></td><td align=center>33.5%</td></tr>
</table><p>

Mit diesen beiden Einstellungen werden Fehlerkorrektur-Dateien durch 
optimierten Programmcode besonders schnell erzeugt.

Der Wert "normal" ist die Voreinstellung.<p>
</li>

<li> <b>Angabe in Prozent</b><p>

Sie können die Redundanz auch direkt in Prozent angeben:<p> 

<table border="1" cellpadding="3">
<tr><td>Beispiel</td><td>Wertebereich</td></tr>
<tr><td><tt>-n 25%</tt></td><td align=center>3.2% bis 64.5%</td></tr>
</table><p>

Aus technischen Gründen kann die wirklich verwendete Redundanz um einige
Nachkommastellen von der Vorgabe abweichen.<p></li>

<li> <b>Korrigierbare Fehler pro Fehlerkorrekturbereich</b><p>

Wenn Sie keine Maßeinheit bei der Redundanz angeben, wird der Wert
so interpretiert, daß Sie einen (255, 255-x)-Reed-Solomon-Kode
erzeugen möchten:<p>

<table border="1" cellpadding="3">
<tr><td>Beispiel</td><td>Wertebereich</td></tr>
<tr><td><tt>-n 32</tt></td><td align=center>8 bis 100</td></tr>
</table><p>

Dies führt dazu, daß jeweils 255-x Datenträger-Sektoren zu einem
Fehlerkorrekturbereich zusammengefaßt werden, innerhalb dessen bis zu
x unlesbare Sektoren korrigiert werden können.<p>

Dabei gilt der folgende Zusammenhang: &nbsp; &nbsp; Redundanz in Prozent = (100 * x) / (255 - x)<p>
</li>

<li> <b>Maximalgröße der Fehlerkorrekturdatei</b><p>

Eine weitere Möglichkeit besteht darin, die Größe der
Fehlerkorrekturdatei (in MB) vorzugeben:<p>

<table border="1" cellpadding="3">
<tr><td>Beispiel</td><td>Wertebereich</td></tr>
<tr><td><tt>-n 50m</tt></td><td align=center>3.2% bis 64.5% der Abbild-Größe</td></tr>
</table><p>

In diesem Fall wählt dvdisaster eine geeignete Redundanz, damit die
Fehlerkorrektur-Datei nicht größer als angegeben wird.<p>

Vorsicht: Wenn man unterschiedlich große Abbilder 
mit der gleichen Einstellung bearbeitet, erhalten die kleineren Abbilder 
mehr Fehlerkorrekturinformationen als die großen Abbilder.<p>

</li>
</ol>
<div align=right><a href="#options">&uarr;</a></div><p>


<a name="speedwarn"><b>--speed-warning [n]: Warnung bei Einbrüchen der Lesegeschwindigkeit</b></a><p>

Gibt eine Warnung aus, wenn sich die Lesegeschwindigkeit um mehr als <i>n</i> Prozent
verändert. Dies ist hilfreich, um beginnende Datenträgerprobleme zu erkennen. 
Wenn der optionale Wert [n] nicht angegeben wird, sind 10 Prozent voreingestellt.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="--spinup"><b>--spinup-delay [n]: Gibt dem Laufwerk Zeit zum Hochdrehen</b></a><p>

Gibt dem Laufwerk die angegebene Anzahl Sekunden Zeit zum Hochdrehen bevor
der Lesevorgang beginnt. Dadurch erscheinen keine abrupten Geschwindigkeitsänderungen
beim Beginn des Lesens und entsprechende Warnungen in Verbindung mit der
<a href="#speedwarn">--speed-warning</a>-Option werden vermieden.<br>

Für den optionalen Wert [n] sind 5 Sekunden voreingestellt.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="split"><b>--split-files: Teilt Dateien in Segmente <= 2GB auf</b></a><p>

Ermöglicht das Anlegen von Abbildern und Fehlerkorrektur-Dateien 
auf Dateisystemen, 
die nur Dateien mit einer Maximalgröße von 2GB zulassen 
(z.B. FAT unter Windows). <br>
Anstelle einer einzigen Datei "abbild.img" werden bis zu 100
Segmente "abbild00.img", "abbild01.img" usw. verwendet; dies kostet
etwas Geschwindigkeit.<p> 

Geben Sie weiterhin den Dateinamen als "abbild.img" bei der <a href="#image">--image</a>-Option an; 
die Nummern werden automatisch hinzugefügt, wenn dieser Schalter aktiv ist.<p>

<div align=right><a href="#options">&uarr;</a></div><p>

EOF
}


# ----- Questions and Answers

function qa_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Fragen und Antworten" ;;

   link)    link_title="Fragen und Antworten" ;;
   link10)   link_title="Technische Fragen" ;;
   link20)   link_title="Fehlermeldungen" ;;

   content*) eval "qa$page$lang $file" ;;
   esac 
}

function qa0de()
{  cat >> $1 <<EOF
<h3><a name="top">Allgemeine Fragen und Antworten</a></h3>

<a href="#pronounce">1.1 Wie spricht man "dvdisaster" aus?</a><p>
<a href="#pipo">1.2 Wo gibt es mehr Informationen zu PI/PO-Analysen?</a><p>
<a href="#compat">1.3 Ist dvdisaster mit nachfolgenden Versionen kompatibel?</a>

<pre> </pre><hr><pre> </pre>

<b><a name="pronounce">1.1 Wie spricht man "dvdisaster" aus?</a></b><p>
Da der Wortstamm <i>disaster</i> aus dem Englischen kommt, 
spricht man es etwa wie "diwidisaster" aus. 
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="pipo">1.2 Wo gibt es mehr Informationen zu PI/PO-Analysen?</a></b><p>
DVDs enthalten einen Fehlerkorrektur-Mechanismus, der nach einem
<a href="background10.html">ähnlichen Prinzip</a> wie dvdisaster arbeitet.
Die Fehlerkorrektur findet bei DVDs in zwei Ebenen bzw. Schritten statt, 
die als PI und PO bezeichnet werden. Erst wenn ein Fehler auf beiden
Ebenen nicht korrigierbar ist, meldet das Laufwerk unlesbare Sektoren
und es gibt Arbeit für dvdisaster (oder der Datenträger ist reif für den Müll). <p>

Für einige DVD-Laufwerke gibt es Programme, die ermitteln können, 
wie häufig die beiden Fehlerkorrektur-Ebenen beim Auslesen einer DVD beansprucht werden. 
Im Internet gibt es viele interessante Foren mit Verweisen auf diese Werkzeuge. 
Dort finden sich auch einige gut gemachte Artikel darüber, 
wie man aus den gemessenen PI/PO-Werten Rückschlüsse
auf die Qualität der betreffenden Datenträger ziehen kann. 
<div align=right><a href="#top">&uarr;</a></div><p>

<b><a name="compat">1.3 Ist dvdisaster mit nachfolgenden Versionen kompatibel?</a></b><p>
Ja, denn dvdisaster ist für eine Datenarchivierung über viele Jahre vorgesehen. 
Sie können beim Umstieg auf eine neuere Version von dvdisaster die Abbild- und
Fehlerkorrekturdateien von Vorgängerversionen weiter verwenden und brauchen
diese <i>nicht</i> neu zu erzeugen.<p>
<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa10de()
{  cat >> $1 <<EOF
<h3><a name="top">Technische Fragen</a></h3>

<a href="#nls">2.1 In welchen Übersetzungen ist das Programm verfügbar?</a><p>
<a href="#media">2.2 Welche Datenträger-Typen werden unterstützt?</a><p>
<a href="#filesystem">2.3 Welche Dateisysteme werden unterstützt?</a><p>
<a href="#aspi">2.4 Was bedeuten "SPTI" und "ASPI"?</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="nls">2.1 In welchen Übersetzungen ist das Programm verfügbar?</a></b><p>

dvdisaster beinhaltet Bildschirmtexte in den folgenden Sprachen:<p>

<table>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Deutsch</td><td>--</td><td>vollständig</td></tr>
<tr><td></td><td>Englisch</td><td>--</td><td>vollständig</td></tr>
<tr><td></td><td>Italienisch</td><td>--</td><td>vollständig</td></tr>
<tr><td></td><td>Tschechisch</td><td>--</td><td>vollständig</td></tr>
</table><p>

Übersetzer/-innen für andere Sprachen sind willkommen!<p>

dvdisaster übernimmt die Spracheinstellungen des Betriebssystems
automatisch. Falls die lokale Sprache noch nicht unterstützt wird,
werden englische Texte verwendet. Eine andere Sprache kann mit Hilfe
von Umgebungsvariablen eingestellt werden.<p>

Zum Beispiel für die bash-Kommandozeile und deutsche Sprache:

<pre>export LANG=de_DE</pre>

Wenn Umlaute nicht richtig oder als "a,"o,"u usw. dargestellt werden,
fehlt eventuell noch: <p>

<tt>export OUTPUT_CHARSET=iso-8859-1</tt> (X11, XTerm)<p>

oder<p>

<tt>export OUTPUT_CHARSET=CP850</tt> (Windows)<p>

<div align=right><a href="#top">&uarr;</a></div>



<b><a name="media">2.2 Welche Datenträger-Typen werden unterstützt?</a></b><p>

dvdisaster unterstützt einmal oder mehrmals beschreibbare CD- und DVD-Datenträger.
Datenträger mit Mehrfachsitzungen (engl.: multisession)
oder einem Kopierschutz können <i>nicht</i> verwendet werden.<p>

Unterstützte Datenträger nach Typ:<p>

<b>DVD-R, DVD+R</b><p>

<ul>
<li>Keine weiteren Einschränkungen bekannt.</li>
</ul>

<b>DVD+R9 (zweischichtig)</b>
<ul>
<li>Das Laufwerk muß die <a href="qa20.html#dvdrom">Erkennung
des Datenträger-Typs</a> ermöglichen.</li>
</ul>

<b>DVD-RW, DVD+RW</b><p>

<ul>
<li>Einige Laufwerke liefern eine <a href="qa20.html#plusrw">falsche Abbild-Größe</a>.<br>
Abhilfe: Option zum <a href="example91.html#iso">Verwenden von Informationen aus dem ISO/UDF-Dateisystem</a> setzen.
</li>
</ul>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>nur Daten-CDs werden unterstützt.</li>
</ul>

<b>Nicht verwendbare Typen</b> (kein Einlesen des Abbildes möglich):<p> 
Audio-CD und Video-CD sowie DVD-ROM und DVD-RAM.</li>

<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="filesystem">2.3 Welche Dateisysteme werden unterstützt?</a></b><p>

dvdisaster arbeitet ausschließlich auf der Ebene von Datenträger-Abbildern,
auf die es sektorweise zugreift. 
Daher ist es unerheblich, mit welchem Dateisystem der Datenträger formatiert wurde. <p>
Weil dvdisaster die Struktur der Dateisysteme nicht kennt bzw. nutzt,
kann es keine logischen Fehler darin beheben und insbesondere keine
verlorengegangenen oder gelöschten Dateien wiederherstellen.
<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="aspi">2.4 Was bedeuten "SPTI" und "ASPI"?</a></b><p>

Windows 98 und ME verwenden ASPI als Treiber, um CD/DVD-Laufwerke 
anzusprechen. Windows NT und seine Nachfolger
sind hingegen mit der Schnittstelle SPTI ausgerüstet, erlauben
jedoch auch noch die zusätzliche Installation von ASPI-Treibern.<p>

<i>&gt;&nbsp; Vor- und Nachteile von ASPI unter Windows NT/2000/XP?</i><p>

<table>
<tr valign=top><td>+</td><td>Es werden keine Systemverwalterrechte für den Zugriff
auf die Laufwerke benötigt.</td></tr>
<tr valign=top><td>-</td><td>ASPI erlaubt manchmal keine eindeutige Zuordnung von Laufwerksbuchstaben.</td></tr>
<tr valign=top><td>-</td><td>Einige Brennprogramme installieren ihre 
eigenen ASPI-Versionen.
Die zusätzliche Installation eines fremden ASPI-Treibers kann die Funktion
der vorhandenen Brennsoftware stören.</td></tr>
</table><p>

<i>&gt;&nbsp; Welche ASPI-Version wird für die Benutzung mit dvdisaster empfohlen?</i><p>

Adaptec stellt seine ASPI-Treiber zum kostenlosen Herunterladen bereit.
Unter Windows 2000/XP erkennen die Versionen 4.72.* manchmal keine
Laufwerke. In diesem Fall hilft typischerweise die Installation der
älteren Version 4.60.<p>

<i>&gt;&nbsp; Die Laufwerksbuchstaben stimmen unter ASPI nicht!</i><p>

Die ASPI-Treiber verwenden ein eigenes System von Laufwerkskennungen,
das nicht zu den Laufwerksbuchstaben von Windows kompatibel ist.<br>
Um eine einheitliche Benutzerschnittstelle für SPTI und ASPI zu bieten, 
versucht dvdisaster zu erraten, wie die Laufwerke unter ASPI zugeordnet sind.
Diese Zuordnung gelingt nicht in allen Konfigurationen.<p>

Mit Hilfe der <a href="syntax90.html#list">--list</a> -Funktion kann man 
die aktuelle Zuordnung von Laufwerksbuchstaben herausfinden.<p>

<i>&gt;&nbsp; Wie kann man zwischen SPTI und ASPI auswählen?</i><p>

In der Grundeinstellung wählt dvdisaster automatisch zwischen SPTI und
ASPI aus, wobei SPTI bevorzugt wird.
Die Benutzung von ASPI kann allerdings erzwungen werden; 
näheres dazu steht in der Beschreibung
der <a href="syntax90.html#list">--list</a> -Funktion.

<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa20de()
{  cat >> $1 <<EOF
<h3>Fehlermeldungen und Probleme</h3>

<a href="#tao">3.1 Was bedeutet "Warnung: 2 Sektoren fehlen am Ende des Datenträgers..."?</a><p>
<a href="#block">3.2 Das Programm hängt nach dem Aufruf.</a><p>
<a href="#crc">3.3 Was bedeutet die Meldung "CRC error, sector: n"?</a><p>
<a href="#plusrw">3.4 Lesefehler oder falsche Abbild-Größe bei -RW/+RW-Datenträgern</a><p>
<a href="#dvdrom">3.5 Selbstgebrannter Datenträger wird als "DVD-ROM" erkannt und abgelehnt.</a><p>
<a href="#freebsd">3.6 Unter FreeBSD erscheinen keine Laufwerke.</a><p>
<pre> </pre><hr><pre> </pre>

<b><a name="tao">3.1 Was bedeutet "Warnung: 2 Sektoren fehlen am Ende des Datenträgers..."?</a></b><p>
Diese Warnung tritt bei CD-Datenträgern auf, die im "TAO"-Modus ("track at once")
gebrannt wurden. Manche Laufwerke liefern für diese Medien eine um 2 zu große Länge
für das Abbild zurück. Dadurch entstehen 2 Pseudo-Lesefehler am Ende des Datenträgers, 
die jedoch <i>keinen</i> Datenverlust bedeuten. <p>

Da man dem Datenträger nicht ansehen kann, in welcher Betriebsart er gebrannt wurde,
geht dvdisaster davon aus, daß eine "TAO"-CD vorliegt, wenn nur die letzten beiden
Sektoren unlesbar sind, und das Abbild wird um diese beiden Sektoren verkürzt.
Ob dies zutreffend ist, müssen Sie von Fall zu Fall selbst entscheiden
und gegebenenfalls mit der <a href="syntax90.html#dao">--dao</a>-Option 
oder dem <a href="example92.html#image">Dialog zum Lesen und Prüfen</a> einen 
Hinweis geben, um diese Sektoren als echte Lesefehler zu betrachten.<p>

Wenn Sie Datenträger mit nur einer Sitzung erzeugen, sollten Sie daher immer
im Modus "DAO / Disc at once" (manchmal auch "SAO / Session at once" genannt) brennen, 
um diese Probleme zu vermeiden. 
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="block">3.2 Das Programm hängt nach dem Aufruf</a></b><p>
Unter alten Linux-Versionen (Kernel 2.4.x) bleibt das Programm 
manchmal nach dem Starten hängen, bevor es mit der ersten
Aktion beginnt. Es läßt sich dann weder mit Strg-C noch mit "kill -9"
unterbrechen.<p>

Werfen Sie den Datenträger aus, damit sich das Programm beendet.
Legen Sie das Speichermedium dann wieder ein und warten Sie, 
bis das Laufwerk den Datenträger erkennt und zur Ruhe kommt.
Ein erneuter Aufruf von dvdisaster sollte jetzt funktionieren. 
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="crc">3.3 Was bedeutet die Meldung "CRC error, sector: n"?</a></b><p>
Der betreffende Sektor konnte gelesen werden, aber die Prüfsumme seines Inhalts
stimmt nicht mit ihrem Gegenstück in der Fehlerkorrekturdatei überein. 
Dies kann mehrere Ursachen haben:<p>
<ul>
<li>Das Datenträger-Abbild wurde mit Schreibrechten in das System eingebunden
und dadurch verändert (typisches Indiz: CRC-Fehler in Sektor 64 und in den Sektoren 200 bis 400).</li>
<li>Es gibt technische Probleme mit dem verwendeten Computersystem,
insbesondere bei der Kommunikation mit den Massenspeichern.</li>
</ul>

Falls Sie technische Probleme vermuten, erstellen  
Sie versuchsweise eine zweite Version des Abbildes und der Fehlerkorrekturdatei
und <a href="example60.html">vergleichen Sie die Dateien erneut</a>. 
Wenn der Fehler verschwindet oder an einer anderen Stelle auftritt,
hat Ihr Rechner möglicherweise ein Problem mit defektem Hauptspeicher, 
fehlerhafter Laufwerks-Verkabelung oder falsch
eingestellten Taktfrequenzen.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="plusrw">3.4 Lesefehler oder falsche Abbild-Größe bei -RW/+RW-Datenträgern</a></b><p>

Einige Laufwerke liefern bei -RW/+RW-Datenträgern fehlerhafte Informationen über die
Abbild-Größe. Besonders häufig sind die folgenden beiden Fälle:<p>

<table>
<tr><td valign="top">Fehler:</td>
<td>Das Laufwerk liefert den Umfang des größten jemals auf den Datenträger geschriebenen Abbildes 
anstelle der tatsächlichen Abbild-Größe.
</td></tr>
<tr><td valign="top">Auswirkung:</td>
<td>Ein Datenträger wird gelöscht und dann mit einer 100MB großen Datei beschrieben.
Beim Zurücklesen ist das Abbild aber einige GB groß und es enthält
noch die Reste älterer Abbilder.
</td></tr>
<tr><td><pre> </pre></td><td></td></tr>
<tr><td valign="top">Fehler:</td>
<td>Das Laufwerk liefert die maximale Datenträger-Kapazität (typischerweise 2295104 Sektoren)
anstelle der tatsächlich genutzten Sektoren.
</td></tr>
<tr><td valign="top">Auswirkung:</td>
<td>Beim Einlesen des Abbilds treten ab einer bestimmten Stelle nur noch Lesefehler auf;
die Dateien auf dem Datenträger sind aber alle vollständig.
</td></tr>
</table><p>

Mögliche Abhilfe: <p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Setzen Sie die Option zum <a href="example91.html#iso">Verwenden von Informationen aus dem ISO/UDF-Dateisystem</a>, damit die Abbild-Größe aus dem ISO/UDF-Dateisystem ermittelt wird.
</td></tr></table><p>

Falls bei der Bearbeitung eines beschädigten Datenträgers 
die benötigten ISO/UDF-Sektoren auf dem Abbild unlesbar sind, haben Sie zwei Möglichkeiten:

<ul>
<li>Führen Sie die <a href="example60.html">"Vergleichen"</a>-Funktion nur mit der 
Fehlerkorrektur-Datei aus. Entnehmen Sie die korrekte Abbild-Größe der Ausgabe und 
<a href="example92.html#read">schränken Sie den Lesebereich</a> entsprechend ein.
</li>
<li>Lesen Sie einfach das Abbild mit der zu großen Länge ein. Wenn Sie nach dem Aufruf
der <a href="example50.html#repair">"Reparieren"</a>-Funktion gefragt werden, ob das Abbild
abgeschnitten werden soll, antworten Sie mit "OK".
</li>
</ul>

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="dvdrom">3.5 Selbstgebrannter Datenträger wird als "DVD-ROM" erkannt und abgelehnt.</a></b><p>

Dies kann die folgenden Ursachen haben:
 
<ol>
<li> Der "book type" des Rohlings wurde auf "DVD-ROM" gesetzt.<br>
In diesem Fall muß das Laufwerk es ermöglichen, 
den wahren Typ des eingelegten Datenträgers zu erkennen.
dvdisaster gibt dann "DVD-ROM (faked book type)" 
(DVD-ROM, vorgetäuschter book type) aus.<p>

Nicht alle Laufwerke unterstützen jedoch die Erkennung eines vorgetäuschten
book types.

<li> Einige Laufwerke betrachten DVD+R9 (zweischichtig) tatsächlich als DVD-ROM.
</li>
</ol>

Versuchen Sie in diesen Fällen, das Abbild mit einem anderen Laufwerk einzulesen.

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="freebsd">3.6 Unter FreeBSD erscheinen keine Laufwerke.</a></b><p>

<ul>
<li>Unter FreeBSD wird für ATAPI-Laufwerke (das sind fast alle heute gebräuchlichen Typen)
möglicherweise ein <a href="download20.html#freebsd">Neuübersetzen des Kernels</a>
benötigt, um die Laufwerke mit dvdisaster verwenden zu können. 
<li>Sie müssen Lese- und Schreibrechte auf dem betreffenden Gerät 
(z.B. /dev/pass0) haben.
</ul>

<div align=right><a href="#top">&uarr;</a></div>

EOF
}


# ----- Background information

function background_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Hintergrundinformationen" ;;
 
   link)    link_title="Hintergrundinformationen" ;;
   link10)   link_title="Eigenschaften der Fehlerkorrektur" ;;   
   link30)   link_title="Lineares Lese-Verfahren" ;;   
   link40)   link_title="Angepaßtes Lese-Verfahren" ;;   
   link50)   link_title="Bemerkungen zu Lesefehlern" ;;   
   link60)   link_title="Fehlerkorrektur-Datei aufbewahren" ;;

   content*) eval "background$page$lang $file"
	    return 0
	    ;;
   esac 
}

function background0de()
{  cat >> $1 <<EOF

<h3>Hintergrundinformationen</h3>

Die Informationen in diesem Abschnitt werden nicht unbedingt für die
Bedienung von dvdisaster benötigt. Sie sind aber hilfreich um zu verstehen
wie dvdisaster funktioniert und können Ihnen dabei helfen, das Programm
entsprechend Ihren Bedürfnissen anzuwenden.

<ol>
<li><a href="background10.html">Eigenschaften der Reed-Solomon-Fehlerkorrektur</a><p></li>
<li><a href="background30.html">Arbeitsweise des linearen Lese-Verfahrens</a><p></li>   
<li><a href="background40.html">Arbeitsweise des angepaßten Lese-Verfahrens</a><p></li>   
<li><a href="background50.html">Einige Bemerkungen zu Lesefehlern</a><p></li>   
<li><a href="background60.html">Tips zum Aufbewahren der Fehlerkorrektur-Datei</a><p></li>
</ol>

EOF
}


function background10de()
{  cat >> $1 <<EOF
<h3>Technische Eigenschaften der Fehlerkorrektur.</h3>

Diese Seite skizziert die Grundideen hinter dvdisaster, damit Sie abschätzen können,
ob es Ihren Anforderungen zur Datensicherung genügt. Im Zweifelsfall sollten Sie
dvdisaster nicht einsetzen oder zusätzliche Sicherungsstrategien anwenden.<p>

<b>Fehlerkorrektur-Verfahren.</b> &nbsp; dvdisaster verwendet einen 
<a href="http://en.wikipedia.org/wiki/Reed-Solomon_error_correction">Reed-Solomon</a>-Kode
mit einem auf die Behandlung von Auslöschungen optimierten Fehlerkorrektur-Algorithmus.
Die Implementierung bezieht eine Menge Inspiration und Programmcode aus der
hervorragenden <a href="http://www.ka9q.net/code/fec/">Reed-Solomon-Bibliothek</a>
von <a href="http://www.ka9q.net/">Phil Karn</a>.

<p>

In der <a href="example93.html#redundancy">Grundeinstellung</a> bilden
jeweils 223 Datenträger-Sektoren einen Fehlerkorrektur ("ECC") - Bereich.
Auf dem Datenträger auftretende Lesefehler werden als Auslöschungen betrachtet,
so daß pro ECC-Bereich bis zu 
32 defekte Sektoren<sup><a href="#footnote1">*)</a></sup> rekonstruierbar sind.<p>

Die 223 Sektoren werden so ausgewählt, daß sie sich gleichmäßig über die gesamte
Datenträger-Oberfläche verteilen. Dadurch können große zusammenhängende Bereiche von defekten Sektoren 
korrigiert werden, bevor die kritische Anzahl von 
32 Defekten<sup><a href="#footnote1">*)</a></sup> pro ECC-Bereich erreicht wird. 
Dieses Fehlermuster ist besonders typisch für alternde Datenträger, bei denen im Außenbereich gehäuft 
Fehler auftreten, und für Kratzer entlang der Datenspirale. <p>

Radiale oder diagonale Kratzer werden hingegen in der Regel schon im Laufwerk selbst korrigiert.
Für diese Fälle ist die verwendete Fehlerkorrektur weder besonders gut noch besonders schlecht geeignet.<p>

<b>Grenzen der Fehlerkorrektur.</b> &nbsp; Im schlechtesten Fall reichen schon 
33 defekte Sektoren<sup><a href="#footnote1">*)</a></sup> auf dem
Datenträger, um seine Wiederherstellung zu verhindern. Damit diese Wirkung eintritt, müssen die Fehler
wie ein Schrotschuß über den Datenträger verteilt sein und alle im gleichen ECC-Bereich liegen -
das ist eher unwahrscheinlich. <br>
Erfahrungstests haben ergeben, daß bei normaler Alterung ca. 10% an Sektoren ausfallen können,
bevor die kritsche Anzahl von 33 Defekten pro ECC-Bereich<sup><a href="#footnote1">*)</a></sup>
erreicht wird.<br>
Bei <a href="index10.html#scratches">Kratzern</a> wird die Ausfallschwelle früher erreicht; deshalb empfiehlt sich eine ständige Sichtkontrolle
und ein sofortiges Umkopieren von Datenträgern, die durch Kratzer verursachte Lesefehler aufweisen. <p>

<b>Technische Einschränkungen.</b> &nbsp; Viele Laufwerke erkennen den Datenträger nicht mehr, 
wenn die Einführungszone ("Lead in") vor dem ersten Sektor (nahe dem Innenloch) beschädigt ist.
In diesem Fall können Sie dvdisaster nicht mehr anwenden, um den Inhalt des Datenträgers zu retten. <p>

Es ist <i>nicht möglich</i>, die Qualität <b>minderwertiger Datenträger</b> durch dvdisaster aufzuwerten.
Billige Rohlinge können bereits nach wenigen Tagen so stark verfallen, daß die Kapazität des
Fehlerkorrekturkodes überschritten wird. 

<pre> </pre>
<table width="50%"><tr><td><hr></td></tr></table>

<font size="-1">
<a name="footnote1"><sup>*)</sup></a> Die angegebene Grenze 
von 32 korrigierbaren Defekten pro ECC-Bereich ist die Grundeinstellung.
Sie können hier andere Werte <a href="example93.html#redundancy">einstellen</a>
und so die Leistungfähigkeit der Fehlerkorrektur anpassen.<p>
</font>
EOF
}


function background30de()
{  cat >> $1 <<EOF
<h3><b>Das lineare Lese-Verfahren.</b></h3>
<p>

In dvdisaster sind zwei verschiedene
<a href="example92.html#read"> Leseverfahren</a> enthalten.<p>

<b>Anwendungen für das lineare Leseverfahren:</b><p>
<ul>
<li>Abbilder von unbeschädigten Datenträgern zum Erzeugen einer Fehlerkorrekturdatei einlesen</li>
<li>Lesegeschwindigkeitskurve zum Prüfen des Datenträger-Zustandes ermitteln</li>
</ul>

<b>Anwendungen für das <a href="background40.html">angepaßte Leseverfahren:</a></b><p>
<ul>
<li> Inhalt von beschädigten Datenträgern rekonstruieren
</li>
</ul>

<pre> </pre>

<b>Eigenschaften des linearen Verfahrens.</b><p>

 CD- und DVD-Datenträger 
sind in Sektoren aufgeteilt, die mit Null beginnend numeriert sind 
und jeweils 2048 Bytes an Daten enthalten.<p>

Das lineare Leseverfahren liest den Datenträger vom Anfang (Sektor 0)
bis zum Ende (letzter Sektor) ein. Die Lesegeschwindigkeit wird 
graphisch dargestellt, um die <a href="background30.html#quality">Qualität des Datenträgers</a>
abschätzen zu können:<p>

<center><img src="../images/bg-linear-de.png"><br>
Bildschirmausschnitt: lineares Leseverfahren</center>

<pre> </pre>


<a name="configure"></a>
<b>Einstellmöglichkeiten.</b><p>

<b>Anzahl der zu überspringenden Sektoren nach einem Lesefehler.</b>
Leseversuche von defekten Sektoren kosten viel Zeit und bewirken in ungünstigen Fällen
einen erhöhten Verschleiß des Laufwerks. Lesefehler treten aber typischerweise nicht einzeln,
sondern über längere Bereiche auf. Daher gibt es eine 
<a href="example92.html#read"> Einstellmöglichkeit</a> nach einem Lesefehler
eine Anzahl nachfolgender Sektoren zu überspringen. Diese Sektoren werden ohne weitere Leseversuche 
als defekt angenommen. Dies hat die folgenden Auswirkungen:<p>


<ul>
<li>Das Überspringen einer großen Anzahl von Sektoren (z.B <b>1024</b>) ergibt eine schnelle
Übersicht über die Beschädigung des Datenträgers.<br>
Es liefert aber in der Regel nicht genügend Daten für eine erfolgreiche Fehlerkorrektur.<p></li> 
<li>Kleinere Werte von <b>16, 32 oder 64</b> sind ein guter Kompromiß zwischen verringerter 
Bearbeitungszeit und Wiederherstellbarkeit des Datenträger-Abbildes.<p></li></ul>

Auf DVD-Datenträgern erstrecken sich Lesefehler aus technischen Gründen meist über
mindestens 16 Sektoren. Daher lohnt es sich für DVD nicht, 
einen Wert kleiner als 16 einzustellen.</li>
</ul>
<p>

<a name="range"></a>
<b>Einschränkung des Lesebereiches.</b>
Der Einlesevorgang kann 
<a href="example92.html#read"> auf einen Teil des Datenträgers eingeschränkt</a> werden.
Dies ist bei mehrfachen Einlese-Versuchen von beschädigten Datenträgern hilfreich.

<pre> </pre>

<a name="quality"></a>
<b>Abschätzung der Datenträger-Qualität.</b><p>

<a name="error"></a>
<b>Die Geschwindigkeitskurve.</b>
Viele Laufwerke verringern ihre Lesegeschwindigkeit in Bereichen
des Datenträgers, die sich in einem schlechten Zustand befinden:
<ul>
<li>Einbrüche in der Lesegeschwindigkeit können ein Warnzeichen für ein
baldiges Versagen des Datenträgers darstellen.</li>
<li>
Es gibt aber auch Laufwerke, die "bis zum bitteren Ende" mit voller
Geschwindigkeit lesen. Man sich also nicht darauf verlassen,
daß sich ein Versagen des Datenträgers durch Unterbrechungen in der 
Geschwindigkeitskurve ankündigt.
</li>
</ul><p>

Die Lesekurve ist bei der
<a href="example40.html"> "Prüfen"</a>-Funktion am aussagekräftigsten.
In der 
<a href="example20.html"> "Lesen"</a>-Betriebsart
werden die gelesenen Daten gleichzeitig auf der
Festplatte abgelegt, was je nach Betriebssystem und verwendeter Hardware kleine
Verzögerungen und damit Unregelmäßigkeiten in der Lesekurve bewirkt.<p>

<a name="error"></a>
<b>Lesefehler.</b>
Lesefehler werden <a href="example40.html">rot in der Spirale markiert</a> bzw. 
<a href="syntax40.html">in der Kommandozeile ausgegeben</a>.
An diesen Stellen konnte der Datenträger im momentanen Durchlauf nicht vollständig gelesen werden:
<ul>
<li>Es ist damit wahrscheinlich, daß der Datenträger defekt ist. </li>
<li>Das Abbild sollte jetzt schnellstmöglich
<a href="example50.html"> rekonstruiert</a> und auf einen neuen Datenträger geschrieben werden.</li>
</ul>
EOF
}


function background40de()
{  cat >> $1 <<EOF
<h3>Das angepaßte Lese-Verfahren</h3>
<p>

dvdisaster enthält zwei verschiedene 
<a href="example92.html#read">Leseverfahren</a>.<p>

<b>Anwendungen für das angepaßte Leseverfahren:</b><p>
<ul>
<li> Inhalt von beschädigten Datenträgern rekonstruieren
</li>
</ul>

<b>Anwendungen für das <a href="background30.html">lineare Leseverfahren:</a></b><p>
<ul>
<li>Abbilder von unbeschädigten Datenträgern zum Erzeugen einer Fehlerkorrekturdatei einlesen</li>
<li>Lesegeschwindigkeitskurve zum Prüfen des Datenträger-Zustandes ermitteln</li>
</ul>

<pre> </pre>

<b>Eigenschaften des angepaßten Verfahrens.</b><p>

Das angepaßte Verfahren setzt eine "Teile-und-Herrsche" ("divide-and-conquer") - Strategie ein,
um möglichst schnell die noch lesbaren Stellen eines beschädigten Datenträgers zu ermitteln 
und auszulesen.
Die Strategie geht auf einen Artikel von Harald Bögeholz im c't-Magazin 16/2005 
zurück, wo sie zusammen mit dem Programm <i>h2cdimage</i> veröffentlicht wurde:

<ol>
<li> 
  Zu Anfang wird der Datenträger als ein einziger noch nicht gelesenener Bereich betrachtet. 
Das Lesen beginnt mit Sektor Null.<p>
</li>
<li>
Der Lesevorgang wird solange linear fortgesetzt,
bis entweder das Ende des momentanen Bereiches erreicht ist oder ein Lesefehler auftritt.<p>
</li>
<li>
Der Lesevorgang wird entweder beendet, wenn (3a) genügend Sektoren für eine Fehlerkorrektur 
gelesen wurden oder (3b) keine unlesbaren Bereiche oberhalb einer bestimmten Größe 
mehr vorhanden sind.
<p>
</li>
<li>Anderenfalls wird der größte noch nicht gelesene Bereich auf dem Datenträger bestimmt
und in der Mitte aufgeteilt. Der Lesevorgang wird in der Mitte wie in Schritt 2 fortgesetzt.
Die erste Hälfte des aufgeteilten Bereiches verbleibt hingegen als noch nicht gelesenener Bereich
für einen späteren Durchlauf.<p>
</li>
</ol>

Das Abbruchkriterium (3a) ist besonders wirkungsvoll: Es beendet das Einlesen sofort,
wenn die absolut notwendigen Sektoren zur Wiederherstellung des Abbildes mit Hilfe 
der Fehlerkorrektur gelesen worden sind.
Dies kann die Bearbeitungszeit  gegenüber einem vollständigen Einlese-Versuch um bis zu 90% verkürzen,
erfordert aber natürlich, daß man die zugehörige Fehlerkorrektur-Datei zur Hand hat.<p>

<center><img src="../images/bg-adaptive-de.png"><br>
Bildschirmausschnitt: angepaßtes Leseverfahren</center>

<pre> </pre>

<a name="configure">
<b>Einstellmöglichkeiten</b><p>

<b>Fehlerkorrekturdatei.</b> Angepaßtes Lesen funktioniert am besten, wenn die zum Abbild gehörende
Fehlerkorrektur-Datei vorhanden ist. Das setzt natürlich voraus, daß man diese Datei
zu einem Zeitpunkt <a href="example30.html">erzeugt</a> hat, 
als der Datenträger noch vollständig lesbar war.

Um die Fehlerkorrektur-Datei zu nutzen, muß sie vor Beginn des Lesens 
<a href="example30.html">ausgewählt</a> werden.<p>

<b>Einschränkung des adaptiven Lesebereiches.</b> Der Einlesevorgang 
kann auf einen Teil des Datenträgers <a href="example92.html#read">eingeschränkt</a> werden. 

Bei der Verwendung einer Fehlerkorrektur-Datei ist das Einschränken 
des Lesebereichs nicht sinnvoll, da es gegebenenfalls das Einlesen von Sektoren
verhindert, die zur Fehlerkorrektur benötigt werden.
Ohne Fehlerkorrektur-Datei kann es hingegen bei mehrfachen Einlese-Versuchen 
von beschädigten Datenträgern hilfreich sein.<p>

<b>Lesen vorzeitig beenden.</b>Wenn keine Fehlerkorrektur-Datei vorhanden ist, wird der Lesevorgang beendet, sobald keine
unlesbaren Bereiche oberhalb 
<a href="example92.html#read">einer bestimmten Größe</a> mehr vorhanden sind.<p>

Der Wert zum Beenden sollte nicht kleiner als 128 eingestellt werden.
Anderenfalls werden in der Schlußphase des Einlesens sehr viele Neupositionierungen des
Laserschlittens im Laufwerk durchgeführt. Darunter leidet sowohl die Lebensdauer als auch die
Lesefähigkeit des Laufwerks. Günstiger ist es typischerweise, früher mit den adaptiven
Lesen aufzuhören und die letzten Sektoren mit dem <a href="background30.html">linearen Leseverfahren</a>
zu vervollständigen.
EOF
}


function background50de()
{  cat >> $1 <<EOF
<h3>Ein paar Hintergründe zu Lesefehlern</h3>

DVDs verfügen über einen eigenen Fehlerkorrektur-Kode, der kleinere
Material- und Brennfehler ausgleicht und so die gespeicherten Daten
schützt. Wenn der Brenner und die Rohlinge kompatibel und 
von hoher Qualität sind, ist die eingebaute Fehlerkorrektur 
direkt nach dem Brennen nur schwach ausgelastet. Sie verfügt dann
über genügend Kapazität, um die während der Benutzung des Datenträgers
auftretenden Verschleiß- und Alterungserscheinungen für viele Jahre
auszugleichen.<p>

Erst wenn die Reserven der eingebauten Fehlerkorrektur erschöpft sind,
entstehen Lesefehler, die Sie in dvdisaster mit 
der <a href="example40.html">"Prüfen"</a>-Funktion feststellen können.
Dabei ist die folgende Unterscheidung wichtig:<p>


<b>Lesefehler direkt nach dem Brennen.</b> <br>Wenn unlesbare Sektoren
direkt nach dem Brennen auftreten, ist das ein Hinweis auf

<ul>
<li>Produktionsfehler bei den Rohlingen, oder</li>
<li>Rohlinge, die nicht mit dem Brenner kompatibel sind</li>
</ul>

In diesem Fall hilft nur ein Entsorgen der defekten Rohlinge und ein
erneutes Brennen auf einwandfreie Ware, gegebenenfalls verbunden mit
einem Herstellerwechsel.<p>

Der Versuch, derartige Fehlbrände mit Hilfe einer Fehlerkorrekturdatei
am Leben zu erhalten, endet hingegen ziemlich sicher mit einem Datenverlust.<p>

<b>Lesefehler nach einigen Monaten/Jahren.</b> Die eingebaute Fehlerkorrektur des
Datenträgers wird mit zunehmender Lebensdauer immer stärker belastet, 
bis schließlich Lesefehler entstehen.
Dies hat sowohl mechanische Ursachen (Kratzer, Verziehen des Materials) als
auch chemische Hintergründe (Zerfall des Farbstoffes und/oder der Spiegelschicht).<p>

Typischerweise treten diese Effekte auf, während der Datenträger für einige 
Monate gelagert wird, und es ist danach auch mit den unten beschriebenen Tips
nicht mehr möglich, den Datenträger wieder komplett einzulesen. <p>

Deshalb ist es wichtig, rechtzeitig mit dvdisaster die zugehörigen
<a href="example30.html">Fehlerkorrektur-Dateien</a> zu erzeugen, weil
dadurch innerhalb <a href="background10.html">bestimmter Grenzen</a> 
der Inhalt von Sektoren berechnet (= wiederhergestellt) werden kann,
die von keinem Laufwerk mehr gelesen werden können.<p>

Dabei braucht man den Datenträger typischerweise nicht bis auf den letzten
lesbaren Sektor "auszuquetschen": 
Das <a href="background40.html">angepaßte Leseverfahren</a>
von dvdisaster überprüft beim Lesen ständig, ob genügend Daten für
die Fehlerkorrektur zur Verfügung stehen. Sobald dies der Fall ist,
wird der Lesevorgang beendet und die bis dahin noch nicht gelesenen
Sektoren werden aus der Fehlerkorrektur-Datei wiederhergestellt.<p>

<a name="reading-tips"><b>Einige Tips zum effektiven Auslesen beschädigter Datenträger</b></a><p>

Die "Ausbeute" beim Lesen beschädigter Datenträger hängt von mehreren Umständen ab:

<ul>
<li><b>Nicht alle Laufwerke sind gleich gebaut.</b><br>
Verschiedene Laufwerke haben auch verschieden gute Lesefähigkeiten. 
Nutzen Sie die Möglichkeit von dvdisaster, ein Abbild mit mehreren Lesevorgängen
zu vervollständigen, und setzen Sie dabei verschiedene Laufwerke ein. 
Übertragen Sie die Abbild-Datei mit Hilfe eines Netzwerks oder mit RW-Datenträgern,
um Laufwerke in verschiedenen Rechnern einsetzen zu können.<p></li>
<li><b>Auswerfen und wieder einlegen.</b><br>
Ab und zu hilft es, den Datenträger auszuwerfen, 
um ein Viertel zu drehen und dann einen neuen Leseversuch zu starten.<p></li>
<li><b>Kalte Laufwerke lesen besser.</b><br>
Einige Laufwerke haben im kalten Zustand bessere Leseeigenschaften.
Schalten Sie den Rechner über Nacht aus und versuchen Sie es am nächsten Morgen noch mal.<p>
"Kalt" heißt übrigens Raumtemperatur - Ein Aufenthalt im Kühlschrank ist für 
Hardware und Datenträger ziemlich ungesund.<p></li>
</ul>
EOF
}


function background60de()
{  cat >> $1 <<EOF
<h3>Tips zum Aufbewahren der Fehlerkorrektur-Datei</h3>

Hier sind ein paar Anregungen zum Aufbewahren der Fehlerkorrektur-Dateien:<p>

<b>1. Fehlerkorrektur-Dateien auf eigenen Datenträgern sammeln:</b><p>

Mit der voreingestellten <a href="example93.html#redundancy">Redundanz</a>
passen typischerweise Fehlerkorrektur-Dateien für 7-10 DVDs auf eine leere DVD. 
So kann man zum Beispiel 9+1 DVDs (9 mit Nutzdaten und 1 mit Fehlerkorrektur)
gut als 10er-Paket lagern. Allerdings:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Vermeiden Sie es, die Fehlerkorrektur-Dateien auf einen Rohling zu brennen,
der aus der gleichen Produktion wie derjenige stammt, von dem Sie das Abbild erzeugt haben.
</td></tr></table><p>

Rohling aus der gleichen Produktion altern auch gleich schnell, d.h. sie
versagen häufig zum gleichen Zeitpunkt. Damit wäre nicht sichergestellt, 
daß die Fehlerkorrektur-Dateien die anderen Datenträger überdauern.<p>

Da auch Rohlinge mit unterschiedlichem Herstelleraufdruck aus der gleichen
Produktionslinie stammen können, empfiehlt es sich, unterschiedliche Rohlingstypen
zu verwenden:

DVD-R und DVD+R - Rohlinge sowie solche aus unterschiedlichen 
Geschwindigkeitsklassen (z.B. 8x und 16x-Rohlinge) wurden
mit hoher Wahrscheinlichkeit auf verschiedenen Produktionslinien
und zu verschiedenen Zeitpunkten gefertigt.<p>

<b>2. Fehlerkorrektur-Dateien jeweils auf dem nächsten Datenträger speichern:</b><p>

Wenn Sie Ihre DVDs nicht randvoll mit Nutzdaten (also mit weniger als 4GB) beschreiben,
können Sie die Fehlerkorrektur-Dateien innerhalb einer Serie von DVDs jeweils
auf dem nächsten Datenträger ablegen.<p>

<b>3. Fehlerkorrektur-Dateien auf (externen) Festplatten speichern:</b><p>

Eine Festplatte mit 400GB reicht für rund 600 Fehlerkorrektur-Dateien
(mit der voreingestellten <a href="example93.html#redundancy">Redundanz</a> bei 
jeweils vollen 4.7GB DVDs). Das kann eine preisliche Alternative sein,
wenn man sich bewußt ist, daß aktuelle ATA-Platten auch nicht für die Ewigkeit
gebaut sind ;-)

EOF
}


# ----- Impressum

function imprint_contents_de()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Impressum" ;;
 
   link)    link_title="Impressum" ;;

   content*) eval "imprint$page$lang $file"
	    return 0
	    ;;
   esac 
}

function imprint0de()
{  cat >> $1 <<EOF

   <h3>Impressum</h3>

   Carsten Gnörlich <br>
   Im Hagen 10 <br>
   59514 Welver / Deutschland<br>
   Tel.: +49 2384 941174<pre> </pre>
   E-Mail: carsten@dvdisaster.de<br>
   Internet: www.dvdisaster.de

   <pre> </pre>

   Inhaltlich Verantwortlicher gemäß § 10 MDStV: <p>
   Carsten Gnörlich (Anschrift wie oben).

   <pre> </pre>

   Das Impressum gilt für die Internetpräsenz unter den Adressen<br>
   <a href="http://www.dvdisaster.com">www.dvdisaster.com</a>, &nbsp;
   <a href="http://www.dvdisaster.de">www.dvdisaster.de</a>, &nbsp;
   <a href="http://www.dvdisaster.org">www.dvdisaster.org</a><br>
   und unter der Unterdomäne <a href="http://dvdisaster.berlios.de">dvdisaster.berlios.de</a>.


EOF
}

