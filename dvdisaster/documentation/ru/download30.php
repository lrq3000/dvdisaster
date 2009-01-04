<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2009 Igor Gorbounov
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

<h3>Установка двоичных версий (только Windows)</h3>

Для установки Windows-версии, запускайте на выполнение двоичный файл
(например,  <?php echo ${pkgname}?>-setup.exe) и следуйте диалогу в процессе установки. <p>

<b>Предупреждение:</b> dvdisaster НЕ устанавливается путем ручной
распаковки программы setup.exe или путем копирования его из уже
существующего установленного варианта. Это может привести к появлению
странных ошибок, которые будут казаться совсем не связанными с неправильной установкой.

<h3>Установка из исходных текстов</h3>

dvdisaster использует обычный механизм: <tt>./configure; make; make install</tt>,- 
для установки из исходных текстов. Более подробно смотрите в файле <tt>INSTALL</tt> в
дистрибутиве исходных текстов.

<!--- do not change below --->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
