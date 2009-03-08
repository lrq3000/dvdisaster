<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2009 Igor Gorbounov
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

news_headline("News archive from 2007");

news_item("24.02.2008", "dvdisaster 0.70.5 / 0.71.24 - исправление проблем с новыми версиями Linux", "
   Исправлена проблема для новых ядер Linux, которая могла вызвать зависание системы
   в некоторых случаях. Обновите программу на системах 
   с ядрами 2.6.17 и выше; может быть, более ранние ядра тоже подвержены этой проблеме.<p> 

   В версии dvdisaster 0.71.24 начато переписывание 
   онлайновой документации, включая перевод на русский язык, сделанный
   Игорем Горбуновым.
"); # end of news_item

if($news_flash == 0) 
   end_page();
?>
