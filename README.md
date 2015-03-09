### The dvdisaster project:

Optical media (CD,DVD,BD) keep their data only for a 
finite time (typically for many years).
After that time, data loss develops slowly with read errors 
growing from the outer media region towards the inside.<p>

You can check a mirror of the official website here:

http://htmlpreview.github.io/?https://github.com/lrq3000/dvdisaster/blob/stable/dvdisaster/documentation/en/index.html

**Archival with data loss protection**<p>

dvdisaster stores data on CD/DVD/BD ([supported media](qa10.html#media))
in a way that it is fully recoverable even after some read errors have developed. 
This enables you to rescue the complete data to a new medium.<p>

Data loss is prevented by 
using error correcting codes.
Error correction data is either added to the medium or kept in separate
error correction files. dvdisaster works at 
the image level so that the recovery 
does not depend on the file system of the medium.
The maximum error correction capacity is user-selectable.<p>

**Common misunderstandings about dvdisaster:**

*   dvdisaster can not make defective media readable again.
Contents of a defective medium can _not_ be recovered without the
error correction data.<p>
*   ![](images/exclude_from_search_terms.png)

    Such functions are outside the scope of dvdisaster's internal design and goals.