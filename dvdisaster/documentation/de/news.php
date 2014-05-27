<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2014 Carsten Gnörlich
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

news_item("28.05.2014", "dvdisaster 0.72.6 veröffentlicht", "
Kleine Fehlerkorrektur im Quellkode, um die Größe zukünftiger
optischer Speichermedien korrekt zu behandeln. Für momentan
verfügbare Speichermedien ist die Aktualisierung nicht notwendig.
", 23, "2014-05-28T01:00:00Z", "2014-05-28T01:00:00Z");


news_item("11.10.2013", "dvdisaster 0.72.5 veröffentlicht", "
Kleine Fehlerkorrektur für GNU/Linux und FreeBSD für den Fall
daß mehr als 9 Laufwerke vorhanden sind.
", 22, "2013-10-12T01:00:00Z", "2013-10-12T01:00:00Z");

news_item("07.04.2012", "dvdisaster 0.72.4 veröffentlicht", "
Anpassung an aktuelle Versionen von GNU/Linux, FreeBSD und NetBSD.
", 21, "2012-04-07T01:00:00Z", "2012-04-07T01:00:00Z");

news_item("05.10.2011", "dvdisaster 0.72.3 veröffentlicht", "
Die \"Vergleichen\"-Funktion hing bei der Verarbeitung von 
RS01-Fehlerkorrektur-Dateien, die größer als 2GB sind (die
Fehlerkorrektur-Dateien selbst werden korrekt erzeugt).
Volodymyr Bychkoviak fand das Problem und schickte eine
Korrektur.
", 20, "2011-10-05T01:00:00Z", "2011-10-05T01:00:00Z");

news_item("28.07.2011", "Dokumentation ergänzt", "
Die zwischenzeitlich verlorengegangenen <a href=\"qa30.php\">Hintergrundinformationen</a> aus der Dokumentation von Version 0.70 wurden überarbeitet und wieder
online gestellt.
", 19, "2011-07-28T00:00:00Z", "2011-07-28T00:00:00Z");

news_finalize();

if($news_flash == 0) 
   end_page();
?>
