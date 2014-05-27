<?php
# dvdisaster: English homepage translation
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

news_headline("Archiv novinek za rok 2010");

news_item("21.11.2010", "Vydán dvdisaster 0.79.3", "V GNU/Linux je pro přístup k optickým mechanikám jako výchozí použit ovladač SG_IO. Díky tomu byl vyřešen problém se zamrzáním systémů s paralelními SCSI adaptéry, který byl způsobován ovladačem CDROM_SEND_PACKET. RS03 kodek nyní obsahuje Altivec optimalizace pro platformu PowerPC.", 18, "2010-11-21T00:00:00Z", "2010-11-21T00:00:00Z");

news_item("07.11.2010", "Přidána Windows a Mac OS X verze pro 0.72.2", "Jsou k dispozici Windows a Mac OS X verze 0.72.2. Byly sestaveny pomocí aktualizovaného vývojového prostředí pro 0.79.x. Program je tak dodáván s aktualizovanými verzemi GTK+ knihoven a může tak mít odlišný vzhled a chování.", 17, "2010-11-07T00:00:00Z", "2010-11-07T00:00:00Z");

news_item("31.10.2010", "Vydán dvdisaster 0.72.2", "Tato verze přináší alternativní řešení chyby kvůli které v Linuxu docházelo k zamrzání paralelních SCSI adaptérů. Vylepšena kompatibilita s 0.79.x.", 16, "2010-10-31T00:00:00Z", "2010-10-31T00:00:00Z");

news_item("28.02.2010", "Vydán dvdisaster 0.79.2", "Opět jsou k dispozici binární soubory pro Mac OS X protože vývojové prostředí pro Mac OS X bylo pro tuto verzi aktualizováno. Vývoj RS03 kodeku pokračuje, stále má ale daleko k dokončení.", 15, "2010-02-28T00:00:00Z", "2010-02-28T00:00:00Z");

news_item("07.02.2010", "Spuštěna nová vývojová větev 0.79", "Byla vydána první verze nové vývojové větve (0.79.1). Tato verze je určena k testování nových funkcí; nelze ji doporučit k běžnému použití. Pro informace o změnách a účasti na testování si prohlédněte <a href=\"download40.php\">stránku ke stažení</a>.", 14, "2010-02-07T00:00:00Z", "2010-02-07T00:00:00Z");

if($news_flash == 0) 
   end_page();
?>
