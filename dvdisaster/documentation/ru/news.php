<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2008 Igor Gorbounov
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

news_item("05.03.2008", "Problem with previous Windows release fixed  (0.70.6 / 0.71.25)", "
  Rolled back support for localized file names in version 0.70.6
  as it broke large file support under Windows. A new handler
  for localized file names will now be tested in the experimental version
  0.71.25 first.
");

news_item("03.03.2008", "Oops - images &gt;2GB fail in 0.70.5 and 0.71.24 under Windows", "
   The fix for localized file names caused problems when processing
   images &gt; 2GB under Windows in the just released versions 0.70.5 and 0.71.24.
   Please stay tuned for fixed versions.
");

news_item("24.02.2008", "dvdisaster 0.70.5 / 0.71.24 fix problems with newer Linux versions", "
   A problem with newer Linux kernels was fixed which would lead
   to a frozen system under some circumstances. Please upgrade on systems 
   running kernels 2.6.17 and above; maybe earlier kernels are also affected.<p> 

   The release of dvdisaster 0.71.24 also marks the start of
   an online documentation rewrite, including a Russian translation made
   by Igor Gorbounov.
"); # end of news_item

news_item("28.10.2007", "Начата новая документация", "
   В настоящее время документация на dvdisaster перерабатывается для предстоящего
   выхода V0.72. Наберитесь терпения; возможно, новая документация будет более
   полезной, чем старая, но нам требуется несколько недель, чтобы все охватить.
"); # end of news_item

if($news_flash == 0) 
   end_page();
?>
