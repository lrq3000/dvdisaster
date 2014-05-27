<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2014 Igor Gorbounov
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

news_item("28.05.2014", "dvdisaster 0.72.6 released", "
Minor fix in the source code to make sure that the size
of future optical media is treated correctly. For
currently available media sizes the update is not
required.
", 23, "2014-05-28T01:00:00Z", "2014-05-28T01:00:00Z");

news_item("11.10.2013", "dvdisaster 0.72.5 released", "
Minor bug fix for GNU/Linux and FreeBSD for systems with more
than 9 drives attached.
", 22, "2013-10-12T01:00:00Z", "2013-10-12T01:00:00Z");

news_item("07.04.2012", "dvdisaster 0.72.4 released", "
Updated to work with recent versions of GNU/Linux, FreeBSD and NetBSD.
", 21, "2012-04-07T01:00:00Z", "2012-04-07T01:00:00Z");

news_item("05.10.2011", "dvdisaster 0.72.3 released", "
The \"Verify\" function hangs when working on RS01 error
correction files which are larger than 2GB (the error correction
files are correctly generated though).
Volodymyr Bychkoviak discovered the problem and sent in
a bug fix.", 20, "2011-10-05T00:00:00Z", "2011-10-05T00:00:00Z");

news_item("28.07.2011", "Documentation added", "
The temporarily lost <a href=\"qa30.php\">background information</a> 
from the 0.70 documentation has been updated and made online again.
", 19, "2011-07-28T00:00:00Z", "2011-07-28T00:00:00Z");

news_finalize();

if($news_flash == 0) 
   end_page();
?>
