#! /bin/bash

#  dvdisaster: English homepage translation
#  Copyright (C) 2004-2006 Carsten Gn�rlich
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA,
#  or direct your browser at http://www.gnu.org.

# ----- Translations for phrases used in create.bash

function dictionary_en()
{  trans_encoding="ISO-8859-1"

   trans_version="Version"

   trans_to_internet="To the Internet version"
   trans_to_hoster="Summary (at BerliOS)"

   trans_back="Back"

   trans_contents="Contents"
   trans_hosting="Hosted by"

   trans_fdl="Verbatim copying and distribution of this entire article is permitted in any medium, provided this notice is preserved."
   trans_copyright="Copyright 2004-2006 Carsten Gn�rlich."
}


# ----- These are the "inlined" html pages.

function inline_contents_en()
{  title="Image page"
}

function inline1en()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Picture: Badly scratched medium</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-img.jpg"></center>
<br clear="all">
</td></tr>
<tr><td>
This medium has been carried around without protection in a backpack;
the round prints were made by a bottle of water. The scratches are
a bit exaggerated due to reflections from the photoflash.</tr></td>
EOF
}


function inline2en()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Figures: Error analysis</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-scan-en.png"></center>
<br clear="all">
</td></tr>
<tr><td>
When reading or scanning media, 
dvdisaster visualizes the reading speed (blue curve) and the number of read errors. 
The spiral gives an estimate of the error distribution between the inner and outer
areas of the medium. However the spiral is not drawn to scale
since the exact angle between two segments does not resemble the situation on the real
medium for technical reasons.

<pre>

</pre>
</tr></td>
<tr align="center"><td>
<center><img src="../images/scratch-corr-en.png"></center>
<br clear="all">
</td></tr>
<tr><td>
This chart shows how many errors have been corrected per error correction block, giving
an estimate of the working load of dvdisasters <a href="background10.html">error correction</a>.

The error graph does <i>not</i> correlate with the spatial arrangement of 
read errors on the disc since the error correction method is explicitly built to 
achieve an equal distribution of 
medium read errors over all error correction blocks.<p>

Upto 32 read errors can be compensated per error correction block (green line)
when the <a href="example93.html#redundancy">standard settings</a> are used.
Exceeding this line means that the medium contents can not be fully recovered.

</tr></td>
EOF
}

# ----- Individual page contents 

# ----- Default home page

function index_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Overview" ;;

   link)    link_title="Overview" ;;
   link0)   link_title="Overview" ;;
   link10)   link_title="Examples of the error correction" ;;
   link20)   link_title="dvdisaster complements PI/PO scans" ;;
   link30)   link_title="Pro and con of dvdisaster" ;;

   content*) eval "index$page$lang $file" ;;
   esac 
}


function index0en()
{  cat >> $1 <<EOF
<h3>The dvdisaster project:</h3>

dvdisaster provides a margin of safety against <b>data loss</b> on 
<a href="qa10.html#media">CD</a> and  <a href="qa10.html#media">DVD</a> media caused 
by <b>aging</b> or <b>scratches</b>.  

<ul>
<li>dvdisaster creates <b>error correction codes</b> to compensate
read errors which are not correctable in the CD/DVD drive.<p></li>
<li>dvdisaster tries to read as much data as possible from defective media.
Afterwards unreadable sectors are recovered using the previously created
error correction code. The maximum error correction capacity is user-selectable.
</li>
</ul>

If you create the error correction code file in time and keep it at a safe place,
you have a good chance of recovering the medium contents from typical read errors
and to transfer your complete data onto a new medium.

<p>
<a href="index10.html">Examples of the error correction...</a>
EOF
}


function index10en()
{
   create_inline inline en 1 index10.html "to the examples of error correction"
   create_inline inline en 2 index10.html "to the examples of error correction"

   cat >> $1 <<EOF
<h3>Examples of error correction</h3>

<table><tr><td valign="top">
<b><a name="scratches">Badly scratched medium.</a></b> &nbsp;

This medium has been carried around without protection in a backpack;
the round prints were made by a bottle of water. The scratches are
a bit exaggerated due to reflections from the photoflash.<p>

A surface scan of the medium yields about 135.000 unreadable sectors
(of 2.200.000 total, giving about 6% defective sectors).
During the <a href="example50.html">recovery</a> of the medium image
(using <a href="example93.html#redundancy">standard settings</a>)
a maximum of 22 errors per <a href="background10.html">ECC block</a> is counted
which equals to a 69% load of the error correction under the applied settings.<p>


</td><td width=110>
<a href="inline1.html">
<img src="../images/scratch-img.jpg" width=100 height=97></a>
<br clear="all">
<a href="inline2.html">
<img src="../images/scratch-scan-en.png" width=100 height=74></a>
<br clear="all">
<a href="inline2.html">
<img src="../images/scratch-corr-en.png" width=100 height=74></a>
</td></tr></table>

<a href="index20.html">Why PI/PO scans won't suffice...</a>
EOF
}


function index20en()
{  cat >> $1 <<EOF
<h3>dvdisaster as a complement to PI/PO scans</h3>

PI/PO scans are a valueable tool to assess and optimize
the quality of burned media.<p>

However, PI/PO scans are <b>not</b> a reliable means of <b>predicting 
the durability</b> of DVD media!

<ul>
<li> PI/PO scans yield no information about how many more scratches
or weeks of aging the medium can sustain before exhibiting the first
unreadable sector.<p></li>

<li>Copying media just because of bad PI/PO values
often turns out as being too premature - sometimes such media remain readable
much longer than expected.<p></li>

<li>But without using additional tools like dvdisaster,
one can not defer the medium backup until the first read errors occur -
that would definitely lead to data loss.</li>
</ul>

<a href="index30.html">Pro and con of dvdisaster at a glance...</a>
EOF
}


function index30en()
{  cat >> $1 <<EOF
<h3>Advantages of using dvdisaster:</h3>

<ul>
<li><b>Protects</b> against unintentional medium damage (within <a href="background10.html">certain limits</a>).<p></li>
<li>Read error tests run <b>faster</b> than PI/PO scans;<br>
up to 16x speed depending on the DVD drive.<p></li>
<li><b>Cost-effective:</b> Media must be replaced with a new copy 
only when they are really defective.</li>
</ul>

<h3>Limitations of using dvdisaster:</h3>
<ul>
<li>Error correction file <b>must be created before the medium fails</b>.<p></li>
<li>Error correction files require <b>additional storage space</b> and
must be stored on reliable media.
Using the <a href="example93.html#redundancy">standard settings</a> the additional
storage space amounts to 15% of the original data size
(approx. 700MB for a full 4.7GB DVD).<p></li>
<li><a href="background10.html">no guaranteed protection</a> against data loss.</li>
</ul>

See also the collection of <a href="background.html">background information</a>
to learn more about the functioning of dvdisaster.
EOF
}

# ----- Examples

function example_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Examples" ;;

   link)    link_title="Examples and screen shots" ;;
   link10)   link_title="Invoking the graphical user interface" ;;
   link20)   link_title="Creating a medium image" ;;
   link30)   link_title="Generating error correction files" ;;
   link40)   link_title="Scanning a medium for errors" ;;
   link50)   link_title="Recovering a medium image" ;;
   link60)   link_title="Compare image against error correction data" ;;

   link90)   link_title="Configuration" ;;
   link91)   link_title="General" ;;
   link92)   link_title="Reading" ;;
   link93)   link_title="Error correction" ;;

   content*) eval "example$page$lang $file" ;;
   esac 
}


function example0en()
{  cat >> $1 <<EOF
<h3>Examples</h3>
The following pages contain a guided tour through dvdisaster:

<ol>
<li><a href="example10.html">Invoking the graphical user interface</a><p></li>
<li><a href="example20.html">Creating the medium image</a><p></li>
<li><a href="example30.html">Generating the error correction file</a><p></li>
<li><a href="example40.html">Scanning the medium for errors</a><p></li>
<li><a href="example50.html">Recovering the medium image</a><p></li>
<li><a href="example60.html">Comparing the image against error correction data</a><p></li>
</ol>
<pre> </pre>

Additional topics: 

<ul>
<li><a href="example90.html">Configuration</a><p></li>

<ul>
<li><a href="example91.html">General settings</a><p></li>
<li><a href="example92.html">Reading preferences</a><p></li>
<li><a href="example93.html">Error correction settings</a><p></li>
</ul>

<li><a href="background60.html">Hints for storing the error correction files</a></li>
</ul>
EOF
}

function example10en()
{  
cat >> $1 <<EOF
<h3>Invoking the graphical user interface</h3>

dvdisaster will open the graphical user interface automatically<br>
when you click at the dvdisaster program icon from your windowing system.<p>

After a short loading time the initial dvdisaster window will appear:<p>

<img src="../images/ex-start-en.png" alt="Start screen"><p>

<b>General hints:</b>

<ul>
<li>The visual appearance of window elements depends on the operating system
and the personal settings for the windowing system.<p></li>
<li>dvdisaster does also provide a <a href="syntax10.html">command line mode</a>.<p></li>
<li>When dvdisaster is invoked in the graphical mode, a configuration file
<i>.dvdisaster</i> will be created for storing the selections made in the
<a href="example90.html">preferences dialog</a>. When this file exists,
command line parameters cease to have an effect in the graphical mode.<p>

</ul>

<b>Hints for the Windows version:</b>

<ul>
<li>Under Windows 2000 and XP either <b>administrator priviledges</b> or an installed
<a href="qa10.html#aspi">ASPI</a> driver are required in order to access the CD/DVD drives.<p></li>
<li>For technical reasons there are two program versions for Windows:
<table>
<tr><td><i>dvdisaster<b>-win</b>.exe</i></td><td>--</td><td>contains the graphical user interface</td></tr>
<tr><td><i>dvdisaster.exe</i></td><td>--</td><td>provides the command line interface</td></tr>
</table>
</li>
</ul>

<a href="example20.html">Creating the medium image...</a>
EOF
}


function example20en()
{  cat >> $1 <<EOF
<h3>Creating the medium image</h3>

Most actions of dvdisaster are performed on a medium image residing on your hard drive.
This makes things faster and reduces wear of your CD/DVD drive.<p>

Please follow these steps to create a medium image (see also the respective
markings in the screen shot):<p>

<b>1. Select the drive  <font color="red">(1)</font> 
where the CD or DVD has been inserted.</b><p>

The Linux version currently only provides those drives for selection
which are contained in the /dev directory. A more flexible selection
scheme will be introduced in a later program version.<p>


<b>2. Select a name for the image file.</b><p>

Click on the
<img src="../images/open-img.png" alt="Image file selection" align="middle"> symbol 
<font color="red">(2a)</font>
to bring up a window for selecting the image file name.<br>
As a shortcut the file name can also be entered directly into 
the text field <font color="red">(2b)</font>.<p>

Make sure to have enough free disk space for the image. Activate the
<a href="example91.html#localfiles">split file option</a> if your file system 
does not support files larger than 2GB.<p>

<b>3. Read the medium image into the file.</b><p>

Click on the "Read" button <font color="red">(3)</font>
to start the reading process.<p>

<hr>
Screen shot: Reading the image
<hr>
<img src="../images/ex-read-en.png" alt="Reading the image"><p>
<hr>


The reading progress will be shown in the graphical representation.<p>

Clicking on the
<img src="../images/btn-protocol-en.png" alt="Protokoll" align="middle"> symbol
provides additional information on the reading process.<p>

<a href="example30.html">Generating the error correction file...</a>
EOF
}


function example30en()
{  cat >> $1 <<EOF
<h3>Generating the error correction file</h3>

The error correction file has an important role in dvdisaster:<br>
It is required to recover unreadable medium sectors.<p>

When you have created the
<a href="example20.html">medium image</a> on your hard disk, 
perform the following actions:<p>

<b>1. Choose the image file <font color="red">(1)</font>.</b><p>

The previously read image file will be pre-selected.<p>

<b>2. Select a name for the error correction file.</b><p>

Click on the
<img src="../images/open-ecc.png" alt="Error correction file selection" align="middle"> symbol 
<font color="red">(2a)</font>
to bring up a window for selecting the error correction file name.<br>
As a shortcut the file name can also be entered directly into 
the text field <font color="red">(2b)</font>.<p>
<p>

<b>3. Create the error correction file.</b><p>

Click on the "Create" button <font color="red">(3)</font> 
to generate the error correction file.<p>

<hr>
Screen shot: Creating the error correction file
<hr>
<img src="../images/ex-create-en.png" alt="Creating the error correction file"><p>
<hr>


The progress will be shown using a percentage reading. 
Processing a single layered DVD image with the 
default <a href="example93.html#redundancy">redundancy</a>
takes about 10 minutes on an average 2Ghz system.<p>

<b>Please note</b>:

<ul>
<li>The error correction file can not be created if the image is incomplete or
containing read errors.<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Generate the error correction file immediately after writing the medium.
</td></tr></table>
</li>

<li>The image file is no longer
required when the error correction file has been successfully created.<br>
dvdisaster will automatically overwrite an existing image file if a different
medium is read in.<p></li>

<li>
Always keep the error correction file on a 
<a href="background60.html">reliable storage medium</a>!
</li>
</ul>

<a href="example40.html">From now on you should regularly scan the medium for read errors...</a>
EOF
}


function example40en()
{  cat >> $1 <<EOF
<h3>Scanning the medium for errors</h3>

To check your medium for read errors insert it into your drive and carry out
these steps:<p>

<b>1. Select the drive <font color="red">(1)</font>
where the CD or DVD has been inserted.</b><p>

The Linux version currently only provides those drives for selection
which are contained in the /dev directory. A more flexible selection
scheme will be introduced in the next program version.<p>

<b>2. Start the medium scan.</b><p>

Click on the "Scan" button <font color="red">(2)</font>
to begin with the medium check.<p>

<hr>
Screen shot: Medium scan
<hr>
<img src="../images/ex-scan-en.png" alt="Medium scan"><p>
<hr>

The graphical representation gives information on the scanning progress and media state:<p>

<ul>
<li>The <b>Speed chart</b> provides a coarse estimation of the medium state since most drives
will slow down as the medium quality degrades.<p>

To exclude any influences from other system activities you should not run any other programs
during the medium scan. Also, do not operate any controls in the dvdisaster window during the scan.<p></li>

<li>The <b>spiral</b> provides a (not drawn to scale) 
overview of the <b>medium state</b>.
Red marks indicate the presence of read errors!
<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
As soon as you notice read errors on your medium, try
<a href="example50.html">recovering the medium image</a> 
and transfer it onto a new storage medium. 
</td></tr></table><p>
</li>

<li>Clicking on the
<img src="../images/btn-protocol-en.png" alt="Protocol" align="middle"> symbol
provides additional information on the reading process.<p></li>
</ul>

<b>How frequently should the media be scanned?</b><br>
This depends on your media quality and storage conditions.
As a guide line for CD- and DVD media of unknown quality:

<ol>
<li> 3 days after burning (keep the media images on hard disc until this test passes!), </li>
<li> 14 days after burning, </li>
<li> 3 months after burning, </li>
<li> then one more scan after each 6 months.</li>
</ol>

If you're using lots of media from the same production of a high quality manufacturer,
it may be sufficient to scan a sample of 2-3 media per month and batch.<p>

<a href="example50.html">Recovering a medium image...</a>
EOF
}

function example50en()
{  cat >> $1 <<EOF
<h3>Recovering a medium image</h3>

To recover data from damaged media you need 
the <a href="example30.html">appropriate error correction file</a>
which you have hopefully generated at a time when the medium was still fully readable.
Perform the following actions:<p>

<b>1. Switch the reading strategy.</b><p>

Go to the Read & Scan preferences dialog and select the
<a href="example92.html#read">adaptive reading strategy</a>.<p>

<b>2. Try to read in as much data as possible into an image file.</b><p>

Select the drive, image file and error correction file <font color="red">(2a)</font>.<br>
The error correction file must be available at this stage so that dvdisaster can determine
when enough data for reconstructing the image has been collected.<br>
Start the reading process by clicking at the "Read" button <font color="red">(2b)</font>.<p>

Please note that reading a damaged medium may take several hours to complete.<p>

<hr>
Screen shot: Reading the image
<hr>
<img src="../images/ex-read-a-en.png" alt="Reading the image"><p>
<hr>

<pre> </pre>

<a name="repair"></a>
<b>3. Perform the image reconstruction.</b><p>

Click on the "Fix" button.
It is normal to experience phases of high hard drive
activity during image reconstruction.<p>

<hr>
Screen shot: Repairing the image
<hr>
<img src="../images/ex-fix-en.png" alt="Repairing the image"><p>
<hr>

The repairing progress will be shown in percent.<p>

<b>Additional hints:</b><p>

<ul>
<li>The message "Good! All sectors are repaired." confirms that the medium image
has been fully recovered. The repaired image can be handled like an ordinary ". i s o" image.
It can be written to a new
medium using a suitable CD/DVD recording software. <p></li>

<li>If uncorrectable sectors remain after this stage, try reading the image again
<a href="background50.html#reading-tips">(some hints for re-reading)</a>.
dvdisaster will try to re-read only the missing sectors, and this might
retrieve enough missing sectors for a successful error correction run.<p></li>

<li>The red error graph does <i>not</i> correlate with the spatial arrangement of 
read errors on the disc: The error correction method is explicitly built to achieve 
an equal distribution of medium read errors over all error correction blocks.<p>
The green line marks the maximum capacity of the error correction. If this line is exceeded,
the image can not be recovered with the currently available data.
In case of tight misses <a href="background50.html#reading-tips">another reading attempt</a>
 might help.
</li>

<li>When the image has been read in by the adaptive reading strategy,
the error correction load will always be maxed out.</li>
</ul>
EOF
}


function example60en()
{  cat >> $1 <<EOF
<h3>Comparing the image against error correction data</h3>

This function provides information about image and error correction files
which are already present on your hard disc:<p>

<b>1. Select the files to compare <font color="red">(1)</font>.</b><p>

Enter the names of an image file and its corresponding error correction file
(as described in steps 2a/2b for creating the <a href="example20.html">image</a> and <a href="example30.html">error correction</a> files).<p>

<b>2. Invoke the comparison <font color="red">(2)</font>.</b><p>

Click the "Compare" button. This process may take a while as both
files need to be read completely (but no changes are applied to them).
<p>

<hr>
Screen shot: Comparing image and error correction files
<hr>
<img src="../images/ex-compare-en.png" alt="Datentr�ger-Inhalt rekonstruieren"><p>
<hr>

When the comparison is finished, you get the following information:<p>

<b>"Image file summary" and "Image state"</b>. 
The number of missing sectors will be above zero if the image contains
still uncorrected read errors. In contrast the appearance of checksum errors 
is usually the result of
<a href="qa20.html#crc">incorrect handling or hardware problems</a>.<p>

<b>"Error correction file summary"</b>. 
This field shows the settings used while creating the error correction file,
and whether it can be processed by your version of dvdisaster.
If the error correction file does not correspond to the image file
or if it is damaged, warnings are printed beneath the last four entries.<p>

<b>Additional hints:</b>

<ul>
<li>The results of the compare are unreliable if the image has been read in
by a software other than dvdisaster. Especially, all missing sectors
will be classfied as checksum errors if a different software has been used.
</li>
</ul>
EOF
}


function example90en()
{  cat >> $1 <<EOF

<h3>Configuration</h3>

Press the
<img src="../images/open-preferences.png" align="middle" alt="preferences button"> button
at the upper right of the tool bar to open the configuration dialog.
The dialog contains several forms:<p>

<center>
<a href="example91.html">
<img src="../images/prefs-general-en.png"><br>
General settings
</a>
</center>
<br clear="all">

<center>
<a href="example92.html">
<img src="../images/prefs-read-en.png"><br>
Reading preferences
</a>
</center>
<br clear="all">

<center>
<a href="example93.html">
<img src="../images/prefs-ecc-en.png"><br>
Error correction settings
</a>
</center>

EOF
}

function example91en()
{  cat >> $1 <<EOF

<h3>General Configuration</h3>

More information on this form follows down this page.<p>

<center><img src="../images/prefs-general-en.png"></center>
<br clear="all">

<p><hr><p>

<a name="iso"></a>
<b>Medium and Image filesystem</b><p>

<center><img src="../images/prefs-general-1-en.png"></center>
<br clear="all">

dvdisaster will determine the image size from the ISO/UDF file system
if this option is checked. This works around some problems
caused by drives
<a href="qa20.html#plusrw">reporting incorrect image lengths</a>
with DVD-RW/+RW media.

<p><hr><p>

<a name="localfiles"></a>
<b>Setting for local files</b><p>
<center><img src="../images/prefs-general-2-en.png"></center>
<br clear="all">
 
<b>Automatic file name extension<font color="red">(1)</font>:</b>
When this switch is set, files will be automatically appended
with ".img" or ".ecc" suffixes if no other file name extension is already present.<p>

<b>Split file into segments <font color="red">(2)</font>:</b>
Allows working with file systems which are limited to 2GB per file
(e.g. FAT from Windows). Created files are spread over upto 100 segments called 
"medium00.img", "medium01.img" etc. at the cost of a small performance hit.<p>

<p><hr><p>

<a name="auto"> </a>
<b>Automatic file creation and deletion</b><p>

<center><img src="../images/prefs-general-3-en.png"></center>
<br clear="all">

<b>Create error correction file <font color="red">(1)</font>:</b>
Automatically creates an error correction file after reading in an image.
Together with the "Remove image" option this will speed up
error correction file generation for a series of different media.<p>

<b>Remove image <font color="red">(2)</font>:</b> If this switch is set, 
the image file will be deleted  following the successful
generation of the respective error correction file.
EOF
}

function example92en()
{  cat >> $1 <<EOF

<h3>Reading preferences</h3>

More information on this form follows down this page.<p>

<center><img src="../images/prefs-read-en.png"></center>
<br clear="all">

<p><hr><p>

<a name="read"></a>
<b>Reading preferences</b><p>

<center><img src="../images/prefs-read-1-en.png"></center>
<br clear="all">

<b>Reading strategy <font color="red">(1)</font>:</b> Choose between the
<a href="background30.html">linear reading strategy</a> and the
<a href="background40.html">adaptive reading strategy</a>.
The linear strategy is suited for undamaged media while the adaptive
strategy is better for media already containing read errors.<p>

<b>Reading range <font color="red">(2)</font>:</b> Reading will be limited to the given range 
of sectors, including the borders: 0-100 will read 101 sectors. The settings are
only effective for the current session and will not be saved.<p>

<b> Skip x sectors after read error / <br>
Stop reading when unreadable intervals < x <font color="red">(3)</font>:</b><br>
The slider value affects the handling of read errors. It has slightly different
effects for the <a href="background30.html#configure">linear</a> and 
<a href="background40.html#configure">adaptive</a> reading strategies.
However large values reduce the processing time and the mechanical wear
on the drive, but will also leave larger gaps in the image when reading defective areas.

<p><hr><p>

<a name="image"></a>
<b>Image properties</b><p>

<center><img src="../images/prefs-read-2-en.png"></center>
<br clear="all">

<b>DAO image <font color="red">(1)</font>:</b>
Works around a <a href="qa20.html#tao">rare Problem</a> when creating images
from "DAO" ("disc at once") CD-R media.<p>

<b>Fill unreadable sectors <font color="red">(2)</font>:</b> 
Provides compatibility with other data recovery software. 
Currently known settings and programs are:<p>
<i>0xb0 (decimal 176)</i> -- h2cdimage/dares (published by "c't", a German periodical)<p>
Uncheck (disable) this field when images are exclusivley processed with dvdisaster.

<p><hr><p>

<b>Drive initialisation</b><p>

<center><img src="../images/prefs-read-3-en.png"></center>
<br clear="all">
Waits the given amount of seconds for the drive to spin up before the real
reading process starts. This avoids speed jumps at the beginning of the
reading curve.
EOF
}

function example93en()
{  cat >> $1 <<EOF

<h3>Error correction settings</h3>

More information on this form follows down this page.<p>

<center><img src="../images/prefs-ecc-en.png"></center>
<br clear="all">

<p><hr><p>

<a name="redundancy"></a>
<b>Redundancy for new error correction files</b><p>

<center><img src="../images/prefs-ecc-1-en.png"></center>
<br clear="all">

 The redundancy specifies the amount of correctable errors
<a href="background10.html">in the best case</a>. Since the ideal case
is rare, it is recommended to apply a reasonable margin when selecting the redundancy:<p>

<font color="red">(1)</font> / <font color="red">(2)</font>
The presets <b>normal</b> and <b>high</b> provide a redundancy 
of 14.3% and 33.5%, respectively. They invoke optimized program code to speed up the
error correction file creation.<p>

<font color="red">(3)</font> Specify the redundancy <b>by percent</b>. Please note:

<ul>
<li>An error correction file with  x% redundancy will be approximately x% of the size
of the corresponding image file.</li>
<li>The error correction capability depends on the statistical distribution of read errors. 
Only changes by 5 percentage points and more may 
have a recognizable effect on the error correction.<p></li>
</ul>

<font color="red">(4)</font> Give the <b>maximum size</b> of the error correction file in MB.
dvdisaster will choose a suitable redundancy setting so that
the overall size of the error correction file does not exceed the
given limit. <p>

Advance notice: When using the same size setting for images of vastly different size,
more error correction information is allotted to the smaller images
and less to the larger ones.<p>

<p><hr><p>

<b>Memory utilization</b><p>

<center><img src="../images/prefs-ecc-2-en.png"></center>
<br clear="all">

dvdisaster optimizes access to the image and error correction files by maintaining 
its own cache. The preset of 32MB is suitable for most systems.<p>

EOF
}

# ----- Downloads

function download_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Download" ;;

   link)    link_title="Download" ;;
   link10)   link_title="System requirements" ;;
   link20)   link_title="Installation" ;;

   content*) eval "download$page$lang $file" ;;
   esac 
}


function download0en()
{  cat >> $1 <<EOF
<h3>Download dvdisaster</h3>

dvdisaster is available for <a href="download10.html">recent versions</a>
of the FreeBSD, Linux and Windows operating systems.<p>

Please download either the source code or a binary version from the list below.
A <a href="#signature">digital signature</a> is provided for verification that the
packets are in their original state.<p>

To install the binary version execute the downloaded program and follow the dialog.
For the source code version see the <a href="download20.html">installation instructions</a>.

<pre> </pre>

<b>Developer versions</b> - new and experimental for experienced users!<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.65</b></td><td align="right">14-Jan-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65.tar.bz2">dvdisaster-0.65.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65.tar.bz2.gpg">dvdisaster-0.65.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65-setup.exe">dvdisaster-0.65-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.65-setup.exe.gpg">dvdisaster-0.65-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
<i>All operating systems:</i>
<ul>
<li>Skips image scanning during error correction file creation<br>
if creation is invoked immediately after the image read.</li>
<li>Error correction files can be split into 2GB segments.</li>
<li>Czech translation by Lubo&#353; Stan&#283;k.</li>
</ul>
<i>FreeBSD:</i>
<ul>
<li>Support started with this version.</li>
</ul>
<i>Windows:</i>
<ul>
<li>Full support for FAT32 file systems (see above).</li>
<li>Binary version now distributed with Gtk+2.6 libraries.</li>
</ul>

</td></tr></table><p>

<pre> </pre>

<b>Stable versions</b> - recommended for getting started.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.64 (pl2)</b></td><td align="right">01-Nov-2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2.tar.bz2">dvdisaster-0.64.2.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2.tar.bz.gpg">dvdisaster-0.64.2.tar.bz.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2-setup.exe">dvdisaster-0.64.2-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.64.2-setup.exe.gpg">dvdisaster-0.64.2-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
This version is capable of determining the
<a href="example91.html#iso">image size from the UDF/ISO file system</a>
to improve the
<a href="qa20.html#plusrw">image size recognition for -RW/+RW media</a>.
Reading images and creating the respective error correction file can now be invoked together
with a <a href="example91.html#auto">single mouse click</a>.
A couple of small improvements have been made
to support more CD/DVD drives, polish existing functionality, and to weed out some
minor bugs ;-)<p>
<b>Patches</b> (small changes after version 0.64; files above have been updated):<br>
<b>pl1</b> Fixes a crash immediately after invoking the program when linked against gtk+-2.6;
only the Linux version is affected.<br>
<b>pl2</b> Repairs the -u command line option and creates file permissions for images using
Unix conventions under Linux.

</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.63</b></td><td align="right">01-Aug-2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63.tbz">dvdisaster-0.63.tbz</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63.tbz.gpg">dvdisaster-0.63.tbz.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63-setup.exe">dvdisaster-0.63-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.63-setup.exe.gpg">dvdisaster-0.63-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Contains an <a href="example50.html">improved strategy</a> for
<a href="syntax90.html#adaptiveread">reading defective media</a> 
which has been motivated by an article in issue 16/2005 of the german
periodical c't.
</td></tr></table><p>

<pre> </pre>

<b>Historic versions</b><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.55</b></td><td align="right">20-Feb-2005</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code (Linux/Windows):&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.tgz">dvdisaster-0.55.tgz</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.tgz.gpg">dvdisaster-0.55.tgz.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.zip">dvdisaster-0.55.zip</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://download.berlios.de/dvdisaster/dvdisaster-0.55.zip.gpg">dvdisaster-0.55.zip.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
This is the last "command-line only" version. 
</td></tr></table><p>

<pre> </pre>

<a name="signature"><b>What is the digital signature?</b></a><p>


The downloadable source code packages have been digitally signed using
<a href="http://www.gnupg.org/gnupg.html">GnuPG</a> so that you can verify
that the software is in its original state.<p>

Checking the authenticity of the <a href="../pubkey.asc">public key</a>
is best done by comparing its "finger print" with that given in the
footer of my emails. Feel free to send me an email with the subject
"GPG finger print" if needed. 
EOF
}


function download10en()
{  cat >> $1 <<EOF
<h3>System requirements</h3>

<ul>
 <li><b>x86 compatible</b> hardware in <b>32bit</b> mode;<br>
     compiling as a <b>64bit</b> application under Linux is possible, but mainly untested.<p></li>
 <li>an up-to-date CD or DVD drive with ATAPI or SCSI interface<p></li>
 <li>at least a P4 at 2Ghz or comparable processor<p></b>
</ul>

<h3>Operating systems</h3>
<ul>
 <li><b>FreeBSD</b> version <b>6.0</b> or later<br>
     (using ATAPI drives requires a kernel recompile)<p>
 </li>
 <li><b>Linux</b> with kernel <b>2.6.7</b> or later<br>
     (using kernels >= 2.4.20 is possible, but not recommended!)<p>
 </li>
 <li><b>Windows</b> <b>2000</b> or <b>XP</b><br>
     (Windows 98 and ME are possible, but are not recommended for handling DVD)
 </li>
</ul>
EOF
}


function download20en()
{  cat >> $1 <<EOF
<h3>Installation of the source code distribution</h3>

<b>Required tools and libraries</b><p>

dvdisaster requires the usual <a href="http://www.gnu.org/">GNU</a> development
tools for compiling C programs. FreeBSD and Linux typically come with suitable 
packages on their installation media.<p>
For Windows, the respective tools are available in the 
<a href="http://www.mingw.org">Mingw</a> framework.
Building under Cygwin is not supported.<p>

Depending on your operating system you will also need:

<ul>
<li>The <a href="http://www.gnu.org/software/gettext/">gettext</a> library</li>
<li>The <a href="http://www.gnu.org/software/libiconv/">iconv</a> library (only for Windows)</li>
<li>The <a href="http://www.freedesktop.org/software/pkgconfig/releases">pkgconfig</a> tool</li>
<li>The <a href="http://www.gtk.org">GTK+ library</a>  version 2.2 or better including all auxiliary libraries</li>
</ul>

These should also be included on the FreeBSD and Linux distribution media.
The <a href="http://www.gtk.org">Gtk+</a> project offers pre-compiled
<a href="http://www.gimp.org/~tml/gimp/win32/downloads.html">Windows versions</a>.
You'll need both the normal and "-devel" versions of the libraries.<p> 

<a name="freebsd"></a>
<b>Hints for FreeBSD</b><p>

FreeBSD does not pre-install an uniform CD-ROM driver for SCSI and ATAPI drives 
(as is the case in Linux and Windows). Therefore dvdisaster can not
use any ATAPI drives in an out-of-the-box FreeBSD installation. To install a suitable
driver please recompile the kernel with the additional device<p> 

<tt>device atapicam</tt><p>

The devices <i>ata</i>, <i>scbus</i>, <i>cd</i>
and <i>pass</i> are also required, but are included by default in FreeBSD 6.0.
Compiling the dvdisaster sources requires GNU make (gmake) and the
bash shell.<p>

<b>Compiling the source code</b><p>

Please open a command prompt window and go through the following steps.<p>

Unpack the source code archive:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>tar xjf $project_package.tar.bz2</b><br>
</td></tr></table><p>

Change into the newly created directory:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>cd $project_package</b><br>
</td></tr></table><p>

Call the configuration script:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>bash configure</b><br>
<i>[... lots of output from the configuration script ...]</i>
</td></tr></table><p>

The script will possibly give some hints about missing libraries.
Type "bash configure --help" to learn about pointing configure to the correct
path of already installed libraries.<p>

When the configuration script finishes without error messages,
compile the source code into the executable program:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake</b><br>
<i>[... lots of output from the compilation process ...]</i>
</td></tr></table><p>

Invoke the program for a quick test:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>./dvdisaster</b><br>
<i>[... after a moment the <a href="example10.html">main window</a> will open ...]</i>
</td></tr></table><p>

Working with the program from the current directory is okay.
A copy of the dvdisaster home page is included in the 
sub directory <i>documentation/en.</i><p>

It is also possible to have the program and documentation
installed permanently on your system:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>gmake install</b><br>
<i>[... Output of the installation process ...]</i>
</td></tr></table><p>

Before doing the install, please review the output of the "<tt>configure</tt>" script
or type "<tt>gmake show</tt>" to learn about the target directories of the installation.<p>
Enter "<tt>bash configure --help</tt>" to get information on changing the installation
paths.

<pre> </pre>

<font size="+1">Enjoy using dvdisaster!</font>
EOF
}

# ----- Command line syntex

function syntax_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Commandline mode" ;;

   link)    link_title="Command line mode" ;;
   link10)   link_title="Using the command line" ;;
   link20)   link_title="Creating a medium image" ;;
   link30)   link_title="Generating error correction files" ;;
   link40)   link_title="Scanning the medium for errors" ;;
   link50)   link_title="Recovering a medium image" ;;

   link60)   link_title="Choosing alternative drives and file names" ;;
   link70)   link_title="Performing several actions within one program call" ;;

   link90)   link_title="Command syntax" ;;

   content*) eval "syntax$page$lang $file" ;;
   esac 
}


function syntax0en()
{  cat >> $1 <<EOF
<h3>dvdisaster as command line tool</h3>

Overview of basic functions:

<ol>
<li><a href="syntax10.html">Using the command line</a><p></li>
<li><a href="syntax20.html">Creating the medium image</a><p></li>
<li><a href="syntax30.html">Generating the error correction file</a><p></li>
<li><a href="syntax40.html">Scanning the medium for errors</a><p></li>
<li><a href="syntax50.html">Recovering the medium image</a><p></li>
</ol>
<pre> </pre>

Some hints for making work easier: 

<ul>
<li><a href="syntax60.html">Choosing alternative drives and file names</a><p></li>
<li><a href="syntax70.html">Performing several actions within one program call</a><p></li>
<li><a href="background60.html">Hints for storing the error correction files</a></li>
</ul>

<pre> </pre>

Complete list of command line options:
<ul>
<li><a href="syntax90.html">Command syntax</a><p></li>
</ul>

EOF
}


function syntax10en()
{  cat >> $1 <<EOF
<h3>Using the command line</h3>

dvdisaster switches into command line mode if it is invoked
with an <a href="syntax90.html#modes">action parameter</a> 
(such as read, create, scan, fix, test). Otherwise a window for the
<a href="example10.html">graphical user interface</a> will be opened.<p>

Command line mode ignores any settings from the graphical user interface and the <i>.dvdisaster</i> configuration file.<p>

<b>Hints for the Windows version:</b>

<ul>
<li>Under Windows 2000 and XP either <b>administrator priviledges</b> or an installed
<a href="qa10.html#aspi">ASPI</a> driver are required in order to access the CD/DVD drives.<p></li>
<li>For technical reasons there are two program versions for Windows:
<table>
<tr><td><i><b>dvdisaster</b>.exe</i></td><td>--</td><td>provides the command line interface</td></tr>
<tr><td><i>dvdisaster-win.exe</i></td><td>--</td><td>contains the graphical user interface</td></tr>
</table>
</li>
</ul>

<a href="syntax20.html">Creating the medium image...</a>

EOF
}


function syntax20en()
{  cat >> $1 <<EOF
<h3>Creating the medium image</h3>

Most actions of dvdisaster are performed on a medium image residing on your hard drive.
This makes things faster and reduces wear of your CD/DVD drive.<p>

In order to create the medium image:
<ul>
<li>Change into a directory with enough free storage space.
<li>Insert the medium into the drive.</li>
<li>Read the medium image onto the hard disk:</li>
</ul>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
This software comes with &nbsp;ABSOLUTELY NO WARRANTY. &nbsp;This<br>
is free software and you are welcome to redistribute it<br>
under the conditions of the GNU GENERAL PUBLIC LICENSE.<br>
See the file "COPYING" for further information.<p>

Device: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Medium: DVD+R, 2224288 sectors, 1 layer(s)<p>

Creating new medium.img image.<br>
Waiting 5 seconds for drive to spin up...<br>
Read position: 100.0% ( 6.3x)<br>
All sectors successfully read.
</td></tr></table><p>

The percentage of already read sectors and the current drive speed
will be shown during the process.<p>

Upon completion you will find the image file <i>medium.img</i> in the current directory:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.img<br>
</td></tr></table><p>

The default setting is to read from drive <i>/dev/cdrom</i> and to create 
file names beginning with <i>medium</i>. Of course it is possible to
select other <a href="syntax60.html">drives</a>
and <a href="syntax60.html">file names</a>.<p>

<a href="syntax30.html">Generating the error correction file...</a>
EOF
}


function syntax30en()
{  cat >> $1 <<EOF
<h3>Generating the error correction file</h3>

The error correction file has an important role in dvdisaster:<br>
It is required to recover unreadable medium sectors.<p>

When you have created the
<a href="syntax20.html">medium image</a> on your hard disk, enter the following:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -c</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Opening medium.img: 2224288 medium sectors.<br>
Scanning image sectors: 100%<br>
Encoding with Codec RS01: 32 roots, 14.3% redundancy.<br>
Ecc generation: 100.0%<br>
Error correction file "medium.ecc" created.<br>
Make sure to keep this file on a reliable medium.<br>
</td></tr></table><p>

dvdisaster will first check the image for completeness
<i>because an error correction file can not be generated for media 
already containing unreadable sectors!</i> <br>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Generate the error correction file immediately after writing the medium.
</td></tr></table>

The progress will be shown using a percentage reading. 
Processing a single layered DVD image with the 
default <a href="syntax90.html#redundancy">redundancy</a>
takes about 10 minutes on an average 2Ghz system.<p>

When finished, the error correction file <i>medium.ecc</i> appears in the current directory:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    653721680 2004-07-02 22:45 medium.ecc<br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 medium.img<br>
</td></tr></table><p>

You can remove the image file now, <p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>rm -f medium.img</b><br>
</td></tr></table><p>

but keep the error correction file on a <a href="background60.html">reliable storage medium</a>!
<p>

<a href="syntax40.html">From now on you should regularly scan the medium for read errors...</a>
EOF
}


function syntax40en()
{  cat >> $1 <<EOF
<h3>Scanning the medium for errors</h3>

To check your medium for read errors insert it into your drive and enter:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -s</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Device: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Medium: DVD+R, 2224288 sectors, 1 layer(s)<p>

Scanning medium for read errors.<br>
Waiting 5 seconds for drive to spin up...<br>
Sector 57664: Medium Error; Unrecovered read error. Skipping 15 sectors.<br>
Sector 57728: Medium Error; Unrecovered read error. Skipping 15 sectors.<br>
[... more read error reports ...] <br>
Sector 716640: Medium Error; Unrecovered read error. Skipping 15 sectors.<br>
Read position: 100.0% ( 2.4x)<br>
33840 unreadable sectors.
</td></tr></table><p>

The reported read errors may be different from "Medium Error; ..."
as these depend on the drive's firmware. <p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
As soon as you notice read errors on your medium, try
<a href="syntax50.html">recovering the medium image</a> 
and transfer it onto a new storage medium. </td></tr></table><p>

<b>How frequently should the media be scanned?</b> 
This depends on your media quality and storage conditions.
As a guide line for CD- and DVD media of unknown quality:

<ol>
<li> 3 days after burning (keep the media images on hard disc until this test passes!), </li>
<li> 14 days after burning, </li>
<li> 3 months after burning, </li>
<li> then one more scan after each 6 months.</li>
</ol>

If you're using lots of media from the same production of a high quality manufacturer,
it may be sufficient to scan a sample of 2-3 media per month and batch.<p>

<a href="syntax50.html">Recovering a medium image...</a>
EOF
}


function syntax50en()
{  cat >> $1 <<EOF
<h3>Recovering a medium image</h3>

To recover data from damaged media, you must first try
to read as much data as possible from it into an image file.<br>
If you use the <a href="background40.html">adaptive reading strategy</a> 
and point dvdisaster to the name
of the error correction file, reading will stop when enough data
for recovering the image has been collected - resulting in a major time saving!<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r --adaptive-read -e medium.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Device: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Medium: DVD+R, 2224288 sectors, 1 layer(s)<p>

Adaptive reading: Trying to collect enough data for error correction.<br>
Creating new medium.img image.<br>
Repairable:  2.6% (correctable: 0; now reading [0..2224288], size 2224288)<br>
Sectors 57264-57279: Medium Error; Unrecovered read error.<br>
Filling image area [57280..1083504]<br>
[... more descriptions of reading process ...] <br>
Repairable: 100.0% (correctable: 319200; now reading [320304..327065], size 6762)<br>
Sufficient data for reconstructing the image is available.
</td></tr></table><p>

Then you need the <a href="syntax30.html">appropriate error correction file</a>,
which you have hopefully generated at a time when the medium was still fully readable.
Enter:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -f</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Opening medium.img: 2224288 medium sectors.<p>

Fix mode: Repairable sectors will be fixed in the image.<br>
&nbsp;&nbsp;&nbsp;    5 repaired sectors: 708225 59850 69825 79800 199500 <br>
&nbsp;&nbsp;&nbsp;    6 repaired sectors: 708226 59851 69826 79801 89776 199501<br>
[... more reports on repaired sectors ...] <br>
&nbsp;&nbsp;&nbsp;    5 repaired sectors: 708224 59849 69824 79799 199499<br>
Ecc progress: 100.0% <br>
Repaired sectors: 33264 <br>  
Good! All sectors are repaired.<br>
Erasure counts per ecc block:  avg =  3.3; worst = 7.
</td></tr></table><p>

The message "Good! All sectors are repaired." confirms that the medium image
("medium.img") has been fully recovered. 
The repaired image can be handled like an ordinary ". i s o" image.
It can be written to a new
medium using a suitable CD/DVD recording software.<p>

If uncorrectable sectors remain after this stage, 
try reading the image again
<a href="background50.html#reading-tips">(some hints for re-reading)</a>.
dvdisaster will try to re-read only the missing sectors, and this might
retrieve enough missing sectors for a successful error correction run.
EOF
}


function syntax60en()
{  cat >> $1 <<EOF
<h3>Choosing a drive for reading</h3>

Use the <a href="syntax90.html#device">"-d" - option</a>
to specify the drive for reading media images:<p>

<table class="example-bsd" width=100%><tr><td>
user@freebsd&gt; dvdisaster <b>-d /dev/pass0</b> -r
</td></tr></table><p>

<table class="example" width=100%><tr><td>
user@linux&gt; dvdisaster <b>-d /dev/hdc</b> -r
</td></tr></table><p>

<table class="example-win" width=100%><tr><td>
user@windows&gt; dvdisaster <b>-d E:</b> -r
</td></tr></table><p>

The FreeBSD version accesses devices using the passthrough drivers 
<tt>/dev/pass*</tt>.
Under Linux some commonly used device names are
<tt>/dev/hd*</tt> for ATAPI drives and <tt>/dev/scd*</tt>
for SCSI drives. The Windows version works with the usual drive letters.

<pre> </pre>

<h3>Specifying image and error correction files names</h3>

You can change the names of image and error correction files
using the <a href="syntax90.html#image">"-i and -e" - options</a>:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-i myimage.img -e medium01.ecc</b> -c
</td></tr></table><p>

It is possible to omit the file suffixes ".img" bzw. ".ecc" 
or to use other file endings.
EOF
}


function syntax70en()
{  cat >> $1 <<EOF
<h3>Performing several actions within one program call</h3>

dvdisaster can execute several <a href="syntax90.html#modes">actions</a>
within one program invocation.<br>
For example, to read an image from <tt>/dev/hdc</tt>,
create an error correction file "medium42.ecc" for it and
finally delete the temporary image file "medium.img", enter:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-r -c -u</b> -d /dev/hdc -e medium42.ecc
</td></tr></table><p>

Actions will always be executed in the same order as they are 
<a href="syntax90.html#modes">listed in the command syntax section</a>,
even if they are specified in a different sequence on the command line.
EOF
}


function syntax90en()
{  cat >> $1 <<EOF
<h3>Command syntax</h3>

<b><a name="modes">Actions.</a>&nbsp;</b>dvdisaster needs to be called with at least one action 
specified either in long form (--read) or just by the beginning letter (-r):<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#scan">--scan</a></td><td>Scan medium for read errors</tr>
<tr valign=top><td></td><td><a href="#read">--read</a></td><td>Read the medium image to hard disc</td></tr>
<tr valign=top><td></td><td><a href="#create">--create</a> &nbsp; &nbsp;</td><td>Create the error correction file</td></tr>
<tr valign=top><td></td><td><a href="#fix">--fix</a></td><td>Try to fix an image using the error correction file</td></tr>
<tr valign=top><td></td><td><a href="#test">--test</a></td><td>Test whether image and error correction file match</td></tr>
<tr valign=top><td></td><td><a href="#unlink">--unlink</a></td><td>Delete the image file at program exit</td></tr>
</table>
<pre> </pre> 

<b><a name="drive">Device and filenames.</a></b> These options do also have a long form (--device) and a 
first letter abbreviation (-d). They are used to change the pre-selected drive and file names.<p>

<table>
<tr><td>&nbsp; &nbsp;</td><td><a href="#device">--device</a>  &nbsp; &nbsp;</td><td>Device selection</td></tr>
<tr><td></td><td><a href="#prefix">--prefix</a></td><td>Prefix for image and error correction files</td></tr>
<tr><td></td><td><a href="#image">--image</a></td><td>Name of image file</td></tr>
<tr valign=top><td></td><td><a href="#eccfile">--eccfile</a></td><td>Name of error correction file<p></td></tr>
<tr><td></td><td><a href="#list">--list</a></td><td>shows drives available under ASPI (Windows version only)</td></tr>
</table><pre> </pre> 

<b><a name="options">Other Options.</a></b> These less frequently used options are only available in the
long form unless noted otherwise.<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#adaptiveread">--adaptive-read</a></td><td>use adaptive strategy for reading damaged media</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#autosuffix">--auto-suffix</a></td><td>automatically add .img and .ecc file suffixes</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#cache">--cache-size</a> &nbsp; &nbsp;</td><td>Cache size during creation of the error correction file</td></tr>
<tr valign=top><td></td><td><a href="#dao">--dao</a></td><td>Assumes "disk at once" medium</td></tr>
<tr valign=top><td></td><td><a href="#fillunreadable">--fill-unreadable [n]</a></td><td>fill unreadable sectors with given byte</td></tr>
<tr valign=top><td></td><td><a href="#jump">-j / --jump</td><td>Skip sectors after a read error</td></tr>
<tr valign=top><td></td><td><a href="#parse-udf">--parse-udf</td><td>Use information from ISO/UDF filesystem</td></tr>
<tr valign=top><td></td><td><a href="#redundancy">-n / --redundancy</td><td>Set error correction code redundancy</td></tr>
<tr valign=top><td></td><td><a href="#speedwarn">--speed-warning [n]</a>&nbsp; &nbsp;</td><td>Warns when reading speed drops a certain amount</td></tr>
<tr valign=top><td></td><td><a href="#spinup">--spinup-delay [n]</a>&nbsp; &nbsp;</td><td>Gives drive time to spin up</td></tr>
<tr valign=top><td></td><td><a href="#split">--split-files</a>&nbsp; &nbsp;</td><td>Splits files into segments <= 2GB</td></tr>
</table>

<h3>Actions.</h3>

<a name="scan"><b>--scan [n-m]: Scan medium for read errors</b></a><p>

Reads every medium sector 
and prints the number of unreadable sectors when finished.<p>

It is possible to limit the scan to a certain range of sectors. Sector counting
starts with 0; "end" refers to the last sector.  
The given interval is inclusive; 0-100 will read 101 sectors.<p>

Do not use white space in the abbreviated form between the "-s" and the interval: 
<pre>
dvdisaster -s0-100          # reads sectors 0 to 100
dvdisaster --scan 0-100     # long form
dvdisaster --scan 3000-end  # reads from sector 3000 until the end of medium
</pre>
<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="read"><b>--read [n-m]: Read the medium image to hard disc </b></a><p>

Creates a medium image on the hard disc.<p>

If the image file is already present, only sectors missing in the image will be
re-read. This allows for completing the image in several reading passes,
and optionally to employ different drives in each of them.<p>

--read will use the <a href="background30.html">linear reading strategy</a> by default.
Add the <a href="syntax90.html#adaptiveread">--adaptive-read</a> option
to employ the <a href="background40.html">adaptive reading strategy</a> instead.<p>

The range of sectors to be read in can be limited analogous to the
<a href="syntax90.html#scan">--scan</a> option. 

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="create"><b>--create: Create the error correction file</b></a><p>

Creates the error correction file for an image.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="fix"><b>--fix: Fix an image</b></a><p>

Try to fix an image using the error correction file.<p>

If not all sectors can be repaired during the first pass,
try reading the image again with another <a href="#read">--read</a> pass
<a href="background50.html#reading-tips">(some hints for re-reading)</a>.
This might gather enough missing sectors 
for a successful error correction run.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="test"><b>--test: Test whether image and error correction files match</b></a><p>

This action simply checks whether the image and error correction files match
(e.g. belong to the same medium), and whether their internal check sums are correct.<p>

See also <a href="qa20.html#crc">topic 3.3</a> in the <a href="qa20.html">Questions and
Answers</a>.


<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="unlink"><b>--unlink: Delete the image file at program exit</b></a><p>

This function deletes the image file if all preceeding actions were successful.

<div align=right><a href="#modes">&uarr;</a></div><p>



<h3>Devices and file names.</h3>

<a name="device"><b>--device &lt;drive&gt;: Select a drive</b></a><p>

The preset is "/dev/cdrom" for Linux. Under Windows, the first CD/DVD drive 
(according to the drive letter order) is pre-selected.<p>

<b>FreeBSD:</b> &nbsp; dvdisaster supports drives which are accessible through 
the passthrough driver <tt>/dev/pass*</tt> and which are MMC3 compliant. 
Access to ATAPI drives may require a <a href="download20.html#freebsd">kernel recompile</a>.<p>

<b>Linux:</b> &nbsp; dvdisaster supports drives which are accessible through 
the Linux uniform CD-ROM driver and which are MMC3 compliant. 
These are particularly ATAPI drives attached to the symbolic devices
<tt>/dev/hd*</tt> and SCSI drives using the symbolic devices <tt>/dev/scd*</tt>.
External drives using other bus systems (like USB, IEEE 1394) usually also appear
as SCSI devices.<p>

Accessing ATAPI drives using the <i>ide-scsi</i> module is not recommended,
but possible since these devices are usually available under the
<tt>/dev/scd*</tt> interface.<p>

The general SCSI driver <i>sg</i> (<tt>/dev/sg*</tt>) is not supported.
This should not be a problem since the respective drives are also available
using the <tt>/dev/scd*</tt> or <tt>/dev/sr*</tt> devices.<p>

<b>Windows:</b> &nbsp; dvdisaster supports local drives implementing the MMC3 standard
and which are referenced through a drive letter in the system.


<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="prefix"><b>--prefix &lt;prefix&gt;: Prefix for image and error correction files</b></a><p>

The preset is "medium"; the image and error correction files will automatically receive the
endings ".img" and ".ecc".

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="image"><b>--image &lt;file name&gt;: Name of image file</b></a><p>

The preset is "medium.img"; the specified file name will be used exactly (without appending a suffix)
for the image file
as long as <a href="#autosuffix">--auto-suffix</a> is not specified also.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="eccfile"><b>--eccfile &lt;file name&gt;: Name of error correction file</b></a><p>

The preset is "medium.ecc"; the specified file name will be used exactly (without appending a suffix)
for the error correction file
as long as <a href="#autosuffix">--auto-suffix</a> is not specified also.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="list"><b>--list: shows drives available under ASPI</b></a><p>

This option lists all CD/DVD drives which are accessible through
the <a href="qa10.html#aspi">ASPI</a> layer of Windows:<p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-l</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

List of ASPI CD/DVD drives:<p>
 1: (H:) ATAPI DVD+RW 8X4X12 B2K7<br>
 2: (I:) MYDRIVE CD-R MY-401610X 1.05<p>

To force ASPI usage over SPTI, refer to the drive by the<br>
above numbers (use 1:, 2:,... instead of C:, D:,...)
</td></tr></table><p>

The default is trying to access a drive using SPTI first
and only fall back to ASPI if the former does not work.
Usage of the ASPI layer can be forced by
specifying the drive by its number rather than by its letter.
To read in a disc using ASPI and the "MYDRIVE" unit from the above
example, you would therefore enter:<p>

<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-d 2:</b> -r<br>
</table>

<div align=right><a href="#drive">&uarr;</a></div><p>


<h3>Other Options.</h3>

<a name="adaptiveread"><b>--adaptive-read: Use adaptive strategy for reading damaged media</b></a><p>

This switch selects the <a href="background40.html">adaptive reading strategy</a>
which is particularly suited for reading in damaged media.
Use this switch in conjunction with <a href="#read">-r/--read</a> and with
<a href="#eccfile">-e/--eccfile</a>. Specifying the error correction file 
causes the reading process to finish as soon as enough information has been 
collected for recovering the medium image.
<p>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="autosuffix"><b>--auto-suffix: automatically add .img and .ecc file suffixes</b></a><p>

When this option is given, file names specified by the
<a href="#image">-i/--image</a>- or <a href="#eccfile">-e/--eccfile</a> option
will be automatically appended with ".img" or ".ecc" respectively
if no other file name extension is already present.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="cache"><b>--cache-size &lt;size in MB&gt;: Specify the cache size</b></a><p>

dvdisaster optimizes access to the image and error correction
files by maintaining its own cache.
The cache size can be between 1 and 2048 MB. 
The preset is  32MB, which should be suitable for most systems.<p>

This option is only effective during the <a href="#create">--create</a> action.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="dao"><b>--dao: Assumes "disk at once" medium</b></a><p>

Media written in "TAO" ("track at once") mode may contain two sectors
with <a href="qa20.html#tao">pseudo read errors</a> at the end.
By default these two errors are ignored by dvdisaster.<p>

If you are extremely unlucky to have a "DAO" ("disc at once") medium
with exactly one or two real read errors at the end,
use the "--dao" option to have these read errors handled correctly.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="fillunreadable"><b>--fill-unreadable &lt;fill byte&gt;: fill unreadable sectors with given byte </b></a><p>

dvdisaster marks unreadable sectors with a special filling sequence which is very unlikely
to occur in undamaged media.<br>
In other data recovery software it is common to fill unreadable sectors with a certain byte
value. To allow interoperability with such programs, you can specify the byte value
they are using:<p>

<ul>
<li><b>0xb0 (176 decimal)</b>: for compatibility with <i>h2cdimage</i> 
published by "c't", a German periodical.
</li>
</ul>

Using zero filling (0x00, decimal 0) is <b>highly discouraged</b>.
Most media contain regular zero filled sectors. At a later error correction pass,
these can not be told apart from unreadable sectors if zero filling is used.
</li>
</ul>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="jump"><b>-j / --jump &lt;number of sectors&gt;: Skip sectors after a read error </b></a><p>

This option has slightly different behaviour depending on the used reading strategy:
<ul>
<li>when using the <a href="background30.html#configure">linear reading strategy</a>
(default setting), 
the given number of sectors will be skipped after a read error.</li>
<li>the <a href="background40.html#configure">adaptive reading strategy</a> 
(<a href="#adaptiveread">--adaptive-read</a> option) will terminate
when no unreadable areas are left in the image which are larger than the given value.</li>
</ul> 

Large values reduce the processing time and the mechanical wear on the drive,
but will also leave larger gaps in the image when reading defective areas.
The number of skipped sectors must be a multiple of 16.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="parse-udf"><b>--parse-udf: Use information from ISO/UDF filesystem</b></a><p>
dvdisaster will determine the image size from the ISO/UDF file system
if this option is given. This works around some problems caused by drives
<a href="qa20.html#plusrw">reporting incorrect image lengths</a>
with DVD-RW/+RW media.
<div align=right><a href="#options">&uarr;</a></div><p>



<a name="redundancy"><b>-n / --redundancy: Set error correction code redundancy</b></a><p>

The redundancy specifies the percentage of correctable errors
<a href="background10.html">in the best case</a>.
Since the ideal case is rare, it is recommended to apply a reasonable margin 
to the redundancy. Also, consider the following properties of the error correction:<p>

<ul>
<li>An error correction file with  x% redundancy will be approximately x% of the size
of the corresponding image file.</li>
<li>The error correction capability depends on the statistical distribution of read errors. 
Only changes by 5 percentage points and more may 
have a recognizable effect on the error correction.<p></li>
</ul>

The are several ways of specifying the redundancy:<p>

<ol>
<li> <b>"normal" or "high"</b><p>

Enter "normal" or "high" to get the following redundancies:<p>

<table border="1" cellpadding="3">
<tr><td align=center>Value</td><td>Redundancy</td></tr>
<tr><td><tt>-n normal</tt></td><td align=center>14.3%</td></tr>
<tr><td><tt>-n high</tt></td><td align=center>33.5%</td></tr>
</table><p>

These values invoke optimized program code to speed up the
error correction file creation. The default value is "normal".<p>
</li>

<li> <b>Percentage values</b><p>

Specifying the redundancy by percent is also possible:<p>

<table border="1" cellpadding="3">
<tr><td>Example</td><td>Valid range</td></tr>
<tr><td><tt>-n 25%</tt></td><td align=center>3.2% to 64.5%</td></tr>
</table><p>

For technical reasons the actual redundancy can deviate a few tenths
from the specified value.<p>
</li>

<li> <b>Correctable errors per ECC block</b><p>

When no redundancy unit is given, the value is interpreted to
create a (255, 255-x)-Reed-Solomon code:<p>

<table border="1" cellpadding="3">
<tr><td>Example</td><td>Valid range</td></tr>
<tr><td><tt>-n 32</tt></td><td align=center>8 to 100</td></tr>
</table><p>

This combines 255-x media sectors into one ECC block. Within this block
up to x unreadable sectors can be recovered.<p>

By the way: &nbsp; &nbsp; Redundancy in percent = (100 * x) / (255 - x)<p>
</li>

<li> <b>Maximum size of error correction file</b><p>

Another way of specifying redundancy is to
give the size of the error correction file in MB:<p>

<table border="1" cellpadding="3">
<tr><td>Example</td><td>Valid range</td></tr>
<tr><td><tt>-n 50m</tt></td><td align=center>3.2% to 64.5% of image size</td></tr>
</table><p>

In this case dvdisaster will choose a suitable redundancy setting so that
the overall size of the error correction file does not exceed the
given limit.<p>

Advance notice: When using the same size setting for images of vastly different size,
more error correction information is allotted to the smaller images
and less to the larger ones.<p>
</li>
</ol>

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="speedwarn"><b>--speed-warning [n]: Warns when reading speed drops a certain amount</b></a><p>

Prints a warning when the reading speed changes by more than <i>n</i> percent.
This is useful for recognizing a beginning decay in media quality.
If the optional value [n] is omitted, a preset of 10 percent is used.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="--spinup"><b>--spinup-delay [n]: Gives drive time to spin up</b></a><p>

Waits the given amount of seconds for the drive to spin up before the real
reading process starts.
This avoids spurious warnings about speed changes at the beginning of the medium
when the <a href="#speedwarn">--speed-warning</a> option is used.<br>

If the optional value [n] is not given, dvdisaster will wait for 5 seconds.

<div align=right><a href="#options">&uarr;</a></div><p>

<a name="split"><b>--split-files: Splits files into segments <= 2GB</b></a><p>

Allows the creation of large images and error correction files 
 on file systems which are limited to 2GB per file
(e.g. FAT from Windows). The files are spread over upto 100 segments "medium00.img", "medium01.img" etc. at the cost of a small performance hit.<p>

The file name must still be specified as
"medium.img" using the <a href="#image">--image</a> option; 
the numbering will be automatically inserted when this switch is active.<p>

<div align=right><a href="#options">&uarr;</a></div><p>

EOF
}

# ----- Questions and Answers

function qa_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Questions and Answers" ;;

   link)    link_title="Questions and Answers" ;;
   link10)   link_title="Technical Questions" ;;
   link20)   link_title="Error messages" ;;

   content*) eval "qa$page$lang $file" ;;
   esac 
}


function qa0en()
{  cat >> $1 <<EOF
<h3><a name="top">General questions and answers</a></h3>

<a href="#pronounce">1.1 How is "dvdisaster" pronounced?</a><p>
<a href="#pipo">1.2 Where do I get more information on PI/PO scans?</a><p>
<a href="#compat">1.3 Is dvdisaster compatible with future releases?</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="pronounce">1.1 How is "dvdisaster" pronounced?</a></b><p>
Since the word stems from the english language, simply spell "dv" before
saying "disaster". Perhaps "dee-vee-disaster" is a suitable
phonetic circumscription.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="pipo">1.2 Where do I get more information on PI/PO scans?</a></b><p>
The error correction in DVD media is similar to the
<a href="background10.html">method used in dvdisaster</a>.
DVDs have two steps (or layers) of error correction which are called PI and PO.
Only when both layers fail to correct an error, 
the drive will report an unreadable sector and dvdisaster gets some work to do
(or the medium can be thrown away).<p>

For certain drives software has been published
which can report how frequently both error correction layers are utilized
when reading from the DVD. There are lots of interesting internet forums containing 
links to such tools. Many well informed articles on interpreting PI/PO values 
with respect to the media quality are also available.
<div align=right><a href="#top">&uarr;</a></div><p>

<b><a name="compat">1.3 Is dvdisaster compatible with future releases?</a></b><p>
Yes, dvdisaster files are intended for an archival time of many years.
When upgrading to a newer version of dvdisaster you can continue using
existing image and error correction files from previous versions.
There is <i>no</i> need to recreate them again.
<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa10en()
{  cat >> $1 <<EOF
<h3><a name="top">Technical Questions</a></h3>

<a href="#nls">2.1 Which translations of the program are available?</a><p>
<a href="#media">2.2 Which media types are supported?</a><p>
<a href="#filesystem">2.3 Which file systems are supported?</a><p>
<a href="#aspi">2.4 What does "SPTI" and "ASPI" mean?</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="nls">2.1 Which translations of the program are available?</a></b><p>

The current version of dvdisaster contains screen texts in the following languages:<p>

<table>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Czech</td><td>--</td><td>complete</td></tr>
<tr><td></td><td>English</td><td>--</td><td>complete</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>German</td><td>--</td><td>complete</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Italian</td><td>--</td><td>complete</td></tr>
</table><p>

Translators for other languages are welcome!<p>

dvdisaster will automatically obtain language settings from the operating system.
If the local language is not yet supported, english text will be used. 
A different language can be selected using environment variables.<p>

Example for the bash shell and german language:

<pre>export LANG=de_DE</pre>

If special characters like german umlauts are not displayed properly,
try the following:<p>

<tt>export OUTPUT_CHARSET=iso-8859-1</tt> (X11, XTerm)<p>

or<p>

<tt>export OUTPUT_CHARSET=CP850</tt> (Windows)<p>

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="media">2.2 Which media types are supported?</a></b><p>

dvdisaster supports (re-)writeable CD and DVD media. <br>
Media containing multiple sessions or copy protections can <i>not</i> be used.<p>

Usable media by type:<p>

<b>DVD-R, DVD+R</b><p>

<ul>
<li>No further limitations are known.</li>
</ul>

<b>DVD+R9 (two layers)</b>
<ul>
<li>The drive must be able to <a href="qa20.html#dvdrom">identify the medium type</a>.</li>
</ul>

<b>DVD-RW, DVD+RW</b><p>

<ul>
<li>Some drives report wrong <a href="qa20.html#plusrw">image sizes</a>.<br>
Remedy: Activate <a href="example91.html#iso">"Use information from the ISO/UDF file system"</a>
</li></ul>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>only Data CD are supported.</li>
</ul>

<b>Not usable types</b> (image can not be extracted):<p>
CD-Audio and CD-Video as well as DVD-ROM and DVD-RAM.

<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="filesystem">2.3 Which file systems are supported?</a></b><p>

dvdisaster works exclusively on the image level which is accessed sector-wise.
That means it does not matter with which file system the medium has been formatted.<p>

Since dvdisaster neither knows nor uses the file system structure,
it can not repair logical errors at the file system level.
It can not recover lost or deleted files.
<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="aspi">2.4 What does "SPTI" and "ASPI" mean?</a></b><p>

Windows 98 and ME use drivers called "ASPI" to access CD/DVD drives.
Windows NT and its successors provide similar functionality
through an interface named "SPTI", but do still allow the installation
of additional ASPI drivers.<p>

<i>&gt;&nbsp; Pro and Con of installing ASPI under Windows NT/2000/XP:</i><p>

<table>
<tr valign=top><td>+</td><td>No administrator priviledges needed for accessing the drives.</td></tr>
<tr valign=top><td>-</td><td>ASPI does not map well onto the Windows drive letters.</td></tr>
<tr valign=top><td>-</td><td>Some CD/DVD writing programs install
their own ASPI drivers. Installing another ASPI driver may interfere 
with the existing software.</td></tr>
</table><p>

<i>&gt;&nbsp; Which ASPI versions are recommended for use with dvdisaster?</i><p>

Adaptec provides their ASPI drivers free of charge for download.
Sometimes versions 4.72.* have difficulties recognizing drives under
Windows 2000/XP. Downgrading to version 4.60 usually helps in such
cases.<p>

<i>&gt;&nbsp; Drive letters are wrong under ASPI!</i><p>

ASPI drivers use their own system of identifying drives, which does
not support the drive letter scheme of Windows.<br>
In order to provide a unified access to SPTI and ASPI,
dvdisaster tries to guess the mapping of ASPI drives to drive letters.
However this does not work under all configurations.<p>

The <a href="syntax90.html#list">--list</a> function prints out
the current mapping of drive letters.<p>

<i>&gt;&nbsp; How to select between SPTI and ASPI?</i><p>

dvdisaster will automatically choose between SPTI and ASPI, 
deciding in favor of SPTI if both are available. 
Usage of ASPI can be forced; see the description of the
<a href="syntax90.html#list">--list</a> function for details.

<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa20en()
{  cat >> $1 <<EOF
<h3>Error messages</h3>

<a href="#tao">3.1 "Warning: 2 sectors missing at the end of the disc".</a><p>
<a href="#block">3.2 Program blocks right after invocation.</a><p>
<a href="#crc">3.3 What does "CRC error, sector: n" mean?</a><p>
<a href="#plusrw">3.4 Read errors or wrong image size with -RW/+RW media</a><p>
<a href="#dvdrom">3.5 My self-written media is recognized as "DVD-ROM" and rejected.</a><p>
<a href="#freebsd">3.6 No drives appear under FreeBSD.</a><p>

<pre> </pre><hr><pre> </pre>

<b><a name="tao">3.1 "Warning: 2 sectors missing at the end of the disc"</a></b><p>
This warning appears with CD media written in "TAO" (track at once) mode.
Some drives report an image size which is 2 sectors too large for such media,
producing 2 pseudo read errors at the end of the medium 
which do <i>not</i> mean data loss in this case.<p>

Since the writing mode can not be determined from the medium, dvdisaster assumes
a "TAO" CD if exactly the last two sectors are unreadable, and trims the image
accordingly. It is up to you to decide whether this is okay. You can
advise dvdisaster to treat these sectors as real read errors by using the
<a href="syntax90.html#dao">--dao</a> option or 
the <a href="example92.html#image">preferences tab for reading/scanning</a>.<p>

To avoid these problems, consider using the "DAO / Disc at once" (sometimes
also called "SAO / Session at once") mode for writing single session media. 
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="block">3.2  Program blocks right after invocation</a></b><p>
Under old Linux versions (kernel 2.4.x) the program occasionally 
blocks right after the start and before any actions are
carried out. It can not be terminated using Ctrl-C or "kill -9".<p>

Eject the medium to make the program terminate. Insert the medium again
and wait until the drive recognizes the medium and spins down.
Calling dvdisaster again should work now.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="crc">3.3 What does "CRC error, sector: n" mean?</a></b><p>
The respective sector could be read, but the checksum of its contents does
not match the value noted in the error correction file.  Some possible causes are:<p>

<ul>
<li>The image has been mounted with write permission and
was therefore altered (typical evidence: CRC errors in sector 64 and in
sectors 200 to 400).</li>
<li>The computer has some hardware problems, especially when
communicating with its mass storage devices.</li>
</ul>


If you suspect technical problems, try creating another version of the image 
and error correction files and <a href="example60.html">compare
them again</a>.
When the error disappears or surfaces at a different location,
your computer may be suffering from defective memory, 
broken drive cabling, or wrong CPU/system frequency settings.
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="plusrw">3.4 Read errors or wrong image size with -RW/+RW media</a></b><p>

Some drives report incorrect image sizes when -RW/+RW media are used. Two common
cases are:<p>

<table>
<tr><td valign="top">Problem:</td>
<td>The drive reports the size of the largest image ever written to the medium, not that
of the actual image.
</td></tr>
<tr><td valign="top">Symptoms:</td>
<td>After erasing a medium it is written with a file sized about 100MB.
But the image read back is several GB long and contains the remainings
of older images.
</td></tr>
<tr><td><pre> </pre></td><td></td></tr>
<tr><td valign="top">Problem:</td>
<td>The drive reports the maximum possible medium capacity (typically 2295104 sectors)
instead of the number of actually used sectors.
</td></tr>
<tr><td valign="top">Symptoms:</td>
<td>When reading beyond a certain point of the medium, only read errors occur,
but all files on the medium are still readable and complete.
</td></tr>
</table><p>

Possible remedy:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Activate the option  <a href="example91.html#iso">Use information from the
ISO/UDF file system</a> so that the image size will be determined from the
ISO/UDF file system.
</td></tr></table><p>

If the required ISO/UDF sectors are unreadable when trying to recover damaged media
there are two possible workarounds:

<ul>
<li>Execute the <a href="example60.html">"Compare"</a> function with only
the error correction file being selected/given. Note down the correct image size 
from the output and <a href="example92.html#read">restrict the
reading range</a> accordingly.
</li>
<li>Simply read in the image with the incorrect (larger) size.
When invoking the <a href="example50.html#repair">"Fix"</a> function,
answer "OK" when you are asked whether the image should be truncated.
</li>
</ul>

<div align=right><a href="#top">&uarr;</a></div>

<b><a name="dvdrom">3.5 My self-written media is recognized as "DVD-ROM" and rejected.</a></b><p>

This can have the following reasons:
 
<ol>
<li> The medium book type has been set to "DVD-ROM".<br>
In this case the drive must support another way to find out the
real medium type. Only then dvdisaster will accept the medium and report it
as "DVD-ROM (faked book type)".<p>

But some drives do not support the recognition of faked book types.

<li> Some drives can not tell DVD+R9 (two layers) from DVD-ROM.
</li>
</ol>

Try another drive for reading images in these cases.

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="freebsd">3.6 No drives appear under FreeBSD.</a></b><p>

<ul>
<li>FreeBSD may require a <a href="download20.html#freebsd">kernel recompile</a>
in order for ATAPI drives (nearly all current models) to become usable for
dvdisaster.
<li>You need read and write permission for the respective device
(e.g. /dev/pass0).
</ul>

<div align=right><a href="#top">&uarr;</a></div>

EOF
}

# ----- Background information

function background_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Background information" ;;
 
   link)    link_title="Background information" ;;
   link10)   link_title="Error correction properties" ;;
   link30)   link_title="Linear reading strategy" ;;
   link40)   link_title="Adaptive reading strategy" ;;
   link50)   link_title="Remarks on read errors" ;;
   link60)   link_title="Hints for storing the error correction files" ;;

   content*) eval "background$page$lang $file"
	    return 0
	    ;;
   esac 
}


function background0en()
{  cat >> $1 <<EOF

<h3>Background information</h3>

The information in this sub section is not required for operating dvdisaster.
However it is helpful on understanding how dvdisaster works and may help
you getting the most out of the program according to your needs.

<ol>
<li><a href="background10.html">Properties of the Reed-Solomon error correction</a><p></li>
<li><a href="background30.html">Details of the linear reading strategy</a><p></li>   
<li><a href="background40.html">Details of the adaptive reading strategy</a><p></li>   
<li><a href="background50.html">Some remarks on read errors</a><p></li>   
<li><a href="background60.html">Hints for storing the error correction files</a><p></li>
</ol>

EOF
}


function background10en()
{  cat >> $1 <<EOF
<h3>Technical properties of the error correction</h3>

This page outlines the basic ideas behind dvdisaster
so that you can see for yourself whether
it meets your demands on data safety.
If in doubt, you should not use dvdisaster
or deploy additional data backup strategies.<p>

<b>Method of error correction.</b> &nbsp; dvdisaster uses a
<a href="http://en.wikipedia.org/wiki/Reed-Solomon_error_correction">Reed-Solomon</a> code
together with an error correction algorithm optimized for the treatment of erasures.
The implementation draws a lot of inspiration and program code from the excellent
<a href="http://www.ka9q.net/code/fec/">Reed-Solomon library</a> written by
<a href="http://www.ka9q.net/">Phil Karn</a>.

<p>

Using the <a href="example93.html#redundancy">standard settings</a>
223 medium sectors are combined into one error correction code ("ECC") block.
Medium read errors are regarded as "erasures"; therefore a maximum 
of 32 bad medium sectors<sup><a href="#footnote1">*)</a></sup>
are correctable per ECC block. <p>

The 223 sectors are selected so that they are evenly distributed over the medium surface.
Therefore large contigous areas of defective sectors can be corrected before the threshold
of 32 defects per ECC block<sup><a href="#footnote1">*)</a></sup>
is reached. This kind of error pattern is especially
common for an aging medium where the outer area is starting to degenerate, 
and for scratches along the data spiral. <p>

On the other hand, radial or diagonal scratches are expected to be correctable in the
CD/DVD drive itself. If not, the employed error correction strategy performs rather neutral
in these cases (neither especially good nor extraordinary bad). <p>

<b>Limits of error correction.</b> &nbsp; In the wost case, 
33 defective sectors<sup><a href="#footnote1">*)</a></sup> are sufficient
to prevent a full data restoration. However to achieve this effect, the errors have to
be distributed over the medium in such a way that they occur in the same ECC block - such a pattern
is very unlikely.<br>
Empirical tests have shown that on aging media about 10% of the overall sector count
may become defective before
the threshold of 33 defects per ECC block<sup><a href="#footnote1">*)</a></sup>
 is reached. <br>
<a href="index10.html#scratches">Scratches</a> will cause the threshold to be reached earlier, 
therefore it is recommended to visually check the media in regular intervals.
Media with read errors caused by scratches should be replaced immediately.<p>

<b>Hardware limits.</b> &nbsp; Most drives will not recognize media when the lead-in area
before the first sector
(near the center hole) is damaged. In such cases, dvdisaster will not be able to recover
any contents from the media. <p>

It is <i>not feasible</i> to enhance the reliability of poor quality media by using
dvdisaster. Cheap media can decay within a few days to an extent which will exceed
the capabilities of the error correction code. <p>

<pre> </pre>
<table width="50%"><tr><td><hr></td></tr></table>

<font size="-1">
<a name="footnote1"><sup>*)</sup></a> 
The given threshold of 32 correctable errors per ECC block is the standard setting.
It is possible to <a href="example93.html#redundancy">select other values</a>
for higher or lower error correction capabilities.
<p>
</font>

EOF
}


function background30en()
{  cat >> $1 <<EOF
<h3>The linear reading strategy.</h3>

dvdisaster contains two different
<a href="example92.html#read">reading strategies</a>.<p>

<b>The linear reading strategy is recommended for:</b><p>
<ul>
<li>creating images from undamaged media, e.g. to generate the error correction file</li>
<li>scanning the medium for reading speed and read errors</li>
</ul>

<b>The <a href="background40.html">adaptive reading strategy</a> is recommended for:</b><p>
<ul>
<li> extracting data from damaged media
</li>
</ul>

<pre> </pre>

<b>Properties of the linear reading strategy.</b><p>

CD- and DVD media are organized into sectors which are 
continously numbered beginning with zero. Each sector contains 2048 bytes of data.<p>

The linear reading strategy reads the medium from the start (sector 0)
until the end (last sector). The reading speed is shown graphically to provide
information about the <a href="background30.html#quality">medium quality</a>:<p>

<center><img src="../images/bg-linear-en.png"><br>
partial screen shot: linear reading strategy</center>

<pre> </pre>


<a name="configure"></a>
<b>Configuration.</b><p>

<b>Number of sectors to skip after a read error.</b>
Reading attempts for defective sectors are slow and may wear out the drive mechanics
under unfavourable conditions. A series of read errors, spread over a continous sector
range, is more common than single spot defects. Therefore a 
<a href="example92.html#read"> configuration option</a> exists so that a certain number
of sectors will be skipped after the occurance of a read error. The skipped sectors are
assumed to be defective without further reading attempts. 
Some guide lines for selecting the number of skipped sectors are:<p>

<ul>
<li>Skipping a large number of sectors (e.g. <b>1024</b>) gives a quick overview of the 
degree of damage, but will usually not collect enough data for repairing the medium image.
<p></li> 
<li>Smaller values like <b>16, 32 or 64</b> are a good trade-off:
The processing time will be considerably shortened, but still enough data for repairing
the image will be collected.<p></li></ul>

On DVD media read errors do usually extend over at least 16 sectors for technical
reasons. Therefore a sector skip less than 16 is not recommended for DVD media.
</li>
</ul>
<p>

<a name="range"></a>
<b>Limiting the reading range.</b>
Reading can be
<a href="example92.html#read">limited to a given range of sectors</a>. This comes in handy
during multiple read attempts of damaged media.

<pre> </pre>

<a name="quality"></a>
<b>Estimating media quality.</b><p>

<a name="error"></a>
<b>The speed curve.</b>
Most drives will slow down while reading medium areas which are in bad condition:
<ul>
<li>
Drops in the reading speed can be a warning for an imminent medium failure.
</li>
<li>
However some drives will read with full speed until the bitter end.
With such drives media deterioration may not show up in the reading curve
until actual read errors occur.
</li>
</ul><p>

The reading curve is most accurate when using the
<a href="example40.html"> "Scan"</a> function. During the
<a href="example20.html"> "Read"</a> operation the read data will be written to
the hard drive at the same time, which may cause irregularities in the reading
curve depending on the operating system and hardware used.<p>

<a name="error"></a>
<b>Read errors.</b>
Read errors cause <a href="example40.html">red markings in the spiral</a> or respective
messages <a href="syntax40.html">at the command line</a>. 
This means that the medium could not be
read at these places during the current reading pass:
<ul>
<li>The medium is most likely defective.</li>
<li>The image should be 
<a href="example50.html">repaired</a> as soon as possible and then be transferred
to a new medium.</li>
</ul>
EOF
}


function background40en()
{  cat >> $1 <<EOF
<h3>The adaptive reading strategy.</h3>

dvdisaster contains two different
<a href="example92.html#read">reading strategies</a>.<p>

<b>The adaptive reading strategy is recommended for:</b><p>
<ul>
<li> extracting data from damaged media
</li>
</ul>

<b>The <a href="background30.html">linear reading strategy</a> is recommended for:</b><p>
<ul>
<li>creating images from undamaged media, e.g. to generate the error correction file</li>
<li>scanning the medium for reading speed and read errors</li>
</ul>

<pre> </pre>

<b>Properties of the adaptive reading strategy.</b><p>

The adaptive reading strategy uses a "divide and conquer" approach for locating
still readable portions of a damaged medium. The strategy is based upon an article
published by Harald B�geholz in c't-Magazin 16/2005 where it was published together
with the program <i>h2cdimage</i>:

<ol>
<li> 
At the beginning the medium is considered as a single unread interval. Reading begins
with sector zero.<p>
</li>
<li>
The reading process continues sequentially unless either the end of the current interval
or a read error is encountered.<p>
</li>
<li>
The reading process is terminated if either (3a) sufficient sectors for a successful
error correction have been read or (3b) no unreadable intervals exceeding a given size
remain.
<p>
</li>
<li>
Otherwise the largest remaining unread interval will be determined. Reading continues in the middle
(e.g. second half) of this interval; 
the first half of this interval is kept for a later reading pass.
</li>
</ol>

The termination criterium (3a) is especially efficient: Reading will stop as soon
as enough sectors have been collected for a successful image recovery using the
error correction file. This can reduce the reading time by as much as 90 percent
compared with a full read attempt, but does of course only work when 
an error correction file is available.<p>

<center><img src="../images/bg-adaptive-en.png"><br>
partial screen shot: adaptive reading strategy</center>

<pre> </pre>

<a name="configure">
<b>Configuration</b><p>

<b>Error correction file.</b> 
Adaptive reading works best when an error correction file is available. 
Obviously the error correction file must have been <a href="example30.html">created</a>
at a time where the medium was still fully readable. To use the error correction file
during adaptive reading, 
<a href="example30.html">enter its name</a> before starting the reading process.<p>

<b>Limiting the adaptive reading range.</b> Reading can be 
<a href="example92.html#read">limited</a> to a part of the medium.

This is not recommended when an error correction file is used since the limit
might prevent sectors from being read which are required for a succesful error correction. 
If no error correction file is available, limiting the reading range might be helpful
during multiple reading attempts.<p>

<b>Early reading termination.</b>
If no error correction file is available, adaptive reading will stop when no unread
intervals
<a href="example92.html#read">larger than a selectable size</a> remain.<p>

The termination value should not be smaller than 128.
Otherwise the laser head will have to carry out lots of repositioning moves during the
final phase of the reading process. This will negatively affect both the life expectancy
of the drive and its reading capability. A better approach is to stop adaptive 
reading earlier and then try reading the remaining sectors with an additional
<a href="background30.html">linear reading</a> pass.
EOF
}


function background50en()
{  cat >> $1 <<EOF
<h3>Remarks on read errors</h3>

DVD media have their own error correction code which protects the data
against small manufacturing errors and inaccuracies during writing.
If the DVD writer and medium are compatible and of high quality, the
error correction built into the medium will at first be mainly idle.
This leaves enough reserves to compensate normal wear and aging effects 
during many years of the medium usage.
<p>

When the capacity of the built-in error correction is finally exhausted,
read errors will start to appear on the medium. These will be reported by
the <a href="example40.html">"Scan"</a>-operation of dvdisaster.
Depending on the time of first occurrence, 
two types of read errors are of particular interest:<p>

<b>Read errors appearing right after writing the medium.</b>
This is a sign of:

<ul>
<li>media from a faulty production run, or</li>
<li>media which are not compatible with the DVD writer.</li>
</ul>

A prudential choice is to dispose of the faulty media and
to write the data on error-free media, possibly switching to a 
different producer.<p>

Please withstand the temptation of trying to preserve 
the faulty media by means of an error correction file - this
will most likely end with data loss.<p>

<b>Read errors after a few months/years.</b> 
The built-in error correction of the medium will be increasingly loaded
during its life time until it finally fails and read errors show up.
This happens for mechanical reasons (scratches, warping of the plastic material)
as well as for chemical causes (decaying dye and/or reflective layer).<p>

These effects typically occur while the medium is stored away for a few months,
and it may not be possible to read in all sectors afterwards.<p>

Therefore it is crucial to create the 
<a href="example30.html">error correction file (ecc file)</a> in time. 
The ecc file contains information for recalculating the contents of
missing sectors
<a href="background10.html">(within certain limits)</a>.
Therefore with the help of the ecc file
dvdisaster can recover images even if not all sectors
could actually be read by the drive.<p>

Since the error correction can reconstruct missing sectors up to a certain number,
it is not necessary to squeeze out a defective medium for every readable sector.
The <a href="background40.html">adaptive reading strategy</a> checks during
reading whether enough data for error correction has been collected.
As soon as this is the case, reading stops and still unread sectors
will be recovered using the ecc file.<p>

<a name="reading-tips"><b>
Some hints for effectively reading damaged media</b></a><p>

The outcome from reading damaged media depends on several factors:

<ul>
<li><b>Not all drives are built the same.</b><br>
Different drives have different reading capabilities.
Take advantage of dvdisaster's function for completing an image 
with several reading passes and use different drives for each pass.
Transfer the image file between computers using a network or rewritable media
in order to use drives installed in different machines.
<p></li>
<li><b>Eject and insert the medium again.</b><br>
Sometimes it makes a difference to eject the medium, turn it about a quarter,
and then load it again for another reading pass.
<p></li>
<li><b>Some drives read better while being cold.</b><br>
Turn off the computer over night and perform another reading attempt in the next
morning.<p>
But note: "Cold" refers to normal living room conditions - putting hardware
or media into the fridge can be very unhealthy for them.<p></li>
</ul>
EOF
}


function background60en()
{  cat >> $1 <<EOF
<h3>Hints for storing the error correction files</h3>

Here are a few suggestions for storing the error correction files:<p>

<b>1. Storing the error correction files on dedicated media:</b><p>

An empty DVD can typically hold about 7-10 error correction files from 
same-sized media when using the default <a href="example93.html#redundancy">redundancy</a>.

So one could store 9+1 DVDs (9 carrying actual data and 1
used for the error correction files) using common 10 pack boxes. However:<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td>
<td>Avoid storing the error correction files on media coming from the same 
batch as those holding the data you are going to protect.
</td></tr></table><p>

Media from the same production run have similar aging characteristics,
so they may all start to fail at the same time. That is not good as
we want the error correction files to outlast the other media.<p>

Since even media with different labels printed on them can actually come from
the same production, it is a good idea to use different media types:

DVD-R and DVD+R media as well as media classified for different writing speeds
(e.g. 8x and 16x media) are very likely to come from different
production lines and have been manufactured at different times.<p>

<b>2. Putting the error correction file on the next medium of a series:</b><p>

If you do not fill your DVDs to the max (e.g. with less than 4GB for single layered
media), you can store the error correction file of one medium
on the succeeding medium within a series.<p>

<b>3. Using (external) hard discs for storing the error correction files:</b><p>

A 400GB hard disc can hold approximately 600 error correction files
(assuming full 4.7GB media and default <a href="example93.html#redundancy">redundancy</a>). 
This might be an alternative in price if you
keep in mind that current ATA hard discs are not built for eternity, either ;-) 

EOF
}

# ----- Impressum

function imprint_contents_en()
{  local file="$1.html"
   local query=$2
   local page=$3
   local lang=$4

   case $query in
   title)   title="Imprint" ;;
 
   link)    link_title="Imprint" ;;

   content*) eval "imprint$page$lang $file"
	    return 0
	    ;;
   esac 
}


function imprint0en()
{  cat >> $1 <<EOF

   <h3>Imprint</h3>

   Carsten Gn�rlich <br>
   Im Hagen 10 <br>
   59514 Welver / Germany<br>
   Tel.: +49 2384 941174<pre> </pre>
   E-Mail: carsten@dvdisaster.de<br>
   Internet: www.dvdisaster.de

   <pre> </pre>

   Responsible for the website content under � 10 MDStV: <p>
   Carsten Gn�rlich (see above for address).

   <pre> </pre>

   This imprint covers the web sites under<br>
   <a href="http://www.dvdisaster.com">www.dvdisaster.com</a>, &nbsp;
   <a href="http://www.dvdisaster.de">www.dvdisaster.de</a>, &nbsp;
   <a href="http://www.dvdisaster.org">www.dvdisaster.org</a><br>
   and under the subdomain <a href="http://dvdisaster.berlios.de">dvdisaster.berlios.de</a>.

EOF
}