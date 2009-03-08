<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2009 Carsten Gnörlich
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

news_item("08.03.2009", "dvdisaster 0.71.28 veröffentlicht", "
Diese Version behebt Abstürze durch unzulässige Befehle 
(\"illegal Instruction\") auf x86-Maschinen die SSE2 nicht unterstützen.
Sie enthält weitere kleinere Änderungen auf dem Weg zum 
Veröffentlichungskandidaten für den stabilen Zweig.
");


news_item("18.01.2009", "dvdisaster 0.71.27 veröffentlicht", "
Die Dokumentation ist wieder komplett. Einige Crashes und
Inkompatibilitäten mit ungewöhnlichen Laufwerken und Datenträgern wurden behoben.
Diese Version enthält die letzte große interne Änderung, die für die 0.71.x-Serie
geplant war. Wenn alles gut geht wird die nächste Version ein 
Veröffentlichungskandidat für den stabilen Zweig (0.72).
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

if($news_flash == 0) 
   end_page();
?>
