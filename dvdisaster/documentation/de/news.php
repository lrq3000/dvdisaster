<?php
# dvdisaster: German homepage translation
# Copyright (C) 2004-2007 Carsten Gnörlich
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

/*
news_item("xx.xx.2007", "dvdisaster 0.71.24 veröffentlicht", "
   dvdisaster 0.71.24 wurde veröffentlicht. Mit dieser Version beginnt die Überarbeitung
   der Online-Dokumentation, einschließlich einer russischen Übersetzung von Igor 
   Gorbounov. Ebenfalls neu sind der Rohdaten-Editor für CD-Sektoren sowie einige
   Verbesserungen in den Heuristiken zum Lesen von Roh-Sektoren.
"); # end of news_item
*/

news_item("28.10.2007", "Überarbeitung der Dokumentation", "
   Die Dokumentation wird momentan für die bevorstehende Veröffentlichung von
   Version 0.72 überarbeitet. Bitte haben Sie ein wenig Geduld; die neue Dokumentation
   wird hoffentlich hilfreicher als die alte sein, aber es wird ein paar Wochen dauern
   bis alle Lücken wieder gefüllt sind.
"); # end of news_item

if($news_flash == 0) 
   end_page();
?>
