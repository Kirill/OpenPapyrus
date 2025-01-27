/*
 * Copyright 2017-2018 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at  https://www.openssl.org/source/license.html
 */

#ifndef S390X_ARCH_H
#define S390X_ARCH_H

#ifndef __ASSEMBLER__

void s390x_kimd(const uchar *in, size_t len, uint fc,
                void *param);
void s390x_klmd(const uchar *in, size_t inlen, uchar *out,
                size_t outlen, uint fc, void *param);
void s390x_km(const uchar *in, size_t len, uchar *out,
              uint fc, void *param);
void s390x_kmac(const uchar *in, size_t len, uint fc,
                void *param);
void s390x_kmo(const uchar *in, size_t len, uchar *out,
               uint fc, void *param);
void s390x_kmf(const uchar *in, size_t len, uchar *out,
               uint fc, void *param);
void s390x_kma(const uchar *aad, size_t alen, const uchar *in,
               size_t len, uchar *out, uint fc, void *param);

/*
 * The field elements of OPENSSL_s390xcap_P are the 64-bit words returned by
 * the STFLE instruction followed by the 64-bit word pairs returned by
 * instructions' QUERY functions. If STFLE returns fewer data or an instruction
 * is not supported, the corresponding field elements are zero.
 */
struct OPENSSL_s390xcap_st {
    ulong long stfle[4];
    ulong long kimd[2];
    ulong long klmd[2];
    ulong long km[2];
    ulong long kmc[2];
    ulong long kmac[2];
    ulong long kmctr[2];
    ulong long kmo[2];
    ulong long kmf[2];
    ulong long prno[2];
    ulong long kma[2];
};

extern struct OPENSSL_s390xcap_st OPENSSL_s390xcap_P;

/* convert facility bit number or function code to bit mask */
#define S390X_CAPBIT(i)	(1ULL << (63 - (i) % 64))

#endif

/* OPENSSL_s390xcap_P offsets [bytes] */
#define S390X_STFLE		0x00
#define S390X_KIMD		0x20
#define S390X_KLMD		0x30
#define S390X_KM		0x40
#define S390X_KMC		0x50
#define S390X_KMAC		0x60
#define S390X_KMCTR		0x70
#define S390X_KMO		0x80
#define S390X_KMF		0x90
#define S390X_PRNO		0xa0
#define S390X_KMA		0xb0

/* Facility Bit Numbers */
#define S390X_VX		129
#define S390X_VXD		134
#define S390X_VXE		135

/* Function Codes */

/* all instructions */
#define S390X_QUERY		0

/* kimd/klmd */
#define S390X_SHA3_224		32
#define S390X_SHA3_256		33
#define S390X_SHA3_384		34
#define S390X_SHA3_512		35
#define S390X_SHAKE_128	36
#define S390X_SHAKE_256	37
#define S390X_GHASH		65

/* km/kmc/kmac/kmctr/kmo/kmf/kma */
#define S390X_AES_128		18
#define S390X_AES_192		19
#define S390X_AES_256		20

/* prno */
#define S390X_TRNG		114

/* Register 0 Flags */
#define S390X_DECRYPT		0x80
#define S390X_KMA_LPC		0x100
#define S390X_KMA_LAAD		0x200
#define S390X_KMA_HS		0x400

#endif
