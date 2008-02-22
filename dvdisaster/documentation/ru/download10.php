<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007 Igor Gorbounov
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
begin_page();
?>

<!--- Insert actual page content below --->

<h3>Системные требования</h3>

<ul>
 <li>Процессоры: x86, PowerPC или Sparc;<p></li>
 <li>со скоростью обработки равной или лучше, чем скорость P4 при 2ГГц;<p></li>
 <li>современный привод CD/DVD/HD DVD/BD с интерфейсом ATAPI или SCSI;<p></li>
 <li>достаточное пространство на жестком диске для создания .iso-образов обрабатываемых носителей.<p>
</ul>

<h3>Операционные системы</h3>
<ul>
 <li><a name="#freebsd"></a><b>FreeBSD</b> версия <b>6.0</b> или новее<br>
     (для использования ATAPI-приводов требуется перекомпиляция ядра -- см. файл INSTALL)<p>
 </li>
 <li><b>Linux</b> с ядром <b>2.6.7</b> или новее,<br>
     (использование ядер >= 2.4.20 возможно, но не рекомендуется!)<p>
 </li>
 <li><b>Mac OS X</b> версия 10.4 (Tiger) или новее,<br> 
      на платформах x86 и PowerPC.<p>
 <li><b>NetBSD</b> версия 3.1 или новее.<p></li> 
 <li><b>Windows</b> <b>2000</b> или <b>XP</b><br>
     Windows 98 и ME возможно использовать, но не рекомендуется для обработки DVD
     и более крупных носителей.<br>
     Работа под Windows Vista (R) возможна, но по большей части не протестирована.
 </li>
</ul>

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
