<?php
# dvdisaster: English homepage translation
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

news_headline("dvdisaster News");

news_item("11.04.2009", "dvdisaster 0.72.rc1 released", "
  The first release candidate for the stable 0.72 branch is available now.
  Among the new features are Blu-Ray media support, raw reading and C2 scans
  for CD and a native version for Mac OS X.
  See the <a href=\"download.php\">download page</a> for more information.
");

news_item("08.03.2009", "dvdisaster 0.71.28 released", "
  This version fixes \"illegal instruction\" crashes on x86 machines
  which do not support SSE2. More minor fixes towards the release
  candidate.
");

news_item("18.01.2009", "dvdisaster 0.71.27 released", "
  The documentation is finally complete. Some crashes and incompatibilities
  with uncommon media and drives have been fixed. This version includes
  the last major internal overhaul planned for the 0.71.x series, so if
  all goes well the next version will be a stable release candidate.
");

if($news_flash == 0) 
   end_page();
?>
