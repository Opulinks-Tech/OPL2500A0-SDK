/*
 * EAP peer state machine functions (RFC 4137)
 * Copyright (c) 2004-2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EAP_H
#define EAP_H

#include "common/defs.h"
#include "eap_peer/eap_defs.h"
#include "eap_peer/eap_config.h"

struct eap_sm;

struct eap_method_type {
	int vendor;
	EapType method;
};

const u8 * eap_get_eapKeyData(struct eap_sm *sm, size_t *len);
void eap_deinit_prev_method(struct eap_sm *sm, const char *txt);
struct wpabuf * eap_sm_build_nak(struct eap_sm *sm, EapType type, u8 id);
struct wpabuf * eap_sm_buildIdentity(struct eap_sm *sm, int id, int encrypted);
int eap_peer_blob_init(struct eap_sm *sm);
void eap_peer_blob_deinit(struct eap_sm *sm);
int eap_peer_config_init(struct eap_sm *sm);
void eap_peer_config_deinit(struct eap_sm *sm);
void eap_sm_abort(struct eap_sm *sm);
int eap_peer_register_methods(void);
void eap_sm_request_identity(struct eap_sm *sm);

#endif /* EAP_H */
