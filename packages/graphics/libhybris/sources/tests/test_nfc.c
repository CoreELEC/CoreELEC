/*
 * Copyright (C) 2012 Jolla Ltd.
 * Contact: Aaron McCarthy <aaron.mccarthy@jollamobile.com>
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

#include <android-config.h>

#if HAS_LIBNFC_NXP_HEADERS

#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <getopt.h>

#include <hardware/hardware.h>
#include <hardware/nfc.h>
#include <libnfc-nxp/phLibNfc.h>
#include <libnfc-nxp/phDal4Nfc_messageQueueLib.h>

static int messageThreadRunning = 0;
static int numberOfDiscoveredTargets = 0;
static phLibNfc_RemoteDevList_t *discoveredTargets = 0;
static NFCSTATUS targetStatus = 0xFFFF;
static phLibNfc_sRemoteDevInformation_t *connectedRemoteDevInfo = 0;
static phLibNfc_ChkNdef_Info_t ndefInfo;
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *messageThreadFunc(void *arg)
{
    int *clientId = (int *)arg;
    messageThreadRunning = 1;
    while (messageThreadRunning) {
        phDal4Nfc_Message_Wrapper_t message;
        int ret = phDal4Nfc_msgrcv(*clientId, &message,
                                   sizeof(phLibNfc_Message_t), 0, 0);
        if (ret == -1) {
            fprintf(stderr, "Failed to receive message from NFC stack.\n");
            continue;
        }

        switch (message.msg.eMsgType) {
        case PH_LIBNFC_DEFERREDCALL_MSG: {
            phLibNfc_DeferredCall_t *msg = (phLibNfc_DeferredCall_t *)message.msg.pMsgData;
            if (msg->pCallback)
                msg->pCallback(msg->pParameter);
            break;
        }
        default:
            fprintf(stderr, "Unknown message type %d.", message.msg.eMsgType);
        }

        pthread_cond_signal(&cond);
    }

    return 0;
}

static pthread_t createMessageThread(void *arg)
{
    pthread_t thread_id;
    int error = pthread_create(&thread_id, NULL, messageThreadFunc, arg);
    if (error != 0)
        return 0;

    return thread_id;
}

static void terminateMessageThread(int clientId)
{
    messageThreadRunning = 0;

    phDal4Nfc_Message_Wrapper_t message;
    phLibNfc_DeferredCall_t *msg;

    msg = (phLibNfc_DeferredCall_t *)malloc(sizeof(phLibNfc_DeferredCall_t));
    msg->pCallback = 0;
    msg->pParameter = 0;
    message.msg.eMsgType = PH_LIBNFC_DEFERREDCALL_MSG;
    message.msg.pMsgData = msg;
    message.msg.Size = sizeof(phLibNfc_DeferredCall_t);

    phDal4Nfc_msgsnd(clientId, &message, sizeof(phLibNfc_Message_t), 0);
}

void initializeCallback(void *pContext, NFCSTATUS status)
{
    NFCSTATUS *callbackStatus = (NFCSTATUS *)pContext;
    *callbackStatus = status;
}

void discoveryNotificationCallback(void *pContext, phLibNfc_RemoteDevList_t *psRemoteDevList,
                                   uint8_t uNofRemoteDev, NFCSTATUS status)
{
    if (status == NFCSTATUS_DESELECTED) {
        fprintf(stderr, "Target deselected\n");
    } else {
        fprintf(stderr, "Discovered %d targets\n", uNofRemoteDev);

        uint8_t i;
        for (i = 0; i < uNofRemoteDev; ++i) {
            fprintf(stderr, "Target[%d]\n\tType: %d\n\tSession: %d\n", i,
                    psRemoteDevList[i].psRemoteDevInfo->RemDevType,
                    psRemoteDevList[i].psRemoteDevInfo->SessionOpened);
        }
        numberOfDiscoveredTargets = uNofRemoteDev;
        discoveredTargets = psRemoteDevList;
        targetStatus = status;
    }
}

void discoveryCallback(void *pContext, NFCSTATUS status)
{
    fprintf(stderr, "discoveryCallback %d\n", status);
}

void remoteDevConnectCallback(void *pContext, phLibNfc_Handle hRemoteDev,
                              phLibNfc_sRemoteDevInformation_t *psRemoteDevInfo, NFCSTATUS status)
{
    targetStatus = status;
    connectedRemoteDevInfo = psRemoteDevInfo;
}

void remoteDevNdefReadCheckCallback(void *pContext, phLibNfc_ChkNdef_Info_t Ndef_Info,
                                    NFCSTATUS status)
{
    ndefInfo = Ndef_Info;
    targetStatus = status;
}

void remoteDevNdefReadCallback(void *pContext, NFCSTATUS status)
{
    targetStatus = status;
}

void testNfc(int readNdefMessages)
{
    printf("Starting test_nfc.\n");

    const hw_module_t *hwModule = 0;
    nfc_pn544_device_t *nfcDevice = 0;

    printf("Finding NFC hardware module.\n");
    hw_get_module(NFC_HARDWARE_MODULE_ID, &hwModule);
    assert(hwModule != NULL);

    printf("Opening NFC device.\n");
    assert(nfc_pn544_open(hwModule, &nfcDevice) == 0);
    assert(nfcDevice != 0);

    assert(nfcDevice->num_eeprom_settings != 0);
    assert(nfcDevice->eeprom_settings);

    printf("Configuring NFC driver.\n");
    phLibNfc_sConfig_t driverConfig;
    driverConfig.nClientId = phDal4Nfc_msgget(0, 0600);
    assert(driverConfig.nClientId);

    void *hwRef;
    NFCSTATUS status = phLibNfc_Mgt_ConfigureDriver(&driverConfig, &hwRef);
    assert(hwRef);
    assert(status == NFCSTATUS_SUCCESS);

    pthread_t messageThread = createMessageThread(&driverConfig.nClientId);

    printf("Initializing NFC stack.\n");
    NFCSTATUS callbackStatus = 0xFFFF;
    status = phLibNfc_Mgt_Initialize(hwRef, initializeCallback, &callbackStatus);
    assert(status == NFCSTATUS_PENDING);

    pthread_mutex_lock(&mut);
    while (callbackStatus == 0xFFFF)
        pthread_cond_wait(&cond, &mut);
    pthread_mutex_unlock(&mut);

    assert(callbackStatus == NFCSTATUS_SUCCESS);

    printf("Getting NFC stack capabilities.\n");
    phLibNfc_StackCapabilities_t capabilities;
    status = phLibNfc_Mgt_GetstackCapabilities(&capabilities, &callbackStatus);
    assert(status == NFCSTATUS_SUCCESS);

    printf("NFC capabilities:\n"
           "\tHAL version: %u\n"
           "\tFW version: %u\n"
           "\tHW version: %u\n"
           "\tModel: %u\n"
           "\tHCI version: %u\n"
           "\tVendor: %s\n"
           "\tFull version: %u %u\n"
           "\tFW Update: %u\n",
           capabilities.psDevCapabilities.hal_version, capabilities.psDevCapabilities.fw_version,
           capabilities.psDevCapabilities.hw_version, capabilities.psDevCapabilities.model_id,
           capabilities.psDevCapabilities.hci_version,
           capabilities.psDevCapabilities.vendor_name,
           capabilities.psDevCapabilities.full_version[NXP_FULL_VERSION_LEN-1],
           capabilities.psDevCapabilities.full_version[NXP_FULL_VERSION_LEN-2],
           capabilities.psDevCapabilities.firmware_update_info);

    if (readNdefMessages) {
        /* Start tag discovery */
        phLibNfc_Registry_Info_t registryInfo;

        registryInfo.MifareUL = 1;
        registryInfo.MifareStd = 1;
        registryInfo.ISO14443_4A = 1;
        registryInfo.ISO14443_4B = 1;
        registryInfo.Jewel = 1;
        registryInfo.Felica = 1;
        registryInfo.NFC = 1;
        registryInfo.ISO15693 = 1;

        int context;
        status = phLibNfc_RemoteDev_NtfRegister(&registryInfo, discoveryNotificationCallback, &context);
        assert(status == NFCSTATUS_SUCCESS);

        phLibNfc_sADD_Cfg_t discoveryConfig;
        discoveryConfig.NfcIP_Mode = phNfc_eP2P_ALL;
#if (ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR >= 1) || (ANDROID_VERSION_MAJOR >= 5)
        discoveryConfig.NfcIP_Target_Mode = 0x0E;
#endif
        discoveryConfig.Duration = 300000;
        discoveryConfig.NfcIP_Tgt_Disable = 0;
        discoveryConfig.PollDevInfo.PollCfgInfo.EnableIso14443A = 1;
        discoveryConfig.PollDevInfo.PollCfgInfo.EnableIso14443B = 1;
        discoveryConfig.PollDevInfo.PollCfgInfo.EnableFelica212 = 1;
        discoveryConfig.PollDevInfo.PollCfgInfo.EnableFelica424 = 1;
        discoveryConfig.PollDevInfo.PollCfgInfo.EnableIso15693 = 1;
        discoveryConfig.PollDevInfo.PollCfgInfo.EnableNfcActive = 1;
        discoveryConfig.PollDevInfo.PollCfgInfo.DisableCardEmulation = 1;

        targetStatus = 0xFFFF;
        status = phLibNfc_Mgt_ConfigureDiscovery(NFC_DISCOVERY_CONFIG, discoveryConfig,
                                                 discoveryCallback, &context);

        for (;;) {
            pthread_mutex_lock(&mut);
            while (targetStatus == 0xFFFF)
                pthread_cond_wait(&cond, &mut);
            pthread_mutex_unlock(&mut);

            fprintf(stderr, "Discovered %d targets\n", numberOfDiscoveredTargets);
            if (numberOfDiscoveredTargets > 0) {
                targetStatus = 0xFFFF;
                status = phLibNfc_RemoteDev_Connect(discoveredTargets[0].hTargetDev,
                        remoteDevConnectCallback, &context);
                if (status == NFCSTATUS_PENDING) {
                    pthread_mutex_lock(&mut);
                    while (targetStatus == 0xFFFF)
                        pthread_cond_wait(&cond, &mut);
                    pthread_mutex_unlock(&mut);

                    if (targetStatus == NFCSTATUS_SUCCESS) {
                        targetStatus = 0xFFFF;
                        status = phLibNfc_Ndef_CheckNdef(discoveredTargets[0].hTargetDev,
                                remoteDevNdefReadCheckCallback, &context);

                        pthread_mutex_lock(&mut);
                        while (targetStatus == 0xFFFF)
                            pthread_cond_wait(&cond, &mut);
                        pthread_mutex_unlock(&mut);

                        if (targetStatus == NFCSTATUS_SUCCESS &&
                            (ndefInfo.NdefCardState == PHLIBNFC_NDEF_CARD_READ_WRITE ||
                             ndefInfo.NdefCardState == PHLIBNFC_NDEF_CARD_READ_ONLY)) {
                            phLibNfc_Data_t ndefBuffer;
                            ndefBuffer.length = ndefInfo.MaxNdefMsgLength;
                            ndefBuffer.buffer = malloc(ndefBuffer.length);

                            targetStatus = 0xFFFF;
                            status = phLibNfc_Ndef_Read(discoveredTargets[0].hTargetDev, &ndefBuffer,
                                    phLibNfc_Ndef_EBegin, remoteDevNdefReadCallback, &context);

                            pthread_mutex_lock(&mut);
                            while (targetStatus == 0xFFFF)
                                pthread_cond_wait(&cond, &mut);
                            pthread_mutex_unlock(&mut);

                            if (targetStatus == NFCSTATUS_SUCCESS) {
                                int i;
                                fprintf(stderr, "NDEF: ");
                                for (i = 0; i < ndefBuffer.length; ++i)
                                    fprintf(stderr, "%02x", ndefBuffer.buffer[i]);
                                fprintf(stderr, "\n");
                            }

                            free(ndefBuffer.buffer);
                        }
                    }
                }
            }

            if (status == NFCSTATUS_FAILED) {
                fprintf(stderr, "Failed to connect to remote device\n");
                break;
            }

            targetStatus = 0xFFFF;
            status = phLibNfc_Mgt_ConfigureDiscovery(NFC_DISCOVERY_RESUME, discoveryConfig,
                                                     discoveryCallback, &context);
            assert(status == NFCSTATUS_SUCCESS || status == NFCSTATUS_PENDING);
        }
    }

    printf("Deinitializing NFC stack.\n");
    callbackStatus = 0xFFFF;
    status = phLibNfc_Mgt_DeInitialize(hwRef, initializeCallback, &callbackStatus);
    assert(status == NFCSTATUS_PENDING);

    pthread_mutex_lock(&mut);
    while (callbackStatus == 0xFFFF)
        pthread_cond_wait(&cond, &mut);
    pthread_mutex_unlock(&mut);

    assert(callbackStatus == NFCSTATUS_SUCCESS);

    terminateMessageThread(driverConfig.nClientId);
    pthread_join(messageThread, NULL);

    printf("Unconfiguring NFC driver.\n");
    status = phLibNfc_Mgt_UnConfigureDriver(hwRef);
    assert(status == NFCSTATUS_SUCCESS);

    int result = phDal4Nfc_msgctl(driverConfig.nClientId, 0, 0);
    assert(result == 0);

    printf("Closing NFC device.\n");
    nfc_pn544_close(nfcDevice);
}

int main(int argc, char **argv)
{
    int opt;
    int readNdefMessages = 0;
    int restartOnFailure = 0;

    while ((opt = getopt(argc, argv, "nrh")) != -1) {
        switch (opt) {
        case 'n':
            readNdefMessages = 1;
            fprintf(stdout, "Reading NDEF messages from targets\n");
            break;
        case 'r':
            restartOnFailure = 1;
            readNdefMessages = 1;
            fprintf(stdout, "Restarting on NDEF read failure, implies -n\n");
            break;
        case 'h':
        default:
            fprintf(stderr, "\n Usage: %s \n"
                            "\t-n    Read NDEF message from targets,\n"
                            "\t-r    Restart on NDEF read failure.\n", argv[0]);
            return 1;
        }
    }

    do  {
        testNfc(readNdefMessages);
    } while (restartOnFailure && readNdefMessages);

	return 0;
}

#else
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("test_nfc is not supported in this build\n");
    return 0;
}

#endif

