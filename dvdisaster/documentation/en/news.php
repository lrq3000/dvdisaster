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

news_item("30.03.2008", "dvdisaster 0.71.26 released", "
  This version contains a new dialog providing information about inserted 
  media (menu Tools/Medium info);
  during this course detection of media types (CD/DVD/BD) was changed. 
  Please report if this breaks compatibility with formerly working drives.
  Improved support for BD media.
");

news_item("05.03.2008", "Problem with previous Windows release fixed (0.70.6 / 0.71.25)", "
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

if($news_flash == 0) 
   end_page();
?>
