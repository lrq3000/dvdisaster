<?php
# dvdisaster: English homepage translation
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

news_headline("dvdisaster News");

news_item("24.02.2008", "dvdisaster 0.70.5 / 0.71.24 fix problems with newer Linux versions", "
   A problem with newer Linux kernels was fixed which would lead
   to a frozen system under some circumstances. Please upgrade on systems 
   running kernels 2.6.17 and above; maybe earlier kernels are also affected.<p> 

   The release of dvdisaster 0.71.24 also marks the start of
   an online documentation rewrite, including a Russian translation made
   by Igor Gorbounov.

   <i>Currently, the english documentation is far from being complete.
   Please bear with us; we'll catch up soon.</i>

"); # end of news_item

news_item("28.10.2007", "New documentation started", "
   The dvdisaster documentation is currently being reworked for the upcoming
   V0.72 release. Please be patient; the new documentation will hopefully be more
   useful than the old one, but we will need a few weeks to fill in all parts.
"); # end of news_item

if($news_flash == 0) 
   end_page();
?>