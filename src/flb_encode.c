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

#include <fluent-bit/flb_encode.h>
#include <fluent-bit/flb_mem.h>

#ifdef FLB_HAVE_ENCODE
#include <tutf8e.h>
#endif

const size_t TUTF8_DEFAULT_BUFFER = 256;

void flb_msgpack_iso_8859_2_as_utf8(msgpack_packer* pk, const void* b, size_t l)
{
#ifdef FLB_HAVE_ENCODE
    size_t size = 0;
    if (!tutf8e_buffer_length_iso_8859_2(b, l, &size) && size)
    {
        /* Already UTF8 encoded? */
        if (size == l) {
        }
        /* Small enough for encoding to stack? */
        else if (size<=TUTF8_DEFAULT_BUFFER)
        {
            size = TUTF8_DEFAULT_BUFFER;
            char buffer[TUTF8_DEFAULT_BUFFER];
            if (!tutf8e_buffer_encode_iso_8859_2(buffer, &size, b, l) && size) {
                msgpack_pack_str(pk, size);
                msgpack_pack_str_body(pk, buffer, size);
                return;
            }
        }
        /* malloc/free the encoded copy */
        else {
            char *buffer = (char *) flb_malloc(size);
            if (buffer && !tutf8e_buffer_encode_iso_8859_2(buffer, &size, b, l) && size) {
                msgpack_pack_str(pk, size);
                msgpack_pack_str_body(pk, buffer, size);
                free(buffer);
                return;
            }                        
            free(buffer);
        }
    }
#endif

    /* Could not or need not encode to UTF8 */
    msgpack_pack_str(pk, l);
    msgpack_pack_str_body(pk, b, l);
}
