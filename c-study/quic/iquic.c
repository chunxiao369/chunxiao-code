#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#define HAVE_LIBGCRYPT
#ifdef HAVE_LIBGCRYPT
#include <gcrypt.h>
#endif

#include "iquic.h"

#ifndef MIN
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#endif

static int is_version_gquic(uint32_t version)
{
  return ((version & 0xFFFFFF00) == 0x54303500) /* T05X */ ||
    ((version & 0xFFFFFF00) == 0x51303500) /* Q05X */ ||
    ((version & 0xFFFFFF00) == 0x51303400) /* Q04X */ ||
    ((version & 0xFFFFFF00) == 0x51303300) /* Q03X */ ||
    ((version & 0xFFFFFF00) == 0x51303200) /* Q02X */;
}
int is_version_quic(uint32_t version)
{
  return ((version & 0xFFFFFF00) == 0xFF000000) /* IETF */ ||
    ((version & 0xFFFFF000) == 0xfaceb000) /* Facebook */;
}
int is_version_valid(uint32_t version)
{
  return is_version_gquic(version) || is_version_quic(version);
}
static uint8_t get_u8_quic_ver(uint32_t version)
{
  if((version >> 8) == 0xff0000)
    return (uint8_t)version;
  return 0;
}
#ifdef HAVE_LIBGCRYPT
static int is_quic_ver_less_than(uint32_t version, uint8_t max_version)
{
  uint8_t u8_ver = get_u8_quic_ver(version);
  return u8_ver && u8_ver <= max_version;
}
#endif
static int is_quic_ver_greater_than(uint32_t version, uint8_t min_version)
{
  return get_u8_quic_ver(version) >= min_version;
}
static uint8_t get_u8_gquic_ver(uint32_t version)
{
  if(is_version_gquic(version)) {
    version = ntohl(((uint16_t)version) << 16);
    return atoi((char *)&version);
  }
  return 0;
}
int is_gquic_ver_less_than(uint32_t version, uint8_t max_version)
{
  uint8_t u8_ver = get_u8_gquic_ver(version);
  return u8_ver && u8_ver <= max_version;
}
int is_version_supported(uint32_t version)
{
  return (version == V_Q024 ||
          version == V_Q025 ||
          version == V_Q030 ||
          version == V_Q033 ||
          version == V_Q034 ||
          version == V_Q035 ||
          version == V_Q037 ||
          version == V_Q039 ||
          version == V_Q043 ||
          version == V_Q046 ||
          version == V_Q050 ||
          version == V_T050 ||
          version == V_T051 ||
	  version == V_MVFST_22 ||
	  version == V_MVFST_27 ||
	  version == V_MVFST_EXP ||
          is_quic_ver_greater_than(version, 23));
}
int is_version_with_encrypted_header(uint32_t version)
{
  return is_version_quic(version) ||
    ((version & 0xFFFFFF00) == 0x51303500) /* Q05X */ ||
    ((version & 0xFFFFFF00) == 0x54303500) /* T05X */;
}
int is_version_with_tls(uint32_t version)
{
  return is_version_quic(version) ||
    ((version & 0xFFFFFF00) == 0x54303500) /* T05X */;
}
int is_version_with_var_int_transport_params(uint32_t version)
{
  return (is_version_quic(version) && is_quic_ver_greater_than(version, 27)) ||
    (version == V_T051);
}

uint64_t ntohll(uint64_t val)
{
    return (((uint64_t) ntohl(val)) << 32) + ntohl(val >> 32);
}

int quic_len(const uint8_t *buf, uint64_t *value)
{
  *value = buf[0];
  switch((*value) >> 6) {
  case 0:
    (*value) &= 0x3F;
    return 1;
  case 1:
    *value = ntohs(*(uint16_t *)buf) & 0x3FFF;
    return 2;
  case 2:
    *value = ntohl(*(uint32_t *)buf) & 0x3FFFFFFF;
    return 4;
  case 3:
    *value = ntohll(*(uint64_t *)buf) & 0x3FFFFFFFFFFFFFFF;
    return 8;
  default: /* No Possible */
    return 0;
  }
}
int quic_len_buffer_still_required(uint8_t value)
{
  switch(value >> 6) {
  case 0:
    return 0;
  case 1:
    return 1;
  case 2:
    return 3;
  case 3:
    return 7;
  default: /* No Possible */
    return 0;
  }
}


uint16_t gquic_get_u16(const uint8_t *buf, uint32_t version)
{
  if(version >= V_Q039)
    return ntohs(*(uint16_t *)buf);
  return (*(uint16_t *)buf);
}


#ifdef HAVE_LIBGCRYPT

#ifdef DEBUG_CRYPT
char *__gcry_err(gpg_error_t err, char *buf, size_t buflen)
{
#ifdef HAVE_LIBGPG_ERROR
  gpg_strerror_r(err, buf, buflen);
  /* I am not sure if the string will be always null-terminated...
     Better safe than sorry */
  if(buflen > 0)
    buf[buflen - 1] = '\0';
#else
  if(buflen > 0)
    buf[0] = '\0';
#endif
  return buf;
}
#endif /* DEBUG_CRYPT */

static uint64_t pntoh64(const void *p)
{
  return (uint64_t)*((const uint8_t *)(p)+0)<<56|
    (uint64_t)*((const uint8_t *)(p)+1)<<48|
    (uint64_t)*((const uint8_t *)(p)+2)<<40|
    (uint64_t)*((const uint8_t *)(p)+3)<<32|
    (uint64_t)*((const uint8_t *)(p)+4)<<24|
    (uint64_t)*((const uint8_t *)(p)+5)<<16|
    (uint64_t)*((const uint8_t *)(p)+6)<<8|
    (uint64_t)*((const uint8_t *)(p)+7)<<0;
}
static void phton64(uint8_t *p, uint64_t v)
{
  p[0] = (uint8_t)(v >> 56);
  p[1] = (uint8_t)(v >> 48);
  p[2] = (uint8_t)(v >> 40);
  p[3] = (uint8_t)(v >> 32);
  p[4] = (uint8_t)(v >> 24);
  p[5] = (uint8_t)(v >> 16);
  p[6] = (uint8_t)(v >> 8);
  p[7] = (uint8_t)(v >> 0);
}

static void *memdup(const uint8_t *orig, size_t len)
{
  void *dest = malloc(len);
  if(dest)
    memcpy(dest, orig, len);
  return dest;
}


typedef struct _StringInfo {
  unsigned char *data;		/* Backing storage which may be larger than data_len */
  unsigned int data_len;	/* Length of the meaningful part of data */
} StringInfo;


/*
 * From wsutil/wsgcrypt.{c,h}
 */

static gcry_error_t ws_hmac_buffer(int algo, void *digest, const void *buffer,
				   size_t length, const void *key, size_t keylen)
{
  gcry_md_hd_t hmac_handle;
  gcry_error_t result = gcry_md_open(&hmac_handle, algo, GCRY_MD_FLAG_HMAC);
  if(result) {
    return result;
  }
  result = gcry_md_setkey(hmac_handle, key, keylen);
  if(result) {
    gcry_md_close(hmac_handle);
    return result;
  }
  gcry_md_write(hmac_handle, buffer, length);
  memcpy(digest, gcry_md_read(hmac_handle, 0), gcry_md_get_algo_dlen(algo));
  gcry_md_close(hmac_handle);
  return GPG_ERR_NO_ERROR;
}
static gcry_error_t hkdf_expand(int hashalgo, const uint8_t *prk, uint32_t prk_len,
				const uint8_t *info, uint32_t info_len,
				uint8_t *out, uint32_t out_len)
{
  /* Current maximum hash output size: 48 bytes for SHA-384. */
  uint8_t lastoutput[48];
  gcry_md_hd_t h;
  gcry_error_t err;
  const unsigned int hash_len = gcry_md_get_algo_dlen(hashalgo);

  /* Some sanity checks */
  if(!(out_len > 0 && out_len <= 255 * hash_len) ||
     !(hash_len > 0 && hash_len <= sizeof(lastoutput))) {
    return GPG_ERR_INV_ARG;
  }

  err = gcry_md_open(&h, hashalgo, GCRY_MD_FLAG_HMAC);
  if(err) {
    return err;
  }

  for(uint32_t offset = 0; offset < out_len; offset += hash_len) {
    gcry_md_reset(h);
    gcry_md_setkey(h, prk, prk_len); /* Set PRK */
    if(offset > 0) {
      gcry_md_write(h, lastoutput, hash_len); /* T(1..N) */
    }
    gcry_md_write(h, info, info_len);                   /* info */

    uint8_t c = offset / hash_len + 1;
    gcry_md_write(h, &c, sizeof(c));                    /* constant 0x01..N */

    memcpy(lastoutput, gcry_md_read(h, hashalgo), hash_len);
    memcpy(out + offset, lastoutput, MIN(hash_len, out_len - offset));
  }

  gcry_md_close(h);
  return 0;
}
/*
 * Calculate HKDF-Extract(salt, IKM) -> PRK according to RFC 5869.
 * Caller MUST ensure that 'prk' is large enough to store the digest from hash
 * algorithm 'hashalgo' (e.g. 32 bytes for SHA-256).
 */
static gcry_error_t hkdf_extract(int hashalgo, const uint8_t *salt, size_t salt_len,
				 const uint8_t *ikm, size_t ikm_len, uint8_t *prk)
{
  /* PRK = HMAC-Hash(salt, IKM) where salt is key, and IKM is input. */
  return ws_hmac_buffer(hashalgo, prk, ikm, ikm_len, salt, salt_len);
}


/*
 * From epan/dissectors/packet-tls-utils.c
 */

/*
 * Computes HKDF-Expand-Label(Secret, Label, Hash(context_value), Length) with a
 * custom label prefix. If "context_hash" is NULL, then an empty context is
 * used. Otherwise it must have the same length as the hash algorithm output.
 */
static int tls13_hkdf_expand_label_context(int md, const StringInfo *secret,
					   const char *label_prefix, const char *label,
					   const uint8_t *context_hash, uint8_t context_length,
					   uint16_t out_len, uint8_t **out)
{
  /* RFC 8446 Section 7.1:
   * HKDF-Expand-Label(Secret, Label, Context, Length) =
   *      HKDF-Expand(Secret, HkdfLabel, Length)
   * struct {
   *     uint16 length = Length;
   *     opaque label<7..255> = "tls13 " + Label; // "tls13 " is label prefix.
   *     opaque context<0..255> = Context;
   * } HkdfLabel;
   *
   * RFC 5869 HMAC-based Extract-and-Expand Key Derivation Function (HKDF):
   * HKDF-Expand(PRK, info, L) -> OKM
   */
  gcry_error_t err;
  const unsigned int label_prefix_length = (unsigned int)strlen(label_prefix);
  const unsigned label_length = (unsigned int)strlen(label);
#ifdef DEBUG_CRYPT
  char buferr[128];
#endif

  /* Some sanity checks */
  if(!(label_length > 0 && label_prefix_length + label_length <= 255)) {
#ifdef DEBUG_CRYPT
    printf("Failed sanity checks\n");
#endif
    return 0;
  }

  /* info = HkdfLabel { length, label, context } */
  /* Keep original Wireshark code as reference */
#if 0
  GByteArray *info = g_byte_array_new();
  const uint16_t length = htons(out_len);
  g_byte_array_append(info, (const guint8 *)&length, sizeof(length));

  const uint8_t label_vector_length = label_prefix_length + label_length;
  g_byte_array_append(info, &label_vector_length, 1);
  g_byte_array_append(info, (const uint8_t *)label_prefix, label_prefix_length);
  g_byte_array_append(info, (const uint8_t *)label, label_length);

  g_byte_array_append(info, &context_length, 1);
  if (context_length) {
    g_byte_array_append(info, context_hash, context_length);
  }
#else
  uint32_t info_len = 0;
  uint8_t *info_data = (uint8_t *)malloc(1024);
  if(!info_data)
    return 0;
  const uint16_t length = htons(out_len);
  memcpy(&info_data[info_len], &length, sizeof(length));
  info_len += sizeof(length);

  const uint8_t label_vector_length = label_prefix_length + label_length;
  memcpy(&info_data[info_len], &label_vector_length, 1);
  info_len += 1;
  memcpy(&info_data[info_len], (const uint8_t *)label_prefix, label_prefix_length);
  info_len += label_prefix_length;
  memcpy(&info_data[info_len], (const uint8_t *)label, label_length);
  info_len += label_length;

  memcpy(&info_data[info_len], &context_length, 1);
  info_len += 1;
  if(context_length) {
    memcpy(&info_data[info_len], context_hash, context_length);
    info_len += context_length;
  }
#endif

  *out = (uint8_t *)malloc(out_len);
  if(!*out)
    return 0;
  err = hkdf_expand(md, secret->data, secret->data_len, info_data, info_len, *out, out_len);
  free(info_data);

  if(err) {
#ifdef DEBUG_CRYPT
    printf("Failed hkdf_expand: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    free(*out);
    *out = NULL;
    return 0;
  }

  return 1;
}
static int tls13_hkdf_expand_label(int md, const StringInfo *secret,
				   const char *label_prefix, const char *label,
				   uint16_t out_len, unsigned char **out)
{
  return tls13_hkdf_expand_label_context(md, secret, label_prefix, label, NULL, 0, out_len, out);
}


/*
 * From epan/dissectors/packet-quic.c
 */

static int quic_hkdf_expand_label(int hash_algo, uint8_t *secret, uint32_t secret_len,
				  const char *label, uint8_t *out, uint32_t out_len)
{
  const StringInfo secret_si = { secret, secret_len };
  uint8_t *out_mem = NULL;
  if(tls13_hkdf_expand_label(hash_algo, &secret_si, "tls13 ", label, out_len, &out_mem)) {
    memcpy(out, out_mem, out_len);
    free(out_mem);
    return 1;
  }
  return 0;
}
void quic_cipher_reset(quic_cipher *cipher)
{
  gcry_cipher_close(cipher->hp_cipher);
  gcry_cipher_close(cipher->pp_cipher);
#if 0
  memset(cipher, 0, sizeof(*cipher));
#endif
}
/**
 * Expands the secret (length MUST be the same as the "hash_algo" digest size)
 * and initialize cipher with the new key.
 */
static int quic_cipher_init(quic_cipher *cipher, int hash_algo,
			    uint8_t key_length, uint8_t *secret)
{
  uint8_t write_key[256/8];   /* Maximum key size is for AES256 cipher. */
  uint8_t hp_key[256/8];
  uint32_t hash_len = gcry_md_get_algo_dlen(hash_algo);

  if(key_length > sizeof(write_key)) {
    return 0;
  }

  if(!quic_hkdf_expand_label(hash_algo, secret, hash_len, "quic key", write_key, key_length) ||
     !quic_hkdf_expand_label(hash_algo, secret, hash_len, "quic iv", cipher->pp_iv, sizeof(cipher->pp_iv)) ||
     !quic_hkdf_expand_label(hash_algo, secret, hash_len, "quic hp", hp_key, key_length)) {
    return 1;
  }

  return gcry_cipher_setkey(cipher->hp_cipher, hp_key, key_length) == 0 &&
    gcry_cipher_setkey(cipher->pp_cipher, write_key, key_length) == 0;
}
/**
 * Maps a Packet Protection cipher to the Packet Number protection cipher.
 * See https://tools.ietf.org/html/draft-ietf-quic-tls-22#section-5.4.3
 */
static int quic_get_pn_cipher_algo(int cipher_algo, int *hp_cipher_mode)
{
  switch (cipher_algo) {
  case GCRY_CIPHER_AES128:
  case GCRY_CIPHER_AES256:
    *hp_cipher_mode = GCRY_CIPHER_MODE_ECB;
    return 1;
  default:
    return 0;
  }
}
/*
 * (Re)initialize the PNE/PP ciphers using the given cipher algorithm.
 * If the optional base secret is given, then its length MUST match the hash
 * algorithm output.
 */
int quic_cipher_prepare(quic_cipher *cipher, int hash_algo, int cipher_algo,
			       int cipher_mode, uint8_t *secret)
{
#if 0
  /* Clear previous state (if any). */
  quic_cipher_reset(cipher);
#endif

  int hp_cipher_mode;
  if(!quic_get_pn_cipher_algo(cipher_algo, &hp_cipher_mode)) {
#ifdef DEBUG_CRYPT
    printf("Unsupported cipher algorithm\n");
#endif
    return 0;
  }

  if(gcry_cipher_open(&cipher->hp_cipher, cipher_algo, hp_cipher_mode, 0) ||
     gcry_cipher_open(&cipher->pp_cipher, cipher_algo, cipher_mode, 0)) {
    quic_cipher_reset(cipher);
#ifdef DEBUG_CRYPT
    printf("Failed to create ciphers\n");
#endif
    return 0;
  }

  if(secret) {
    uint32_t cipher_keylen = (uint8_t)gcry_cipher_get_algo_keylen(cipher_algo);
    if(!quic_cipher_init(cipher, hash_algo, cipher_keylen, secret)) {
      quic_cipher_reset(cipher);
#ifdef DEBUG_CRYPT
      printf("Failed to derive key material for cipher\n");
#endif
      return 0;
    }
  }

  return 1;
}
/**
 * Given a header protection cipher, a buffer and the packet number offset,
 * return the unmasked first byte and packet number.
 */
int quic_decrypt_header(const uint8_t *packet_payload,
			       uint32_t pn_offset, gcry_cipher_hd_t hp_cipher,
			       int hp_cipher_algo, uint8_t *first_byte, uint32_t *pn)
{
  gcry_cipher_hd_t h = hp_cipher;
  if(!hp_cipher) {
    /* Need to know the cipher */
    return 0;
  }

  /* Sample is always 16 bytes and starts after PKN (assuming length 4).
     https://tools.ietf.org/html/draft-ietf-quic-tls-22#section-5.4.2 */
  uint8_t sample[16];
  memcpy(sample, packet_payload + pn_offset + 4, 16);

  uint8_t mask[5] = { 0 };
  switch (hp_cipher_algo) {
  case GCRY_CIPHER_AES128:
  case GCRY_CIPHER_AES256:
    /* Encrypt in-place with AES-ECB and extract the mask. */
    if(gcry_cipher_encrypt(h, sample, sizeof(sample), NULL, 0)) {
      return 0;
    }
    memcpy(mask, sample, sizeof(mask));
    break;
  default:
    return 0;
  }

  /* https://tools.ietf.org/html/draft-ietf-quic-tls-22#section-5.4.1 */
  uint8_t packet0 = packet_payload[0];
  if((packet0 & 0x80) == 0x80) {
    /* Long header: 4 bits masked */
    packet0 ^= mask[0] & 0x0f;
  } else {
    /* Short header: 5 bits masked */
    packet0 ^= mask[0] & 0x1f;
  }
  uint32_t pkn_len = (packet0 & 0x03) + 1;
  /* printf("packet0 0x%x pkn_len %d\n", packet0, pkn_len); */

  uint8_t pkn_bytes[4];
  memcpy(pkn_bytes, packet_payload + pn_offset, pkn_len);
  uint32_t pkt_pkn = 0;
  for(uint32_t i = 0; i < pkn_len; i++) {
    pkt_pkn |= (uint32_t)(pkn_bytes[i] ^ mask[1 + i]) << (8 * (pkn_len - 1 - i));
  }
  *first_byte = packet0;
  *pn = pkt_pkn;
  return 1;
}
/**
 * Given a QUIC message (header + non-empty payload), the actual packet number,
 * try to decrypt it using the cipher.
 * As the header points to the original buffer with an encrypted packet number,
 * the (encrypted) packet number length is also included.
 *
 * The actual packet number must be constructed according to
 * https://tools.ietf.org/html/draft-ietf-quic-transport-22#section-12.3
 */
void quic_decrypt_message(quic_cipher *cipher, const uint8_t *packet_payload, uint32_t packet_payload_len,
				 uint32_t header_length, uint8_t first_byte, uint32_t pkn_len,
				 uint64_t packet_number, quic_decrypt_result_t *result)
{
  gcry_error_t err;
  uint8_t *header;
  uint8_t nonce[TLS13_AEAD_NONCE_LENGTH];
  uint8_t *buffer;
  uint8_t atag[16];
  uint32_t buffer_length;
#ifdef DEBUG_CRYPT
  char buferr[128];
#endif

  if(!(cipher != NULL) ||
     !(cipher->pp_cipher != NULL) ||
     !(pkn_len < header_length) ||
     !(1 <= pkn_len && pkn_len <= 4)) {
#ifdef DEBUG_CRYPT
    printf("Failed sanity checks\n");
#endif
    return;
  }
  /* Copy header, but replace encrypted first byte and PKN by plaintext. */
  header = (uint8_t *)memdup(packet_payload, header_length);
  if(!header)
    return;
  header[0] = first_byte;
  for(uint32_t i = 0; i < pkn_len; i++) {
    header[header_length - 1 - i] = (uint8_t)(packet_number >> (8 * i));
  }

  /* Input is "header || ciphertext (buffer) || auth tag (16 bytes)" */
  buffer_length = packet_payload_len - (header_length + 16);
  if(buffer_length == 0) {
#ifdef DEBUG_CRYPT
    printf("Decryption not possible, ciphertext is too short\n");
#endif
    free(header);
    return;
  }
  buffer = (uint8_t *)memdup(packet_payload + header_length, buffer_length);
  if(!buffer) {
    free(header);
    return;
  }
  memcpy(atag, packet_payload + header_length + buffer_length, 16);

  memcpy(nonce, cipher->pp_iv, TLS13_AEAD_NONCE_LENGTH);
  /* Packet number is left-padded with zeroes and XORed with write_iv */
  phton64(nonce + sizeof(nonce) - 8, pntoh64(nonce + sizeof(nonce) - 8) ^ packet_number);

  gcry_cipher_reset(cipher->pp_cipher);
  err = gcry_cipher_setiv(cipher->pp_cipher, nonce, TLS13_AEAD_NONCE_LENGTH);
  if(err) {
#ifdef DEBUG_CRYPT
    printf("Decryption (setiv) failed: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    free(header);
    free(buffer);
    return;
  }

  /* associated data (A) is the contents of QUIC header */
  err = gcry_cipher_authenticate(cipher->pp_cipher, header, header_length);
  if(err) {
#ifdef DEBUG_CRYPT
    printf("Decryption (authenticate) failed: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    free(header);
    free(buffer);
    return;
  }

  free(header);

  /* Output ciphertext (C) */
  err = gcry_cipher_decrypt(cipher->pp_cipher, buffer, buffer_length, NULL, 0);
  if(err) {
#ifdef DEBUG_CRYPT
    printf("Decryption (decrypt) failed: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    free(buffer);
    return;
  }

  err = gcry_cipher_checktag(cipher->pp_cipher, atag, 16);
  if(err) {
#ifdef DEBUG_CRYPT
    printf("Decryption (checktag) failed: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    free(buffer);
    return;
  }

  result->data = buffer;
  result->data_len = buffer_length;
}
/**
 * Compute the client and server initial secrets given Connection ID "cid".
 */
int quic_derive_initial_secrets(uint32_t version,
				       const uint8_t *cid, uint8_t cid_len,
				       uint8_t client_initial_secret[HASH_SHA2_256_LENGTH])
{
  /*
   * https://tools.ietf.org/html/draft-ietf-quic-tls-29#section-5.2
   *
   * initial_secret = HKDF-Extract(initial_salt, client_dst_connection_id)
   *
   * client_initial_secret = HKDF-Expand-Label(initial_secret,
   *                                           "client in", "", Hash.length)
   *
   * Hash for handshake packets is SHA-256 (output size 32).
   */
  static const uint8_t handshake_salt_draft_22[20] = {
						      0x7f, 0xbc, 0xdb, 0x0e, 0x7c, 0x66, 0xbb, 0xe9, 0x19, 0x3a,
						      0x96, 0xcd, 0x21, 0x51, 0x9e, 0xbd, 0x7a, 0x02, 0x64, 0x4a
  };
  static const uint8_t handshake_salt_draft_23[20] = {
						      0xc3, 0xee, 0xf7, 0x12, 0xc7, 0x2e, 0xbb, 0x5a, 0x11, 0xa7,
						      0xd2, 0x43, 0x2b, 0xb4, 0x63, 0x65, 0xbe, 0xf9, 0xf5, 0x02,
  };
  static const uint8_t handshake_salt_draft_29[20] = {
						      0xaf, 0xbf, 0xec, 0x28, 0x99, 0x93, 0xd2, 0x4c, 0x9e, 0x97,
						      0x86, 0xf1, 0x9c, 0x61, 0x11, 0xe0, 0x43, 0x90, 0xa8, 0x99
  };
  static const uint8_t hanshake_salt_draft_q50[20] = {
						      0x50, 0x45, 0x74, 0xEF, 0xD0, 0x66, 0xFE, 0x2F, 0x9D, 0x94,
						      0x5C, 0xFC, 0xDB, 0xD3, 0xA7, 0xF0, 0xD3, 0xB5, 0x6B, 0x45
  };
  static const uint8_t hanshake_salt_draft_t50[20] = {
						      0x7f, 0xf5, 0x79, 0xe5, 0xac, 0xd0, 0x72, 0x91, 0x55, 0x80,
						      0x30, 0x4c, 0x43, 0xa2, 0x36, 0x7c, 0x60, 0x48, 0x83, 0x10
  };
  static const uint8_t hanshake_salt_draft_t51[20] = {
						      0x7a, 0x4e, 0xde, 0xf4, 0xe7, 0xcc, 0xee, 0x5f, 0xa4, 0x50,
						      0x6c, 0x19, 0x12, 0x4f, 0xc8, 0xcc, 0xda, 0x6e, 0x03, 0x3d
  };

  gcry_error_t err;
  uint8_t secret[HASH_SHA2_256_LENGTH];
#ifdef DEBUG_CRYPT
  char buferr[128];
#endif

  if(version == V_Q050) {
    err = hkdf_extract(GCRY_MD_SHA256, hanshake_salt_draft_q50,
		       sizeof(hanshake_salt_draft_q50),
                       cid, cid_len, secret);
  } else if(version == V_T050) {
    err = hkdf_extract(GCRY_MD_SHA256, hanshake_salt_draft_t50,
		       sizeof(hanshake_salt_draft_t50),
                       cid, cid_len, secret);
  } else if(version == V_T051) {
    err = hkdf_extract(GCRY_MD_SHA256, hanshake_salt_draft_t51,
		       sizeof(hanshake_salt_draft_t51),
                       cid, cid_len, secret);
  } else if(is_quic_ver_less_than(version, 22) ||
	    version == V_MVFST_22) {
    err = hkdf_extract(GCRY_MD_SHA256, handshake_salt_draft_22,
		       sizeof(handshake_salt_draft_22),
                       cid, cid_len, secret);
  } else if(is_quic_ver_less_than(version, 28) ||
	    version == V_MVFST_27 ||
	    version == V_MVFST_EXP) {
    err = hkdf_extract(GCRY_MD_SHA256, handshake_salt_draft_23,
		       sizeof(handshake_salt_draft_23),
                       cid, cid_len, secret);
  } else {
    err = hkdf_extract(GCRY_MD_SHA256, handshake_salt_draft_29,
		       sizeof(handshake_salt_draft_29),
                       cid, cid_len, secret);
  }
  if(err) {
#ifdef DEBUG_CRYPT
    printf("Failed to extract secrets: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    return -1;
  }

  if(!quic_hkdf_expand_label(GCRY_MD_SHA256, secret, sizeof(secret), "client in",
			     client_initial_secret, HASH_SHA2_256_LENGTH)) {
#ifdef DEBUG_CRYPT
    printf("Key expansion (client) failed: %s\n", __gcry_err(err, buferr, sizeof(buferr)));
#endif
    return -1;
  }

  return 0;
}

#endif
