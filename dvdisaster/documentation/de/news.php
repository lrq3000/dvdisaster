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

news_item("11.04.2009", "dvdisaster 0.72.rc1 veröffentlicht", "
Der erste Veröffentlichungskandidat für den stabilen 0.72er Zweig 
steht nun bereit. Unter anderem gibt es Unterstützung für Blu-Ray-Datenträger,
\"Raw\"-Lesen und C2-Überprüfungen für CD sowie erstmals eine \"native\"
Version für Mac OS X. Auf der <a href=\"download.php\">Herunterladen</a>-Seite
finden Sie mehr Informationen.
");

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

if($news_flash == 0) 
   end_page();
?>
