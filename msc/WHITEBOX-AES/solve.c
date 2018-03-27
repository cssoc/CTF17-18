/* crypto/aes/aes_core.c -*- mode:C; c-file-style: "eay" -*- */
/**
 * rijndael-alg-fst.c
 *
 * @version 3.0 (December 2000)
 *
 * Optimised ANSI C code for the Rijndael cipher (now AES)
 *
 * @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
 * @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
 * @author Paulo Barreto <paulo.barreto@terra.com.br>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Note: rewritten a little bit to provide error control and an OpenSSL-
   compatible API */

#ifndef AES_DEBUG
# ifndef NDEBUG
#  define NDEBUG
# endif
#endif
#include <assert.h>

#include <stdlib.h>
#include <openssl/aes.h>
#include "aes_locl.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
Te0[x] = S [x].[02, 01, 01, 03];
Te1[x] = S [x].[03, 02, 01, 01];
Te2[x] = S [x].[01, 03, 02, 01];
Te3[x] = S [x].[01, 01, 03, 02];

Td0[x] = Si[x].[0e, 09, 0d, 0b];
Td1[x] = Si[x].[0b, 0e, 09, 0d];
Td2[x] = Si[x].[0d, 0b, 0e, 09];
Td3[x] = Si[x].[09, 0d, 0b, 0e];
Td4[x] = Si[x].[01];
*/

static const u32 Td0[256] = {
	0x51f4a750U, 0x7e416553U, 0x1a17a4c3U, 0x3a275e96U,
	0x3bab6bcbU, 0x1f9d45f1U, 0xacfa58abU, 0x4be30393U,
	0x2030fa55U, 0xad766df6U, 0x88cc7691U, 0xf5024c25U,
	0x4fe5d7fcU, 0xc52acbd7U, 0x26354480U, 0xb562a38fU,
	0xdeb15a49U, 0x25ba1b67U, 0x45ea0e98U, 0x5dfec0e1U,
	0xc32f7502U, 0x814cf012U, 0x8d4697a3U, 0x6bd3f9c6U,
	0x038f5fe7U, 0x15929c95U, 0xbf6d7aebU, 0x955259daU,
	0xd4be832dU, 0x587421d3U, 0x49e06929U, 0x8ec9c844U,
	0x75c2896aU, 0xf48e7978U, 0x99583e6bU, 0x27b971ddU,
	0xbee14fb6U, 0xf088ad17U, 0xc920ac66U, 0x7dce3ab4U,
	0x63df4a18U, 0xe51a3182U, 0x97513360U, 0x62537f45U,
	0xb16477e0U, 0xbb6bae84U, 0xfe81a01cU, 0xf9082b94U,
	0x70486858U, 0x8f45fd19U, 0x94de6c87U, 0x527bf8b7U,
	0xab73d323U, 0x724b02e2U, 0xe31f8f57U, 0x6655ab2aU,
	0xb2eb2807U, 0x2fb5c203U, 0x86c57b9aU, 0xd33708a5U,
	0x302887f2U, 0x23bfa5b2U, 0x02036abaU, 0xed16825cU,
	0x8acf1c2bU, 0xa779b492U, 0xf307f2f0U, 0x4e69e2a1U,
	0x65daf4cdU, 0x0605bed5U, 0xd134621fU, 0xc4a6fe8aU,
	0x342e539dU, 0xa2f355a0U, 0x058ae132U, 0xa4f6eb75U,
	0x0b83ec39U, 0x4060efaaU, 0x5e719f06U, 0xbd6e1051U,
	0x3e218af9U, 0x96dd063dU, 0xdd3e05aeU, 0x4de6bd46U,
	0x91548db5U, 0x71c45d05U, 0x0406d46fU, 0x605015ffU,
	0x1998fb24U, 0xd6bde997U, 0x894043ccU, 0x67d99e77U,
	0xb0e842bdU, 0x07898b88U, 0xe7195b38U, 0x79c8eedbU,
	0xa17c0a47U, 0x7c420fe9U, 0xf8841ec9U, 0x00000000U,
	0x09808683U, 0x322bed48U, 0x1e1170acU, 0x6c5a724eU,
	0xfd0efffbU, 0x0f853856U, 0x3daed51eU, 0x362d3927U,
	0x0a0fd964U, 0x685ca621U, 0x9b5b54d1U, 0x24362e3aU,
	0x0c0a67b1U, 0x9357e70fU, 0xb4ee96d2U, 0x1b9b919eU,
	0x80c0c54fU, 0x61dc20a2U, 0x5a774b69U, 0x1c121a16U,
	0xe293ba0aU, 0xc0a02ae5U, 0x3c22e043U, 0x121b171dU,
	0x0e090d0bU, 0xf28bc7adU, 0x2db6a8b9U, 0x141ea9c8U,
	0x57f11985U, 0xaf75074cU, 0xee99ddbbU, 0xa37f60fdU,
	0xf701269fU, 0x5c72f5bcU, 0x44663bc5U, 0x5bfb7e34U,
	0x8b432976U, 0xcb23c6dcU, 0xb6edfc68U, 0xb8e4f163U,
	0xd731dccaU, 0x42638510U, 0x13972240U, 0x84c61120U,
	0x854a247dU, 0xd2bb3df8U, 0xaef93211U, 0xc729a16dU,
	0x1d9e2f4bU, 0xdcb230f3U, 0x0d8652ecU, 0x77c1e3d0U,
	0x2bb3166cU, 0xa970b999U, 0x119448faU, 0x47e96422U,
	0xa8fc8cc4U, 0xa0f03f1aU, 0x567d2cd8U, 0x223390efU,
	0x87494ec7U, 0xd938d1c1U, 0x8ccaa2feU, 0x98d40b36U,
	0xa6f581cfU, 0xa57ade28U, 0xdab78e26U, 0x3fadbfa4U,
	0x2c3a9de4U, 0x5078920dU, 0x6a5fcc9bU, 0x547e4662U,
	0xf68d13c2U, 0x90d8b8e8U, 0x2e39f75eU, 0x82c3aff5U,
	0x9f5d80beU, 0x69d0937cU, 0x6fd52da9U, 0xcf2512b3U,
	0xc8ac993bU, 0x10187da7U, 0xe89c636eU, 0xdb3bbb7bU,
	0xcd267809U, 0x6e5918f4U, 0xec9ab701U, 0x834f9aa8U,
	0xe6956e65U, 0xaaffe67eU, 0x21bccf08U, 0xef15e8e6U,
	0xbae79bd9U, 0x4a6f36ceU, 0xea9f09d4U, 0x29b07cd6U,
	0x31a4b2afU, 0x2a3f2331U, 0xc6a59430U, 0x35a266c0U,
	0x744ebc37U, 0xfc82caa6U, 0xe090d0b0U, 0x33a7d815U,
	0xf104984aU, 0x41ecdaf7U, 0x7fcd500eU, 0x1791f62fU,
	0x764dd68dU, 0x43efb04dU, 0xccaa4d54U, 0xe49604dfU,
	0x9ed1b5e3U, 0x4c6a881bU, 0xc12c1fb8U, 0x4665517fU,
	0x9d5eea04U, 0x018c355dU, 0xfa877473U, 0xfb0b412eU,
	0xb3671d5aU, 0x92dbd252U, 0xe9105633U, 0x6dd64713U,
	0x9ad7618cU, 0x37a10c7aU, 0x59f8148eU, 0xeb133c89U,
	0xcea927eeU, 0xb761c935U, 0xe11ce5edU, 0x7a47b13cU,
	0x9cd2df59U, 0x55f2733fU, 0x1814ce79U, 0x73c737bfU,
	0x53f7cdeaU, 0x5ffdaa5bU, 0xdf3d6f14U, 0x7844db86U,
	0xcaaff381U, 0xb968c43eU, 0x3824342cU, 0xc2a3405fU,
	0x161dc372U, 0xbce2250cU, 0x283c498bU, 0xff0d9541U,
	0x39a80171U, 0x080cb3deU, 0xd8b4e49cU, 0x6456c190U,
	0x7bcb8461U, 0xd532b670U, 0x486c5c74U, 0xd0b85742U,
};
static const u32 Td1[256] = {
	0x5051f4a7U, 0x537e4165U, 0xc31a17a4U, 0x963a275eU,
	0xcb3bab6bU, 0xf11f9d45U, 0xabacfa58U, 0x934be303U,
	0x552030faU, 0xf6ad766dU, 0x9188cc76U, 0x25f5024cU,
	0xfc4fe5d7U, 0xd7c52acbU, 0x80263544U, 0x8fb562a3U,
	0x49deb15aU, 0x6725ba1bU, 0x9845ea0eU, 0xe15dfec0U,
	0x02c32f75U, 0x12814cf0U, 0xa38d4697U, 0xc66bd3f9U,
	0xe7038f5fU, 0x9515929cU, 0xebbf6d7aU, 0xda955259U,
	0x2dd4be83U, 0xd3587421U, 0x2949e069U, 0x448ec9c8U,
	0x6a75c289U, 0x78f48e79U, 0x6b99583eU, 0xdd27b971U,
	0xb6bee14fU, 0x17f088adU, 0x66c920acU, 0xb47dce3aU,
	0x1863df4aU, 0x82e51a31U, 0x60975133U, 0x4562537fU,
	0xe0b16477U, 0x84bb6baeU, 0x1cfe81a0U, 0x94f9082bU,
	0x58704868U, 0x198f45fdU, 0x8794de6cU, 0xb7527bf8U,
	0x23ab73d3U, 0xe2724b02U, 0x57e31f8fU, 0x2a6655abU,
	0x07b2eb28U, 0x032fb5c2U, 0x9a86c57bU, 0xa5d33708U,
	0xf2302887U, 0xb223bfa5U, 0xba02036aU, 0x5ced1682U,
	0x2b8acf1cU, 0x92a779b4U, 0xf0f307f2U, 0xa14e69e2U,
	0xcd65daf4U, 0xd50605beU, 0x1fd13462U, 0x8ac4a6feU,
	0x9d342e53U, 0xa0a2f355U, 0x32058ae1U, 0x75a4f6ebU,
	0x390b83ecU, 0xaa4060efU, 0x065e719fU, 0x51bd6e10U,
	0xf93e218aU, 0x3d96dd06U, 0xaedd3e05U, 0x464de6bdU,
	0xb591548dU, 0x0571c45dU, 0x6f0406d4U, 0xff605015U,
	0x241998fbU, 0x97d6bde9U, 0xcc894043U, 0x7767d99eU,
	0xbdb0e842U, 0x8807898bU, 0x38e7195bU, 0xdb79c8eeU,
	0x47a17c0aU, 0xe97c420fU, 0xc9f8841eU, 0x00000000U,
	0x83098086U, 0x48322bedU, 0xac1e1170U, 0x4e6c5a72U,
	0xfbfd0effU, 0x560f8538U, 0x1e3daed5U, 0x27362d39U,
	0x640a0fd9U, 0x21685ca6U, 0xd19b5b54U, 0x3a24362eU,
	0xb10c0a67U, 0x0f9357e7U, 0xd2b4ee96U, 0x9e1b9b91U,
	0x4f80c0c5U, 0xa261dc20U, 0x695a774bU, 0x161c121aU,
	0x0ae293baU, 0xe5c0a02aU, 0x433c22e0U, 0x1d121b17U,
	0x0b0e090dU, 0xadf28bc7U, 0xb92db6a8U, 0xc8141ea9U,
	0x8557f119U, 0x4caf7507U, 0xbbee99ddU, 0xfda37f60U,
	0x9ff70126U, 0xbc5c72f5U, 0xc544663bU, 0x345bfb7eU,
	0x768b4329U, 0xdccb23c6U, 0x68b6edfcU, 0x63b8e4f1U,
	0xcad731dcU, 0x10426385U, 0x40139722U, 0x2084c611U,
	0x7d854a24U, 0xf8d2bb3dU, 0x11aef932U, 0x6dc729a1U,
	0x4b1d9e2fU, 0xf3dcb230U, 0xec0d8652U, 0xd077c1e3U,
	0x6c2bb316U, 0x99a970b9U, 0xfa119448U, 0x2247e964U,
	0xc4a8fc8cU, 0x1aa0f03fU, 0xd8567d2cU, 0xef223390U,
	0xc787494eU, 0xc1d938d1U, 0xfe8ccaa2U, 0x3698d40bU,
	0xcfa6f581U, 0x28a57adeU, 0x26dab78eU, 0xa43fadbfU,
	0xe42c3a9dU, 0x0d507892U, 0x9b6a5fccU, 0x62547e46U,
	0xc2f68d13U, 0xe890d8b8U, 0x5e2e39f7U, 0xf582c3afU,
	0xbe9f5d80U, 0x7c69d093U, 0xa96fd52dU, 0xb3cf2512U,
	0x3bc8ac99U, 0xa710187dU, 0x6ee89c63U, 0x7bdb3bbbU,
	0x09cd2678U, 0xf46e5918U, 0x01ec9ab7U, 0xa8834f9aU,
	0x65e6956eU, 0x7eaaffe6U, 0x0821bccfU, 0xe6ef15e8U,
	0xd9bae79bU, 0xce4a6f36U, 0xd4ea9f09U, 0xd629b07cU,
	0xaf31a4b2U, 0x312a3f23U, 0x30c6a594U, 0xc035a266U,
	0x37744ebcU, 0xa6fc82caU, 0xb0e090d0U, 0x1533a7d8U,
	0x4af10498U, 0xf741ecdaU, 0x0e7fcd50U, 0x2f1791f6U,
	0x8d764dd6U, 0x4d43efb0U, 0x54ccaa4dU, 0xdfe49604U,
	0xe39ed1b5U, 0x1b4c6a88U, 0xb8c12c1fU, 0x7f466551U,
	0x049d5eeaU, 0x5d018c35U, 0x73fa8774U, 0x2efb0b41U,
	0x5ab3671dU, 0x5292dbd2U, 0x33e91056U, 0x136dd647U,
	0x8c9ad761U, 0x7a37a10cU, 0x8e59f814U, 0x89eb133cU,
	0xeecea927U, 0x35b761c9U, 0xede11ce5U, 0x3c7a47b1U,
	0x599cd2dfU, 0x3f55f273U, 0x791814ceU, 0xbf73c737U,
	0xea53f7cdU, 0x5b5ffdaaU, 0x14df3d6fU, 0x867844dbU,
	0x81caaff3U, 0x3eb968c4U, 0x2c382434U, 0x5fc2a340U,
	0x72161dc3U, 0x0cbce225U, 0x8b283c49U, 0x41ff0d95U,
	0x7139a801U, 0xde080cb3U, 0x9cd8b4e4U, 0x906456c1U,
	0x617bcb84U, 0x70d532b6U, 0x74486c5cU, 0x42d0b857U,
};
static const u32 Td2[256] = {
	0xa75051f4U, 0x65537e41U, 0xa4c31a17U, 0x5e963a27U,
	0x6bcb3babU, 0x45f11f9dU, 0x58abacfaU, 0x03934be3U,
	0xfa552030U, 0x6df6ad76U, 0x769188ccU, 0x4c25f502U,
	0xd7fc4fe5U, 0xcbd7c52aU, 0x44802635U, 0xa38fb562U,
	0x5a49deb1U, 0x1b6725baU, 0x0e9845eaU, 0xc0e15dfeU,
	0x7502c32fU, 0xf012814cU, 0x97a38d46U, 0xf9c66bd3U,
	0x5fe7038fU, 0x9c951592U, 0x7aebbf6dU, 0x59da9552U,
	0x832dd4beU, 0x21d35874U, 0x692949e0U, 0xc8448ec9U,
	0x896a75c2U, 0x7978f48eU, 0x3e6b9958U, 0x71dd27b9U,
	0x4fb6bee1U, 0xad17f088U, 0xac66c920U, 0x3ab47dceU,
	0x4a1863dfU, 0x3182e51aU, 0x33609751U, 0x7f456253U,
	0x77e0b164U, 0xae84bb6bU, 0xa01cfe81U, 0x2b94f908U,
	0x68587048U, 0xfd198f45U, 0x6c8794deU, 0xf8b7527bU,
	0xd323ab73U, 0x02e2724bU, 0x8f57e31fU, 0xab2a6655U,
	0x2807b2ebU, 0xc2032fb5U, 0x7b9a86c5U, 0x08a5d337U,
	0x87f23028U, 0xa5b223bfU, 0x6aba0203U, 0x825ced16U,
	0x1c2b8acfU, 0xb492a779U, 0xf2f0f307U, 0xe2a14e69U,
	0xf4cd65daU, 0xbed50605U, 0x621fd134U, 0xfe8ac4a6U,
	0x539d342eU, 0x55a0a2f3U, 0xe132058aU, 0xeb75a4f6U,
	0xec390b83U, 0xefaa4060U, 0x9f065e71U, 0x1051bd6eU,
	0x8af93e21U, 0x063d96ddU, 0x05aedd3eU, 0xbd464de6U,
	0x8db59154U, 0x5d0571c4U, 0xd46f0406U, 0x15ff6050U,
	0xfb241998U, 0xe997d6bdU, 0x43cc8940U, 0x9e7767d9U,
	0x42bdb0e8U, 0x8b880789U, 0x5b38e719U, 0xeedb79c8U,
	0x0a47a17cU, 0x0fe97c42U, 0x1ec9f884U, 0x00000000U,
	0x86830980U, 0xed48322bU, 0x70ac1e11U, 0x724e6c5aU,
	0xfffbfd0eU, 0x38560f85U, 0xd51e3daeU, 0x3927362dU,
	0xd9640a0fU, 0xa621685cU, 0x54d19b5bU, 0x2e3a2436U,
	0x67b10c0aU, 0xe70f9357U, 0x96d2b4eeU, 0x919e1b9bU,
	0xc54f80c0U, 0x20a261dcU, 0x4b695a77U, 0x1a161c12U,
	0xba0ae293U, 0x2ae5c0a0U, 0xe0433c22U, 0x171d121bU,
	0x0d0b0e09U, 0xc7adf28bU, 0xa8b92db6U, 0xa9c8141eU,
	0x198557f1U, 0x074caf75U, 0xddbbee99U, 0x60fda37fU,
	0x269ff701U, 0xf5bc5c72U, 0x3bc54466U, 0x7e345bfbU,
	0x29768b43U, 0xc6dccb23U, 0xfc68b6edU, 0xf163b8e4U,
	0xdccad731U, 0x85104263U, 0x22401397U, 0x112084c6U,
	0x247d854aU, 0x3df8d2bbU, 0x3211aef9U, 0xa16dc729U,
	0x2f4b1d9eU, 0x30f3dcb2U, 0x52ec0d86U, 0xe3d077c1U,
	0x166c2bb3U, 0xb999a970U, 0x48fa1194U, 0x642247e9U,
	0x8cc4a8fcU, 0x3f1aa0f0U, 0x2cd8567dU, 0x90ef2233U,
	0x4ec78749U, 0xd1c1d938U, 0xa2fe8ccaU, 0x0b3698d4U,
	0x81cfa6f5U, 0xde28a57aU, 0x8e26dab7U, 0xbfa43fadU,
	0x9de42c3aU, 0x920d5078U, 0xcc9b6a5fU, 0x4662547eU,
	0x13c2f68dU, 0xb8e890d8U, 0xf75e2e39U, 0xaff582c3U,
	0x80be9f5dU, 0x937c69d0U, 0x2da96fd5U, 0x12b3cf25U,
	0x993bc8acU, 0x7da71018U, 0x636ee89cU, 0xbb7bdb3bU,
	0x7809cd26U, 0x18f46e59U, 0xb701ec9aU, 0x9aa8834fU,
	0x6e65e695U, 0xe67eaaffU, 0xcf0821bcU, 0xe8e6ef15U,
	0x9bd9bae7U, 0x36ce4a6fU, 0x09d4ea9fU, 0x7cd629b0U,
	0xb2af31a4U, 0x23312a3fU, 0x9430c6a5U, 0x66c035a2U,
	0xbc37744eU, 0xcaa6fc82U, 0xd0b0e090U, 0xd81533a7U,
	0x984af104U, 0xdaf741ecU, 0x500e7fcdU, 0xf62f1791U,
	0xd68d764dU, 0xb04d43efU, 0x4d54ccaaU, 0x04dfe496U,
	0xb5e39ed1U, 0x881b4c6aU, 0x1fb8c12cU, 0x517f4665U,
	0xea049d5eU, 0x355d018cU, 0x7473fa87U, 0x412efb0bU,
	0x1d5ab367U, 0xd25292dbU, 0x5633e910U, 0x47136dd6U,
	0x618c9ad7U, 0x0c7a37a1U, 0x148e59f8U, 0x3c89eb13U,
	0x27eecea9U, 0xc935b761U, 0xe5ede11cU, 0xb13c7a47U,
	0xdf599cd2U, 0x733f55f2U, 0xce791814U, 0x37bf73c7U,
	0xcdea53f7U, 0xaa5b5ffdU, 0x6f14df3dU, 0xdb867844U,
	0xf381caafU, 0xc43eb968U, 0x342c3824U, 0x405fc2a3U,
	0xc372161dU, 0x250cbce2U, 0x498b283cU, 0x9541ff0dU,
	0x017139a8U, 0xb3de080cU, 0xe49cd8b4U, 0xc1906456U,
	0x84617bcbU, 0xb670d532U, 0x5c74486cU, 0x5742d0b8U,
};
static const u32 Td3[256] = {
	0xf4a75051U, 0x4165537eU, 0x17a4c31aU, 0x275e963aU,
	0xab6bcb3bU, 0x9d45f11fU, 0xfa58abacU, 0xe303934bU,
	0x30fa5520U, 0x766df6adU, 0xcc769188U, 0x024c25f5U,
	0xe5d7fc4fU, 0x2acbd7c5U, 0x35448026U, 0x62a38fb5U,
	0xb15a49deU, 0xba1b6725U, 0xea0e9845U, 0xfec0e15dU,
	0x2f7502c3U, 0x4cf01281U, 0x4697a38dU, 0xd3f9c66bU,
	0x8f5fe703U, 0x929c9515U, 0x6d7aebbfU, 0x5259da95U,
	0xbe832dd4U, 0x7421d358U, 0xe0692949U, 0xc9c8448eU,
	0xc2896a75U, 0x8e7978f4U, 0x583e6b99U, 0xb971dd27U,
	0xe14fb6beU, 0x88ad17f0U, 0x20ac66c9U, 0xce3ab47dU,
	0xdf4a1863U, 0x1a3182e5U, 0x51336097U, 0x537f4562U,
	0x6477e0b1U, 0x6bae84bbU, 0x81a01cfeU, 0x082b94f9U,
	0x48685870U, 0x45fd198fU, 0xde6c8794U, 0x7bf8b752U,
	0x73d323abU, 0x4b02e272U, 0x1f8f57e3U, 0x55ab2a66U,
	0xeb2807b2U, 0xb5c2032fU, 0xc57b9a86U, 0x3708a5d3U,
	0x2887f230U, 0xbfa5b223U, 0x036aba02U, 0x16825cedU,
	0xcf1c2b8aU, 0x79b492a7U, 0x07f2f0f3U, 0x69e2a14eU,
	0xdaf4cd65U, 0x05bed506U, 0x34621fd1U, 0xa6fe8ac4U,
	0x2e539d34U, 0xf355a0a2U, 0x8ae13205U, 0xf6eb75a4U,
	0x83ec390bU, 0x60efaa40U, 0x719f065eU, 0x6e1051bdU,
	0x218af93eU, 0xdd063d96U, 0x3e05aeddU, 0xe6bd464dU,
	0x548db591U, 0xc45d0571U, 0x06d46f04U, 0x5015ff60U,
	0x98fb2419U, 0xbde997d6U, 0x4043cc89U, 0xd99e7767U,
	0xe842bdb0U, 0x898b8807U, 0x195b38e7U, 0xc8eedb79U,
	0x7c0a47a1U, 0x420fe97cU, 0x841ec9f8U, 0x00000000U,
	0x80868309U, 0x2bed4832U, 0x1170ac1eU, 0x5a724e6cU,
	0x0efffbfdU, 0x8538560fU, 0xaed51e3dU, 0x2d392736U,
	0x0fd9640aU, 0x5ca62168U, 0x5b54d19bU, 0x362e3a24U,
	0x0a67b10cU, 0x57e70f93U, 0xee96d2b4U, 0x9b919e1bU,
	0xc0c54f80U, 0xdc20a261U, 0x774b695aU, 0x121a161cU,
	0x93ba0ae2U, 0xa02ae5c0U, 0x22e0433cU, 0x1b171d12U,
	0x090d0b0eU, 0x8bc7adf2U, 0xb6a8b92dU, 0x1ea9c814U,
	0xf1198557U, 0x75074cafU, 0x99ddbbeeU, 0x7f60fda3U,
	0x01269ff7U, 0x72f5bc5cU, 0x663bc544U, 0xfb7e345bU,
	0x4329768bU, 0x23c6dccbU, 0xedfc68b6U, 0xe4f163b8U,
	0x31dccad7U, 0x63851042U, 0x97224013U, 0xc6112084U,
	0x4a247d85U, 0xbb3df8d2U, 0xf93211aeU, 0x29a16dc7U,
	0x9e2f4b1dU, 0xb230f3dcU, 0x8652ec0dU, 0xc1e3d077U,
	0xb3166c2bU, 0x70b999a9U, 0x9448fa11U, 0xe9642247U,
	0xfc8cc4a8U, 0xf03f1aa0U, 0x7d2cd856U, 0x3390ef22U,
	0x494ec787U, 0x38d1c1d9U, 0xcaa2fe8cU, 0xd40b3698U,
	0xf581cfa6U, 0x7ade28a5U, 0xb78e26daU, 0xadbfa43fU,
	0x3a9de42cU, 0x78920d50U, 0x5fcc9b6aU, 0x7e466254U,
	0x8d13c2f6U, 0xd8b8e890U, 0x39f75e2eU, 0xc3aff582U,
	0x5d80be9fU, 0xd0937c69U, 0xd52da96fU, 0x2512b3cfU,
	0xac993bc8U, 0x187da710U, 0x9c636ee8U, 0x3bbb7bdbU,
	0x267809cdU, 0x5918f46eU, 0x9ab701ecU, 0x4f9aa883U,
	0x956e65e6U, 0xffe67eaaU, 0xbccf0821U, 0x15e8e6efU,
	0xe79bd9baU, 0x6f36ce4aU, 0x9f09d4eaU, 0xb07cd629U,
	0xa4b2af31U, 0x3f23312aU, 0xa59430c6U, 0xa266c035U,
	0x4ebc3774U, 0x82caa6fcU, 0x90d0b0e0U, 0xa7d81533U,
	0x04984af1U, 0xecdaf741U, 0xcd500e7fU, 0x91f62f17U,
	0x4dd68d76U, 0xefb04d43U, 0xaa4d54ccU, 0x9604dfe4U,
	0xd1b5e39eU, 0x6a881b4cU, 0x2c1fb8c1U, 0x65517f46U,
	0x5eea049dU, 0x8c355d01U, 0x877473faU, 0x0b412efbU,
	0x671d5ab3U, 0xdbd25292U, 0x105633e9U, 0xd647136dU,
	0xd7618c9aU, 0xa10c7a37U, 0xf8148e59U, 0x133c89ebU,
	0xa927eeceU, 0x61c935b7U, 0x1ce5ede1U, 0x47b13c7aU,
	0xd2df599cU, 0xf2733f55U, 0x14ce7918U, 0xc737bf73U,
	0xf7cdea53U, 0xfdaa5b5fU, 0x3d6f14dfU, 0x44db8678U,
	0xaff381caU, 0x68c43eb9U, 0x24342c38U, 0xa3405fc2U,
	0x1dc37216U, 0xe2250cbcU, 0x3c498b28U, 0x0d9541ffU,
	0xa8017139U, 0x0cb3de08U, 0xb4e49cd8U, 0x56c19064U,
	0xcb84617bU, 0x32b670d5U, 0x6c5c7448U, 0xb85742d0U,
};
static const u8 Td4[256] = {
	0x52U, 0x09U, 0x6aU, 0xd5U, 0x30U, 0x36U, 0xa5U, 0x38U,
	0xbfU, 0x40U, 0xa3U, 0x9eU, 0x81U, 0xf3U, 0xd7U, 0xfbU,
	0x7cU, 0xe3U, 0x39U, 0x82U, 0x9bU, 0x2fU, 0xffU, 0x87U,
	0x34U, 0x8eU, 0x43U, 0x44U, 0xc4U, 0xdeU, 0xe9U, 0xcbU,
	0x54U, 0x7bU, 0x94U, 0x32U, 0xa6U, 0xc2U, 0x23U, 0x3dU,
	0xeeU, 0x4cU, 0x95U, 0x0bU, 0x42U, 0xfaU, 0xc3U, 0x4eU,
	0x08U, 0x2eU, 0xa1U, 0x66U, 0x28U, 0xd9U, 0x24U, 0xb2U,
	0x76U, 0x5bU, 0xa2U, 0x49U, 0x6dU, 0x8bU, 0xd1U, 0x25U,
	0x72U, 0xf8U, 0xf6U, 0x64U, 0x86U, 0x68U, 0x98U, 0x16U,
	0xd4U, 0xa4U, 0x5cU, 0xccU, 0x5dU, 0x65U, 0xb6U, 0x92U,
	0x6cU, 0x70U, 0x48U, 0x50U, 0xfdU, 0xedU, 0xb9U, 0xdaU,
	0x5eU, 0x15U, 0x46U, 0x57U, 0xa7U, 0x8dU, 0x9dU, 0x84U,
	0x90U, 0xd8U, 0xabU, 0x00U, 0x8cU, 0xbcU, 0xd3U, 0x0aU,
	0xf7U, 0xe4U, 0x58U, 0x05U, 0xb8U, 0xb3U, 0x45U, 0x06U,
	0xd0U, 0x2cU, 0x1eU, 0x8fU, 0xcaU, 0x3fU, 0x0fU, 0x02U,
	0xc1U, 0xafU, 0xbdU, 0x03U, 0x01U, 0x13U, 0x8aU, 0x6bU,
	0x3aU, 0x91U, 0x11U, 0x41U, 0x4fU, 0x67U, 0xdcU, 0xeaU,
	0x97U, 0xf2U, 0xcfU, 0xceU, 0xf0U, 0xb4U, 0xe6U, 0x73U,
	0x96U, 0xacU, 0x74U, 0x22U, 0xe7U, 0xadU, 0x35U, 0x85U,
	0xe2U, 0xf9U, 0x37U, 0xe8U, 0x1cU, 0x75U, 0xdfU, 0x6eU,
	0x47U, 0xf1U, 0x1aU, 0x71U, 0x1dU, 0x29U, 0xc5U, 0x89U,
	0x6fU, 0xb7U, 0x62U, 0x0eU, 0xaaU, 0x18U, 0xbeU, 0x1bU,
	0xfcU, 0x56U, 0x3eU, 0x4bU, 0xc6U, 0xd2U, 0x79U, 0x20U,
	0x9aU, 0xdbU, 0xc0U, 0xfeU, 0x78U, 0xcdU, 0x5aU, 0xf4U,
	0x1fU, 0xddU, 0xa8U, 0x33U, 0x88U, 0x07U, 0xc7U, 0x31U,
	0xb1U, 0x12U, 0x10U, 0x59U, 0x27U, 0x80U, 0xecU, 0x5fU,
	0x60U, 0x51U, 0x7fU, 0xa9U, 0x19U, 0xb5U, 0x4aU, 0x0dU,
	0x2dU, 0xe5U, 0x7aU, 0x9fU, 0x93U, 0xc9U, 0x9cU, 0xefU,
	0xa0U, 0xe0U, 0x3bU, 0x4dU, 0xaeU, 0x2aU, 0xf5U, 0xb0U,
	0xc8U, 0xebU, 0xbbU, 0x3cU, 0x83U, 0x53U, 0x99U, 0x61U,
	0x17U, 0x2bU, 0x04U, 0x7eU, 0xbaU, 0x77U, 0xd6U, 0x26U,
	0xe1U, 0x69U, 0x14U, 0x63U, 0x55U, 0x21U, 0x0cU, 0x7dU,
};

static const u32 rcon[] = {
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x1B000000, 0x36000000, /* for 128-bit blocks, Rijndael never uses more than 10 rcon values */
};




/*
 * Decrypt a single block
 * in and out can overlap
 */
void
AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key)
{
	const u32 *rk;
	u32 s0, s1, s2, s3, t0, t1, t2, t3;
#ifndef FULL_UNROLL
	int r;
#endif /* ?FULL_UNROLL */

	assert(in && out && key);
	rk = key->rd_key;

	/*
	 * map byte array block to cipher state
	 * and add initial round key:
	 */
	s0 = GETU32(in) ^ rk[0];
	s1 = GETU32(in + 4) ^ rk[1];
	s2 = GETU32(in + 8) ^ rk[2];
	s3 = GETU32(in + 12) ^ rk[3];
#ifdef FULL_UNROLL
	/* round 1: */
	t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[ 4];
	t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[ 5];
	t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[ 6];
	t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[ 7];
	/* round 2: */
	s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >>  8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[ 8];
	s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >>  8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[ 9];
	s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >>  8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[10];
	s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >>  8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[11];
	/* round 3: */
	t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[12];
	t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[13];
	t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[14];
	t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[15];
	/* round 4: */
	s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >>  8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[16];
	s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >>  8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[17];
	s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >>  8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[18];
	s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >>  8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[19];
	/* round 5: */
	t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[20];
	t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[21];
	t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[22];
	t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[23];
	/* round 6: */
	s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >>  8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[24];
	s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >>  8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[25];
	s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >>  8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[26];
	s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >>  8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[27];
	/* round 7: */
	t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[28];
	t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[29];
	t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[30];
	t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[31];
	/* round 8: */
	s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >>  8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[32];
	s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >>  8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[33];
	s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >>  8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[34];
	s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >>  8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[35];
	/* round 9: */
	t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[36];
	t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[37];
	t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[38];
	t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[39];
	if (key->rounds > 10) {
		/* round 10: */
		s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >>  8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[40];
		s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >>  8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[41];
		s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >>  8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[42];
		s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >>  8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[43];
		/* round 11: */
		t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[44];
		t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[45];
		t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[46];
		t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[47];
		if (key->rounds > 12) {
			/* round 12: */
			s0 = Td0[t0 >> 24] ^ Td1[(t3 >> 16) & 0xff] ^ Td2[(t2 >>  8) & 0xff] ^ Td3[t1 & 0xff] ^ rk[48];
			s1 = Td0[t1 >> 24] ^ Td1[(t0 >> 16) & 0xff] ^ Td2[(t3 >>  8) & 0xff] ^ Td3[t2 & 0xff] ^ rk[49];
			s2 = Td0[t2 >> 24] ^ Td1[(t1 >> 16) & 0xff] ^ Td2[(t0 >>  8) & 0xff] ^ Td3[t3 & 0xff] ^ rk[50];
			s3 = Td0[t3 >> 24] ^ Td1[(t2 >> 16) & 0xff] ^ Td2[(t1 >>  8) & 0xff] ^ Td3[t0 & 0xff] ^ rk[51];
			/* round 13: */
			t0 = Td0[s0 >> 24] ^ Td1[(s3 >> 16) & 0xff] ^ Td2[(s2 >>  8) & 0xff] ^ Td3[s1 & 0xff] ^ rk[52];
			t1 = Td0[s1 >> 24] ^ Td1[(s0 >> 16) & 0xff] ^ Td2[(s3 >>  8) & 0xff] ^ Td3[s2 & 0xff] ^ rk[53];
			t2 = Td0[s2 >> 24] ^ Td1[(s1 >> 16) & 0xff] ^ Td2[(s0 >>  8) & 0xff] ^ Td3[s3 & 0xff] ^ rk[54];
			t3 = Td0[s3 >> 24] ^ Td1[(s2 >> 16) & 0xff] ^ Td2[(s1 >>  8) & 0xff] ^ Td3[s0 & 0xff] ^ rk[55];
		}
	}
	rk += key->rounds << 2;
#else  /* !FULL_UNROLL */
    /*
     * Nr - 1 full rounds:
     */
	r = key->rounds >> 1;
	for (;;) {
		t0 =
		    Td0[(s0 >> 24)] ^
		    Td1[(s3 >> 16) & 0xff] ^
		    Td2[(s2 >> 8) & 0xff] ^
		    Td3[(s1) & 0xff] ^
		    rk[4];
		t1 =
		    Td0[(s1 >> 24)] ^
		    Td1[(s0 >> 16) & 0xff] ^
		    Td2[(s3 >> 8) & 0xff] ^
		    Td3[(s2) & 0xff] ^
		    rk[5];
		t2 =
		    Td0[(s2 >> 24)] ^
		    Td1[(s1 >> 16) & 0xff] ^
		    Td2[(s0 >> 8) & 0xff] ^
		    Td3[(s3) & 0xff] ^
		    rk[6];
		t3 =
		    Td0[(s3 >> 24)] ^
		    Td1[(s2 >> 16) & 0xff] ^
		    Td2[(s1 >> 8) & 0xff] ^
		    Td3[(s0) & 0xff] ^
		    rk[7];

		rk += 8;
		if (--r == 0) {
			break;
		}

		s0 =
		    Td0[(t0 >> 24)] ^
		    Td1[(t3 >> 16) & 0xff] ^
		    Td2[(t2 >> 8) & 0xff] ^
		    Td3[(t1) & 0xff] ^
		    rk[0];
		s1 =
		    Td0[(t1 >> 24)] ^
		    Td1[(t0 >> 16) & 0xff] ^
		    Td2[(t3 >> 8) & 0xff] ^
		    Td3[(t2) & 0xff] ^
		    rk[1];
		s2 =
		    Td0[(t2 >> 24)] ^
		    Td1[(t1 >> 16) & 0xff] ^
		    Td2[(t0 >> 8) & 0xff] ^
		    Td3[(t3) & 0xff] ^
		    rk[2];
		s3 =
		    Td0[(t3 >> 24)] ^
		    Td1[(t2 >> 16) & 0xff] ^
		    Td2[(t1 >> 8) & 0xff] ^
		    Td3[(t0) & 0xff] ^
		    rk[3];
	}
#endif /* ?FULL_UNROLL */
    /*
	 * apply last round and
	 * map cipher state to byte array block:
	 */
	s0 =
	    (Td4[(t0 >> 24)] << 24) ^
	    (Td4[(t3 >> 16) & 0xff] << 16) ^
	    (Td4[(t2 >> 8) & 0xff] <<  8) ^
	    (Td4[(t1) & 0xff]) ^
	    rk[0];
	PUTU32(out, s0);
	s1 =
	    (Td4[(t1 >> 24)] << 24) ^
	    (Td4[(t0 >> 16) & 0xff] << 16) ^
	    (Td4[(t3 >> 8) & 0xff] <<  8) ^
	    (Td4[(t2) & 0xff]) ^
	    rk[1];
	PUTU32(out + 4, s1);
	s2 =
	    (Td4[(t2 >> 24)] << 24) ^
	    (Td4[(t1 >> 16) & 0xff] << 16) ^
	    (Td4[(t0 >> 8) & 0xff] <<  8) ^
	    (Td4[(t3) & 0xff]) ^
	    rk[2];
	PUTU32(out + 8, s2);
	s3 =
	    (Td4[(t3 >> 24)] << 24) ^
	    (Td4[(t2 >> 16) & 0xff] << 16) ^
	    (Td4[(t1 >> 8) & 0xff] <<  8) ^
	    (Td4[(t0) & 0xff]) ^
	    rk[3];
	PUTU32(out + 12, s3);
}


/**
 * Expand the cipher key into the decryption key schedule.
 */
int
private_AES_custom_set_decrypt_key(const int bits,
    AES_KEY *key)
{
	u32 *rk;
	int i, j, status;
	u32 temp;


	rk = key->rd_key;

	/* invert the order of the round keys: */
	for (i = 0, j = 4*(key->rounds); i < j; i += 4, j -= 4) {
		temp = rk[i];
		rk[i] = rk[j];
		rk[j] = temp;
		temp = rk[i + 1];
		rk[i + 1] = rk[j + 1];
		rk[j + 1] = temp;
		temp = rk[i + 2];
		rk[i + 2] = rk[j + 2];
		rk[j + 2] = temp;
		temp = rk[i + 3];
		rk[i + 3] = rk[j + 3];
		rk[j + 3] = temp;
	}
	/* apply the inverse MixColumn transform to all round keys but the first and the last: */
	for (i = 1; i < (key->rounds); i++) {
		rk += 4;
		for (j = 0; j < 4; j++) {
			u32 tp1, tp2, tp4, tp8, tp9, tpb, tpd, tpe, m;

			tp1 = rk[j];
			m = tp1 & 0x80808080;
			tp2 = ((tp1 & 0x7f7f7f7f) << 1) ^
			    ((m - (m >> 7)) & 0x1b1b1b1b);
			m = tp2 & 0x80808080;
			tp4 = ((tp2 & 0x7f7f7f7f) << 1) ^
			    ((m - (m >> 7)) & 0x1b1b1b1b);
			m = tp4 & 0x80808080;
			tp8 = ((tp4 & 0x7f7f7f7f) << 1) ^
			    ((m - (m >> 7)) & 0x1b1b1b1b);
			tp9 = tp8 ^ tp1;
			tpb = tp9 ^ tp2;
			tpd = tp9 ^ tp4;
			tpe = tp8 ^ tp4 ^ tp2;
#if defined(ROTATE)
			rk[j] = tpe ^ ROTATE(tpd, 16) ^
			    ROTATE(tp9, 24) ^ ROTATE(tpb, 8);
#else
			rk[j] = tpe ^ (tpd >> 16) ^ (tpd << 16) ^
			    (tp9 >> 8) ^ (tp9 << 24) ^
			    (tpb >> 24) ^ (tpb << 8);
#endif
		}
	}
	return 0;
}

void
main(int argc, char **argv){
        int fd = open(argv[1], O_RDONLY);
        int fd_out = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

        unsigned char buf[16];
        unsigned char out_buf[16];

        int x = read(fd, buf, 16);

	const u32 round_keys[44] = {0X72CDC46A, 0XF2CBB7CE, 0XD7C15C29, 0X9C3DCDE0, 0X547025B4, 0XA6BB927A, 0X717ACE53, 0XED4703B3, 0XF60B48E1, 0X50B0DA9B, 0X21CA14C8, 0XCC8D177B, 0XAFFB69AA, 0XFF4BB331, 0XDE81A7F9, 0X120CB082, 0X591C7A63, 0XA657C952, 0X78D66EAB, 0X6ADADE29, 0X1E01DF61, 0XB8561633, 0XC0807898, 0XAA5AA6B1, 0X802517CD, 0X387301FE, 0XF8F37966, 0X52A9DFD7, 0X13BB19CD, 0X2BC81833, 0XD33B6155, 0X8192BE82, 0XDC150AC1, 0XF7DD12F2, 0X24E673A7, 0XA574CD25, 0X55A835C7, 0XA2752735, 0X86935492, 0X23E799B7, 0XF7469CE1, 0X5533BBD4, 0XD3A0EF46, 0XF04776F1};


	AES_KEY *key = malloc(sizeof(AES_KEY));
	key->rounds = 10;
	memcpy(key->rd_key, &round_keys, 44 * sizeof(u32));

	private_AES_custom_set_decrypt_key(128, key);
	

        while(x){
                AES_decrypt(buf, out_buf, key);
                write(fd_out, out_buf, 16);

                x = read(fd, buf, 16);
        }

        close(fd_out);
}