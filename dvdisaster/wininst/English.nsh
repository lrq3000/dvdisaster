; English messages

; WARNING!!! the file encoding is Win-1252
; Copyright (C) 2004-2006 THE dvdisaster'S COPYRIGHT HOLDER
; Copyright 2006 Lubos Stanek
; This file is distributed under the same license as the dvdisaster package.

; this file must include specific dvdisaster setup's messages
; remember that string are formatted according to the NSIS spec


; Language
!define LANG_ID_ENGLISH 1033

LangString LANG_PUBLISHER ${LANG_ENGLISH} "Carsten Gnörlich"
LangString LANG_WEB_SITE ${LANG_ENGLISH} "http://www.dvdisaster.org/index.html"
LangString LANG_UPDATE_SITE ${LANG_ENGLISH} "http://www.dvdisaster.org/download.html"
LangString LANG_PUBLISHER_SITE ${LANG_ENGLISH} "http://www.dvdisaster.org/imprint.html"

LangString LANG_INSTALLTYPE_STANDARD ${LANG_ENGLISH} "Standard"
LangString LANG_INSTALLTYPE_FULL ${LANG_ENGLISH} "Complete"

LangString LANG_SECTION_APPLICATION ${LANG_ENGLISH} "Application"
LangString LANG_SECTION_ERROR ${LANG_ENGLISH} "The $\"%s$\" section were not installed properly!"
LangString LANG_SECTION_MANUAL ${LANG_ENGLISH} "Manual"
; the following refers to previous; if it is not possible to translate it as it is,
; prepend the previous string into all localizations (e.g. Manual in Czech)
; and translate the full expression
LangString LANG_SECTION_MANUAL_CZECH ${LANG_ENGLISH} "in Czech"
LangString LANG_SECTION_MANUAL_ENGLISH ${LANG_ENGLISH} "in English"
LangString LANG_SECTION_MANUAL_GERMAN ${LANG_ENGLISH} "in German"

LangString LANG_SECTION_SHORTCUTS ${LANG_ENGLISH} "Start menu shortcuts"
LangString LANG_SECTION_SHORTCUTS_DESC ${LANG_ENGLISH} "Additional error protection for CD/DVD media"

LangString LANG_SECTION_DESKTOP ${LANG_ENGLISH} "Desktop shortcut"

LangString DESC_SectionApp ${LANG_ENGLISH} "Installs the application, libraries, library settings, and message catalogs."
LangString DESC_SectionManual ${LANG_ENGLISH} "Installs the localized manual."
LangString DESC_SectionShortcuts ${LANG_ENGLISH} "Creates shortcuts."

LangString LANG_APPISRUNNING ${LANG_ENGLISH} "${CFG_PACKAGE} is running!$\r$\rTerminate all instances of ${CFG_PACKAGE} and try again."
LangString LANG_NOINSTALLPERMS ${LANG_ENGLISH} "You do not possess required rights for installations!$\r$\rThe installer will be terminated."
