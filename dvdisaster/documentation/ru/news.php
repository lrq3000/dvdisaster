<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007 Igor Gorbounov
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

news_headline("Новости dvdisaster");

/*
news_item("xx.xx.2007", "Выпущен dvdisaster 0.71.24", "
   Только что вышел dvdisaster 0.71.24. Начиная с этой версии переписывается
   интерактивная документация, включая русский перевод, сделанный
   Игорем Горбуновым. Новшеством также является редактор необработанных секторов CD и некоторые улучшения
   в эвристике низкоуровневого чтения. 
"); # end of news_item
*/

news_item("28.10.2007", "Начата новая документация", "
   В настоящее время документация на dvdisaster перерабатывается для предстоящего
   выхода V0.72. Наберитесь терпения; возможно, новая документация будет более
   полезной, чем старая, но нам требуется несколько недель, чтобы все охватить.
"); # end of news_item

if($news_flash == 0) 
   end_page();
?>
