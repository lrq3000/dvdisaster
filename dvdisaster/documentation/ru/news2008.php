<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2011 Igor Gorbounov
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

news_headline("News archive from 2008");

news_item("30.03.2008", "выпущен dvdisaster 0.71.26", "
  В этой версии содержится новый диалог с информацией о вставленном 
  носителе (меню Инструменты/Информация о носителе);
  кроме того, изменено распознавание типов носителей (CD/DVD/BD). 
  Сообщите, если это нарушает совместимость с ранее работавшими приводами.
  Улучшена поддержка BD-носителей.
", 5, "2008-03-30T00:00:00Z", "2008-03-30T00:00:00Z");

news_item("05.03.2008", "Исправлена проблема с предыдущей версией для Windows (0.70.6 / 0.71.25)", "
  Сделан откат назад поддержки локализованных имен файлов в версии 0.70.6,
  поскольку она нарушила поддержку больших файлов под Windows. Новый обработчик
  локализованных имен файлов будет сначала тестироваться в экспериментальной версии
  0.71.25.
", 4, "2008-03-05T00:00:00Z", "2008-03-05T00:00:00Z");

news_item("03.03.2008", "Ой! - образы &gt;2ГБ не работают в 0.70.5 и 0.71.24 под Windows", "
   Исправление для локализованных имен файлов вызвало проблемы при обработке
   образов &gt; 2GB под Windows в только что выпущенных версиях 0.70.5 и 0.71.24.
   Пожалуйста, следите за выходом исправленных версий.
", 3, "2008-03-03T00:00:00Z", "2008-03-03T00:00:00Z");

news_item("24.02.2008", "dvdisaster 0.70.5 / 0.71.24 - исправление проблем с новыми версиями Linux", "
   Исправлена проблема для новых ядер Linux, которая могла вызвать зависание системы
   в некоторых случаях. Обновите программу на системах 
   с ядрами 2.6.17 и выше; может быть, более ранние ядра тоже подвержены этой проблеме.<p> 

   В версии dvdisaster 0.71.24 начато переписывание 
   онлайновой документации, включая перевод на русский язык, сделанный
   Игорем Горбуновым.
", 2, "2008-02-24T00:00:00Z", "2008-02-24T00:00:00Z");

if($news_flash == 0) 
   end_page();
?>
