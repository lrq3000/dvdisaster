<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2008 Carsten Gnörlich
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

if($news_flash == 0) 
{  require("../include/dvdisaster.php");
   begin_page();
}

$news_counter = 0;

# The news page is different from the other pages;
# you must only use the news_*() functions below to create content.
# Do not insert plain HTML!

news_headline("Neues über dvdisaster");

news_item("29.09.2008", "Gut Ding will Weile haben :-)", "

Bis zum ersten Veröffentlichungskandidaten der 0.72er-Reihe müssen noch
umfangreiche interne Tests durchlaufen werden, da zwischen der Version
0.71.26 und der kommenden Version 0.71.27 noch eine große interne Änderung
stattfindet. Der Wechsel von 0.70.* auf 0.72 wird damit der längste
und umfangreichste in der bisherigen Projektgeschichte.<p>
In der Zwischenzeit gibt es schon mal eine Ergänzung der deutschen Dokumentation;
diese ist jetzt bis auf ein Unterkapitel fertig. Danach wird die englische
Übersetzung beginnen.
");

news_item("30.03.2008", "dvdisaster 0.71.26 veröffentlicht", "
Diese Version enthält einen neuen Dialog mit Informationen über den
eingelegten Datenträger (Menü Werkzeuge/Datenträger-Info); 
dazu wurde die Erkennung des Datenträger-Typs (CD/DVD/BD) verändert. 
Falls dadurch vorher unterstützte Laufwerke nicht mehr funktionieren, bitte
eine Rückmeldung geben. Die Unterstützung von BD-Datenträgern wurde verbessert.
");

news_item("05.03.2008", "Problem mit vorheriger Windows-Version behoben (0.70.6 / 0.71.25)", "
  Die Unterstützung für Dateinamen mit Umlauten wurde aus Version 0.70.6
  wieder herausgenommen weil dadurch Dateien &gt; 2GB
  unter Windows nicht mehr verarbeitet werden konnten.
  Eine korrekte Behandlung von Dateinamen mit Sonderzeichen wird jetzt
  erst in der experimentellen Version 0.71.25 erprobt.
");

news_item("03.03.2008", "Ups - Abbilder >2GB erzeugen Fehler in 0.70.5 and 0.71.24 unter Windows", "
   Die Behandlung von Umlauten in Dateinamen hat dazu geführt, daß unter Windows
   keine Abbilder >2GB mehr verarbeitet werden können. Betroffen sind nur die gerade
   veröffentlichten Versionen 0.70.5 and 0.71.24. Verbesserte Versionen kommen 
   in den nächsten Tagen.
");

news_item("24.02.2008", "dvdisaster 0.70.5 / 0.71.24 beheben Problem mit neuen Linux-Versionen", "
   Die neuen Versionen beheben ein Problem mit neueren Linux-Kernen, das unter
   bestimmten Umständen ein Einfrieren des Systems bewirken kann. Bitte 
   aktualisieren Sie dvdisaster, wenn Sie Kernel 2.6.17 oder einen neueren
   einsetzen.
   Möglicherweise sind auch noch ältere Kernel-Versionen betroffen.

   Mit dieser Version beginnt außerdem die Überarbeitung
   der Online-Dokumentation, einschließlich einer russischen Übersetzung von Igor 
   Gorbounov. 
"); # end of news_item

news_item("28.10.2007", "Überarbeitung der Dokumentation", "
   Die Dokumentation wird momentan für die bevorstehende Veröffentlichung von
   Version 0.72 überarbeitet. Bitte haben Sie ein wenig Geduld; die neue Dokumentation
   wird hoffentlich hilfreicher als die alte sein, aber es wird ein paar Wochen dauern
   bis alle Lücken wieder gefüllt sind.
"); # end of news_item

if($news_flash == 0) 
   end_page();
?>
