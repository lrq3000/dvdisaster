#ifndef MD5_H
#define MD5_H

#ifdef PNGPACK
  typedef unsigned int guint32;
#endif

struct MD5Context {
	guint32 buf[4];
	guint32 bits[2];
	unsigned char in[64];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf,
			unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);

#endif /* MD5_H */
