<?php
# dvdisaster: English homepage translation
# Copyright (C) 2004-2011 Carsten Gnörlich
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

<h3 class="top">Результаты для файлов с данными для исправления ошибок</h3>

<?php begin_howto_shot("Образ и файл для исправления ошибок.","compat-okay-rs01.png", ""); ?>

Результаты сравнение образа с файлом для исправления ошибок выводится
в двух полях; каждое поле относится к одному из файлов:
<?php end_howto_shot(); ?>

<table>
<tr><td colspan="2">Выходное поле <b>"Отчет по файлу образа":</b><br><hr></td><td></td></tr>
<tr>
<td class="valignt">Секторов на носителе:</td>
<td>Число секторов в ISO-образе (один сектор = 2КБ).</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>
<tr>
<td class="valignt">Ошибки контрольной суммы:</td>
<td>В файле для исправления ошибок содержатся контрольные суммы CRC32 для каждого сектора образа. Если это значение больше нуля, то некоторые секторы читаются,
но у их содержимого не совпадает контрольная сумма. Алгоритм исправления ошибок попробует
пересчитать содержимое этих секторов.
</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Недостающие секторы:</td>
<td>Это число секторов, которые не удается прочитать с носителя.
Алгоритм исправления ошибок попробует восстановить содержимое этих секторов.
</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Контрольная сумма образа:</td>
<td>Для завершенного ISO-образа рассчитывается контрольная сумма MD5.
Это значение можно воспроизвести, воспользовавшись командной строкой GNU/Linux:<br>
<tt>md5sum medium2.iso</tt></td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr><td colspan="2">
Если все значения в этом выходном поле в порядке, появляется сообщение
"<span class="green">Хороший образ.</span>".
В противном случае здесь будет объяснена наиболее важная ошибка.
</td>
</tr>

<tr><td>&nbsp;</td><td></td></tr>
<tr>
<td colspan="2">Выходное поле <b>"Отчет по файлу для исправления ошибок"</b>:<br><hr></td><td></td>
</tr>
<tr>
<td class="valignt">Создан:</td>
<td>Выводится версия dvdisaster'а, использовавшегося для создания
данных для исправления ошибок. Красным выделяются альфа-версии и версии для разработчиков.</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Метод:</td>
<td>Метод и избыточность, использовавшиеся для создания файла для исправления ошибок.</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Требуется:</td>
<td>Для обработки данных для исправления ошибок требуется версия dvdisaster'а, не ниже указанной.</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Секторов на носителе:</td>
<td>Предполагаемое число секторов в файле образа.</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Контрольная сумма образа:</td>
<td>Предполагаемая MD5-сумма для файла образа.</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Отпечаток:</td>
<td>dvdisaster использует контрольную сумму особого сектора для определения, был ли
этот файл для исправления ошибок создан для данного образа.</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Ecc-блоки:</td>
<td>Алгоритм исправления ошибок делит образ на небольшие блоки, которые можно
обрабатывать независимо. Эта информация по большей части бесполезна, пока
число ecc-блоков правильное ;-) 
</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr>
<td class="valignt">Контрольная сумма ecc:</td>
<td>Контрольная сумма MD5 рассчитывается по файлу для исправления ошибок,
не принимая во внимание первые 4КБ. Это значение можно воспроизвести с помощью
командной строки GNU/Linux:<br>
<tt>tail -c +4097 medium.ecc | md5sum</tt>
</td>
</tr>
<tr><td>&nbsp;</td><td></td></tr>

<tr><td colspan="2">
Если все значения в этом выходном поле в порядке, тогда появляется сообщение
"<span class="green">Хороший файл для исправления ошибок.</span>".
В противном случае здесь будет объяснена наиболее важная ошибка.
</td>
</tr>
</table>

<!-- do not change below -->

<?php
# end_page() adds the footer line and closes the HTML properly.

end_page();
?>
