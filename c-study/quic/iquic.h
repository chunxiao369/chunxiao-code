#ifndef __IQUIC_H__
#define __IQUIC_H__

/*
* Wireshark definitions
*/

/* This dissector handles GQUIC and IETF-QUIC both.
   Main references:
   * https://groups.google.com/a/chromium.org/g/proto-quic/c/wVHBir-uRU0?pli=1
   * https://groups.google.com/a/chromium.org/g/proto-quic/c/OAVgFqw2fko/m/jCbjP0AVAAAJ
   * https://groups.google.com/a/chromium.org/g/proto-quic/c/OAVgFqw2fko/m/-NYxlh88AgAJ
   * https://docs.google.com/document/d/1FcpCJGTDEMblAs-Bm5TYuqhHyUqeWpqrItw2vkMFsdY/edit
   * https://tools.ietf.org/html/draft-ietf-quic-tls-29
   * https://tools.ietf.org/html/draft-ietf-quic-transport-29
   */

/* Versions */
#define V_Q024		0x51303234
#define V_Q025		0x51303235
#define V_Q030		0x51303330
#define V_Q033		0x51303333
#define V_Q034		0x51303334
#define V_Q035		0x51303335
#define V_Q037		0x51303337
#define V_Q039		0x51303339
#define V_Q043		0x51303433
#define V_Q046		0x51303436
#define V_Q050		0x51303530
#define V_T050		0x54303530
#define V_T051		0x54303531
#define V_MVFST_22	0xfaceb001
#define V_MVFST_27	0xfaceb002
#define V_MVFST_EXP	0xfaceb00e

#define QUIC_MAX_CID_LENGTH  20
#define HASH_SHA2_256_LENGTH		32
#define TLS13_AEAD_NONCE_LENGTH		12

/* QUIC decryption context. */
typedef struct quic_cipher {
  gcry_cipher_hd_t hp_cipher;  /* Header protection cipher. */
  gcry_cipher_hd_t pp_cipher;  /* Packet protection cipher. */
  uint8_t pp_iv[TLS13_AEAD_NONCE_LENGTH];
} quic_cipher;

typedef struct quic_decrypt_result {
  uint8_t *data; /* Decrypted result on success (file-scoped). */
  uint32_t data_len;   /* Size of decrypted data. */
} quic_decrypt_result_t;

int is_version_quic(uint32_t version);
int is_version_valid(uint32_t version);
int is_version_supported(uint32_t version);
int is_gquic_ver_less_than(uint32_t version, uint8_t max_version);
int quic_len(const uint8_t *buf, uint64_t *value);
void quic_decrypt_message(quic_cipher *cipher, const uint8_t *packet_payload, uint32_t packet_payload_len,
				 uint32_t header_length, uint8_t first_byte, uint32_t pkn_len,
				 uint64_t packet_number, quic_decrypt_result_t *result);
int quic_derive_initial_secrets(uint32_t version,
				       const uint8_t *cid, uint8_t cid_len,
				       uint8_t client_initial_secret[HASH_SHA2_256_LENGTH]);
void quic_cipher_reset(quic_cipher *cipher);
int quic_cipher_prepare(quic_cipher *cipher, int hash_algo, int cipher_algo,
			       int cipher_mode, uint8_t *secret);
int quic_decrypt_header(const uint8_t *packet_payload,
			       uint32_t pn_offset, gcry_cipher_hd_t hp_cipher,
			       int hp_cipher_algo, uint8_t *first_byte, uint32_t *pn);
#endif
