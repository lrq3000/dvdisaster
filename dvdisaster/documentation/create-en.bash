#! /bin/bash

#  dvdisaster: English homepage translation
#  Copyright (C) 2004-2006 Carsten Gnörlich
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
   trans_to_berlios="Summary (at BerliOS)"
   trans_to_sourceforge="Summary (at SourceForge.net)"

   trans_back="Back"

   trans_contents="Contents"
   trans_hosting="Hosted by"

   trans_fdl="Verbatim copying and distribution of this entire article is permitted in any medium, provided this notice is preserved."
   trans_copyright="Copyright 2004-2006 Carsten Gnörlich."
   trans_modified="Information in this handbook relates to the original version of dvdisaster and may not be applicable."
}


# ----- These are the "inlined" html pages.

function inline_contents_en()
{  title="Image page"
}

function inline1en()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Picture: Badly scratched medium</h3><p></td></tr>
<tr align="center"><td>
<center><img src="../images/scratch-img.jpg" alt="scratched medium read"></center>
<br clear="all">
</td></tr>
<tr><td>
This medium has been carried around without protection in a backpack;
the round prints were made by a bottle of water. The scratches are
a bit exaggerated due to reflections from the photoflash.</td></tr>
EOF
}


function inline2en()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Figures: Error analysis</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/scratch-scan.png" alt="scratched medium scan"></center>
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
</td></tr>
<tr align="center"><td>
<center><img src="images/scratch-corr.png" alt="scratched medium recovery"></center>
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
when the <a href="example83.html#redundancy">standard settings</a> are used.
Exceeding this line means that the medium contents can not be fully recovered.

</td></tr>
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
<li>dvdisaster creates <b>error correction data</b> to compensate
read errors which are not correctable in the CD/DVD drive.<p></li>
<li>dvdisaster tries to read as much data as possible from defective media.
Afterwards unreadable sectors are recovered using the previously created
error correction data. The maximum error correction capacity is user-selectable.<p>
</li>
<li>dvdisaster operates at the <a href="background20.html">image level</a> 
and does not depend on the file system.<p></li>
</ul>

If you create the error correction data in time and keep it at a safe place,
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
During the <a href="example40.html">recovery</a> of the medium image
(using <a href="example83.html#redundancy">standard settings</a>)
a maximum of 22 errors per <a href="background10.html">ECC block</a> is counted
which equals to a 69% load of the error correction under the applied settings.<p>


</td><td width=110>
<a href="inline1.html">
<img src="../images/scratch-img.jpg" alt="scratched medium" width=100 height=97></a>
<br clear="all">
<a href="inline2.html">
<img src="images/scratch-scan.png" alt="reading scratched medium" width=100 height=74></a>
<br clear="all">
<a href="inline2.html">
<img src="images/scratch-corr.png" alt="repairing scratched medium" width=100 height=74></a>
</td></tr></table>

<a href="index20.html">Why PI/PO scans won't suffice...</a>
EOF
}


function index20en()
{  cat >> $1 <<EOF
<h3>dvdisaster as a complement to PI/PO scans</h3>

<a href="qa.html#pipo">PI/PO scans</a> are a valueable tool to assess and optimize
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
<li>Error correction data <b>must be created before the medium fails</b>, 
preferably at the same time the medium is written.<p></li>
<li>Error correction data requires <b>additional storage space</b> either on the protected 
medium or by using additional media. 
Using the <a href="example83.html#redundancy">standard settings</a> the additional
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

   link10)   link_title="Scanning media for errors" ;;

   link20)   link_title="Creating error correction data" ;;
   link21)   link_title="as a file" ;;
   link22)   link_title="within the image" ;;

   link30)   link_title="Reading defective media" ;;
   link40)   link_title="Recovering media images" ;;
   link50)   link_title="Getting information about images and error correction data" ;;

   link80)   link_title="Configuration" ;;
   link81)   link_title="General" ;;
   link82)   link_title="Reading" ;;
   link83)   link_title="Error correction/files" ;;
   link84)   link_title="Error correction/images" ;;

   link90)   link_title="Command syntax" ;;

   content*) eval "example$page$lang $file" ;;
   esac 
}

function example0en()
{  cat >> $1 <<EOF
<h3>Which topics are you interested in?</h3>

<table>
<tr>
<td valign="top">
  <a href="example10.html"><b>Scanning media for errors</b></a><br>
  The medium scan analyzes the
  reading speed and readability.
</td>
<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
<td valign="top">
 <a href="example20.html"><b>Creating error correction data</b></a><br>
Error correction data can be appended to an image
or exist as a separate file.
</td>
</tr>
<tr>
<td>
<a href="example1.html">
  <img src="images/ex-scan.png" alt="Media scan" border=0 width=275>
</a>
</td>
<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
<td>
<a href="example3.html#ecc">
  <img src="images/ex-create.png" alt="Ecc data generation" border=0 width=264>
</a>
</td>
</tr>

<tr><td colspan=3>&nbsp;</td></tr>

<tr>
<td valign="top">
  <a href="example30.html"><b>Reading defective media</b></a><br>
  An optimized <a href="background50.html">reading strategy</a>
  extracts data from defective media.
</td>
<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
<td>
  <a href="example40.html"><b>Recovering media images</b></a><br>
  Unreadable sectors are reconstructed  from
  <a href="example20.html">error correction data</a>.
</td>
</tr>
<tr>
<td>
<a href="example4.html">
  <img src="images/ex-read-a.png" alt="Reading defective media" border=0 width=264>
</a>
</td>
<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
<td>
<a href="example5.html">
  <img src="images/ex-fix.png" alt="Repairing defective media" border=0 width=264>
</a>
</td>
</tr>
</table>

<pre> </pre>

Additional topics: 

<ul>
<li><a href="example50.html">Getting information about image and error correction data</a><p></li>

<li><a href="example80.html">Configuration (graphical interface)</a><p></li>

<ul>
<li><a href="example81.html">General settings</a><p></li>
<li><a href="example82.html">Reading preferences</a><p></li>
<li><a href="example83.html">Settings for creating error correction files</a><p></li>
<li><a href="example84.html">Settings for augmenting images with error correction data</a><p></li>
</ul>

<li><a href="example90.html">Command line syntax</a><p></li>

<li><a href="background70.html">Hints for storing the error correction files</a></li>
</ul>
EOF
}


function example1en()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Screenshot: Scanning the medium for errors</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-scan.png" alt="screen shot"></center>
<br clear="all">
</td></tr>
<tr><td>

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
<a href="example40.html">recovering the medium image</a> 
and transfer it onto a new storage medium. 
</td></tr></table><p>
</li>

<li>Clicking on the
<img src="images/btn-protocol.png" alt="Protocol" align="middle"> symbol
provides additional information on the reading process.<p></li>
</ul>

</td></tr>
EOF
}

function example10en()
{  
  create_inline example en 1 example10.html "to scanning the medium"

cat >> $1 <<EOF
<h3>Scanning the medium for errors</h3>

To check your medium for read errors carry out these steps:<p>

(Note: Under Windows 2000 and XP either <b>administrator priviledges</b> or an installed
<a href="qa10.html#aspi">ASPI</a> driver are required in order to access the CD/DVD drives.)<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>Insert the medium into the drive.</td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>Select the drive.</td>
<td><img src="../images/btn-drive.png" alt="drive selection"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3a.</td>
<td>only when error correction files are used (RS01):<p>
If a suitable error correction file is available for your medium,
enter its file name. Scanning does not necessarily need an error correction file, 
but it can take advantage of some information from it.</td>
<td><img src="../images/btn-eccfile.png" alt="ecc file selection"><p>
The <img src="../images/open-ecc.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>
 
<tr valign="top" $BGCOLOR2>
<td>3b.</td>
<td>only when augmented images are used (RS02):<p>
Choose "ECC/RS02" in the settings for getting the image size.<p>
</td>
<td><a href="example81.html#imagesize"><img src="images/prefs-general.png" alt="Screenshot" width="200"></a></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>4.</td>
<td>Click on the "Scan" button to begin the medium check.</td>
<td><img src="images/btn-scan.png" alt="scan button"></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>5.</td>
<td>Watch the scanning progress in the graphical representation.</td>
<td><a href="example1.html"><img src="images/ex-scan.png" alt="screen shot" width="200"></a><p>
</td>
</tr>
</table>

<p>

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

<hr><p>

<a name="cli"></a>
<h3>Scanning the medium using the command line</h3>

Scanning requires the following parameters (defaults are given in parenthesis):

<table>
<tr>
<td><a href="example90.html#scan">-s / --scan</a></td>
<td>Scan</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a>&nbsp;</td>
<td>Drive selection (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>only for <a href="background30.html">RS01</a>: Error correction file (medium.ecc)</td>
</tr>
<tr>
<td><a href="example90.html#jump">-j / --jump</a></td>
<td>Skip sectors after a read error (16)</td>
</tr>
</table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -s -d /dev/hdc -e corrdata.ecc</b><br>
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
as these depend on the drive's firmware. 
EOF
}

function example20en()
{  
   cat >> $1 <<EOF
<h3>Creating the error correction data</h3>

The error correction data has an important role in dvdisaster:<br>
It is required to recover unreadable medium sectors.<p>

Error correction data can be either kept in a separate
<a href="background30.html#file">error correction file</a>
or be incorporated <a href="background30.html#image">into the image</a>,
with respective <a href="background30.html#table">advantages and disadvantages</a>.<p>

As a short decision help:<p>

<b>Do you need error correction data for an existing medium?</b><p>

&nbsp; Yes: <a href="example21.html">Create an error correction file.</a><p>

&nbsp; No: see next question.<p>

<b>Does the medium which is to be created contain at least 20% free space?</b><p>

&nbsp; Yes: <a href="example22.html">Augment the image with error correction data.</a><p>

&nbsp; No: <a href="example21.html">Create an error correction file.</a><p>
EOF
}

function example2en()
{  cat >> $1 <<EOF

<tr align="center"><td><h3>Screenshot: Creating a medium image</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-read.png" alt="screen shot"></center>
<br clear="all">
</td></tr>

<tr><td>
The reading progress will be shown in the graphical representation.<p>

<ul>
<li>The <b>spiral</b> provides a (not drawn to scale) 
overview of the <b>medium state</b>.
Red marks indicate the presence of read errors!<p></li>

<li>
The error correction file can <i>not be generated</i>
if the image is incomplete or contains unreadble sectors.
Generate the error correction file immediately after writing the medium.
<p></li>

<li>Clicking on the
<img src="images/btn-protocol.png" alt="Protokoll" align="middle"> symbol
provides additional information on the reading process.</li>
</ul>

</td></tr>
EOF
}

function example3en()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Screen shot: Creating the error correction file</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-create.png" alt="screen shot"></center>
<br clear="all">
</td></tr>

<tr><td>
The progress will be shown using a percentage reading. 
Processing a single layered DVD image with the 
default <a href="example83.html#redundancy">redundancy</a>
takes about 10 minutes on an average 2Ghz system.<p>

<b>Please note</b>:

<ul>
<li>The image file is no longer
required when the error correction file has been successfully created.<br>
dvdisaster will automatically overwrite an existing image file if a different
medium is read in.<p></li>

<li>
Always keep the error correction file on a 
<a href="background70.html">reliable storage medium</a>!<p>
</li>

<li>From now on you should regularly scan the medium for read errors.</li>
</ul>
</td></tr>

EOF
}

function example7en()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Screen shot: Augmenting an image with error correction data</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-create-rs02.png" alt="screen shot"></center>
<br clear="all">
</td></tr>

<tr><td>
The encoding speed depends on the free space left on the medium.
Processing a single layered DVD image will take
about 10 to 20  minutes on an average 2Ghz system.<p>

<b>Please note</b>:

<ul>
<li>From now on you should regularly scan the medium for read errors.</li>
</ul>
</td></tr>

EOF
}

function example21en()
{  create_inline example en 2 example21.html "to creating the error correction file"
   create_inline example en 3 example21.html "to creating the error correction file"
 
   cat >> $1 <<EOF
<h3>Creating the error correction file</h3>

<b>First create a medium image on the hard drive:</b><p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>Insert the medium into the drive.</td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>Select the drive.</td>
<td><img src="../images/btn-drive.png" alt="Drive selection"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Select the image file name.

Make sure to have enough free disk space for the image. Activate the
<a href="example81.html#localfiles">split file option</a> if your file system 
does not support files larger than 2GB.<p>
</td>
<td><img src="../images/btn-image.png" alt="image file selection"><p>
The <img src="../images/open-img.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Click on the "Read" button to start the reading process.</td>
<td><img src="images/btn-read.png" alt="Read button"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>Watch the reading progress in the graphical representation.</td>
<td><a href="example2.html"><img src="images/ex-read.png" alt="screen shot" width="200"></a><p>
</td>
</tr>
</table>

<pre> </pre>

<b>Now create the error correction file:</b><p>
The image must not contain any read errors.<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Choose the image file.
The previously read image file is pre-selected.<p>
</td>
<td><img src="../images/btn-image.png" alt="image file selection"><p>
The <img src="../images/open-img.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Select the error correction file name.<p>
</td>
<td><img src="../images/btn-eccfile.png" alt="ecc file selection"><p>
The <img src="../images/open-ecc.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>
Configure the error correction.<p>
Choose "Storage method: Error correction file (RS01)."<p>
</td>
<td><a href="example83.html"><img src="images/prefs-ecc.png" alt="screen shot" width="200"></a></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Click on the "Create" button to generate the error correction file.</td>
<td><img src="images/btn-create.png" alt="Create button"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>Watch the progress of the error correction file generation.</td>
<td><a href="example3.html"><img src="images/ex-create.png" alt="screen shot" width="200"></a><p>
</td>
</tr>
</table><p>

Keep the error correction file on a 
<a href="background70.html">reliable storage medium</a>.
Scan the protected medium for read errors on a regular basis.<p>

<hr><p>

<h3>Creating the error correction file using the command line</h3>

You will need the following parameters for reading the image
(defaults are given in parenthesis):

<table>
<tr>
<td><a href="example90.html#read">-r/ --read</a></td>
<td>Read</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a>&nbsp;</td>
<td>Drive selection (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
</table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r -d /dev/hdc -i image.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
This software comes with &nbsp;ABSOLUTELY NO WARRANTY. &nbsp;This<br>
is free software and you are welcome to redistribute it<br>
under the conditions of the GNU GENERAL PUBLIC LICENSE.<br>
See the file "COPYING" for further information.<p>

Device: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Medium: DVD+R, 2224288 sectors, 1 layer(s)<p>

Creating new image.iso image.<br>
Waiting 5 seconds for drive to spin up...<br>
Read position: 100.0% ( 6.3x)<br>
All sectors successfully read.
</td></tr></table><p>

The percentage of already read sectors and the current drive speed
will be shown during the process. When reading is finished,
create the error correction file:<p>

<table>
<tr>
<td><a href="example90.html#create">-c/ --create</a></td>
<td>Create the error correction file</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>Error correction file (medium.ecc)</td>
</tr>
<tr>
<td><a href="example90.html#redundancy">-n / --redundancy</a>&nbsp;</td>
<td>Choose redundancy (32 roots = 14.3%)</td>
</tr>
</table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -c -i image.iso -e corrdata.ecc </b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Opening image.iso: 2224288 medium sectors.<br>
Scanning image sectors: 100%<br>
Encoding with Codec RS01: 32 roots, 14.3% redundancy.<br>
Ecc generation: 100.0%<br>
Error correction file "medium.ecc" created.<br>
Make sure to keep this file on a reliable medium.<br>
</td></tr></table><p>

The progress will be shown using a percentage reading.<p>

When finished, the error correction file <i>medium.ecc</i> appears in the current directory:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>ls -lA</b><br>
-rwx------    1 user     linux    653721680 2004-07-02 22:45 corrdata.ecc<br>
-rwx------    1 user     linux    4555341824 2004-07-02 21:31 image.iso<br>
</td></tr></table><p>

You can remove the image file now, <p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>rm -f image.iso</b><br>
</td></tr></table><p>

but keep the error correction file on a <a href="background70.html">reliable storage medium</a>!<p>

<u>Hint:</u> Combine the actions for reading the image
and for creating the error correction file. This will save the image scan in the 
second step:<p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-r -c</b> -d /dev/hdc -i image.iso -e corrdata.ecc<br>
(and so on)
</td></tr></table>
EOF
}

function example22en()
{  create_inline example en 7 example22.html "to creating the error correction data"
 
   cat >> $1 <<EOF
<h3>Augmenting the image with error correction data</h3>

Using the <a href="background30.html#image">RS02 error correction</a> method
error correction data can be stored on the same medium with the protected data.<br>

This requires the image to be processed with dvdisaster prior to writing it on the medium.

Create the image on hard disk using your favourite CD/DVD writing software. 
Then use dvdisaster to augment the image with error correction information 
and finally write the augmented image to the medium:<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Use your favourite CD/DVD writing software 
to create an ISO or UDF image on your hard disk. 
Using other image types may lead to data loss!<p>
<p></td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Choose the image file you have created.<p>
</td>
<td><img src="../images/btn-image.png" alt="image file selection"><p>
The <img src="../images/open-img.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>
Configure the error correction.<p>
Choose "Storage method: Augmented image (RS02)."<p>
</td>
<td><a href="example84.html"><img src="images/prefs-rs02-0.png" alt="screen shot" width="200"></a></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Click on the "Create" button to generate the error correction data.</td>
<td><img src="images/btn-create.png" alt="Create button"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>Watch the progress of the error correction file generation.</td>
<td><a href="example7.html"><img src="images/ex-create-rs02.png" alt="screen shot" width="200"></a><p>
</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>6.</td>
<td>Use your CD/DVD writing software to transfer the augmented image to the medium.</td>
<td></td>
</tr>
</table><p>

<pre> </pre>

<h3>Augmenting the image with error correction data (using the command line)</h3>

<b>Create an image of the data to be protected.</b><p>

Use your favourite CD/DVD writing software to create an ISO or UDF image 
on your hard disk. 
Using other image types may lead to data loss!<p>

<b>Append the error correction data to the image.</b><p>

This requires the following parameters (defaults are given in parenthesis):

<table>
<tr>
<td><a href="example90.html#create">-c/ --create</a></td>
<td>Create the error correction data</td>
</tr>
<tr>
<td><a href="example90.html#method">-m / --method</a>&nbsp;</td>
<td>Select the error correction method</td>
</tr>
<tr>
<td><a href="example90.html#redundancy">-n / --redundancy</a></td>
<td>maximum medium size in sectors (size of smallest possible medium)</td>
</tr>
</table><p>

When you have created the image "image.iso" using the CD/DVD writing software, enter:<p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -c -mRS02 -i image.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Opening /dvd/image.iso: 284234 medium sectors.<p>

Augmending image with Method RS02: 555M data, 143M ecc (52 roots; 25.6% redundancy)<br>
Calculating sector checksums: 100%<br>
Expanding image: 100%<br>
Ecc generation: 100.0%<br>
Image has been appended with error correction data.<br>
New image size is 357818 sectors.
</td></tr>
</table><p>

<b>Write the image to the medium.</b><p>

Use your CD/DVD writing software to transfer the augmented image to the medium.<p>

Your writing software may not be compatible with the augmented
image and destroy the error correction information. At least when using it the first time,
perform the following check:<p>

Re-read the image with dvdisaster after burning the medium:<p>

<table>
<tr>
<td><a href="example90.html#read">-r/ --read</a></td>
<td>Read</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a>&nbsp;</td>
<td>Drive selection (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
</table><p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -r -d/dev/hdc -i image-new.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Device: /dev/hdc, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Medium: Data CD Mode 1, 357818 sectors, 1 layer(s)<p>

Creating new image-new.iso image.<br>
Waiting 5 seconds for drive to spin up...<br>
Read position: 100.0% ( 6.3x)<br>
All sectors successfully read.
</td></tr>
</table><p>

Check the image:<p>

<table>
<tr>
<td><a href="example90.html#test">-t/ --test</a></td>
<td>Check the image</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
</table><p>

<table class="example" width="100%" bgcolor="#ffffe0"><tr><td>
user@linux&gt; <b>dvdisaster -t -i image-new.iso</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

dvd/new-image.iso: present, contains 357818 medium sectors.<br>
- good image       : all sectors present<br>
- data md5sum      : 4eca3615a88a11ba68466226295c4bcb<p>

Error correction data: created by dvdisaster-0.66<br>
- method           : RS02, 52 roots, 25.6% redundancy.<br>
- requires         : dvdisaster-0.65 (good)<br>
- data md5sum      : 4eca3615a88a11ba68466226295c4bcb (good)<br>
- crc md5sum       : 9776489c12e12cbabfe5145da4f6ae66 (good)<br>
- ecc md5sum       : e975aed39c962eeabedd9d6cbdbfc45e (good)
</td></tr>
</table>
EOF
}

function example4en()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Screen shot: Reading a defective medium</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-read-a.png" alt="screen shot"></center>
<br clear="all">
</td></tr>

<tr><td>
Please note that reading a damaged medium may take several hours to complete.
</td></tr>
EOF
}

function example30en()
{  create_inline example en 4 example30.html "to reading the defective medium"
 
   cat >> $1 <<EOF
<h3>Reading a defective medium</h3>

dvdisaster contains a <a href="background50.html">reading strategy</a>
which is especially suited for reading damaged media:<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>Insert the medium into the drive.</td>
<td></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>Select the drive.</td>
<td><img src="../images/btn-drive.png" alt="drive selection"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>
Select the image file name.<p>
</td>
<td><img src="../images/btn-image.png" alt="image file selection"><p>
The <img src="../images/open-img.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>
Select the error correction file.
The error correction file must be available at this stage so that dvdisaster can determine
when enough data for reconstructing the image has been collected.<p>
</td>
<td><img src="../images/btn-eccfile.png" alt="ecc file selection"><p>
The <img src="../images/open-ecc.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>5.</td>
<td>
Select the adaptive reading strategy.<p>
</td>
<td><a href="example82.html"><img src="images/prefs-read.png" alt="screenshot" width="200"></a></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>6.</td>
<td>Start the reading process by clicking at the "Read" button.</td>
<td><img src="images/btn-read.png" alt="Read button"></td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>7.</td>
<td>Watch the reading progress.</td>
<td><a href="example4.html"><img src="images/ex-read-a.png" alt="screenshot" width="200"></a><p>
</td>
</tr>
</table><p>

If the reading process states that enough data has been collected for a
successful recovery, <a href="example40.html">repair</a> the missing sectors</a> 
using the <a href="example20.html">error correction data</a>.<p>

Otherwise,  try reading the image 
again <a href="background60.html#reading-tips">(some hints for re-reading)</a>.
In that case dvdisaster will try to re-read only the missing sectors.

<hr><p>

<h3>Reading damaged media from the command line</h3>

This requires the following parameters (defaults are given in parenthesis):

<table>
<tr>
<td><a href="example90.html#read">-r/ --read</a></td>
<td>Read</td>
</tr>
<tr>
<td><a href="example90.html#adaptiveread">--adaptive-read</a></td>
<td>Use reading strategy for damaged media</td>
</tr>
<tr>
<td><a href="example90.html#device">-d / --device</a>&nbsp;</td>
<td>Drive selection (/dev/cdrom)</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>only for <a href="background30.html">RS01</a>: Error correction file (medium.ecc)</td>
</tr>
<tr>
<td><a href="example90.html#parse-ecc">--parse-ecc</a></td>
<td>only for <a href="background30.html">RS02</a>: Use information from Ecc header</td>
</tr>
<tr>
<td><a href="example90.html#jump">-j / --jump</a></td>
<td>Skip sectors after a read error (16)</td>
</tr>
</table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -r --adaptive-read -i image.iso -e corrdata.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Device: /dev/cdrom, &nbsp;ATAPI DVD+RW 8X4X12 B2K7<br>
Medium: DVD+R, 2224288 sectors, 1 layer(s)<p>

Adaptive reading: Trying to collect enough data for error correction.<br>
Creating new image.iso image.<br>
Repairable:  2.6% (correctable: 0; now reading [0..2224288], size 2224288)<br>
Sectors 57264-57279: Medium Error; Unrecovered read error.<br>
Filling image area [57280..1083504]<br>
[... more descriptions of reading process ...] <br>
Repairable: 100.0% (correctable: 319200; now reading [320304..327065], size 6762)<br>
Sufficient data for reconstructing the image is available.
</td></tr></table><p>
EOF
}

function example5en()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Screen shot: Recovering the medium image</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-fix.png" alt="screen shot"></center>
<br clear="all">
</td></tr>

<tr><td>
The repairing progress will be shown in percent.<p>

<ul>
<li>The message "Good! All sectors are repaired." confirms that the medium image
has been fully recovered. The repaired image can be written to a new
medium using a suitable CD/DVD recording software. <p></li>

<li>If uncorrectable sectors remain after this stage, try reading the image again
<a href="background60.html#reading-tips">(some hints for re-reading)</a>.
dvdisaster will try to re-read only the missing sectors, and this might
retrieve enough missing sectors for a successful error correction run.<p></li>

<li>The red error graph does <i>not</i> correlate with the spatial arrangement of 
read errors on the disc: The error correction method is explicitly built to achieve 
an equal distribution of medium read errors over all error correction blocks.<p>
The green line marks the maximum capacity of the error correction. If this line is exceeded,
the image can not be recovered with the currently available data.
In case of tight misses <a href="background60.html#reading-tips">another reading attempt</a>
 might help.
</li>

<li>When the image has been read in by the adaptive reading strategy,
the error correction load will always be maxed out.</li>
</ul>
</td></tr>
EOF
}

function example40en()
{  create_inline example en 5 example40.html "to recovering the medium image"

   cat >> $1 <<EOF
<h3>Recovering the medium image</h3>

To recover a defective medium you must first try to
<a href="example30.html">read as much data as possible from it</a>.
Then you need the <a href="example20.html">appropriate error correction data</a>
which you have hopefully generated at a time when the medium was still fully readable.<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Select the image file.<p>
</td>
<td><img src="../images/btn-image.png" alt="image file selection"><p>
The <img src="../images/open-img.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Select the respective error correction file.<p>
</td>
<td><img src="../images/btn-eccfile.png" alt="ecc file selection"><p>
The <img src="../images/open-ecc.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Click on the "Fix" button.</td>
<td><img src="images/btn-fix.png" alt="Fix button"></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Watch the recovery progress.<p>
It is normal to experience phases of high hard drive
activity during image reconstruction</td>
<td><a href="example5.html"><img src="images/ex-fix.png" alt="screen shot" width="200"></a><p>
</td>
</tr>
</table><p>

When the image has been successfully recovered, transfer it to a new medium
using a CD/DVD writing software.

<hr><p>

<h3>Recovering the image using the command line</h3>

Recovering requires the following parameters (defaults are given in parenthesis):

<table>
<tr>
<td><a href="example90.html#fix">-f/ --fix</a></td>
<td>Recover</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>Error correction file (medium.ecc)</td>
</tr>
</table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -f -i image.iso -e corrdata.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnoerlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

Opening image.iso: 2224288 medium sectors.<p>

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

EOF
}

function example6en()
{  cat >> $1 <<EOF

<tr align="center"><td>
<h3>Screenshot: Comparing image and error correction files</h3><p></td></tr>
<tr align="center"><td>
<center><img src="images/ex-compare.png" alt="screen shot"></center>
<br clear="all">
</td></tr>

<tr><td>

This process may take a while as both
files need to be read completely (but no changes are applied to them).<p>

When the comparison is finished, you get the following information:<p>

<ul>
<li><b>"Image file summary" and "Image state"</b>. 
The number of missing sectors will be above zero if the image contains
still uncorrected read errors. In contrast the appearance of checksum errors 
is usually the result of
<a href="qa20.html#crc">incorrect handling or hardware problems</a>.<p></li>

<li><b>"Error correction file summary"</b>.<br>
This field shows the settings used while creating the error correction file,
and whether it can be processed by your version of dvdisaster.
If the error correction file does not correspond to the image file
or if it is damaged, warnings are printed beneath the last four entries.<p></li>

<li><b>Hint:</b> The results of the compare are unreliable if the image has been read in
by a software other than dvdisaster. Especially, all missing sectors
will be classfied as checksum errors if a different software has been used.</li>
</ul>
</td></tr>
EOF
}

function example50en()
{  create_inline example en 6 example50.html "to getting information on images and error correction data"

   cat >> $1 <<EOF
<h3>Getting information about images and error correction data</h3>

This function provides information about image and error correction files
which are already present on your hard disc:<p>

<table width="100%" border="0" cellspacing="0" cellpadding="10">
<tr valign="top" $BGCOLOR1>
<td>1.</td>
<td>
Select the image file.<p>
</td>
<td><img src="../images/btn-image.png" alt="image file selection"><p>
The <img src="../images/open-img.png" alt="filechooser button" align="middle"> symbol opens the file chooser.</td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>2.</td>
<td>
Select the respective error correction file.<p>
</td>
<td><img src="../images/btn-eccfile.png" alt="eccfile selection"><p>
The <img src="../images/open-ecc.png" alt="filechooser button" align="middle"> symbol opens the file chooser</td>
</tr>

<tr valign="top" $BGCOLOR1>
<td>3.</td>
<td>Click the "Compare" button.</td>
<td><img src="images/btn-compare.png" alt="compare button"></td>
</tr>

<tr valign="top" $BGCOLOR2>
<td>4.</td>
<td>Watch the comparison progress.</td>
<td><a href="example6.html"><img src="images/ex-compare.png" alt="screen shot" width="200"></a><p>
</td>
</tr>
</table><p>

<hr><p>

<h3>Getting the information from the command line</h3>

You will need the following parameters (defaults are given in parenthesis):

<table>
<tr>
<td><a href="example90.html#test">-t/ --test</a></td>
<td>Compare</td>
</tr>
<tr>
<td><a href="example90.html#image">-i / --image</a></td>
<td>Image file (medium.iso)</td>
</tr>
<tr>
<td><a href="example90.html#ecc">-e / --ecc</a></td>
<td>Error correction file (medium.ecc)</td>
</tr>
</table><p>

<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; <b>dvdisaster -t -i image.iso -e corrdata.ecc</b><br>
dvdisaster-${project_version} Copyright 2004-2006 Carsten Gnörlich.<br>
<i>[... remainder of the GPL announcement ...]</i><p>

/dvd/image.iso: present, contains 2224288 medium sectors.<br>
- good image       : all sectors present<br>
- image md5sum     : 55cdd507e8d96af0da0667ce4365c7ae<p>

/dvd/corrdata.ecc: created by dvdisaster-0.62<br>
- method           : RS01, 32 roots, 14.3% redundancy.<br>
- requires         : dvdisaster-0.55 (good)<br>
- medium sectors   : 2224288 (good)<br>
- image md5sum     : 55cdd507e8d96af0da0667ce4365c7ae (good)<br>
- fingerprint match: good<br>
- ecc blocks       : 20428800 (good)<br>
- ecc md5sum       : 4bdf5ed398e7662ac93c9d08e1ba9ff2 (good)
</td></tr></table><p>
EOF
}

function example80en()
{  cat >> $1 <<EOF

<h3>Configuration</h3>

Press the
<img src="../images/open-preferences.png" align="middle" alt="preferences button"> button
at the upper right of the tool bar to open the configuration dialog.
The dialog contains several forms:<p>

<center>
<a href="example81.html">
<img src="images/prefs-general.png" alt="screen shot"><br>
General settings
</a>
</center>
<br clear="all">

<center>
<a href="example82.html">
<img src="images/prefs-read.png" alt="screen shot"><br>
Reading preferences
</a>
</center>
<br clear="all">

<center>
<a href="example83.html">
<img src="images/prefs-ecc.png" alt="screen shot"><br>
Settings for creating error correction files
</a>
</center>
<br clear="all">

<center>
<a href="example84.html">
<img src="images/prefs-rs02.png" alt="screen shot"><br>
Settings for augmenting images with error correction data
</a>
</center>


EOF
}

function example81en()
{  cat >> $1 <<EOF

<h3>General Configuration</h3>

More information on this form follows down this page.<p>

<center><img src="images/prefs-general.png" alt="screen shot"></center>
<br clear="all">

<p><hr><p>

<a name="imagesize"></a>
<b>Medium and Image filesystem</b><p>

<center><img src="images/prefs-general-1.png" alt="screen shot"></center>
<br clear="all">

These options work around some problems caused by drives
<a href="qa20.html#plusrw">reporting incorrect image lengths</a>
with DVD-RW/+RW media.<p>

<b>Use information from Ecc headers <font color="red">(1)</font>:</b>
The image size will be determined from the headers of the
<a href="background30.html">RS02</a> error correction information.
If the image does not contain RS02 error correction data, the start of the reading
process will be delayed significantly by setting this option.<p>

<b>Use information from ISO/UDF file system <font color="red">(2)</font>:</b>
dvdisaster will determine the image size from the ISO/UDF file system
if this option is checked. If the image contains
<a href="background30.html">RS02</a> error correction information you must
also set option <font color="red">(1)</font>. Otherwise the 
error correction data will not be included into the image.


<p><hr><p>

<a name="localfiles"></a>
<b>Setting for local files</b><p>
<center><img src="images/prefs-general-2.png" alt="screen shot"></center>
<br clear="all">
 
<b>Automatic file name extension<font color="red">(1)</font>:</b>
When this switch is set, files will be automatically appended
with ".iso" or ".ecc" suffixes if no other file name extension is already present.<p>

<b>Split file into segments <font color="red">(2)</font>:</b>
Allows working with file systems which are limited to 2GB per file
(e.g. FAT from Windows). Created files are spread over upto 100 segments called 
"medium00.iso", "medium01.iso" etc. at the cost of a small performance hit.<p>

<p><hr><p>

<a name="auto"> </a>
<b>Automatic file creation and deletion</b><p>

<center><img src="images/prefs-general-3.png" alt="screen shot"></center>
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

function example82en()
{  cat >> $1 <<EOF

<h3>Reading preferences</h3>

More information on this form follows down this page.<p>

<center><img src="images/prefs-read.png" alt="screen shot"></center>
<br clear="all">

<p><hr><p>

<a name="read"></a>
<b>Reading preferences</b><p>

<center><img src="images/prefs-read-1.png" alt="screen shot"></center>
<br clear="all">

<b>Reading strategy <font color="red">(1)</font>:</b> Choose between the
<a href="background40.html">linear reading strategy</a> and the
<a href="background50.html">adaptive reading strategy</a>.
The linear strategy is suited for undamaged media while the adaptive
strategy is better for media already containing read errors.<p>

<b>Reading range <font color="red">(2)</font>:</b> Reading will be limited to the given range 
of sectors, including the borders: 0-100 will read 101 sectors. The settings are
only effective for the current session and will not be saved.<p>

<b> Skip x sectors after read error / <br>
Stop reading when unreadable intervals < x <font color="red">(3)</font>:</b><br>
The slider value affects the handling of read errors. It has slightly different
effects for the <a href="background40.html#configure">linear</a> and 
<a href="background50.html#configure">adaptive</a> reading strategies.
However large values reduce the processing time and the mechanical wear
on the drive, but will also leave larger gaps in the image when reading defective areas.

<p><hr><p>

<a name="image"></a>
<b>Image properties</b><p>

<center><img src="images/prefs-read-2.png" alt="screen shot"></center>
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

<center><img src="images/prefs-read-3.png" alt="screen shot"></center>
<br clear="all">
Waits the given amount of seconds for the drive to spin up before the real
reading process starts. This avoids speed jumps at the beginning of the
reading curve.
EOF
}

function example83en()
{  cat >> $1 <<EOF

<h3>Error correction settings</h3>

More information on this form follows down this page.<p>

<center><img src="images/prefs-ecc.png" alt="screen shot"></center>
<br clear="all">

<p><hr><p>

<a name="redundancy"></a>
<b>Redundancy for new error correction files</b><p>

<center><img src="images/prefs-ecc-1.png" alt="screen shot"></center>
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

<center><img src="images/prefs-ecc-2.png" alt="screen shot"></center>
<br clear="all">

dvdisaster optimizes access to the image and error correction files by maintaining 
its own cache. The preset of 32MB is suitable for most systems.<p>

EOF
}

function example84en()
{  cat >> $1 <<EOF

<h3>Error correction settings</h3>

More information on this form follows down this page.<p>

<center><img src="images/prefs-ecc.png" alt="screen shot"></center>
<br clear="all">

<p><hr><p>

<a name="redundancy"></a>
<b>Redundancy for new error correction files</b><p>

<center><img src="images/prefs-ecc-1.png" alt="screen shot"></center>
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

<center><img src="images/prefs-ecc-2.png" alt="screen shot"></center>
<br clear="all">

dvdisaster optimizes access to the image and error correction files by maintaining 
its own cache. The preset of 32MB is suitable for most systems.<p>

EOF
}

function example90en()
{  cat >> $1 <<EOF
<h3>Command syntax</h3>

<b>Using the command line.</b> 
dvdisaster will work in command line mode if it is called with one of the
<a href="#modes">actions</a> described below (read, create, scan, fix, test). 
Otherwise a window for the graphical user interface will be opened.<p>

Command line mode ignores any settings from the graphical interface and the
<i>.dvdisaster</i> file. Likewise, the command line options described here
have no effect in the graphical user interface.<p>

For technical reasons there are two program versions for Windows:<p>
<table>
<tr><td><i>dvdisaster.exe</i></td><td>--</td><td>provides the command line interface</td></tr>
<tr><td><i>dvdisaster-win.exe</i></td><td>--</td><td>contains the graphical user interface</td></tr>
</table><p>

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
<tr valign=top><td></td><td><a href="#ecc">--ecc</a></td><td>Name of error correction file<p></td></tr>
<tr><td></td><td><a href="#list">--list</a></td><td>shows drives available under ASPI (Windows version only)</td></tr>
</table><pre> </pre> 

<b><a name="options">Other Options.</a></b> These less frequently used options are only available in the
long form unless noted otherwise.<p>

<table>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#adaptiveread">--adaptive-read</a></td><td>use adaptive strategy for reading damaged media</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#autosuffix">--auto-suffix</a></td><td>automatically add .iso and .ecc file suffixes</td></tr>
<tr valign=top><td>&nbsp; &nbsp;</td><td><a href="#cache">--cache-size</a> &nbsp; &nbsp;</td><td>Cache size during creation of the error correction file</td></tr>
<tr valign=top><td></td><td><a href="#dao">--dao</a></td><td>Assumes "disk at once" medium</td></tr>
<tr valign=top><td></td><td><a href="#fillunreadable">--fill-unreadable [n]</a></td><td>fill unreadable sectors with given byte</td></tr>
<tr valign=top><td></td><td><a href="#jump">-j / --jump</a></td><td>Skip sectors after a read error</td></tr>
<tr valign=top><td></td><td><a href="#parse-ecc">--parse-ecc</a></td><td>Use information from error correction headers</td></tr>
<tr valign=top><td></td><td><a href="#parse-udf">--parse-udf</a></td><td>Use information from ISO/UDF filesystem</td></tr>
<tr valign=top><td></td><td><a href="#redundancy">-n / --redundancy</a></td><td>Set error correction code redundancy</td></tr>
<tr valign=top><td></td><td><a href="#method">-m / --method</a>&nbsp; &nbsp;</td><td>Select error correction method</td></tr>
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

--read will use the <a href="background40.html">linear reading strategy</a> by default.
Add the <a href="example90.html#adaptiveread">--adaptive-read</a> option
to employ the <a href="background50.html">adaptive reading strategy</a> instead.<p>

The range of sectors to be read in can be limited analogous to the
<a href="example90.html#scan">--scan</a> option. 

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="create"><b>--create: Create the error correction file</b></a><p>

Creates the error correction file for an image.<p>

<div align=right><a href="#modes">&uarr;</a></div><p>



<a name="fix"><b>--fix: Fix an image</b></a><p>

Try to fix an image using the error correction file.<p>

If not all sectors can be repaired during the first pass,
try reading the image again with another <a href="#read">--read</a> pass
<a href="background60.html#reading-tips">(some hints for re-reading)</a>.
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

<b>FreeBSD:</b><br>
<table class="example-bsd" width=100%><tr><td>
user@freebsd&gt; dvdisaster <b>-d /dev/pass0</b> -r
</td></tr></table><p>

dvdisaster supports drives which are accessible through 
the passthrough driver <tt>/dev/pass*</tt> and which are MMC3 compliant. 
Access to ATAPI drives may require a <a href="download20.html#freebsd">kernel recompile</a>.<p>

<b>Linux:</b><br>
 
<table class="example" width=100% bgcolor=#ffffe0><tr><td>
user@linux&gt; dvdisaster <b>-d /dev/hdc</b> -r
</td></tr></table><p>

dvdisaster supports drives which are accessible through 
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

<b>Windows:</b><br>
<table class="example-win" width=100% bgcolor=#e0e0ff><tr><td>
user@windows&gt; dvdisaster <b>-d E:</b> -r
</td></tr></table><p>

dvdisaster supports local drives implementing the MMC3 standard
and which are referenced through a drive letter in the system.


<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="prefix"><b>--prefix &lt;prefix&gt;: Prefix for image and error correction files</b></a><p>

The preset is "medium"; the image and error correction files will automatically receive the
endings ".iso" and ".ecc".

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="image"><b>--image &lt;file name&gt;: Name of image file</b></a><p>

The preset is "medium.iso"; the specified file name will be used exactly (without appending a suffix)
for the image file
as long as <a href="#autosuffix">--auto-suffix</a> is not specified also.

<div align=right><a href="#drive">&uarr;</a></div><p>

<a name="ecc"><b>--ecc &lt;file name&gt;: Name of error correction file</b></a><p>

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

<table class="example-win" width="100%" bgcolor="#e0e0ff"><tr><td>
user@windows&gt; dvdisaster <b>-d 2:</b> -r<br>
</td></tr>
</table>

<div align=right><a href="#drive">&uarr;</a></div><p>


<h3>Other Options.</h3>

<a name="adaptiveread"><b>--adaptive-read: Use adaptive strategy for reading damaged media</b></a><p>

This switch selects the <a href="background50.html">adaptive reading strategy</a>
which is particularly suited for reading in damaged media.
Use this switch in conjunction with <a href="#read">-r/--read</a> and with
<a href="#ecc">-e/--ecc</a>. Specifying the error correction file 
causes the reading process to finish as soon as enough information has been 
collected for recovering the medium image.
<p>

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="autosuffix"><b>--auto-suffix: automatically add .iso and .ecc file suffixes</b></a><p>

When this option is given, file names specified by the
<a href="#image">-i/--image</a>- or <a href="#ecc">-e/--ecc</a> option
will be automatically appended with ".iso" or ".ecc" respectively
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

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="jump"><b>-j / --jump &lt;number of sectors&gt;: Skip sectors after a read error </b></a><p>

This option has slightly different behaviour depending on the used reading strategy:
<ul>
<li>when using the <a href="background40.html#configure">linear reading strategy</a>
(default setting), 
the given number of sectors will be skipped after a read error.</li>
<li>the <a href="background50.html#configure">adaptive reading strategy</a> 
(<a href="#adaptiveread">--adaptive-read</a> option) will terminate
when no unreadable areas are left in the image which are larger than the given value.</li>
</ul> 

Large values reduce the processing time and the mechanical wear on the drive,
but will also leave larger gaps in the image when reading defective areas.
The number of skipped sectors must be a multiple of 16.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="parse-ecc"><b>--parse-ecc: Use information from error correction headers</b></a><p>
dvdisaster will determine the image size from the headers created by the 
<a href="background30.html">RS02 method</a> if this option is given. 
This works around some problems caused by drives
<a href="qa20.html#plusrw">reporting incorrect image lengths</a>
with DVD-RW/+RW media.<p>

Note: Use this option only for reading images which have been augmented with 
error correction data. Otherwise the reading process will be delayed significantly at the
beginning.

<div align=right><a href="#options">&uarr;</a></div><p>




<a name="parse-udf"><b>--parse-udf: Use information from ISO/UDF filesystem</b></a><p>
dvdisaster will determine the image size from the ISO/UDF file system
if this option is given. This works around some problems caused by drives
<a href="qa20.html#plusrw">reporting incorrect image lengths</a>
with DVD-RW/+RW media.<p>

Warning: Always use this option together with <a href="#parse-ecc">--parse-ecc</a>
to read images which have been augmented with error correction data by the
<a href="background30.html">RS02 method</a>. Otherwise the error correction data
will not be read.

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


<a name="method"><b>-m / --method &lt;m&gt;: Select error correction method</b></a><p>

Choose between the
<a href="background30.html">RS01</a> (default) 
and <a href="background30.html">RS02</a> method.

<div align=right><a href="#options">&uarr;</a></div><p>



<a name="speedwarn"><b>--speed-warning [n]: Warns when reading speed drops a certain amount</b></a><p>

Prints a warning when the reading speed changes by more than <i>n</i> percent.
This is useful for recognizing a beginning decay in media quality.
If the optional value [n] is omitted, a preset of 10 percent is used.

<div align=right><a href="#options">&uarr;</a></div><p>


<a name="spinup"><b>--spinup-delay [n]: Gives drive time to spin up</b></a><p>

Waits the given amount of seconds for the drive to spin up before the real
reading process starts.
This avoids spurious warnings about speed changes at the beginning of the medium
when the <a href="#speedwarn">--speed-warning</a> option is used.<br>

If the optional value [n] is not given, dvdisaster will wait for 5 seconds.

<div align=right><a href="#options">&uarr;</a></div><p>

<a name="split"><b>--split-files: Splits files into segments <= 2GB</b></a><p>

Allows the creation of large images and error correction files 
 on file systems which are limited to 2GB per file
(e.g. FAT from Windows). The files are spread over upto 100 segments "medium00.iso", "medium01.iso" etc. at the cost of a small performance hit.<p>

The file name must still be specified as
"medium.iso" using the <a href="#image">--image</a> option; 
the numbering will be automatically inserted when this switch is active.<p>

<div align=right><a href="#options">&uarr;</a></div><p>

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
<tr><td><b>dvdisaster-0.66</b></td><td align="right">25-Mar-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="${tar_0_66}">dvdisaster-0.66.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="${tar_0_66_sig}">dvdisaster-0.66.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="${setup_0_66}">dvdisaster-0.66.3-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="${setup_0_66_sig}">dvdisaster-0.66.3-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">

Images are checked against the error correction file during
the <a href="example10.html">Scan</a> function. Creation of files can
be continued after encountering and removing out-out-space conditions on hard disc.<p>

<i>New <a href="background30.html">error correction method</a> without 
error correction files:</i>
Error correction information can be appended directly to the image.
This method is still experimental and currently only available in the
command line. Please visit 
the <a href="http://sourceforge.net/cvs/?group_id=157550">CVS archive</a>
to follow development of the new method (requires some skills in building
programs from source).<p>

<b>Patches</b> (small changes after version 0.66; files above have been updated):<br>

<b>pl3</b> fixes incomplete installation of the user manual under some versions
of Windows. Other operating systems are not affected.
</td></tr></table><p>

<pre> </pre>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.65</b></td><td align="right">14-Jan-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="${tar_0_65}">dvdisaster-0.65.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="${tar_0_65_sig}">dvdisaster-0.65.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="${setup_0_65}">dvdisaster-0.65-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="${setup_0_65_sig}">dvdisaster-0.65-setup.exe.gpg</a></td></tr>
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
<a href="example81.html#imagesize">image size from the UDF/ISO file system</a>
to improve the
<a href="qa20.html#plusrw">image size recognition for -RW/+RW media</a>.
Reading images and creating the respective error correction file can now be invoked together
with a <a href="example81.html#auto">single mouse click</a>.
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
Contains an <a href="example30.html">improved strategy</a> for
<a href="example80.html#adaptiveread">reading defective media</a> 
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
 <li><b>x86 compatible</b> hardware in <b>32bit</b> mode 
     (see operating systems for 64bit support),<p></li>
 <li>an up-to-date CD or DVD drive with ATAPI or SCSI interface,<p></li>
 <li>at least a P4 at 2Ghz or comparable processor<p>
</ul>

<h3>Operating systems</h3>
<ul>
 <li><b>FreeBSD</b> version <b>6.0</b> or later<br>
     (using ATAPI drives requires a kernel recompile)<p>
 </li>
 <li><b>Linux</b> with kernel <b>2.6.7</b> or later, 64bit supported<br>
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
<i>[... after a moment the main window will open ...]</i>
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
Remedy: Activate <a href="example81.html#imagesize">"Use information from the ISO/UDF file system"</a>
</li></ul>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>only Data CD are supported.</li>
</ul>

<b>Not usable types</b> (image can not be extracted):<p>
CD-Audio and CD-Video as well as DVD-ROM and DVD-RAM.

<div align=right><a href="#top">&uarr;</a></div><p>


<b><a name="filesystem">2.3 Which file systems are supported?</a></b><p>

dvdisaster works exclusively on the <a href="background20.html">image level</a>
which is accessed sector-wise.
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

The <a href="example90.html#list">--list</a> function prints out
the current mapping of drive letters.<p>

<i>&gt;&nbsp; How to select between SPTI and ASPI?</i><p>

dvdisaster will automatically choose between SPTI and ASPI, 
deciding in favor of SPTI if both are available. 
Usage of ASPI can be forced; see the description of the
<a href="example90.html#list">--list</a> function for details.

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
<a href="#v40error">3.7 "Ecc file has been created with version 0.40.7."</a><p>

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
<a href="example90.html#dao">--dao</a> option or 
the <a href="example82.html#image">preferences tab for reading/scanning</a>.<p>

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
and error correction files and <a href="example50.html">compare
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
Activate the option  <a href="example81.html#imagesize">Use information from the
ISO/UDF file system</a> so that the image size will be determined from the
ISO/UDF file system.
</td></tr></table><p>

If the required ISO/UDF sectors are unreadable when trying to recover damaged media
there are two possible workarounds:

<ul>
<li>Execute the <a href="example50.html">"Compare"</a> function with only
the error correction file being selected/given. Note down the correct image size 
from the output and <a href="example82.html#read">restrict the
reading range</a> accordingly.
</li>
<li>Simply read in the image with the incorrect (larger) size.
When invoking the <a href="example40.html#repair">"Fix"</a> function,
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


<b><a name="v40error">3.7 "Ecc file has been created with version 0.40.7."</a></b><p>

The <a href="http://sourceforge.net/cvs/?group_id=157550">CVS versions</a>
of dvdisaster mark their ecc files with a special bit. This causes dvdisaster
versions upto 0.65 to falsely display the above error message. Please use the
CVS versions only together with dvdisaster 0.66 or newer versions.
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
   link20)   link_title="Image level data recovery" ;;
   link30)   link_title="RS01 and RS02" ;;
   link40)   link_title="Linear reading strategy" ;;
   link50)   link_title="Adaptive reading strategy" ;;
   link60)   link_title="Remarks on read errors" ;;
   link70)   link_title="Hints for storing the error correction files" ;;

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
<li><a href="background20.html">Image level data recovery</a><p></li>
<li><a href="background30.html">The RS01 and RS02 methods</a><p></li>
<li><a href="background40.html">Details of the linear reading strategy</a><p></li>   
<li><a href="background50.html">Details of the adaptive reading strategy</a><p></li>   
<li><a href="background60.html">Some remarks on read errors</a><p></li>   
<li><a href="background70.html">Hints for storing the error correction files</a><p></li>
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

Using the <a href="example83.html#redundancy">standard settings</a>
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
<a href="index10.html">Scratches</a> will cause the threshold to be reached earlier, 
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
It is possible to <a href="example83.html#redundancy">select other values</a>
for higher or lower error correction capabilities.
</font>
<p>

EOF
}

function background20en()
{  cat >> $1 <<EOF
<h3>Image level data recovery</h3>

Media recovery with error correcting codes takes place in two steps:

<ol>
<li>First as much data as possible is read from the defective medium.<p></li>
<li>Then the still missing data is recovered using the error correction code.</li>
</ol>

The amount of readable data (step 1) does not only depend on the reading capabilities
of the drive, but also on which logical level the reading process takes place.
This page discusses the logical levels and explains why dvdisaster uses image level reading.<p>

<b>Logical levels of a medium</b><p>

CD and DVD media are organized into <i>data sectors</i> containing 2048 bytes each.
Consecutively reading and storing these sectors produces a medium <i>image</i>.<p>

But working with single data sectors is unwieldy from a users perspective. Therefore
media contain <i>file systems</i> which combine data sectors into <i>files</i>.
This requires accurate book-keeping about which data sectors the files are comprised of,
and further attributes like file names and access permissions. For this book-keeping
some data sectors are reserved on the medium 
and filled with respective data structures.<p>

As a consequence media contain different <i>logical levels</i>:
Viewing the medium as a sequence of data sectors means working at the <i>image level</i>.
However looking at the medium as a collection of files is the perspective of
the <i>file(system) level</i>.<p>

The two levels have different properties when it comes to data recovery:<p>

<a name="file"> </a>
<b>Shortcomings of reading a medium at the file level</b><p>

Reading a defective medium at the <b>file level</b> means trying 
to read as much data as possible from each file.<p>

But a problem arises when data sectors are damaged which have
book-keeping functions in the file system. The list of files on the medium
may be truncated. Or the mapping of data sectors to files is incomplete.
Therefore files or parts from files may be lost even though the respective
data sectors would still be readable by the hardware. This is very bad
since even small readable portions of damaged files are valuable for
the error correcting code.<p>

An extremely bad case occurs when the error correction data is also
stored in files. Then the error correction data is required to repair the
file system, but the defective file system prevents access to the error
correction data. That means total data loss and rises some issues on the
<a href="#eccfile">treatment of error correction files</a> (more on that later).<p>

However the situation improves greatly when employing an image-based approach:<p>

<a name="image"> </a>
<b>Advantages of reading at the image level</b><p>

Reading at the image level uses direct communication with the drive hardware
to access the data sectors.<p>

The number of readable sectors depends only on the reading capabilities of the drive,
but not on the state of the file system. A read error in one sector does not
block access to other data sectors. Since <i>all</i> sectors are recovered 
which are still readable by the hardware, the method provides the best foundation
for the error correction.<p>

The image contains all data sectors of the medium. When the image is completely
recovered the file system stored within is also fully repaired. A protection at the
image level is therefore more extensive than error correction at the file level.<p>

dvdisaster is working exclusively at the image level to take advantage of these
properties. The new <a href="background30.html">RS02 method</a> even allows for
storing the error correction data at the same medium. This is possible since reading
the error correction information at the image level can not be blocked by errors
at other medium locations (damaged sectors with error correction data will reduce the error
recovery capacity, but not make recovery impossible).<p>

The <a href="background30.html">RS01 method</a> protects media at the image level, too,
but stores the error correction data in files. The next section hints at some 
pitfalls arising from that.<p>

<a name="eccfile"> </a>
<b>Consequences for error correction file storage</b><p>

The error correction data created by dvdisaster protects media at the image level.
But how are the error correction <i>files</i> protected?<p>

Since error correction files are read at the file level they are subject
to the problems discussed above. If the medium containing the error correction
files becomes damaged it may not be possible to access or read them completely.
<p>

<table width=100%><tr><td bgcolor=#000000 width=2><img width=1 height=1 alt=""></td><td>
Therefore it is important to protect error correcion files at the image level 
as well:
<a href="background70.html">Media containing error correction files</a>
must be protected with dvdisaster, too.
</td></tr></table><p>


Since image level protection is assumed there is <i>no further damage protection</i>
contained in the error correction files! This would not help much,
anyways: Error correction files could be created in a way that allows 
them to provide a reduced data recovering capacity even when being damaged. 
But however such internal protection would be designed, the
error correction file would still be only protected at the file system
level with all the disadvantages discussed above!<p>

In addition, the computing time and redundancy used for internal protection
is better spent at the image level: The Reed-Solomon error correction works 
best when error correction information
is spread over huge amounts of data. 
It is better to protect the image as a whole
than individually protecting each file within.<p></li>

EOF
}

function background30en()
{  cat >> $1 <<EOF
<h3>The RS01 and RS02 methods</h3>

dvdisaster contains two error correction methods named RS01 and RS02.
RS01 is the existing and proven method while RS02 is still
under development. RS02 is currently only available at the command line
and will be fully integrated into the graphical user interface in Version 0.70.<p>

<b>Comparison of both methods.</b>

RS01 and RS02 build on the same
<a href="background10.html">Reed-Solomon</a> error correction.
They calculate error correction information for CD/DVD images which
is used to recover unreadable sectors if the disc becomes damaged afterwards.<p>

The methods differ in the way the error correction information is stored:<p>

<ul>
<li>
<a name="file"> </a>
RS01 creates <b>error correction files</b> which are stored separately from 
the image they belong to. Since data protection at the
<a href="background20.html#file">file level</a> is difficult,
error correction files must be stored on media which are protected
against data loss by dvdisaster, too.<p></li>

<li>
<a name="image"> </a>
To apply the RS02 method an image is first created on hard disc using a
CD/DVD writing software. Before the image is written on the medium,
dvdisaster is used to <b>augment the image</b> with error correction data.
Therefore the data to be protected and the error correction information
are located at the same medium. Damaged sectors in the error correction
information reduce the data recovery capacity, but do not make recovery
impossible - a second medium for keeping or protecting the error correction
information is not required.<p></li>
</ul>


<a name="table"> </a>
<b>Comparison of error correction storage.</b><p>

<table width="100%" border="1" cellspacing="0" cellpadding="5">
<tr>
<td width="50%"><i>Error correction files</i></td>
<td width="50%"><i>Image augmented with error correction data</i></td>
</tr>
<tr valign="top">
<td> 
any possible redundancy can be chosen</td>
<td> redundancy is limited by free space on medium<br>
(= medium capacity - size of data image)</td>
</tr>

<tr valign="top">
<td>already effective at 15% redundancy;
medium can be completely filled with data</td>
<td>requires at least 20%-30% redundancy;
usable medium capacity is reduced accordingly</td> 
</tr>

<tr valign="top">
<td> can be created for already existing media</td>
<td> only applicable before writing the new medium since
the image must be augmented with error correction information in advance</td>
</tr>

<tr valign="top">
<td> separately storing the error correction file from user data
strengthens data protection</td>
<td> common storage of user data and error correction data may reduce error
correction capacity</td>
</tr>

<tr valign="top">
<td>Mapping between error correction files and media must be kept.
Error correction files must be protected against damage.</td>
<td>Easy one-medium solution; error correction information
needs not to be cataloged or explicitly protected.</td></tr>

<tr valign="top">
<td> no compatibilty issues with play-back units</td>
<td> media with augmented images may not play correctly on all units</td>
</tr>
</table><p>

EOF
}

function background40en()
{  cat >> $1 <<EOF
<h3>The linear reading strategy</h3>

dvdisaster contains two different
<a href="example82.html#read">reading strategies</a>.<p>

<b>The linear reading strategy is recommended for:</b><p>
<ul>
<li>creating images from undamaged media, e.g. to generate the error correction file</li>
<li>scanning the medium for reading speed and read errors</li>
</ul>

<b>The <a href="background50.html">adaptive reading strategy</a> is recommended for:</b><p>
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
information about the <a href="background40.html#quality">medium quality</a>:<p>

<center><img src="images/bg-linear.png" alt="screen shot"><br>
partial screen shot: linear reading strategy</center>

<pre> </pre>


<a name="configure"></a>
<b>Configuration.</b><p>

<b>Number of sectors to skip after a read error.</b>
Reading attempts for defective sectors are slow and may wear out the drive mechanics
under unfavourable conditions. A series of read errors, spread over a continous sector
range, is more common than single spot defects. Therefore a 
<a href="example82.html#read"> configuration option</a> exists so that a certain number
of sectors will be skipped after the occurance of a read error. The skipped sectors are
assumed to be defective without further reading attempts. 
Some guide lines for selecting the number of skipped sectors are:<p>

<ul>
<li>Skipping a large number of sectors (e.g. <b>1024</b>) gives a quick overview of the 
degree of damage, but will usually not collect enough data for repairing the medium image.
<p></li> 
<li>Smaller values like <b>16, 32 or 64</b> are a good trade-off:
The processing time will be considerably shortened, but still enough data for repairing
the image will be collected.<p></li>
</ul>

On DVD media read errors do usually extend over at least 16 sectors for technical
reasons. Therefore a sector skip less than 16 is not recommended for DVD media.
<p>

<a name="range"></a>
<b>Limiting the reading range.</b>
Reading can be
<a href="example82.html#read">limited to a given range of sectors</a>. This comes in handy
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
<a href="example10.html"> "Scan"</a> function. During the
"Read" operation the read data will be written to
the hard drive at the same time, which may cause irregularities in the reading
curve depending on the operating system and hardware used.<p>

<b>Read errors.</b>
Read errors cause <a href="example1.html">red markings in the spiral</a> or respective
messages at the <a href="example10.html#cli">command line</a>. 
This means that the medium could not be
read at these places during the current reading pass:
<ul>
<li>The medium is most likely defective.</li>
<li>The image should be 
<a href="example40.html">repaired</a> as soon as possible and then be transferred
to a new medium.</li>
</ul>
EOF
}


function background50en()
{  cat >> $1 <<EOF
<h3>The adaptive reading strategy</h3>

dvdisaster contains two different
<a href="example82.html#read">reading strategies</a>.<p>

<b>The adaptive reading strategy is recommended for:</b><p>
<ul>
<li> extracting data from damaged media
</li>
</ul>

<b>The <a href="background40.html">linear reading strategy</a> is recommended for:</b><p>
<ul>
<li>creating images from undamaged media, e.g. to generate the error correction file</li>
<li>scanning the medium for reading speed and read errors</li>
</ul>

<pre> </pre>

<b>Properties of the adaptive reading strategy.</b><p>

The adaptive reading strategy uses a "divide and conquer" approach for locating
still readable portions of a damaged medium. The strategy is based upon an article
published by Harald Bögeholz in c't-Magazin 16/2005 where it was published together
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

<center><img src="images/bg-adaptive.png" alt="screen shot"><br>
partial screen shot: adaptive reading strategy</center>

<pre> </pre>

<a name="configure"></a>
<b>Configuration</b><p>

<b>Error correction file.</b> 
Adaptive reading works best when an error correction file is available. 
Obviously the error correction file must have been <a href="example20.html">created</a>
at a time where the medium was still fully readable. To use the error correction file
during adaptive reading, 
<a href="example30.html">enter its name</a> before starting the reading process.<p>

<b>Limiting the adaptive reading range.</b> Reading can be 
<a href="example82.html#read">limited</a> to a part of the medium.

This is not recommended when an error correction file is used since the limit
might prevent sectors from being read which are required for a succesful error correction. 
If no error correction file is available, limiting the reading range might be helpful
during multiple reading attempts.<p>

<b>Early reading termination.</b>
If no error correction file is available, adaptive reading will stop when no unread
intervals
<a href="example82.html#read">larger than a selectable size</a> remain.<p>

The termination value should not be smaller than 128.
Otherwise the laser head will have to carry out lots of repositioning moves during the
final phase of the reading process. This will negatively affect both the life expectancy
of the drive and its reading capability. A better approach is to stop adaptive 
reading earlier and then try reading the remaining sectors with an additional
<a href="background40.html">linear reading</a> pass.
EOF
}


function background60en()
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
the <a href="example10.html">"Scan"</a>-operation of dvdisaster.
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
<a href="example20.html">error correction data</a> in time. 
The ecc file contains information for recalculating the contents of
missing sectors
<a href="background10.html">(within certain limits)</a>.
Therefore with the help of the ecc data
dvdisaster can recover images even if not all sectors
could actually be read by the drive.<p>

Since the error correction can reconstruct missing sectors up to a certain number,
it is not necessary to squeeze out a defective medium for every readable sector.
The <a href="background50.html">adaptive reading strategy</a> checks during
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


function background70en()
{  cat >> $1 <<EOF
<h3>Hints for storing the error correction files</h3>

Currently there are few exchangeable media technologies
which can be a cost-effective alternative to the various CD/DVD formats.
So you will probably not only use CD/DVD for data archival, but store
the respective error correction files on CD/DVD as well.<p>

There is nothing wrong with that, but bear in mind that your archived data
and the error correction files are stored on media with the same degree of
reliability. When read errors occur on the archived data, be prepared that
the disc with the respective error correction file might have aged beyond
full readability, too.

Therefore it is important to protect your error correction files with the same
care as your other data. This is best achieved by integrating the error correction
files into your normal data backup scheme. Here are two ideas:<p>

<b>1. Storing the error correction files on dedicated media:</b><p>

If you decide to store error correction files on separate media, it is
<a href="background20.html#eccfile">important</a> to protect those media
with dvdisaster as well. To avoid a never-ending chain
(error correction files for media of error correction files for ...),
try the following:<p>

Lets assume that five error correction files can be stored at each medium.
Write the first five error correction files to the first medium and create
another error correction file for that medium. Now save that error correction
file together with four other error correction files on the second medium.
If you continue that way, all error correction files except for those from the
last medium (which may still be kept on hard disc) are protected by dvdisaster.<p>

<b>2. Putting the error correction file on the next medium of a series:</b><p>

If you do not fill your DVDs to the max (e.g. with less than 4GB for single layered
media), you can store the error correction file of one medium
on the succeeding medium within a series.<p>

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

   Carsten Gnörlich <br>
   Im Hagen 10 <br>
   59514 Welver / Germany<br>
   Tel.: +49 2384 941174<pre> </pre>
   E-Mail: carsten@dvdisaster.de<br>
   Internet: www.dvdisaster.de

   <pre> </pre>

   Responsible for the website content under § 10 MDStV: <p>
   Carsten Gnörlich (see above for address).

   <pre> </pre>

   This imprint covers the web sites under<br>
   <a href="http://www.dvdisaster.com">www.dvdisaster.com</a>, &nbsp;
   <a href="http://www.dvdisaster.de">www.dvdisaster.de</a>, &nbsp;
   <a href="http://www.dvdisaster.org">www.dvdisaster.org</a><br>
   and under the subdomains
   <a href="http://dvdisaster.berlios.de">dvdisaster.berlios.de</a> and
   <a href="http://dvdisaster.sourceforge.net">dvdisaster.sourceforge.net</a>.

EOF
}
