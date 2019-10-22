/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2019      The Fluent Bit Authors
 *  Copyright (C) 2015-2018 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef FLB_ENCODE_H
#define FLB_ENCODE_H

#include <msgpack.h>

typedef void *flb_encoder;

#ifdef FLB_HAVE_ENCODE
flb_encoder flb_get_encoder(const char *encoding);
void flb_msgpack_encode_utf8(flb_encoder enc, msgpack_packer* pk, const void* b, size_t l);
#else
static inline flb_encoder flb_get_encoder(const char *encoding)
{
    return NULL;
}

static inline void flb_msgpack_encode_utf8(flb_encoder enc, msgpack_packer* pk, const void* b, size_t l)
{
    msgpack_pack_str(pk, l);
    msgpack_pack_str_body(pk, b, l);
}
#endif

#endif /* FLB_ENCODE_H */
