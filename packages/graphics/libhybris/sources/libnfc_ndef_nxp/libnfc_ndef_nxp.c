/*
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jollamobile.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * Bindings generated using scripts in: utils/generate_nfc
 **/

#if HAS_LIBNFC_NXP_HEADERS

#include <libnfc-nxp/phNfcStatus.h>
#include <libnfc-nxp/phNfcTypes.h>
#include <libnfc-nxp/phNfcIoctlCode.h>
#include <libnfc-nxp/phLibNfc.h>
#include <libnfc-nxp/phDal4Nfc_messageQueueLib.h>
#include <libnfc-nxp/phFriNfc_NdefMap.h>

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>

#include <hybris/common/binding.h>

HYBRIS_LIBRARY_INITIALIZE(libnfc_ndef_so, "libnfc_ndef.so");

HYBRIS_IMPLEMENT_FUNCTION5(libnfc_ndef_so, NFCSTATUS, phFriNfc_NdefRecord_GetRecords, uint8_t *, uint32_t, uint8_t **, uint8_t *, uint32_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_ndef_so, uint32_t, phFriNfc_NdefRecord_GetLength, phFriNfc_NdefRecord_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_ndef_so, NFCSTATUS, phFriNfc_NdefRecord_Parse, phFriNfc_NdefRecord_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_ndef_so, NFCSTATUS, phFriNfc_NdefRecord_Generate, phFriNfc_NdefRecord_t *, uint8_t *, uint32_t, uint32_t *);
/* XXX No prototype for exported symbol: __on_dlclose */
#endif

