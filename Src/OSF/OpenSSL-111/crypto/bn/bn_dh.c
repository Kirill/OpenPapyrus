/*
 * Copyright 2014-2017 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */
#include "internal/cryptlib.h"
#pragma hdrstop
#include "bn_lcl.h"
#include "internal/nelem.h"
#ifndef OPENSSL_NO_DH
#include <openssl/dh.h>
#include <bn_dh.h>
/* DH parameters from RFC5114 */

#if BN_BITS2 == 64
static const BN_ULONG dh1024_160_p[] = {
	0xDF1FB2BC2E4A4371ULL, 0xE68CFDA76D4DA708ULL, 0x45BF37DF365C1A65ULL,
	0xA151AF5F0DC8B4BDULL, 0xFAA31A4FF55BCCC0ULL, 0x4EFFD6FAE5644738ULL,
	0x98488E9C219A7372ULL, 0xACCBDD7D90C4BD70ULL, 0x24975C3CD49B83BFULL,
	0x13ECB4AEA9061123ULL, 0x9838EF1E2EE652C0ULL, 0x6073E28675A23D18ULL,
	0x9A6A9DCA52D23B61ULL, 0x52C99FBCFB06A3C6ULL, 0xDE92DE5EAE5D54ECULL,
	0xB10B8F96A080E01DULL
};

static const BN_ULONG dh1024_160_g[] = {
	0x855E6EEB22B3B2E5ULL, 0x858F4DCEF97C2A24ULL, 0x2D779D5918D08BC8ULL,
	0xD662A4D18E73AFA3ULL, 0x1DBF0A0169B6A28AULL, 0xA6A24C087A091F53ULL,
	0x909D0D2263F80A76ULL, 0xD7FBD7D3B9A92EE1ULL, 0x5E91547F9E2749F4ULL,
	0x160217B4B01B886AULL, 0x777E690F5504F213ULL, 0x266FEA1E5C41564BULL,
	0xD6406CFF14266D31ULL, 0xF8104DD258AC507FULL, 0x6765A442EFB99905ULL,
	0xA4D1CBD5C3FD3412ULL
};

static const BN_ULONG dh1024_160_q[] = {
	0x64B7CB9D49462353ULL, 0x81A8DF278ABA4E7DULL, 0x00000000F518AA87ULL
};

static const BN_ULONG dh2048_224_p[] = {
	0x0AC4DFFE0C10E64FULL, 0xCF9DE5384E71B81CULL, 0x7EF363E2FFA31F71ULL,
	0xE3FB73C16B8E75B9ULL, 0xC9B53DCF4BA80A29ULL, 0x23F10B0E16E79763ULL,
	0xC52172E413042E9BULL, 0xBE60E69CC928B2B9ULL, 0x80CD86A1B9E587E8ULL,
	0x315D75E198C641A4ULL, 0xCDF93ACC44328387ULL, 0x15987D9ADC0A486DULL,
	0x7310F7121FD5A074ULL, 0x278273C7DE31EFDCULL, 0x1602E714415D9330ULL,
	0x81286130BC8985DBULL, 0xB3BF8A3170918836ULL, 0x6A00E0A0B9C49708ULL,
	0xC6BA0B2C8BBC27BEULL, 0xC9F98D11ED34DBF6ULL, 0x7AD5B7D0B6C12207ULL,
	0xD91E8FEF55B7394BULL, 0x9037C9EDEFDA4DF8ULL, 0x6D3F8152AD6AC212ULL,
	0x1DE6B85A1274A0A6ULL, 0xEB3D688A309C180EULL, 0xAF9A3C407BA1DF15ULL,
	0xE6FA141DF95A56DBULL, 0xB54B1597B61D0A75ULL, 0xA20D64E5683B9FD1ULL,
	0xD660FAA79559C51FULL, 0xAD107E1E9123A9D0ULL
};

static const BN_ULONG dh2048_224_g[] = {
	0x84B890D3191F2BFAULL, 0x81BC087F2A7065B3ULL, 0x19C418E1F6EC0179ULL,
	0x7B5A0F1C71CFFF4CULL, 0xEDFE72FE9B6AA4BDULL, 0x81E1BCFE94B30269ULL,
	0x566AFBB48D6C0191ULL, 0xB539CCE3409D13CDULL, 0x6AA21E7F5F2FF381ULL,
	0xD9E263E4770589EFULL, 0x10E183EDD19963DDULL, 0xB70A8137150B8EEBULL,
	0x051AE3D428C8F8ACULL, 0xBB77A86F0C1AB15BULL, 0x6E3025E316A330EFULL,
	0x19529A45D6F83456ULL, 0xF180EB34118E98D1ULL, 0xB5F6C6B250717CBEULL,
	0x09939D54DA7460CDULL, 0xE247150422EA1ED4ULL, 0xB8A762D0521BC98AULL,
	0xF4D027275AC1348BULL, 0xC17669101999024AULL, 0xBE5E9001A8D66AD7ULL,
	0xC57DB17C620A8652ULL, 0xAB739D7700C29F52ULL, 0xDD921F01A70C4AFAULL,
	0xA6824A4E10B9A6F0ULL, 0x74866A08CFE4FFE3ULL, 0x6CDEBE7B89998CAFULL,
	0x9DF30B5C8FFDAC50ULL, 0xAC4032EF4F2D9AE3ULL
};

static const BN_ULONG dh2048_224_q[] = {
	0xBF389A99B36371EBULL, 0x1F80535A4738CEBCULL, 0xC58D93FE99717710ULL,
	0x00000000801C0D34ULL
};

static const BN_ULONG dh2048_256_p[] = {
	0xDB094AE91E1A1597ULL, 0x693877FAD7EF09CAULL, 0x6116D2276E11715FULL,
	0xA4B54330C198AF12ULL, 0x75F26375D7014103ULL, 0xC3A3960A54E710C3ULL,
	0xDED4010ABD0BE621ULL, 0xC0B857F689962856ULL, 0xB3CA3F7971506026ULL,
	0x1CCACB83E6B486F6ULL, 0x67E144E514056425ULL, 0xF6A167B5A41825D9ULL,
	0x3AD8347796524D8EULL, 0xF13C6D9A51BFA4ABULL, 0x2D52526735488A0EULL,
	0xB63ACAE1CAA6B790ULL, 0x4FDB70C581B23F76ULL, 0xBC39A0BF12307F5CULL,
	0xB941F54EB1E59BB8ULL, 0x6C5BFC11D45F9088ULL, 0x22E0B1EF4275BF7BULL,
	0x91F9E6725B4758C0ULL, 0x5A8A9D306BCF67EDULL, 0x209E0C6497517ABDULL,
	0x3BF4296D830E9A7CULL, 0x16C3D91134096FAAULL, 0xFAF7DF4561B2AA30ULL,
	0xE00DF8F1D61957D4ULL, 0x5D2CEED4435E3B00ULL, 0x8CEEF608660DD0F2ULL,
	0xFFBBD19C65195999ULL, 0x87A8E61DB4B6663CULL
};

static const BN_ULONG dh2048_256_g[] = {
	0x664B4C0F6CC41659ULL, 0x5E2327CFEF98C582ULL, 0xD647D148D4795451ULL,
	0x2F63078490F00EF8ULL, 0x184B523D1DB246C3ULL, 0xC7891428CDC67EB6ULL,
	0x7FD028370DF92B52ULL, 0xB3353BBB64E0EC37ULL, 0xECD06E1557CD0915ULL,
	0xB7D2BBD2DF016199ULL, 0xC8484B1E052588B9ULL, 0xDB2A3B7313D3FE14ULL,
	0xD052B985D182EA0AULL, 0xA4BD1BFFE83B9C80ULL, 0xDFC967C1FB3F2E55ULL,
	0xB5045AF2767164E1ULL, 0x1D14348F6F2F9193ULL, 0x64E67982428EBC83ULL,
	0x8AC376D282D6ED38ULL, 0x777DE62AAAB8A862ULL, 0xDDF463E5E9EC144BULL,
	0x0196F931C77A57F2ULL, 0xA55AE31341000A65ULL, 0x901228F8C28CBB18ULL,
	0xBC3773BF7E8C6F62ULL, 0xBE3A6C1B0C6B47B1ULL, 0xFF4FED4AAC0BB555ULL,
	0x10DBC15077BE463FULL, 0x07F4793A1A0BA125ULL, 0x4CA7B18F21EF2054ULL,
	0x2E77506660EDBD48ULL, 0x3FB32C9B73134D0BULL
};

static const BN_ULONG dh2048_256_q[] = {
	0xA308B0FE64F5FBD3ULL, 0x99B1A47D1EB3750BULL, 0xB447997640129DA2ULL,
	0x8CF83642A709A097ULL
};

/* Primes from RFC 7919 */
static const BN_ULONG ffdhe2048_p[] = {
	0xFFFFFFFFFFFFFFFFULL, 0x886B423861285C97ULL, 0xC6F34A26C1B2EFFAULL,
	0xC58EF1837D1683B2ULL, 0x3BB5FCBC2EC22005ULL, 0xC3FE3B1B4C6FAD73ULL,
	0x8E4F1232EEF28183ULL, 0x9172FE9CE98583FFULL, 0xC03404CD28342F61ULL,
	0x9E02FCE1CDF7E2ECULL, 0x0B07A7C8EE0A6D70ULL, 0xAE56EDE76372BB19ULL,
	0x1D4F42A3DE394DF4ULL, 0xB96ADAB760D7F468ULL, 0xD108A94BB2C8E3FBULL,
	0xBC0AB182B324FB61ULL, 0x30ACCA4F483A797AULL, 0x1DF158A136ADE735ULL,
	0xE2A689DAF3EFE872ULL, 0x984F0C70E0E68B77ULL, 0xB557135E7F57C935ULL,
	0x856365553DED1AF3ULL, 0x2433F51F5F066ED0ULL, 0xD3DF1ED5D5FD6561ULL,
	0xF681B202AEC4617AULL, 0x7D2FE363630C75D8ULL, 0xCC939DCE249B3EF9ULL,
	0xA9E13641146433FBULL, 0xD8B9C583CE2D3695ULL, 0xAFDC5620273D3CF1ULL,
	0xADF85458A2BB4A9AULL, 0xFFFFFFFFFFFFFFFFULL
};

static const BN_ULONG ffdhe3072_p[] = {
	0xFFFFFFFFFFFFFFFFULL, 0x25E41D2B66C62E37ULL, 0x3C1B20EE3FD59D7CULL,
	0x0ABCD06BFA53DDEFULL, 0x1DBF9A42D5C4484EULL, 0xABC521979B0DEADAULL,
	0xE86D2BC522363A0DULL, 0x5CAE82AB9C9DF69EULL, 0x64F2E21E71F54BFFULL,
	0xF4FD4452E2D74DD3ULL, 0xB4130C93BC437944ULL, 0xAEFE130985139270ULL,
	0x598CB0FAC186D91CULL, 0x7AD91D2691F7F7EEULL, 0x61B46FC9D6E6C907ULL,
	0xBC34F4DEF99C0238ULL, 0xDE355B3B6519035BULL, 0x886B4238611FCFDCULL,
	0xC6F34A26C1B2EFFAULL, 0xC58EF1837D1683B2ULL, 0x3BB5FCBC2EC22005ULL,
	0xC3FE3B1B4C6FAD73ULL, 0x8E4F1232EEF28183ULL, 0x9172FE9CE98583FFULL,
	0xC03404CD28342F61ULL, 0x9E02FCE1CDF7E2ECULL, 0x0B07A7C8EE0A6D70ULL,
	0xAE56EDE76372BB19ULL, 0x1D4F42A3DE394DF4ULL, 0xB96ADAB760D7F468ULL,
	0xD108A94BB2C8E3FBULL, 0xBC0AB182B324FB61ULL, 0x30ACCA4F483A797AULL,
	0x1DF158A136ADE735ULL, 0xE2A689DAF3EFE872ULL, 0x984F0C70E0E68B77ULL,
	0xB557135E7F57C935ULL, 0x856365553DED1AF3ULL, 0x2433F51F5F066ED0ULL,
	0xD3DF1ED5D5FD6561ULL, 0xF681B202AEC4617AULL, 0x7D2FE363630C75D8ULL,
	0xCC939DCE249B3EF9ULL, 0xA9E13641146433FBULL, 0xD8B9C583CE2D3695ULL,
	0xAFDC5620273D3CF1ULL, 0xADF85458A2BB4A9AULL, 0xFFFFFFFFFFFFFFFFULL
};

static const BN_ULONG ffdhe4096_p[] = {
	0xFFFFFFFFFFFFFFFFULL, 0xC68A007E5E655F6AULL, 0x4DB5A851F44182E1ULL,
	0x8EC9B55A7F88A46BULL, 0x0A8291CDCEC97DCFULL, 0x2A4ECEA9F98D0ACCULL,
	0x1A1DB93D7140003CULL, 0x092999A333CB8B7AULL, 0x6DC778F971AD0038ULL,
	0xA907600A918130C4ULL, 0xED6A1E012D9E6832ULL, 0x7135C886EFB4318AULL,
	0x87F55BA57E31CC7AULL, 0x7763CF1D55034004ULL, 0xAC7D5F42D69F6D18ULL,
	0x7930E9E4E58857B6ULL, 0x6E6F52C3164DF4FBULL, 0x25E41D2B669E1EF1ULL,
	0x3C1B20EE3FD59D7CULL, 0x0ABCD06BFA53DDEFULL, 0x1DBF9A42D5C4484EULL,
	0xABC521979B0DEADAULL, 0xE86D2BC522363A0DULL, 0x5CAE82AB9C9DF69EULL,
	0x64F2E21E71F54BFFULL, 0xF4FD4452E2D74DD3ULL, 0xB4130C93BC437944ULL,
	0xAEFE130985139270ULL, 0x598CB0FAC186D91CULL, 0x7AD91D2691F7F7EEULL,
	0x61B46FC9D6E6C907ULL, 0xBC34F4DEF99C0238ULL, 0xDE355B3B6519035BULL,
	0x886B4238611FCFDCULL, 0xC6F34A26C1B2EFFAULL, 0xC58EF1837D1683B2ULL,
	0x3BB5FCBC2EC22005ULL, 0xC3FE3B1B4C6FAD73ULL, 0x8E4F1232EEF28183ULL,
	0x9172FE9CE98583FFULL, 0xC03404CD28342F61ULL, 0x9E02FCE1CDF7E2ECULL,
	0x0B07A7C8EE0A6D70ULL, 0xAE56EDE76372BB19ULL, 0x1D4F42A3DE394DF4ULL,
	0xB96ADAB760D7F468ULL, 0xD108A94BB2C8E3FBULL, 0xBC0AB182B324FB61ULL,
	0x30ACCA4F483A797AULL, 0x1DF158A136ADE735ULL, 0xE2A689DAF3EFE872ULL,
	0x984F0C70E0E68B77ULL, 0xB557135E7F57C935ULL, 0x856365553DED1AF3ULL,
	0x2433F51F5F066ED0ULL, 0xD3DF1ED5D5FD6561ULL, 0xF681B202AEC4617AULL,
	0x7D2FE363630C75D8ULL, 0xCC939DCE249B3EF9ULL, 0xA9E13641146433FBULL,
	0xD8B9C583CE2D3695ULL, 0xAFDC5620273D3CF1ULL, 0xADF85458A2BB4A9AULL,
	0xFFFFFFFFFFFFFFFFULL
};

static const BN_ULONG ffdhe6144_p[] = {
	0xFFFFFFFFFFFFFFFFULL, 0xA40E329CD0E40E65ULL, 0xA41D570D7938DAD4ULL,
	0x62A69526D43161C1ULL, 0x3FDD4A8E9ADB1E69ULL, 0x5B3B71F9DC6B80D6ULL,
	0xEC9D1810C6272B04ULL, 0x8CCF2DD5CACEF403ULL, 0xE49F5235C95B9117ULL,
	0x505DC82DB854338AULL, 0x62292C311562A846ULL, 0xD72B03746AE77F5EULL,
	0xF9C9091B462D538CULL, 0x0AE8DB5847A67CBEULL, 0xB3A739C122611682ULL,
	0xEEAAC0232A281BF6ULL, 0x94C6651E77CAF992ULL, 0x763E4E4B94B2BBC1ULL,
	0x587E38DA0077D9B4ULL, 0x7FB29F8C183023C3ULL, 0x0ABEC1FFF9E3A26EULL,
	0xA00EF092350511E3ULL, 0xB855322EDB6340D8ULL, 0xA52471F7A9A96910ULL,
	0x388147FB4CFDB477ULL, 0x9B1F5C3E4E46041FULL, 0xCDAD0657FCCFEC71ULL,
	0xB38E8C334C701C3AULL, 0x917BDD64B1C0FD4CULL, 0x3BB454329B7624C8ULL,
	0x23BA4442CAF53EA6ULL, 0x4E677D2C38532A3AULL, 0x0BFD64B645036C7AULL,
	0xC68A007E5E0DD902ULL, 0x4DB5A851F44182E1ULL, 0x8EC9B55A7F88A46BULL,
	0x0A8291CDCEC97DCFULL, 0x2A4ECEA9F98D0ACCULL, 0x1A1DB93D7140003CULL,
	0x092999A333CB8B7AULL, 0x6DC778F971AD0038ULL, 0xA907600A918130C4ULL,
	0xED6A1E012D9E6832ULL, 0x7135C886EFB4318AULL, 0x87F55BA57E31CC7AULL,
	0x7763CF1D55034004ULL, 0xAC7D5F42D69F6D18ULL, 0x7930E9E4E58857B6ULL,
	0x6E6F52C3164DF4FBULL, 0x25E41D2B669E1EF1ULL, 0x3C1B20EE3FD59D7CULL,
	0x0ABCD06BFA53DDEFULL, 0x1DBF9A42D5C4484EULL, 0xABC521979B0DEADAULL,
	0xE86D2BC522363A0DULL, 0x5CAE82AB9C9DF69EULL, 0x64F2E21E71F54BFFULL,
	0xF4FD4452E2D74DD3ULL, 0xB4130C93BC437944ULL, 0xAEFE130985139270ULL,
	0x598CB0FAC186D91CULL, 0x7AD91D2691F7F7EEULL, 0x61B46FC9D6E6C907ULL,
	0xBC34F4DEF99C0238ULL, 0xDE355B3B6519035BULL, 0x886B4238611FCFDCULL,
	0xC6F34A26C1B2EFFAULL, 0xC58EF1837D1683B2ULL, 0x3BB5FCBC2EC22005ULL,
	0xC3FE3B1B4C6FAD73ULL, 0x8E4F1232EEF28183ULL, 0x9172FE9CE98583FFULL,
	0xC03404CD28342F61ULL, 0x9E02FCE1CDF7E2ECULL, 0x0B07A7C8EE0A6D70ULL,
	0xAE56EDE76372BB19ULL, 0x1D4F42A3DE394DF4ULL, 0xB96ADAB760D7F468ULL,
	0xD108A94BB2C8E3FBULL, 0xBC0AB182B324FB61ULL, 0x30ACCA4F483A797AULL,
	0x1DF158A136ADE735ULL, 0xE2A689DAF3EFE872ULL, 0x984F0C70E0E68B77ULL,
	0xB557135E7F57C935ULL, 0x856365553DED1AF3ULL, 0x2433F51F5F066ED0ULL,
	0xD3DF1ED5D5FD6561ULL, 0xF681B202AEC4617AULL, 0x7D2FE363630C75D8ULL,
	0xCC939DCE249B3EF9ULL, 0xA9E13641146433FBULL, 0xD8B9C583CE2D3695ULL,
	0xAFDC5620273D3CF1ULL, 0xADF85458A2BB4A9AULL, 0xFFFFFFFFFFFFFFFFULL
};

static const BN_ULONG ffdhe8192_p[] = {
	0xFFFFFFFFFFFFFFFFULL, 0xD68C8BB7C5C6424CULL, 0x011E2A94838FF88CULL,
	0x0822E506A9F4614EULL, 0x97D11D49F7A8443DULL, 0xA6BBFDE530677F0DULL,
	0x2F741EF8C1FE86FEULL, 0xFAFABE1C5D71A87EULL, 0xDED2FBABFBE58A30ULL,
	0xB6855DFE72B0A66EULL, 0x1EFC8CE0BA8A4FE8ULL, 0x83F81D4A3F2FA457ULL,
	0xA1FE3075A577E231ULL, 0xD5B8019488D9C0A0ULL, 0x624816CDAD9A95F9ULL,
	0x99E9E31650C1217BULL, 0x51AA691E0E423CFCULL, 0x1C217E6C3826E52CULL,
	0x51A8A93109703FEEULL, 0xBB7099876A460E74ULL, 0x541FC68C9C86B022ULL,
	0x59160CC046FD8251ULL, 0x2846C0BA35C35F5CULL, 0x54504AC78B758282ULL,
	0x29388839D2AF05E4ULL, 0xCB2C0F1CC01BD702ULL, 0x555B2F747C932665ULL,
	0x86B63142A3AB8829ULL, 0x0B8CC3BDF64B10EFULL, 0x687FEB69EDD1CC5EULL,
	0xFDB23FCEC9509D43ULL, 0x1E425A31D951AE64ULL, 0x36AD004CF600C838ULL,
	0xA40E329CCFF46AAAULL, 0xA41D570D7938DAD4ULL, 0x62A69526D43161C1ULL,
	0x3FDD4A8E9ADB1E69ULL, 0x5B3B71F9DC6B80D6ULL, 0xEC9D1810C6272B04ULL,
	0x8CCF2DD5CACEF403ULL, 0xE49F5235C95B9117ULL, 0x505DC82DB854338AULL,
	0x62292C311562A846ULL, 0xD72B03746AE77F5EULL, 0xF9C9091B462D538CULL,
	0x0AE8DB5847A67CBEULL, 0xB3A739C122611682ULL, 0xEEAAC0232A281BF6ULL,
	0x94C6651E77CAF992ULL, 0x763E4E4B94B2BBC1ULL, 0x587E38DA0077D9B4ULL,
	0x7FB29F8C183023C3ULL, 0x0ABEC1FFF9E3A26EULL, 0xA00EF092350511E3ULL,
	0xB855322EDB6340D8ULL, 0xA52471F7A9A96910ULL, 0x388147FB4CFDB477ULL,
	0x9B1F5C3E4E46041FULL, 0xCDAD0657FCCFEC71ULL, 0xB38E8C334C701C3AULL,
	0x917BDD64B1C0FD4CULL, 0x3BB454329B7624C8ULL, 0x23BA4442CAF53EA6ULL,
	0x4E677D2C38532A3AULL, 0x0BFD64B645036C7AULL, 0xC68A007E5E0DD902ULL,
	0x4DB5A851F44182E1ULL, 0x8EC9B55A7F88A46BULL, 0x0A8291CDCEC97DCFULL,
	0x2A4ECEA9F98D0ACCULL, 0x1A1DB93D7140003CULL, 0x092999A333CB8B7AULL,
	0x6DC778F971AD0038ULL, 0xA907600A918130C4ULL, 0xED6A1E012D9E6832ULL,
	0x7135C886EFB4318AULL, 0x87F55BA57E31CC7AULL, 0x7763CF1D55034004ULL,
	0xAC7D5F42D69F6D18ULL, 0x7930E9E4E58857B6ULL, 0x6E6F52C3164DF4FBULL,
	0x25E41D2B669E1EF1ULL, 0x3C1B20EE3FD59D7CULL, 0x0ABCD06BFA53DDEFULL,
	0x1DBF9A42D5C4484EULL, 0xABC521979B0DEADAULL, 0xE86D2BC522363A0DULL,
	0x5CAE82AB9C9DF69EULL, 0x64F2E21E71F54BFFULL, 0xF4FD4452E2D74DD3ULL,
	0xB4130C93BC437944ULL, 0xAEFE130985139270ULL, 0x598CB0FAC186D91CULL,
	0x7AD91D2691F7F7EEULL, 0x61B46FC9D6E6C907ULL, 0xBC34F4DEF99C0238ULL,
	0xDE355B3B6519035BULL, 0x886B4238611FCFDCULL, 0xC6F34A26C1B2EFFAULL,
	0xC58EF1837D1683B2ULL, 0x3BB5FCBC2EC22005ULL, 0xC3FE3B1B4C6FAD73ULL,
	0x8E4F1232EEF28183ULL, 0x9172FE9CE98583FFULL, 0xC03404CD28342F61ULL,
	0x9E02FCE1CDF7E2ECULL, 0x0B07A7C8EE0A6D70ULL, 0xAE56EDE76372BB19ULL,
	0x1D4F42A3DE394DF4ULL, 0xB96ADAB760D7F468ULL, 0xD108A94BB2C8E3FBULL,
	0xBC0AB182B324FB61ULL, 0x30ACCA4F483A797AULL, 0x1DF158A136ADE735ULL,
	0xE2A689DAF3EFE872ULL, 0x984F0C70E0E68B77ULL, 0xB557135E7F57C935ULL,
	0x856365553DED1AF3ULL, 0x2433F51F5F066ED0ULL, 0xD3DF1ED5D5FD6561ULL,
	0xF681B202AEC4617AULL, 0x7D2FE363630C75D8ULL, 0xCC939DCE249B3EF9ULL,
	0xA9E13641146433FBULL, 0xD8B9C583CE2D3695ULL, 0xAFDC5620273D3CF1ULL,
	0xADF85458A2BB4A9AULL, 0xFFFFFFFFFFFFFFFFULL
};

#elif BN_BITS2 == 32

static const BN_ULONG dh1024_160_p[] = {
	0x2E4A4371, 0xDF1FB2BC, 0x6D4DA708, 0xE68CFDA7, 0x365C1A65, 0x45BF37DF,
	0x0DC8B4BD, 0xA151AF5F, 0xF55BCCC0, 0xFAA31A4F, 0xE5644738, 0x4EFFD6FA,
	0x219A7372, 0x98488E9C, 0x90C4BD70, 0xACCBDD7D, 0xD49B83BF, 0x24975C3C,
	0xA9061123, 0x13ECB4AE, 0x2EE652C0, 0x9838EF1E, 0x75A23D18, 0x6073E286,
	0x52D23B61, 0x9A6A9DCA, 0xFB06A3C6, 0x52C99FBC, 0xAE5D54EC, 0xDE92DE5E,
	0xA080E01D, 0xB10B8F96
};

static const BN_ULONG dh1024_160_g[] = {
	0x22B3B2E5, 0x855E6EEB, 0xF97C2A24, 0x858F4DCE, 0x18D08BC8, 0x2D779D59,
	0x8E73AFA3, 0xD662A4D1, 0x69B6A28A, 0x1DBF0A01, 0x7A091F53, 0xA6A24C08,
	0x63F80A76, 0x909D0D22, 0xB9A92EE1, 0xD7FBD7D3, 0x9E2749F4, 0x5E91547F,
	0xB01B886A, 0x160217B4, 0x5504F213, 0x777E690F, 0x5C41564B, 0x266FEA1E,
	0x14266D31, 0xD6406CFF, 0x58AC507F, 0xF8104DD2, 0xEFB99905, 0x6765A442,
	0xC3FD3412, 0xA4D1CBD5
};

static const BN_ULONG dh1024_160_q[] = {
	0x49462353, 0x64B7CB9D, 0x8ABA4E7D, 0x81A8DF27, 0xF518AA87
};

static const BN_ULONG dh2048_224_p[] = {
	0x0C10E64F, 0x0AC4DFFE, 0x4E71B81C, 0xCF9DE538, 0xFFA31F71, 0x7EF363E2,
	0x6B8E75B9, 0xE3FB73C1, 0x4BA80A29, 0xC9B53DCF, 0x16E79763, 0x23F10B0E,
	0x13042E9B, 0xC52172E4, 0xC928B2B9, 0xBE60E69C, 0xB9E587E8, 0x80CD86A1,
	0x98C641A4, 0x315D75E1, 0x44328387, 0xCDF93ACC, 0xDC0A486D, 0x15987D9A,
	0x1FD5A074, 0x7310F712, 0xDE31EFDC, 0x278273C7, 0x415D9330, 0x1602E714,
	0xBC8985DB, 0x81286130, 0x70918836, 0xB3BF8A31, 0xB9C49708, 0x6A00E0A0,
	0x8BBC27BE, 0xC6BA0B2C, 0xED34DBF6, 0xC9F98D11, 0xB6C12207, 0x7AD5B7D0,
	0x55B7394B, 0xD91E8FEF, 0xEFDA4DF8, 0x9037C9ED, 0xAD6AC212, 0x6D3F8152,
	0x1274A0A6, 0x1DE6B85A, 0x309C180E, 0xEB3D688A, 0x7BA1DF15, 0xAF9A3C40,
	0xF95A56DB, 0xE6FA141D, 0xB61D0A75, 0xB54B1597, 0x683B9FD1, 0xA20D64E5,
	0x9559C51F, 0xD660FAA7, 0x9123A9D0, 0xAD107E1E
};

static const BN_ULONG dh2048_224_g[] = {
	0x191F2BFA, 0x84B890D3, 0x2A7065B3, 0x81BC087F, 0xF6EC0179, 0x19C418E1,
	0x71CFFF4C, 0x7B5A0F1C, 0x9B6AA4BD, 0xEDFE72FE, 0x94B30269, 0x81E1BCFE,
	0x8D6C0191, 0x566AFBB4, 0x409D13CD, 0xB539CCE3, 0x5F2FF381, 0x6AA21E7F,
	0x770589EF, 0xD9E263E4, 0xD19963DD, 0x10E183ED, 0x150B8EEB, 0xB70A8137,
	0x28C8F8AC, 0x051AE3D4, 0x0C1AB15B, 0xBB77A86F, 0x16A330EF, 0x6E3025E3,
	0xD6F83456, 0x19529A45, 0x118E98D1, 0xF180EB34, 0x50717CBE, 0xB5F6C6B2,
	0xDA7460CD, 0x09939D54, 0x22EA1ED4, 0xE2471504, 0x521BC98A, 0xB8A762D0,
	0x5AC1348B, 0xF4D02727, 0x1999024A, 0xC1766910, 0xA8D66AD7, 0xBE5E9001,
	0x620A8652, 0xC57DB17C, 0x00C29F52, 0xAB739D77, 0xA70C4AFA, 0xDD921F01,
	0x10B9A6F0, 0xA6824A4E, 0xCFE4FFE3, 0x74866A08, 0x89998CAF, 0x6CDEBE7B,
	0x8FFDAC50, 0x9DF30B5C, 0x4F2D9AE3, 0xAC4032EF
};

static const BN_ULONG dh2048_224_q[] = {
	0xB36371EB, 0xBF389A99, 0x4738CEBC, 0x1F80535A, 0x99717710, 0xC58D93FE,
	0x801C0D34
};

static const BN_ULONG dh2048_256_p[] = {
	0x1E1A1597, 0xDB094AE9, 0xD7EF09CA, 0x693877FA, 0x6E11715F, 0x6116D227,
	0xC198AF12, 0xA4B54330, 0xD7014103, 0x75F26375, 0x54E710C3, 0xC3A3960A,
	0xBD0BE621, 0xDED4010A, 0x89962856, 0xC0B857F6, 0x71506026, 0xB3CA3F79,
	0xE6B486F6, 0x1CCACB83, 0x14056425, 0x67E144E5, 0xA41825D9, 0xF6A167B5,
	0x96524D8E, 0x3AD83477, 0x51BFA4AB, 0xF13C6D9A, 0x35488A0E, 0x2D525267,
	0xCAA6B790, 0xB63ACAE1, 0x81B23F76, 0x4FDB70C5, 0x12307F5C, 0xBC39A0BF,
	0xB1E59BB8, 0xB941F54E, 0xD45F9088, 0x6C5BFC11, 0x4275BF7B, 0x22E0B1EF,
	0x5B4758C0, 0x91F9E672, 0x6BCF67ED, 0x5A8A9D30, 0x97517ABD, 0x209E0C64,
	0x830E9A7C, 0x3BF4296D, 0x34096FAA, 0x16C3D911, 0x61B2AA30, 0xFAF7DF45,
	0xD61957D4, 0xE00DF8F1, 0x435E3B00, 0x5D2CEED4, 0x660DD0F2, 0x8CEEF608,
	0x65195999, 0xFFBBD19C, 0xB4B6663C, 0x87A8E61D
};

static const BN_ULONG dh2048_256_g[] = {
	0x6CC41659, 0x664B4C0F, 0xEF98C582, 0x5E2327CF, 0xD4795451, 0xD647D148,
	0x90F00EF8, 0x2F630784, 0x1DB246C3, 0x184B523D, 0xCDC67EB6, 0xC7891428,
	0x0DF92B52, 0x7FD02837, 0x64E0EC37, 0xB3353BBB, 0x57CD0915, 0xECD06E15,
	0xDF016199, 0xB7D2BBD2, 0x052588B9, 0xC8484B1E, 0x13D3FE14, 0xDB2A3B73,
	0xD182EA0A, 0xD052B985, 0xE83B9C80, 0xA4BD1BFF, 0xFB3F2E55, 0xDFC967C1,
	0x767164E1, 0xB5045AF2, 0x6F2F9193, 0x1D14348F, 0x428EBC83, 0x64E67982,
	0x82D6ED38, 0x8AC376D2, 0xAAB8A862, 0x777DE62A, 0xE9EC144B, 0xDDF463E5,
	0xC77A57F2, 0x0196F931, 0x41000A65, 0xA55AE313, 0xC28CBB18, 0x901228F8,
	0x7E8C6F62, 0xBC3773BF, 0x0C6B47B1, 0xBE3A6C1B, 0xAC0BB555, 0xFF4FED4A,
	0x77BE463F, 0x10DBC150, 0x1A0BA125, 0x07F4793A, 0x21EF2054, 0x4CA7B18F,
	0x60EDBD48, 0x2E775066, 0x73134D0B, 0x3FB32C9B
};

static const BN_ULONG dh2048_256_q[] = {
	0x64F5FBD3, 0xA308B0FE, 0x1EB3750B, 0x99B1A47D, 0x40129DA2, 0xB4479976,
	0xA709A097, 0x8CF83642
};

/* Primes from RFC 7919 */

static const BN_ULONG ffdhe2048_p[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0x61285C97, 0x886B4238, 0xC1B2EFFA, 0xC6F34A26,
	0x7D1683B2, 0xC58EF183, 0x2EC22005, 0x3BB5FCBC, 0x4C6FAD73, 0xC3FE3B1B,
	0xEEF28183, 0x8E4F1232, 0xE98583FF, 0x9172FE9C, 0x28342F61, 0xC03404CD,
	0xCDF7E2EC, 0x9E02FCE1, 0xEE0A6D70, 0x0B07A7C8, 0x6372BB19, 0xAE56EDE7,
	0xDE394DF4, 0x1D4F42A3, 0x60D7F468, 0xB96ADAB7, 0xB2C8E3FB, 0xD108A94B,
	0xB324FB61, 0xBC0AB182, 0x483A797A, 0x30ACCA4F, 0x36ADE735, 0x1DF158A1,
	0xF3EFE872, 0xE2A689DA, 0xE0E68B77, 0x984F0C70, 0x7F57C935, 0xB557135E,
	0x3DED1AF3, 0x85636555, 0x5F066ED0, 0x2433F51F, 0xD5FD6561, 0xD3DF1ED5,
	0xAEC4617A, 0xF681B202, 0x630C75D8, 0x7D2FE363, 0x249B3EF9, 0xCC939DCE,
	0x146433FB, 0xA9E13641, 0xCE2D3695, 0xD8B9C583, 0x273D3CF1, 0xAFDC5620,
	0xA2BB4A9A, 0xADF85458, 0xFFFFFFFF, 0xFFFFFFFF
};

static const BN_ULONG ffdhe3072_p[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0x66C62E37, 0x25E41D2B, 0x3FD59D7C, 0x3C1B20EE,
	0xFA53DDEF, 0x0ABCD06B, 0xD5C4484E, 0x1DBF9A42, 0x9B0DEADA, 0xABC52197,
	0x22363A0D, 0xE86D2BC5, 0x9C9DF69E, 0x5CAE82AB, 0x71F54BFF, 0x64F2E21E,
	0xE2D74DD3, 0xF4FD4452, 0xBC437944, 0xB4130C93, 0x85139270, 0xAEFE1309,
	0xC186D91C, 0x598CB0FA, 0x91F7F7EE, 0x7AD91D26, 0xD6E6C907, 0x61B46FC9,
	0xF99C0238, 0xBC34F4DE, 0x6519035B, 0xDE355B3B, 0x611FCFDC, 0x886B4238,
	0xC1B2EFFA, 0xC6F34A26, 0x7D1683B2, 0xC58EF183, 0x2EC22005, 0x3BB5FCBC,
	0x4C6FAD73, 0xC3FE3B1B, 0xEEF28183, 0x8E4F1232, 0xE98583FF, 0x9172FE9C,
	0x28342F61, 0xC03404CD, 0xCDF7E2EC, 0x9E02FCE1, 0xEE0A6D70, 0x0B07A7C8,
	0x6372BB19, 0xAE56EDE7, 0xDE394DF4, 0x1D4F42A3, 0x60D7F468, 0xB96ADAB7,
	0xB2C8E3FB, 0xD108A94B, 0xB324FB61, 0xBC0AB182, 0x483A797A, 0x30ACCA4F,
	0x36ADE735, 0x1DF158A1, 0xF3EFE872, 0xE2A689DA, 0xE0E68B77, 0x984F0C70,
	0x7F57C935, 0xB557135E, 0x3DED1AF3, 0x85636555, 0x5F066ED0, 0x2433F51F,
	0xD5FD6561, 0xD3DF1ED5, 0xAEC4617A, 0xF681B202, 0x630C75D8, 0x7D2FE363,
	0x249B3EF9, 0xCC939DCE, 0x146433FB, 0xA9E13641, 0xCE2D3695, 0xD8B9C583,
	0x273D3CF1, 0xAFDC5620, 0xA2BB4A9A, 0xADF85458, 0xFFFFFFFF, 0xFFFFFFFF
};

static const BN_ULONG ffdhe4096_p[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0x5E655F6A, 0xC68A007E, 0xF44182E1, 0x4DB5A851,
	0x7F88A46B, 0x8EC9B55A, 0xCEC97DCF, 0x0A8291CD, 0xF98D0ACC, 0x2A4ECEA9,
	0x7140003C, 0x1A1DB93D, 0x33CB8B7A, 0x092999A3, 0x71AD0038, 0x6DC778F9,
	0x918130C4, 0xA907600A, 0x2D9E6832, 0xED6A1E01, 0xEFB4318A, 0x7135C886,
	0x7E31CC7A, 0x87F55BA5, 0x55034004, 0x7763CF1D, 0xD69F6D18, 0xAC7D5F42,
	0xE58857B6, 0x7930E9E4, 0x164DF4FB, 0x6E6F52C3, 0x669E1EF1, 0x25E41D2B,
	0x3FD59D7C, 0x3C1B20EE, 0xFA53DDEF, 0x0ABCD06B, 0xD5C4484E, 0x1DBF9A42,
	0x9B0DEADA, 0xABC52197, 0x22363A0D, 0xE86D2BC5, 0x9C9DF69E, 0x5CAE82AB,
	0x71F54BFF, 0x64F2E21E, 0xE2D74DD3, 0xF4FD4452, 0xBC437944, 0xB4130C93,
	0x85139270, 0xAEFE1309, 0xC186D91C, 0x598CB0FA, 0x91F7F7EE, 0x7AD91D26,
	0xD6E6C907, 0x61B46FC9, 0xF99C0238, 0xBC34F4DE, 0x6519035B, 0xDE355B3B,
	0x611FCFDC, 0x886B4238, 0xC1B2EFFA, 0xC6F34A26, 0x7D1683B2, 0xC58EF183,
	0x2EC22005, 0x3BB5FCBC, 0x4C6FAD73, 0xC3FE3B1B, 0xEEF28183, 0x8E4F1232,
	0xE98583FF, 0x9172FE9C, 0x28342F61, 0xC03404CD, 0xCDF7E2EC, 0x9E02FCE1,
	0xEE0A6D70, 0x0B07A7C8, 0x6372BB19, 0xAE56EDE7, 0xDE394DF4, 0x1D4F42A3,
	0x60D7F468, 0xB96ADAB7, 0xB2C8E3FB, 0xD108A94B, 0xB324FB61, 0xBC0AB182,
	0x483A797A, 0x30ACCA4F, 0x36ADE735, 0x1DF158A1, 0xF3EFE872, 0xE2A689DA,
	0xE0E68B77, 0x984F0C70, 0x7F57C935, 0xB557135E, 0x3DED1AF3, 0x85636555,
	0x5F066ED0, 0x2433F51F, 0xD5FD6561, 0xD3DF1ED5, 0xAEC4617A, 0xF681B202,
	0x630C75D8, 0x7D2FE363, 0x249B3EF9, 0xCC939DCE, 0x146433FB, 0xA9E13641,
	0xCE2D3695, 0xD8B9C583, 0x273D3CF1, 0xAFDC5620, 0xA2BB4A9A, 0xADF85458,
	0xFFFFFFFF, 0xFFFFFFFF
};

static const BN_ULONG ffdhe6144_p[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xD0E40E65, 0xA40E329C, 0x7938DAD4, 0xA41D570D,
	0xD43161C1, 0x62A69526, 0x9ADB1E69, 0x3FDD4A8E, 0xDC6B80D6, 0x5B3B71F9,
	0xC6272B04, 0xEC9D1810, 0xCACEF403, 0x8CCF2DD5, 0xC95B9117, 0xE49F5235,
	0xB854338A, 0x505DC82D, 0x1562A846, 0x62292C31, 0x6AE77F5E, 0xD72B0374,
	0x462D538C, 0xF9C9091B, 0x47A67CBE, 0x0AE8DB58, 0x22611682, 0xB3A739C1,
	0x2A281BF6, 0xEEAAC023, 0x77CAF992, 0x94C6651E, 0x94B2BBC1, 0x763E4E4B,
	0x0077D9B4, 0x587E38DA, 0x183023C3, 0x7FB29F8C, 0xF9E3A26E, 0x0ABEC1FF,
	0x350511E3, 0xA00EF092, 0xDB6340D8, 0xB855322E, 0xA9A96910, 0xA52471F7,
	0x4CFDB477, 0x388147FB, 0x4E46041F, 0x9B1F5C3E, 0xFCCFEC71, 0xCDAD0657,
	0x4C701C3A, 0xB38E8C33, 0xB1C0FD4C, 0x917BDD64, 0x9B7624C8, 0x3BB45432,
	0xCAF53EA6, 0x23BA4442, 0x38532A3A, 0x4E677D2C, 0x45036C7A, 0x0BFD64B6,
	0x5E0DD902, 0xC68A007E, 0xF44182E1, 0x4DB5A851, 0x7F88A46B, 0x8EC9B55A,
	0xCEC97DCF, 0x0A8291CD, 0xF98D0ACC, 0x2A4ECEA9, 0x7140003C, 0x1A1DB93D,
	0x33CB8B7A, 0x092999A3, 0x71AD0038, 0x6DC778F9, 0x918130C4, 0xA907600A,
	0x2D9E6832, 0xED6A1E01, 0xEFB4318A, 0x7135C886, 0x7E31CC7A, 0x87F55BA5,
	0x55034004, 0x7763CF1D, 0xD69F6D18, 0xAC7D5F42, 0xE58857B6, 0x7930E9E4,
	0x164DF4FB, 0x6E6F52C3, 0x669E1EF1, 0x25E41D2B, 0x3FD59D7C, 0x3C1B20EE,
	0xFA53DDEF, 0x0ABCD06B, 0xD5C4484E, 0x1DBF9A42, 0x9B0DEADA, 0xABC52197,
	0x22363A0D, 0xE86D2BC5, 0x9C9DF69E, 0x5CAE82AB, 0x71F54BFF, 0x64F2E21E,
	0xE2D74DD3, 0xF4FD4452, 0xBC437944, 0xB4130C93, 0x85139270, 0xAEFE1309,
	0xC186D91C, 0x598CB0FA, 0x91F7F7EE, 0x7AD91D26, 0xD6E6C907, 0x61B46FC9,
	0xF99C0238, 0xBC34F4DE, 0x6519035B, 0xDE355B3B, 0x611FCFDC, 0x886B4238,
	0xC1B2EFFA, 0xC6F34A26, 0x7D1683B2, 0xC58EF183, 0x2EC22005, 0x3BB5FCBC,
	0x4C6FAD73, 0xC3FE3B1B, 0xEEF28183, 0x8E4F1232, 0xE98583FF, 0x9172FE9C,
	0x28342F61, 0xC03404CD, 0xCDF7E2EC, 0x9E02FCE1, 0xEE0A6D70, 0x0B07A7C8,
	0x6372BB19, 0xAE56EDE7, 0xDE394DF4, 0x1D4F42A3, 0x60D7F468, 0xB96ADAB7,
	0xB2C8E3FB, 0xD108A94B, 0xB324FB61, 0xBC0AB182, 0x483A797A, 0x30ACCA4F,
	0x36ADE735, 0x1DF158A1, 0xF3EFE872, 0xE2A689DA, 0xE0E68B77, 0x984F0C70,
	0x7F57C935, 0xB557135E, 0x3DED1AF3, 0x85636555, 0x5F066ED0, 0x2433F51F,
	0xD5FD6561, 0xD3DF1ED5, 0xAEC4617A, 0xF681B202, 0x630C75D8, 0x7D2FE363,
	0x249B3EF9, 0xCC939DCE, 0x146433FB, 0xA9E13641, 0xCE2D3695, 0xD8B9C583,
	0x273D3CF1, 0xAFDC5620, 0xA2BB4A9A, 0xADF85458, 0xFFFFFFFF, 0xFFFFFFFF
};

static const BN_ULONG ffdhe8192_p[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xC5C6424C, 0xD68C8BB7, 0x838FF88C, 0x011E2A94,
	0xA9F4614E, 0x0822E506, 0xF7A8443D, 0x97D11D49, 0x30677F0D, 0xA6BBFDE5,
	0xC1FE86FE, 0x2F741EF8, 0x5D71A87E, 0xFAFABE1C, 0xFBE58A30, 0xDED2FBAB,
	0x72B0A66E, 0xB6855DFE, 0xBA8A4FE8, 0x1EFC8CE0, 0x3F2FA457, 0x83F81D4A,
	0xA577E231, 0xA1FE3075, 0x88D9C0A0, 0xD5B80194, 0xAD9A95F9, 0x624816CD,
	0x50C1217B, 0x99E9E316, 0x0E423CFC, 0x51AA691E, 0x3826E52C, 0x1C217E6C,
	0x09703FEE, 0x51A8A931, 0x6A460E74, 0xBB709987, 0x9C86B022, 0x541FC68C,
	0x46FD8251, 0x59160CC0, 0x35C35F5C, 0x2846C0BA, 0x8B758282, 0x54504AC7,
	0xD2AF05E4, 0x29388839, 0xC01BD702, 0xCB2C0F1C, 0x7C932665, 0x555B2F74,
	0xA3AB8829, 0x86B63142, 0xF64B10EF, 0x0B8CC3BD, 0xEDD1CC5E, 0x687FEB69,
	0xC9509D43, 0xFDB23FCE, 0xD951AE64, 0x1E425A31, 0xF600C838, 0x36AD004C,
	0xCFF46AAA, 0xA40E329C, 0x7938DAD4, 0xA41D570D, 0xD43161C1, 0x62A69526,
	0x9ADB1E69, 0x3FDD4A8E, 0xDC6B80D6, 0x5B3B71F9, 0xC6272B04, 0xEC9D1810,
	0xCACEF403, 0x8CCF2DD5, 0xC95B9117, 0xE49F5235, 0xB854338A, 0x505DC82D,
	0x1562A846, 0x62292C31, 0x6AE77F5E, 0xD72B0374, 0x462D538C, 0xF9C9091B,
	0x47A67CBE, 0x0AE8DB58, 0x22611682, 0xB3A739C1, 0x2A281BF6, 0xEEAAC023,
	0x77CAF992, 0x94C6651E, 0x94B2BBC1, 0x763E4E4B, 0x0077D9B4, 0x587E38DA,
	0x183023C3, 0x7FB29F8C, 0xF9E3A26E, 0x0ABEC1FF, 0x350511E3, 0xA00EF092,
	0xDB6340D8, 0xB855322E, 0xA9A96910, 0xA52471F7, 0x4CFDB477, 0x388147FB,
	0x4E46041F, 0x9B1F5C3E, 0xFCCFEC71, 0xCDAD0657, 0x4C701C3A, 0xB38E8C33,
	0xB1C0FD4C, 0x917BDD64, 0x9B7624C8, 0x3BB45432, 0xCAF53EA6, 0x23BA4442,
	0x38532A3A, 0x4E677D2C, 0x45036C7A, 0x0BFD64B6, 0x5E0DD902, 0xC68A007E,
	0xF44182E1, 0x4DB5A851, 0x7F88A46B, 0x8EC9B55A, 0xCEC97DCF, 0x0A8291CD,
	0xF98D0ACC, 0x2A4ECEA9, 0x7140003C, 0x1A1DB93D, 0x33CB8B7A, 0x092999A3,
	0x71AD0038, 0x6DC778F9, 0x918130C4, 0xA907600A, 0x2D9E6832, 0xED6A1E01,
	0xEFB4318A, 0x7135C886, 0x7E31CC7A, 0x87F55BA5, 0x55034004, 0x7763CF1D,
	0xD69F6D18, 0xAC7D5F42, 0xE58857B6, 0x7930E9E4, 0x164DF4FB, 0x6E6F52C3,
	0x669E1EF1, 0x25E41D2B, 0x3FD59D7C, 0x3C1B20EE, 0xFA53DDEF, 0x0ABCD06B,
	0xD5C4484E, 0x1DBF9A42, 0x9B0DEADA, 0xABC52197, 0x22363A0D, 0xE86D2BC5,
	0x9C9DF69E, 0x5CAE82AB, 0x71F54BFF, 0x64F2E21E, 0xE2D74DD3, 0xF4FD4452,
	0xBC437944, 0xB4130C93, 0x85139270, 0xAEFE1309, 0xC186D91C, 0x598CB0FA,
	0x91F7F7EE, 0x7AD91D26, 0xD6E6C907, 0x61B46FC9, 0xF99C0238, 0xBC34F4DE,
	0x6519035B, 0xDE355B3B, 0x611FCFDC, 0x886B4238, 0xC1B2EFFA, 0xC6F34A26,
	0x7D1683B2, 0xC58EF183, 0x2EC22005, 0x3BB5FCBC, 0x4C6FAD73, 0xC3FE3B1B,
	0xEEF28183, 0x8E4F1232, 0xE98583FF, 0x9172FE9C, 0x28342F61, 0xC03404CD,
	0xCDF7E2EC, 0x9E02FCE1, 0xEE0A6D70, 0x0B07A7C8, 0x6372BB19, 0xAE56EDE7,
	0xDE394DF4, 0x1D4F42A3, 0x60D7F468, 0xB96ADAB7, 0xB2C8E3FB, 0xD108A94B,
	0xB324FB61, 0xBC0AB182, 0x483A797A, 0x30ACCA4F, 0x36ADE735, 0x1DF158A1,
	0xF3EFE872, 0xE2A689DA, 0xE0E68B77, 0x984F0C70, 0x7F57C935, 0xB557135E,
	0x3DED1AF3, 0x85636555, 0x5F066ED0, 0x2433F51F, 0xD5FD6561, 0xD3DF1ED5,
	0xAEC4617A, 0xF681B202, 0x630C75D8, 0x7D2FE363, 0x249B3EF9, 0xCC939DCE,
	0x146433FB, 0xA9E13641, 0xCE2D3695, 0xD8B9C583, 0x273D3CF1, 0xAFDC5620,
	0xA2BB4A9A, 0xADF85458, 0xFFFFFFFF, 0xFFFFFFFF
};

#else
#error "unsupported BN_BITS2"
#endif

/* Macro to make a BIGNUM from static data */

#define make_dh_bn(x) extern const BIGNUM _bignum_ ## x; \
	const BIGNUM _bignum_ ## x = { (BN_ULONG*)x, \
				       OSSL_NELEM(x), \
				       OSSL_NELEM(x), \
				       0, BN_FLG_STATIC_DATA };

static const BN_ULONG value_2 = 2;

const BIGNUM _bignum_const_2 =
{ (BN_ULONG*)&value_2, 1, 1, 0, BN_FLG_STATIC_DATA };

make_dh_bn(dh1024_160_p)
make_dh_bn(dh1024_160_g)
make_dh_bn(dh1024_160_q)
make_dh_bn(dh2048_224_p)
make_dh_bn(dh2048_224_g)
make_dh_bn(dh2048_224_q)
make_dh_bn(dh2048_256_p)
make_dh_bn(dh2048_256_g)
make_dh_bn(dh2048_256_q)

make_dh_bn(ffdhe2048_p)
make_dh_bn(ffdhe3072_p)
make_dh_bn(ffdhe4096_p)
make_dh_bn(ffdhe6144_p)
make_dh_bn(ffdhe8192_p)

#endif
