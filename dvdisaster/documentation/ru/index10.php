<?php
# dvdisaster: Russian homepage translation
# Copyright (C) 2007-2011 Igor Gorbounov
#
# UTF-8 trigger: äöüß 
#
# Include our PHP sub routines, then call begin_page()
# to start the HTML page, insert the header, 
# navigation and news if appropriate.

require("../include/dvdisaster.php");
require("../include/screenshot.php");
begin_page();
?>

<!-- Insert actual page content below -->

<h3 class="top">Примеры исправления ошибок</h3>

<?php begin_screen_shot("Чтение поврежденного носителя.","recover-linear.png"); ?>
   <b>Восстановление старого носителя.</b> Обработанный здесь носитель стал
   обесцвеченным и частично нечитаемым во внешней области. Проверка поверхности
   дает примерно 23.000 нечитаемых секторов из общего числа 342.000 секторов; отсюда
   поврежденных секторов около 7,2%.
<?php end_screen_shot(); ?>

<?php begin_screen_shot("Чтение поврежденного образа.","fix-image.png"); ?>
   <b>Восстановление поврежденного образа.</b> Полученный образ все еще неполный,
   поскольку примерно 23.000 секторов не могут быть прочитаны. Эти сектора теперь 
   реконструируются с помощью 
   данных для исправления ошибок,
   созданных программой dvdisaster.
   В процессе восстановления встретилось максимум 20 ошибок
   на ECC блок. Это приводит к пиковой
   загрузке алгоритма коррекции ошибок в 63%, что означает, что с повреждением такой степени хорошо справляются
   данные для исправления ошибок, созданные с помощью стандартных настроек.
<?php end_screen_shot(); ?>

<b>Для восстановления требуются данные для исправления ошибок:</b> В описанном выше 
процессе восстановления используются
данные для исправления ошибок. 
Представьте себе эти данные как особую
форму резервных данных (хотя для них требуется меньше места, чем для обычного резервирования).
Как и в случае обычного резервного копирования, ecc-данные необходимо создать
<i>прежде</i>, чем носитель станет поврежденным.<p>

Поэтому, если у вас есть поврежденный носитель, но вы для него не создавали ecc-данных - увы, 
ваши данные, вероятно, потеряны.<p>


<a href="index20.php">Почему проверки качества носителя недостаточно...</a>

<!-- do not change below -->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
