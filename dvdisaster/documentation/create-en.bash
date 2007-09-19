#! /bin/bash

#  dvdisaster: English homepage translation
#  Copyright (C) 2004-2007 The dvdisaster documentation team
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
   trans_to_hoster="Summary (at SourceForge.net)"

   trans_back="Back"
   trans_screen_shot="Screen shot"

   trans_contents="Contents"
   trans_news="News"
   trans_hosting="Hosted by"

   trans_fdl="Verbatim copying and distribution of this entire article is permitted in any medium, provided this notice is preserved."
   trans_copyright="Copyright 2004-2007 The dvdisaster documentation team."
   trans_modified="Information in this handbook relates to the original version of dvdisaster and may not be applicable."
}

# ----- Individual page contents 

# ----- News page (special case: has no sub pages)

function news_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="dvdisaster News" ;;

   link)    link_title="News" ;;
   link0)   link_title="News" ;;

   content*) eval "news$page$lang $file $file_base $expand full" ;;
   headlines*) eval "news$page$lang $file $file_base $expand headlines" ;;
   esac 
}

function news0en()
{  local file=$1
   local file_base=$2
   local expand=$3
   local mode=$4

   if [ $mode == "full"  ]; then
     echo "<h3>dvdisaster News</h3>">>$file
   fi

   news_item $file "xx.xx.2007" "dvdisaster 0.71.24 released" $mode <<END_NEWS_ITEM
   dvdisaster 0.71.24 has just been released. This release marks the start of
   an online documentation rewrite, including a Russian translation made
   by Igor Gorbounov. Also new are the raw CD sector editor and some improvements
   in the raw reading heuristics. 

END_NEWS_ITEM

   news_item $file "xx.xx.2007" "New documentation started" $mode <<END_NEWS_ITEM
   The dvdisaster documentation is currently being reworked for the upcoming
   V0.72 release. Please be patient; the new documentation will hopefully be more
   useful than the old one, but we will need a few weeks to fill in all parts.
END_NEWS_ITEM
}

# ----- Default home page

function index_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Overview" ;;

   link)    link_title="Overview" ;;
   link0)   link_title="Overview" ;;
   link10)   link_title="Examples of the error correction" ;;
   link20)   link_title="dvdisaster complements quality scans" ;;
   link30)   link_title="Pro and con of dvdisaster" ;;

   content*) eval "index$page$lang $file $file_base $expand" ;;
   esac 
}


function index0en()
{  cat >> $1 <<EOF
<h3>The dvdisaster project:</h3>

CD and DVD media keep their data only for a finite time (typically for many years).
After that time, data loss develops slowly with read errors 
growing from the outer media region towards the inside.<p>

<b>Archival with data loss protection</b><p>

dvdisaster stores data on CD/DVD (<a href="qa10.html#media">supported media</a>)
in a way that it is fully recoverable even after some read errors have developed. 
This enables you to rescue the complete data to a new medium.<p>

Data loss is prevented by 
using <a href="background11.html">error correcting codes</a>.
Error correction data is either added to the medium or kept in separate
error correction files. dvdisaster works at 
the <a href="background20.html">image level</a> so that the recovery 
does not depend on the file system of the medium.
The maximum error correction capacity is user-selectable.<p>

<b>Common misunderstandings about dvdisaster:</b>

<ul>
<li>dvdisaster can not make defective media readable again.
Contents of a defective medium can <i>not</i> be recovered without the
error correction data.<p></li> 
<li><img src="images/exclude_from_search_terms.png" align="middle"><br>
Such functions are outside the scope of dvdisaster's internal design and goals.</li>
</ul>

<p>
<a href="index10.html">Examples of the error correction...</a>
EOF
}

function index10en()
{  local file=$1
   local file_base=$2
   local expand=$3

   cat >> $1 <<EOF
<h3>Examples of the error correction</h3>
EOF

   screenshot $file_base "Reading the defective medium." "recover_linear.png" $expand <<END_SCREENSHOT

   <b>Recovery of aged media.</b> The medium processed here has become
   discolored and partly unreadable in its outer region. A surface scan
   yields about 23.000 unreadable sectors of 342.000 sectors total; resulting
   in about 7,2% defective sectors.
END_SCREENSHOT

   screenshot $file_base "Repairing the defective image." "fix_image.png" $expand <<END_SCREENSHOT

   <b>Repairing the defective image.</b> The resulting image is still incomplete
   since about 23.000 sectors could not be read. These sectors are now 
   reconstructed using 
   the <a href="background11.html#eccblock">error correction data</a>
   created with dvdisaster.
   During the recovery a maximum of 20 errors
   per <a href="background11.html#eccblock">ECC block</a> is encountered. This results in a peak
   error correction load of 63%, meaning that this degree of damage is handled well
   by an error correction data created with default settings.
END_SCREENSHOT

   cat >> $1 <<EOF

<b>Recovery needs error correction data:</b> The recovery process described above uses
<a href="background11.html">error correction data</a> ("ecc data"). 
Think of this data as a special
form of backup data (it needs less space than a normal backup, though).
Like an ordinary backup, the ecc data needs to be created
<i>before</i> the medium goes defective.<p>

So if you have a defective medium but never created ecc data for it - sorry, 
your data is probably lost.<p>


<a href="index20.html">Why quality scans won't suffice...</a>
EOF
}


function index20en()
{  cat >> $1 <<EOF
<h3>dvdisaster as a complement to quality scans</h3>

<a href="qa.html#pipo">Quality scans</a>, e.g. C2 error or PI/PO scans are a valuable 
tool for testing the results of the media writing process.<p>

But quality scans are <b>not</b> a reliable means of <b>predicting 
the lifetime</b> of optical media. <br>
Consider we are looking for the right time
to copy a worn-out medium onto a new one:

<ul>
<li>Too early: Copying media because of a bad quality scan is cost-ineffective.
Sometimes such media remain readable much longer than expected.<p></li>
<li>Too late: When the quality scan reveals unreadable sectors some
data has already been lost.<p></li>
<li>Right before the medium fails: The ideal case, but how to tell?
</ul>

However, we could do it the dvdisaster way:

<ul>
<li><a href="howtos20.html">Create error correction data</a> for the medium.<p></li>
<li><a href="howtos10.html">Scan the medium</a> regularly. Use it until the first read errors occur.<p></li>
<li><a href="howtos30.html">Recover</a> the read errors <a href="howtos40.html">using the error correction data</a>.
Write the recovered image to a new medium.</li>
</ul>

<p>
<a href="index30.html">Pro and con of dvdisaster at a glance...</a>
EOF
}


function index30en()
{  cat >> $1 <<EOF
<h3>Advantages of using dvdisaster:</h3>

<ul>
<li><b>Protects</b> against aging and accidental medium damage (within <a href="background12.html">certain limits</a>).<p></li>
<li><a href="howtos10.html">Read error tests</a> run <b>faster</b> than quality scans;
up to full reading speed depending on the drive.<p></li>
<li><b>Cost-effective:</b> Media must be replaced with a new copy 
only when they are really defective.</li>
</ul>

<h3>Limitations of using dvdisaster:</h3>

You need a backup strategy and at least 15% of additional storage.

<ul>
<li>Error correction data <b>must be created before the medium fails</b>, 
preferably at the same time the medium is written.<p></li>
<li>Error correction data requires <b>additional storage space</b> either on the protected 
medium or by using additional media. 
Using the standard settings the additional
storage space amounts to 15% of the original data size
(approx. 700MB for a full 4.7GB DVD).<p></li>
<li><a href="background12.html">no guaranteed protection</a> against data loss.</li>
</ul>

See also the collection of <a href="background.html">background information</a>
to learn more about the functioning of dvdisaster.
EOF
}

# ----- Screenshots

function screenshots_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Screenshots" ;;

   link)    link_title="Screenshots" ;;

   content*) eval "screenshots$page$lang $file $file_base $expand" ;;
   esac 
}

function screenshots0en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

# ----- Screenshots

function howtos_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Howtos" ;;
   link10)   link_title="Scanning media for errors" ;;

   link20)   link_title="Creating error correction data" ;;
   link21)   link_title="as a file" ;;
   link22)   link_title="within the image" ;;

   link30)   link_title="Reading defective media" ;;
   link40)   link_title="Recovering media images" ;;
   link50)   link_title="Getting information about images and error correction data" ;;

   link90)   link_title="Testing image compatibility" ;;
   link91)   link_title="Creating the reference image" ;;
   link92)   link_title="Creating the error correction file";;
   link93)   link_title="Writing the image to the medium.";;
   link)    link_title="Howtos" ;;

   content*) eval "howtos$page$lang $file $file_base $expand" ;;
   esac 
}

function howtos0en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos10en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos20en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos21en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos22en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos30en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos40en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos50en()
{  cat >> $1 <<EOF
<h3>To be done</h3>
EOF
}

function howtos90en()
{  local file=$1
   local file_base=$2
   local expand=$3

   cat >> $1 <<EOF
<h3>Testing image compatibility</h3>

<b>Purpose.</b>The test described in this section verifies that dvdisaster 
is compatible with your media writing software. Passing this test means that:

<ul>
<li>dvdisaster can directly work with .iso images provided by the media writing software;</li>
<li>which is a prerequisite for creating augmented images;</li>
<li>and that dvdisaster is compatible with the respective drives and media.</li>
</ul>

<b>Overview.</b> In order to do the test the following steps need to be carries out:

<ol>
<li><a href="howtos91.html">Create a reference image with the media writing software.</a></li>
<li><a href="howtos92.html">Create the error correction file.</a></li>
<li><a href="howtos93.html">Write the image to the medium.</a></li>
</ol>

<a href="howtos91.html">Creating the reference image...</a>
EOF
}

function howtos91en()
{  local file=$1
   local file_base=$2
   local expand=$3

   subsection $file "Image compatibility test" "Step 1: Creating a reference image with the media writing software."

   cat >> $1 <<EOF

First we use your favourite media writing software (e.g. k3b on Linux) 
to create an .iso image on your hard disk. 
This image will be used as a reference for the following compatibility tests.

<h4>How to create the reference image:</h4>
Since we do not know which media writing software you are using, 
we can just give a general outline:
 
<ul>
<li>Please invoke your media writing software and start a new project.</li>
<li>Add the files you wish to write on the medium, but do <i>not</i> yet start the writing process.</li>
<li>Save the .iso image to hard disk.
You <i>must</i> create an iso image; using proprietary
formats like .nrg will <i>not</i> work. 
During this example we will assume that the image file location is <tt>D:\images\medium.iso</tt>.
</ul>

<a href="howtos92.html">Creating the error correction file...</a>

EOF
}

function howtos92en()
{  local file=$1
   local file_base=$2
   local expand=$3

   subsection $file "Image compatibility test" "Step 2: Creating the error correction file."

cat >>$file <<EOF
Before doing anything else with the just created image we use dvdisaster
to create an error correction file from it.
This provides us with an easy to verify that the data in the image
is still in its original state when we have written it to a medium 
and read back for testing purposes.

<h4>Howto create the error correction file:</h4>

<table width="100%" border="0" cellspacing="0" cellpadding="3">
<tr valign="top"  $ITEMCOLOR>
<td>1.</td>
<td>Invoke dvdisaster and tell it the required file names:</td>
</tr>
<tr><td></td><td>
<img src="images/file_names_rs01.png"></li>
<ul>
 <li>Tell dvdisaster the name of the iso image you have just created.
 For example, enter <tt>D:\images\medium.iso</tt> into the left file selection field.</li>
 <li>We are going to create an error correction file named <tt>medium.ecc</tt>
 in the directory folder of the image file. For example, enter 
 <tt>D:\images\medium.ecc</tt> into the right file selection field.</li>
</ul>
</td></tr></table>

<table width="100%"  border="0" cellspacing="0" cellpadding="3">
<tr valign="top"  $ITEMCOLOR>
<td>2.</td>
<td>Configure the error correction: </td>
</tr>
<tr><td></td><td>
EOF

screenshot $file_base "Configure the error correction." "prefs_rs01.png" $expand <<END_SCREENSHOT

Open the "Error correction" tab in the preferences
dialog and select "Error correction file (RS01)" as shown in the screen shot. The chosen
redundancy does not really matter for testing purposes, 
so using the default (Normal) is fine.
END_SCREENSHOT

cat >>$file <<EOF
</td></tr></table>

<table width="100%" border="0" cellspacing="0" cellpadding="3">
<tr valign="top"  $ITEMCOLOR>
<td  width="1px">3.</td>
<td>Create the error correction file: </td>
</tr>
<tr><td></td><td>
  <table>
   <tr valign="center">
     <td><img src="images/button_create.png"></td>
     <td>Press the "Create" button. Producing the error correction file may take some time.</td> 
   </tr></table>
</td></tr></table><p>

<a href="howtos93.html">Writing the image to the medium...</a>
EOF
}


function howtos93en()
{  local file=$1
   local file_base=$2
   local expand=$3

   subsection $file "Image compatibility test" "Step 3: Writing the image to the medium."

cat >>$file <<EOF
<h4>Write the image to the medium</h4>

Now write the saved iso image (<tt>medium.iso</tt> in our example) to a medium
using your media recording software.

<hr>

<h4>Read the medium contents back</h4>

We are now using dvdisaster to read the medium contents back into a new iso image file.

<table width="100%" border="0" cellspacing="0" cellpadding="3">
<tr valign="top"  $ITEMCOLOR>
<td>1.</td>
<td>Invoke dvdisaster and tell it to use a new iso image file:</td>
</tr>
<tr><td></td><td>
<img src="images/file_names_rs01_new.png"></li>
<ul>
 <li>Tell dvdisaster the name of the iso image you have just created.
 For example, enter <tt>D:\images\medium.iso</tt> into the left file selection field.</li>
 <li>We are going to create an error correction file named <tt>medium.ecc</tt>
 in the directory folder of the image file. For example, enter 
 <tt>D:\images\medium.ecc</tt> into the right file selection field.</li>
</ul>
</td></tr></table>

EOF
}

# ----- Downloads

function download_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Download" ;;

   link)    link_title="Download" ;;
   link10)   link_title="System requirements" ;;
   link20)   link_title="Digital signature" ;;
   link30)   link_title="Installation" ;;
   link40)   link_title="Alpha (unstable) versions" ;;

   content*) eval "download$page$lang $file $file_base $expand" ;;
   esac 
}


function download0en()
{  cat >> $1 <<EOF
<h3>Download dvdisaster</h3>

dvdisaster is available for <a href="download10.html">recent versions</a>
of the Darwin/Mac OS X, FreeBSD, Linux, NetBSD and Windows operating systems.<p>

Please download either the source code or a binary version from the list below.
A <a href="download20.html">digital signature</a> is provided for verification that the
packets are in their original state.<p>

To <a href="download30.html">install</a> the binary version execute 
the downloaded program and follow the dialog.
The source code version contains a file <tt>INSTALL</tt> with further instructions.

<pre> </pre>

<!---
<b>Developer versions</b> - new and experimental for experienced users!<p>
<b>Current version</b><p>
-->

<b>Alpha (unstable) versions</b> - new and experimental for experienced users!<p>

Testers are welcome for the upcoming dvdisaster release, but should be aware of remaining
bugs and incompatibilities.
The current unstable release is 
<a href="download40.html">${cooked_version}</a>.
<pre> </pre>

<b>Stable version</b> - recommended for getting started.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.70</b></td><td align="right">20-Jan-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4.tar.bz2?download">dvdisaster-0.70.4.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4.tar.bz2.gpg?download">dvdisaster-0.70.4.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4-setup.exe?download">dvdisaster-0.70.4-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.70.4-setup.exe.gpg?download">dvdisaster-0.70.4-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
The RS02 error correction method
is fully supported in the graphical user interface. Images created
with RS02 can be used 
with the adaptive reading strategy.<p>

Julian Einwag started porting dvdisaster 
to Mac OS X / Darwin.<p>

Daniel Nylander provided Swedish translations of the screen texts.<p>

<b>Patches</b> (small changes after version 0.70; files above have been updated):<br>

<b>pl4</b> provides better compatibility with two layered DVDs 
(DVD-R DL and DVD+R DL).<br> 
Some minor bugs have been fixed. <i>(20-Jan-2007)</i>.<p>

<b>pl3</b> fixes incorrect recognization of unsupported CD formats which
produced a Windows blue screen under rare circumstances. Added cancel option
during RS02 initialization on DVD RW media.
<i>(10-Dec-2006)</i>.<p>

<b>pl2</b> fixes incorrect memory freeing when the program window is closed.
Unpacking of documentation screen shots is fixed for PPC platforms.
Only the source code archives have been updated.
<i>(03-Oct-2006)</i>.<p>

<b>pl1</b> fixes a bug in adaptive reading for RS02 which would sometimes result in
reading insufficient data for a successful recovery. Adds some small
improvements of documentation and usability. <i>(30-Jul-2006)</i>
</td></tr></table><p>

The dvdisaster source code is also
<a href="http://sourceforge.net/cvs/?group_id=157550">available via CVS</a>.
<pre> </pre>

<b>Previous release</b> - upgrading to version 0.70.4 is recommended.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.66</b></td><td align="right">25-Mar-2006</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2?download">dvdisaster-0.66.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.tar.bz2.gpg?download">dvdisaster-0.66.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe?download">dvdisaster-0.66.3-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.66.3-setup.exe.gpg?download">dvdisaster-0.66.3-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
</table><p>
EOF
}

function download10en()
{  cat >> $1 <<EOF
<h3>System requirements</h3>

<ul>
 <li>Processors: x86, PowerPC or Sparc;<p></li>
 <li>with processing speed equal or better than a P4 at 2Ghz;<p></li>
 <li>an up-to-date CD/DVD/HD DVD/BD drive with ATAPI or SCSI interface;<p></li>
 <li>enough hard disk space for creating .iso images from processed media.<p>
</ul>

<h3>Operating systems</h3>
<ul>
 <li><b>FreeBSD</b> version <b>6.0</b> or later<br>
     (using ATAPI drives requires a kernel recompile -- see INSTALL doc)<p>
 </li>
 <li><b>Linux</b> with kernel <b>2.6.7</b> or later,<br>
     (using kernels >= 2.4.20 is possible, but not recommended!)<p>
 </li>
 <li><b>Mac OS X</b> version 10.4 (Tiger) or later,<br> 
      on x86 and PowerPC hardware.<p>
 <li><b>NetBSD</b> version 3.1 or later.<p></li> 
 <li><b>Windows</b> <b>2000</b> or <b>XP</b><br>
     Windows 98 and ME are possible, but are not recommended for handling DVD
     and larger media.<br>
     Running under Windows Vista (R) is possible, but mostly untested.
 </li>
</ul>
EOF
}

function download20en()
{  cat >> $1 <<EOF

<h3>Digital signature</h3>

The downloadable dvdisaster packages have been digitally signed using
<a href="http://www.gnupg.org/gnupg.html">GnuPG</a> so that you can verify
that the software is in its original state.<p>


The signature has been made with the following <a href="../pubkey.asc">public key</a>:

<a href="../pubkey.asc">
<pre>
pub   1024D/F5F6C46C 2003-08-22
      Key fingerprint = 12B3 1535 AF90 3ADE 9E73  BA7E 5A59 0EFE F5F6 C46C
uid                  dvdisaster (pkg signing key #1)
sub   1024g/091AD320 2003-08-22
</pre></a>

Feel free to send an email to <img src="../images/email.png" align="top"> to obtain 
the fingerprint directly from the developers. 
Please include "GPG finger print" in the subject line.
EOF
}

function download30en()
{  cat >> $1 <<EOF
<h3>Installation of the binary versions (Windows only)</h3>

Please install the Windows version by executing the setup binary
(e.g. ${project_package}-setup.exe) and follow the 
installation dialog. <p>

<b>Caution:</b> dvdisaster can NOT be installed from
manually unpacking the setup.exe program, or by copying it from
an already existing installation. Doing so may result in strange
errors which seem to be totally unrelated to an improper installation.

<h3>Installation of the source packages</h3>

dvdisaster uses the common <tt>./configure; make; make install</tt> mechanism
for installation from source. Please see the file <tt>INSTALL</tt> in the
source distribution for further information.
EOF
}

function download40en()
{  cat >> $1 <<EOF
<h3>Alpha (developer) versions</h3>

<b>Help us testing!</b> We have some new functions under development and
would like them to be tested by more people before making the next official release.<p>

Especially, support for BD und HD DVD drives has been introduced and needs more
field testing. The <a href="howtos90.html">image compatibility test</a> described
in the Howtos section is very useful for testing new drives. Please 
<a href="feedback.html">report both positive and negative results</a> 
so that we can get a better picture of how the new formats are working.<p>

<b>A word of caution:</b> The alpha versions might lock up the drive and/or the
operating system and require you to reboot. They might contain other bugs
and should not be used to process media containing important data.<p>

If in doubt, please continue using the <a href="download.html">stable version 0.70</a>
and wait for the release of version 0.72.

<hr>

<h3>Downloads</h3>

The alpha versions use the same package format as the regular releases.<p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel24)</b></td><td align="right">xx-xxx-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24.tar.bz2?download">dvdisaster-0.71.devel24.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24.tar.bz2.gpg?download">dvdisaster-0.71.devel24.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24-setup.exe?download">dvdisaster-0.71.devel24-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel24-setup.exe.gpg?download">dvdisaster-0.71.devel24-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
This release marks the start of
an online documentation rewrite, including a Russian translation made
by Igor Gorbounov. Also new are the raw CD sector editor and some improvements
in the raw reading heuristics. 
</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel23)</b></td><td align="right">06-May-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23.tar.bz2?download">dvdisaster-0.71.devel23.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23.tar.bz2.gpg?download">dvdisaster-0.71.devel23.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23-setup.exe?download">dvdisaster-0.71.devel23-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel23-setup.exe.gpg?download">dvdisaster-0.71.devel23-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Added support for C2 scanning of CD media.<br>
Andrei Grecu submitted improved versions of the RAW recovery heuristics.
</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel22)</b></td><td align="right">22-Apr-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22.tar.bz2?download">dvdisaster-0.71.devel22.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22.tar.bz2.gpg?download">dvdisaster-0.71.devel22.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22-setup.exe?download">dvdisaster-0.71.devel22-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel22-setup.exe.gpg?download">dvdisaster-0.71.devel22-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Igor Gorbounov started the Russian translation of on-screen texts.<p>
Added selection of 20h/21h raw reading modes to enable defective sector 
reading on more drives. Fixed some inefficiencies during raw sector recovery.
</td></tr></table><p>

<table width="100%" $IDXCOLOR cellpadding="0" cellspacing="5">
<tr><td><b>dvdisaster-0.71 (devel21)</b></td><td align="right">15-Apr-2007</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
  <table>
    <tr><td align="right">&nbsp;&nbsp;Source code for all operating systems:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21.tar.bz2?download">dvdisaster-0.71.devel21.tar.bz2</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21.tar.bz2.gpg?download">dvdisaster-0.71.devel21.tar.bz2.gpg</a></td></tr>
    <tr><td align="right">Binary for Windows:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21-setup.exe?download">dvdisaster-0.71.devel21-setup.exe</a></td></tr>
    <tr><td align="right">Digital signature:&nbsp;</td>
        <td><a href="http://prdownloads.sourceforge.net/dvdisaster/dvdisaster-0.71.devel21-setup.exe.gpg?download">dvdisaster-0.71.devel21-setup.exe.gpg</a></td></tr>
  </table>
</td></tr>
<tr bgcolor="#000000"><td colspan="2"><img width=1 height=1 alt=""></td></tr>
<tr><td colspan="2">
Andrei Grecu provided an algorithm for recovering defective CD sectors
from multiple incomplete RAW reading attempts.
Sergey Svishchev helped porting dvdisaster to NetBSD.<p>

Other features include: a selectable number of reading attempts, verification
of CD sectors using the raw L-EC and EDC data, and a redesigned preferences dialog.
RS02 checksum handling has been improved in the adaptive reading strategy. 
</td></tr></table><p>
EOF
}

# ----- Questions and Answers

function qa_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Questions and Answers" ;;

   link)    link_title="Questions and Answers" ;;
   link10)   link_title="Technical Questions" ;;
   link20)   link_title="Error messages" ;;

   content*) eval "qa$page$lang $file $file_base $expand" ;;
   esac 
}


function qa0en()
{  cat >> $1 <<EOF
<h3><a name="top">General questions and answers</a></h3>

<a href="#pronounce">1.1 How is "dvdisaster" pronounced?</a><p>
<a href="#pipo">1.2 What are quality scans and why don't you support more?</a><p>
<a href="#compat">1.3 Is dvdisaster compatible with future releases?</a><p>

<hr><p>

<b><a name="pronounce">1.1 How is "dvdisaster" pronounced?</a></b><p>
Since the word stems from the english language, simply spell "dv" before
saying "disaster". Perhaps "dee-vee-disaster" is a suitable
phonetic circumscription.
<div align=right><a href="#top">&uarr;</a></div>

<b><a name="pipo">1.2 What are quality scans and why don't you support more?</a></b><p>
Optical media have a built-in error correction which is similar to 
the <a href="background10.html">method</a> used in dvdisaster. 
Some drives can report the number of errors corrected
while reading a medium. This provides a rough estimate 
of the writing and media quality.<p>

Since dvdisaster is free software, it can only include code and information
which can be redistributed freely. This is currently true 
for C2 <a href="howtos10.html">scanning</a> of CD media, 
which is officially standardized and has free documentation available.<p>

On the other hand, DVD quality scans ("PI/PO scans") are not standardized. 
Those drive vendors who support it are using proprietary programming
interfaces. The respective specifications seem not to be available 
for use in free software. So we must patiently wait until manufacturers 
understand that having more (and free) software available for a drive 
will sell more drives. <p>

<div align=right><a href="#top">&uarr;</a></div>

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

<hr><p>

<b><a name="nls">2.1 Which translations of the program are available?</a></b><p>

The current version of dvdisaster contains screen texts in the following languages:<p>

<table>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Czech</td><td>--</td><td>upto version 0.66</td></tr>
<tr><td></td><td>English</td><td>--</td><td>complete</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>German</td><td>--</td><td>complete</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Italian</td><td>--</td><td>upto version 0.65</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Russian</td><td>--</td><td>complete</td></tr>
<tr><td>&nbsp;&nbsp;&nbsp;</td><td>Swedish</td><td>--</td><td>upto version 0.70</td></tr>
</table><p>

Translators for other languages are welcome!<p>

dvdisaster will automatically obtain language settings from the operating system.
If the local language is not yet supported, english text will be used. 
A different language can be selected using environment variables.<p>

Example for the bash shell and german language:

<pre>export LANG=de_DE</pre>

If special characters like german umlauts are not displayed properly,
try the following:<p>

<tt>export OUTPUT_CHARSET=iso-8859-1</tt> (X11, XTerm)

<div align=right><a href="#top">&uarr;</a></div>


<b><a name="media">2.2 Which media types are supported?</a></b><p>

dvdisaster supports (re-)writeable CD and DVD media. <br>
Media containing multiple sessions or copy protections can <i>not</i> be used.<p>

Usable media by type:<p>

<b>CD-R, CD-RW</b><p>

<ul>
 <li>only Data CD are supported.</li>
</ul>

<b>DVD-R, DVD+R</b><p>

<ul>
<li>No further limitations are known.</li>
</ul>

<b>DVD-R DL, DVD+R DL (two layers)</b>
<ul>
<li>The drive must be able to <a href="qa20.html#dvdrom">identify the medium type</a>.
Typically this is only the case for drives which can also write two layered media.</li>
</ul>

<b>DVD-RW, DVD+RW</b><p>

<ul>
<li>Some drives report wrong <a href="qa20.html#rw">image sizes</a>.<br>
Remedy: Determine the image size from the ISO/UDF file system or the ECC/RS02 data.
</li></ul>

<b>DVD-RAM</b><p>
<ul>
<li>Usable only when written with ISO/UDF images like DVD-R/-RW;</li>
<li>Not supported when used as removeable medium / for packet writing.</li>
<li>Similar issues with <a href="qa20.html#rw">image size</a>
recognition as noted above.</li>
</ul>

<b>Not usable types</b> (image can not be extracted):<p>
DVD-ROM (pressed DVDs), CD-Audio and CD-Video.

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

The --list function prints out
the current mapping of drive letters.<p>

<i>&gt;&nbsp; How to select between SPTI and ASPI?</i><p>

dvdisaster will automatically choose between SPTI and ASPI, 
deciding in favor of SPTI if both are available. 
Usage of ASPI can be forced; see the description of the
--list function for details.

<div align=right><a href="#top">&uarr;</a></div><p>

EOF
}


function qa20en()
{  cat >> $1 <<EOF
<h3>Error messages</h3>

<a href="#tao">3.1 "Warning: 2 sectors missing at the end of the disc".</a><p>
<a href="#block">3.2 Program blocks right after invocation.</a><p>
<a href="#crc">3.3 What does "CRC error, sector: n" mean?</a><p>
<a href="#rw">3.4 Read errors or wrong image size with -RW/+RW/-RAM media</a><p>
<a href="#dvdrom">3.5 My self-written media is recognized as "DVD-ROM" and rejected.</a><p>
<a href="#freebsd">3.6 No drives appear under FreeBSD.</a><p>
<a href="#v40error">3.7 "Ecc file has been created with version 0.40.7."</a><p>

<hr><p>

<b><a name="tao">3.1 "Warning: 2 sectors missing at the end of the disc"</a></b><p>
This warning appears with CD media written in "TAO" (track at once) mode.
Some drives report an image size which is 2 sectors too large for such media,
producing 2 pseudo read errors at the end of the medium 
which do <i>not</i> mean data loss in this case.<p>

Since the writing mode can not be determined from the medium, dvdisaster assumes
a "TAO" CD if exactly the last two sectors are unreadable, and trims the image
accordingly. It is up to you to decide whether this is okay. You can
advise dvdisaster to treat these sectors as real read errors by using the
--dao option or 
the preferences tab for reading/scanning.<p>

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
and error correction files and <a href="howtos50.html">verify
them again</a>.
When the error disappears or surfaces at a different location,
your computer may be suffering from defective memory, 
broken drive cabling, or wrong CPU/system frequency settings.
<div align=right><a href="#top">&uarr;</a></div>


<b><a name="rw">3.4 Read errors or wrong image size with -RW/+RW/-RAM media</a></b><p>

Some drives report incorrect image sizes when -RW/+RW/-RAM media are used. Two common
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
Activate the option for determining the
image size from the ISO/UDF file system or from the ECC/RS02 data.
</td></tr></table><p>

If the required ISO/UDF sectors are unreadable and you are using error correction files
to recover damaged media there are two possible workarounds:

<ul>
<li>Execute the <a href="howtos50.html">"Verify"</a> function with only
the error correction file being selected/given. Note down the correct image size 
from the output and restrict the
reading range accordingly.
</li>
<li>Simply read in the image with the incorrect (larger) size.
When invoking the <a href="howtos40.html#repair">"Fix"</a> function,
answer "OK" when you are asked whether the image should be truncated.
</li>
</ul>

<div align=right><a href="#top">&uarr;</a></div>

<b><a name="dvdrom">3.5 My self-written media is recognized as "DVD-ROM" and rejected.</a></b><p>

The medium book type has probably been set to "DVD-ROM". Typically, a drive
capable of writing the same media format is required for processing the medium
with dvdisaster.<p>

For example, a two-layered DVD+R with a wrong book type 
may only be accepted on a burner which can write such media.<p>

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
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Background information" ;;
 
   link)    link_title="Background information" ;;
   link10)   link_title="Error correction" ;;
   link11)   link_title="General idea" ;;
   link12)   link_title="Limitations" ;;
   link20)   link_title="Image level data recovery" ;;
   link30)   link_title="Reading strategies" ;;
   link40)   link_title="Linear reading strategy" ;;
   link50)   link_title="Adaptive reading strategy" ;;
   link60)   link_title="Remarks on read errors" ;;
   link70)   link_title="Hints for storing the error correction files" ;;

   content*) eval "background$page$lang $file $file_base $expand"
	    return 0
	    ;;
   esac 
}


function background0en()
{  cat >> $1 <<EOF

<h3>To be done</h3>

EOF
}

function background10en()
{  cat >> $1 <<EOF

<h3>To be done</h3>

EOF
}

function background11en()
{  cat >> $1 <<EOF

<h3>To be done</h3>

EOF
}

function background12en()
{  cat >> $1 <<EOF

<h3>To be done</h3>

EOF
}

function background20en()
{  cat >> $1 <<EOF

<h3>To be done</h3>

EOF
}

function background30en()
{  cat >> $1 <<EOF

<h3>To be done</h3>

EOF
}

# ----- Feedback/Bug reporting

function feedback_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Bug reporting" ;;

   link)    link_title="Bug reporting" ;;
   link0)   link_title="Bug reporting" ;;

   content*) eval "feedback$page$lang $file $file_base $expand" ;;
   esac 
}

function feedback0en()
{  local file=$1
   local file_base=$2
   local expand=$3

   cat >> $1 <<EOF
<h3>Bug reporting</h3>
Like all complex software, dvdisaster may contain bugs (programming errors) and
incompatibilities with certain (drive) hardware and software setups.
You are invited to tell us about any difficulties you encounter with the program
or the documentation so that we can improve things in future releases.<p>

To make sure that we are getting the right information, we have provided the
following checklist for bug reporting:


<h4>Please check first that you are really experiencing a bug:</h4>

<ul>
<li>Make sure that you are using the latest genuine version from our
<a href="http://dvdisaster.sourceforge.net/en/download.html">download site 
at SourceForge</a>.
dvdisaster versions provided by third parties
may contain functions and bugs which are not present in the original version.</li>
<li>Double check that the issue you have encountered is not already covered in the
<a href="qa20.html">Questions and Answers</a> section.</li>
<li>Please note that dvdisaster will only work with the (re-)writeable varieties
of media, so seeing it reject DVD-ROM, BD-ROM and HD DVD-ROM is <b>not an
error</b>. Also, CD-Audio, VCD, SVCD and multisession CD are not supported 
(<a href="qa10.html#media">complete list of supported media formats</a>).</li>
<li>dvdisaster works only with real drives. Not supported are
network drives, software drives (e.g. alcohol) and drives in virtual 
machines like VMWare(R) and Xen.</li>
</ul>

<h4>How to report issues with the program:</h4>

Please report your findings by sending an email to 
<img src="../images/email.png" align="top">. Your report should contain:<p>

<ul>
<li>Information about the operating system and dvdisaster version you are using;</li>
<li>the drive and media type(s) which exhibited the problem; </li>
<li>a textual description of the issue you encountered;</li>
<li>a screen shot of the error message and/or output which might provide
further information about the problem;</li>
<li>differences between working and non-working configurations if the issue is
experienced only on certain drives/computers;</li>
<li>a log file if you suspect that the issue is related to a drive or medium
incompatibility.</li>
</ul>
EOF

screenshot $file_base "Creating a log file." "activate_logfile.png" $expand <<END_SCREENSHOT

<b>How to create a log file:</b> If you suspect incompatibilities with your drive and/or media
as the cause of your issue, please activate the log file feature in the preferences 
dialog as shown in the screen shot. Then perform a scanning or reading action 
and attach the log file to your bug report.
END_SCREENSHOT

cat >>$file <<EOF
Thanks for your feedback!
EOF
}

# ----- Impressum

function imprint_contents_en()
{  local file_base=$1;
   local query=$2
   local page=$3
   local lang=$4
   local expand=$5
   local file;

   if [ $expand != 0 ]; 
     then file="${file_base}_${expand}.html";
     else file="${file_base}.html";
   fi

   case $query in
   title)   title="Imprint" ;;
 
   link)    link_title="Imprint" ;;

   content*) eval "imprint$page$lang $file $file_base $expand"
	    return 0
	    ;;
   esac 
}


function imprint0en()
{  cat >> $1 <<EOF

   <h3>Imprint</h3>

   Carsten Gnörlich <br>
   Beerenkamp 23 <br>
   59514 Welver / Germany<br>
   Tel.: +49 2384 941174<pre> </pre>
   E-Mail: carsten@dvdisaster.de<br>
   Internet: www.dvdisaster.de

   <pre> </pre>

   Responsible for the website content: <p>
   Carsten Gnörlich (see above for address).

   <pre> </pre>

   This imprint covers the web sites under<br>
   <a href="http://www.dvdisaster.com">www.dvdisaster.com</a>, &nbsp;
   <a href="http://www.dvdisaster.de">www.dvdisaster.de</a>, &nbsp;
   <a href="http://www.dvdisaster.org">www.dvdisaster.org</a><br>
   and under the subdomain
   <a href="http://dvdisaster.sourceforge.net">dvdisaster.sourceforge.net</a>.

EOF
}
