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

#include <android-config.h>

#include <libnfc-nxp/phNfcTypes.h>
#include <libnfc-nxp/phNfcIoctlCode.h>
#include <libnfc-nxp/phLibNfc.h>
#include <libnfc-nxp/phDal4Nfc.h>
#include <libnfc-nxp/phFriNfc_NdefMap.h>
#include <libnfc-nxp/phLibNfc_Internal.h>

/**
 * Fix a compiler warning, as this macro is defined multiple times
 * to different values - we don't need the value for the wrappers.
 **/
#undef NFCSTATUS_COMMAND_NOT_SUPPORTED

/* taken from Linux_x86/phDal4Nfc.c */
typedef void   (*pphDal4Nfc_DeferFuncPointer_t) (void * );

#include <libnfc-nxp/phHciNfc_Generic.h>
#include <libnfc-nxp/phHal4Nfc.h>
#include <libnfc-nxp/phHal4Nfc_Internal.h>
#include <libnfc-nxp/phFriNfc_SmtCrdFmt.h>
#include <libnfc-nxp/phFriNfc_LlcpUtils.h>
#include <libnfc-nxp/phFriNfc_LlcpTransport.h>
#include <libnfc-nxp/phLlcNfc_DataTypes.h>
#include <libnfc-nxp/phHciNfc_NfcIPMgmt.h>
#include <libnfc-nxp/phHciNfc_WI.h>
#include <libnfc-nxp/phHciNfc_Pipe.h>

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>

#include <hybris/common/binding.h>

HYBRIS_LIBRARY_INITIALIZE(libnfc_so, "libnfc.so");

HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_Mgt_ConfigureDriver, pphLibNfc_sConfig_t, void **);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_Config, pphDal4Nfc_sConfig_t, void **);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_Mgt_UnConfigureDriver, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phDal4Nfc_ConfigRelease, void *);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, NFCSTATUS, phLibNfc_HW_Reset);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phDal4Nfc_Reset, long);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, NFCSTATUS, phLibNfc_Download_Mode);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, NFCSTATUS, phDal4Nfc_Download);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phLibNfc_Load_Firmware_Image);
/* XXX No prototype for exported symbol: dlopen_firmware */
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLibNfc_Mgt_Recovery);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_SetIsoXchgTimeout, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phLibNfc_GetIsoXchgTimeout);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_SetHciTimeout, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phLibNfc_GetHciTimeout);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_SetFelicaTimeout, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phLibNfc_GetFelicaTimeout);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_SetMifareRawTimeout, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phLibNfc_GetMifareRawTimeout);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Mgt_DeInitialize, void *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHal4Nfc_Close, phHal_sHwReference_t *, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_Hal4Reset, phHal_sHwReference_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phOsalNfc_FreeMemory, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLibNfc_Ndef_DeInit);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLibNfc_Pending_Shutdown);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_Mgt_Reset, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_UpdateNextState, pphLibNfc_LibContext_t, phLibNfc_State_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Mgt_Initialize, void *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, void *, phOsalNfc_GetMemory, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHal4Nfc_Open, phHal_sHwReference_t *, phHal4Nfc_InitType_t, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLibNfc_Ndef_Init);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phLibNfc_UpdateCurState, NFCSTATUS, pphLibNfc_LibContext_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHal4Nfc_RegisterNotification, phHal_sHwReference_t *, phHal4Nfc_RegisterType_t, pphHal4Nfc_Notification_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phOsalNfc_RaiseException, phOsalNfc_ExceptionType_t, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_Mgt_GetstackCapabilities, phLibNfc_StackCapabilities_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHal4Nfc_GetDeviceCapabilities, phHal_sHwReference_t *, phHal_sDeviceCapabilities_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Mgt_ConfigureTestMode, void *, pphLibNfc_RspCb_t, phLibNfc_Cfg_Testmode_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phLibNfc_config_discovery_cb, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Mgt_ConfigureDiscovery, phLibNfc_eDiscoveryConfigMode_t, phLibNfc_sADD_Cfg_t, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHal4Nfc_ConfigureDiscovery, phHal_sHwReference_t *, phHal_eDiscoveryConfigMode_t, phHal_sADD_Cfg_t *, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_CheckPresence, phLibNfc_Handle, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHal4Nfc_PresenceCheck, phHal_sHwReference_t *, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHal4Nfc_Transceive, phHal_sHwReference_t *, phHal_sTransceiveInfo_t *, phHal_sRemoteDevInformation_t *, pphHal4Nfc_TransceiveCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHal4Nfc_Connect, phHal_sHwReference_t *, phHal_sRemoteDevInformation_t *, pphHal4Nfc_ConnectCallback_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(libnfc_so, phLibNfc_Reconnect_Mifare_Cb, void *, phHal_sRemoteDevInformation_t *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_NtfRegister, phLibNfc_Registry_Info_t *, phLibNfc_NtfRegister_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_NtfUnregister);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHal4Nfc_UnregisterNotification, phHal_sHwReference_t *, phHal4Nfc_RegisterType_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_ReConnect, phLibNfc_Handle, pphLibNfc_ConnectCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_Connect, phLibNfc_Handle, pphLibNfc_ConnectCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_Disconnect, phLibNfc_Handle, phLibNfc_eReleaseType_t, pphLibNfc_DisconnectCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHal4Nfc_Disconnect, phHal_sHwReference_t *, phHal_sRemoteDevInformation_t *, phHal_eReleaseType_t, pphHal4Nfc_DiscntCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_Transceive, phLibNfc_Handle, phLibNfc_sTransceiveInfo_t *, pphLibNfc_TransceiveCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Mgt_SetP2P_ConfigParams, phLibNfc_sNfcIPCfg_t *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHal4Nfc_ConfigParameters, phHal_sHwReference_t *, phHal_eConfigType_t, phHal_uConfig_t *, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_CloseAll, phFriNfc_LlcpTransport_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Mgt_SetLlcp_ConfigParams, phLibNfc_Llcp_sLinkParameters_t *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_Llcp_EncodeLinkParams, phNfc_sData_t *, phFriNfc_Llcp_sLinkParameters_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION9(libnfc_so, NFCSTATUS, phFriNfc_Llcp_Reset, phFriNfc_Llcp_t *, void *, phFriNfc_Llcp_sLinkParameters_t *, void *, uint16_t, void *, uint16_t, phFriNfc_Llcp_LinkStatus_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Reset, phFriNfc_LlcpTransport_t *, phFriNfc_Llcp_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Llcp_CheckLlcp, phLibNfc_Handle, pphLibNfc_ChkLlcpRspCb_t, pphLibNfc_LlcpLinkStatusCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_Llcp_ChkLlcp, phFriNfc_Llcp_t *, phHal_sRemoteDevInformation_t *, phFriNfc_Llcp_Check_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Activate, phLibNfc_Handle);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Llcp_Activate, phFriNfc_Llcp_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Deactivate, phLibNfc_Handle);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Llcp_Deactivate, phFriNfc_Llcp_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_Llcp_GetLocalInfo, phLibNfc_Handle, phLibNfc_Llcp_sLinkParameters_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_Llcp_GetLocalInfo, phFriNfc_Llcp_t *, phFriNfc_Llcp_sLinkParameters_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_Llcp_GetRemoteInfo, phLibNfc_Handle, phLibNfc_Llcp_sLinkParameters_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_Llcp_GetRemoteInfo, phFriNfc_Llcp_t *, phFriNfc_Llcp_sLinkParameters_t *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phLibNfc_Llcp_DiscoverServices, phLibNfc_Handle, phNfc_sData_t *, uint8_t *, uint8_t, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_DiscoverServices, phFriNfc_LlcpTransport_t *, phNfc_sData_t *, uint8_t *, uint8_t, pphFriNfc_Cr_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Socket, phLibNfc_Llcp_eSocketType_t, phLibNfc_Llcp_sSocketOptions_t *, phNfc_sData_t *, phLibNfc_Handle *, pphLibNfc_LlcpSocketErrCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION7(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Socket, phFriNfc_LlcpTransport_t *, phFriNfc_LlcpTransport_eSocketType_t, phFriNfc_LlcpTransport_sSocketOptions_t *, phNfc_sData_t *, phFriNfc_LlcpTransport_Socket_t **, pphFriNfc_LlcpTransportSocketErrCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Close, phLibNfc_Handle);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Close, phFriNfc_LlcpTransport_Socket_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_Llcp_SocketGetLocalOptions, phLibNfc_Handle, phLibNfc_Llcp_sSocketOptions_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_SocketGetLocalOptions, phFriNfc_LlcpTransport_Socket_t *, phLibNfc_Llcp_sSocketOptions_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Llcp_SocketGetRemoteOptions, phLibNfc_Handle, phLibNfc_Handle, phLibNfc_Llcp_sSocketOptions_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_SocketGetRemoteOptions, phFriNfc_LlcpTransport_Socket_t *, phLibNfc_Llcp_sSocketOptions_t *);
#if (ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR >= 1) || (ANDROID_VERSION_MAJOR >= 5)
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Bind, phLibNfc_Handle, uint8_t, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Bind, phFriNfc_LlcpTransport_Socket_t *, uint8_t, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Listen, phLibNfc_Handle, pphLibNfc_LlcpSocketListenCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Listen, phFriNfc_LlcpTransport_Socket_t *, pphFriNfc_LlcpTransportSocketListenCb_t, void *);
#else
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Bind, phLibNfc_Handle, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Bind, phFriNfc_LlcpTransport_Socket_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Listen, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_LlcpSocketListenCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Listen, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketListenCb_t, void *);
#endif
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Accept, phLibNfc_Handle, phLibNfc_Llcp_sSocketOptions_t *, phNfc_sData_t *, pphLibNfc_LlcpSocketErrCb_t, pphLibNfc_LlcpSocketAcceptCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Accept, phFriNfc_LlcpTransport_Socket_t *, phFriNfc_LlcpTransport_sSocketOptions_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketErrCb_t, pphFriNfc_LlcpTransportSocketAcceptCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Reject, phLibNfc_Handle, phLibNfc_Handle, pphLibNfc_LlcpSocketRejectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Reject, phFriNfc_LlcpTransport_Socket_t *, pphFriNfc_LlcpTransportSocketRejectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Connect, phLibNfc_Handle, phLibNfc_Handle, uint8_t, pphLibNfc_LlcpSocketConnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Connect, phFriNfc_LlcpTransport_Socket_t *, uint8_t, pphFriNfc_LlcpTransportSocketConnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Llcp_ConnectByUri, phLibNfc_Handle, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_LlcpSocketConnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectByUri, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketConnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Disconnect, phLibNfc_Handle, phLibNfc_Handle, pphLibNfc_LlcpSocketDisconnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Disconnect, phFriNfc_LlcpTransport_Socket_t *, pphLibNfc_LlcpSocketDisconnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Recv, phLibNfc_Handle, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_LlcpSocketRecvCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Recv, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketRecvCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Llcp_RecvFrom, phLibNfc_Handle, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_LlcpSocketRecvFromCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_RecvFrom, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketRecvFromCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Llcp_Send, phLibNfc_Handle, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_LlcpSocketSendCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Send, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketSendCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phLibNfc_Llcp_SendTo, phLibNfc_Handle, phLibNfc_Handle, uint8_t, phNfc_sData_t *, pphLibNfc_LlcpSocketSendCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_SendTo, phFriNfc_LlcpTransport_Socket_t *, uint8_t, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketSendCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phLibNfc_Mgt_IoCtl, void *, uint16_t, phNfc_sData_t *, phNfc_sData_t *, pphLibNfc_IoctlCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phHal4Nfc_Ioctl, phHal_sHwReference_t *, uint32_t, phNfc_sData_t *, phNfc_sData_t *, pphHal4Nfc_IoctlCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHal4Nfc_Switch_Swp_Mode, phHal_sHwReference_t *, phHal_eSWP_Mode_t, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_NdefReg_DispatchPacket, phFriNfc_NdefReg_t *, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, uint8_t, phFriNfc_NdefReg_Process, phFriNfc_NdefReg_t *, NFCSTATUS *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_NdefReg_RmCb, phFriNfc_NdefReg_t *, phFriNfc_NdefReg_Cb_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_GetContainerSize, const phFriNfc_NdefMap_t *, uint32_t *, uint32_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phOsalNfc_Timer_Stop, uint32_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phOsalNfc_Timer_Delete, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Ndef_Read, phLibNfc_Handle, phNfc_sData_t *, phLibNfc_Ndef_EOffset_t, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_SetCompletionRoutine, phFriNfc_NdefMap_t *, uint8_t, pphFriNfc_Cr_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_Ndef_Write, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_EraseNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_Ndef_CheckNdef, phLibNfc_Handle, pphLibNfc_ChkNdefRspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION9(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_Reset, phFriNfc_NdefMap_t *, void *, phHal_sRemoteDevInformation_t *, phHal_sDevInputParam_t *, uint8_t *, uint16_t, uint8_t *, uint16_t *, uint16_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phOsalNfc_Timer_Start, uint32_t, uint32_t, ppCallBck_t, void *);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, uint32_t, phOsalNfc_Timer_Create);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_FormatNdef, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_NdefSmtCrd_Reset, phFriNfc_sNdefSmtCrdFmt_t *, void *, phHal_sRemoteDevInformation_t *, phHal_sDevInputParam_t *, uint8_t *, uint16_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_NdefSmtCrd_SetCR, phFriNfc_sNdefSmtCrdFmt_t *, uint8_t, pphFriNfc_Cr_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_NdefSmtCrd_Format, phFriNfc_sNdefSmtCrdFmt_t *, const uint8_t *);
#if (ANDROID_VERSION_MAJOR == 4 && ANDROID_VERSION_MINOR >= 1) || (ANDROID_VERSION_MAJOR >= 5)
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_ConvertToReadOnlyNdef, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_RspCb_t, void *);
#else
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_ConvertToReadOnlyNdef, phLibNfc_Handle, pphLibNfc_RspCb_t, void *);
#endif
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_ConvertToReadOnly, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_NdefSmtCrd_ConvertToReadOnly, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_MifareStdMap_ConvertToReadOnly, phFriNfc_NdefMap_t *, const uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phLibNfc_Ndef_SearchNdefContent, phLibNfc_Handle, phLibNfc_Ndef_SrchType_t *, uint8_t, pphLibNfc_Ndef_Search_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_NdefReg_Reset, phFriNfc_NdefReg_t *, uint8_t **, phFriNfc_NdefRecord_t *, phFriNfc_NdefReg_CbParam_t *, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_NdefReg_AddCb, phFriNfc_NdefReg_t *, phFriNfc_NdefReg_Cb_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_SE_NtfRegister, pphLibNfc_SE_NotificationCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, NFCSTATUS, phLibNfc_SE_NtfUnregister);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLibNfc_SE_GetSecureElementList, phLibNfc_SE_List_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_SE_SetMode, phLibNfc_Handle, phLibNfc_eSE_ActivationMode, pphLibNfc_SE_SetModeRspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHal4Nfc_Switch_SMX_Mode, phHal_sHwReference_t *, phHal_eSmartMX_Mode_t, pphHal4Nfc_GenCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_Receive, phLibNfc_Handle, pphLibNfc_Receive_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHal4Nfc_Receive, phHal_sHwReference_t *, phHal4Nfc_TransactInfo_t *, pphHal4Nfc_ReceiveCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_RemoteDev_Send, phLibNfc_Handle, phNfc_sData_t *, pphLibNfc_RspCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHal4Nfc_Send, phHal_sHwReference_t *, phHal4Nfc_TransactInfo_t *, phNfc_sData_t, pphHal4Nfc_SendCallback_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Configure, void *, void *, phHal_eConfigType_t, phHal_uConfig_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Config_Discovery, void *, void *, phHal_sADD_Cfg_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Restart_Discovery, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(libnfc_so, phHal4Nfc_ConfigureComplete, phHal4Nfc_Hal4Ctxt_t *, void *, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_DisconnectComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_TargetDiscoveryComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Select_Next_Target, void *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_HandleEmulationEvent, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_TransceiveComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_SendCompleteHandler, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_ReactivationComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_PresenceChkComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_ConnectComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_RecvCompleteHandler, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Release, void *, void *, pphNfcIF_Notification_CB_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_HandleP2PDeActivate, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_P2PActivateComplete, phHal4Nfc_Hal4Ctxt_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION7(libnfc_so, NFCSTATUS, phHciNfc_Initialise, void *, void *, phHciNfc_Init_t, phHal_sHwConfig_t *, pphNfcIF_Notification_CB_t, void *, phNfcLayer_sCfg_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phDal4Nfc_Register, phNfcIF_sReference_t *, phNfcIF_sCallBack_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLlcNfc_Register, phNfcIF_sReference_t *, phNfcIF_sCallBack_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_System_Get_Info, void *, void *, uint32_t, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_System_Test, void *, void *, uint32_t, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_System_Configure, void *, void *, uint32_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_PRBS_Test, void *, void *, uint32_t, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phDnldNfc_Upgrade, phHal_sHwReference_t *, pphNfcIF_Notification_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phDnldNfc_Run_Check, phHal_sHwReference_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Switch_SmxMode, void *, void *, phHal_eSmartMX_Mode_t, phHal_sADD_Cfg_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Switch_SwpMode, void *, void *, phHal_eSWP_Mode_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_Data, void *, void *, phHal_sRemoteDevInformation_t *, phHciNfc_XchgInfo_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHal4Nfc_Disconnect_Execute, phHal_sHwReference_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHal4Nfc_Felica_RePoll, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Reactivate, void *, void *, phHal_sRemoteDevInformation_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, int, phOsalNfc_MemCompare, void *, void *, unsigned int);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Connect, void *, void *, phHal_sRemoteDevInformation_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Exchange_Data, void *, void *, phHal_sRemoteDevInformation_t *, phHciNfc_XchgInfo_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Disconnect, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Presence_Check, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_Unregister, void *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(libnfc_so, phHciNfc_Build_HCPFrame, phHciNfc_HCP_Packet_t *, uint8_t, uint8_t, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Send_HCP, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Update_PipeInfo, phHciNfc_sContext_t *, phHciNfc_PipeMgmt_Seq_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Admin_Release, phHciNfc_sContext_t *, void *, phHciNfc_HostID_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Close_Pipe, phHciNfc_sContext_t *, void *, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHciNfc_Send_Admin_Event, phHciNfc_sContext_t *, void *, uint8_t, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phHciNfc_Send_Admin_Cmd, phHciNfc_sContext_t *, void *, uint8_t, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Admin_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Allocate_Resource, void **, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Create_All_Pipes, phHciNfc_sContext_t *, void *, phHciNfc_PipeMgmt_Seq_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Open_Pipe, phHciNfc_sContext_t *, void *, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_Generic_Cmd, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phHciNfc_Set_Param, phHciNfc_sContext_t *, void *, phHciNfc_Pipe_Info_t *, uint8_t, void *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Update_Pipe, phHciNfc_sContext_t *, void *, phHciNfc_PipeMgmt_Seq_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_FSM_Update, phHciNfc_sContext_t *, phHciNfc_eState_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phHciNfc_FSM_Rollback, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(libnfc_so, phHciNfc_Send_Complete, void *, void *, phNfc_sTransactionInfo_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(libnfc_so, phHciNfc_Receive_Complete, void *, void *, phNfc_sTransactionInfo_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phHciNfc_Notify_Event, void *, void *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Release_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHciNfc_Release_Lower, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phHciNfc_Release_Resources, phHciNfc_sContext_t **);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetATRInfo, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t, phHal_sNfcIPCfg_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_SWP_Protection, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_SWP_Update_Sequence, phHciNfc_sContext_t *, phHciNfc_eSeqType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_EmulationCfg_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Emulation_Cfg, phHciNfc_sContext_t *, void *, phHciNfc_eConfigType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Deselect, phHciNfc_sContext_t *, void *, phHal_eRemDevType_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_SWP_Configure_Mode, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_SmartMx_Mode_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Activate_Next, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Select, phHciNfc_sContext_t *, void *, phHal_eRemDevType_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Reactivate, phHciNfc_sContext_t *, void *, phHal_eRemDevType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Disconnect_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Exchange_Data, phHciNfc_sContext_t *, void *, phHciNfc_XchgInfo_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_Send_Data, phHciNfc_sContext_t *, void *, phHciNfc_XchgInfo_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Presence_Check, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Test, void *, void *, uint8_t, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Get_Info, phHciNfc_sContext_t *, void *, uint16_t, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Configure, phHciNfc_sContext_t *, void *, uint16_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Get_Link_Status, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_LinkMgmt_Open, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phHciNfc_Append_HCPFrame, uint8_t *, uint16_t, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Get_Test_Result, phHciNfc_sContext_t *, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Set_Test_Result, phHciNfc_sContext_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Release, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_DevMgmt_Update_Sequence, phHciNfc_sContext_t *, phHciNfc_eSeqType_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phHciNfc_Uicc_Connectivity, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Uicc_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Uicc_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_EmuMgmt_Update_Seq, phHciNfc_sContext_t *, phHciNfc_eSeqType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Uicc_Connect_Status, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_SWP_Get_Status, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_EmuMgmt_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_WI_Configure_Mode, void *, void *, phHal_eSmartMX_Mode_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetMode, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_SWP_Get_Bitrate, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetMergeSak, phHciNfc_sContext_t *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_SWP_Configure_Default, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_WI_Configure_Default, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_WI_Configure_Notifications, void *, void *, phHciNfc_WI_Events_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_EmuMgmt_Release, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_SWP_Config_Sequence, phHciNfc_sContext_t *, void *, phHal_sEmulationCfg_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Update_Sequence, phHciNfc_sContext_t *, phHciNfc_eSeqType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Felica_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_Felica_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Felica_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Felica_Update_Info, phHciNfc_sContext_t *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Felica_Info_Sequence, void *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phHciNfc_Tag_Notify, phHciNfc_sContext_t *, void *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_Felica_Command, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Felica_Request_Mode, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Receive, void *, void *, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Resume_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(libnfc_so, phHciNfc_Error_Sequence, void *, void *, NFCSTATUS, void *, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(libnfc_so, phHciNfc_Notify, pphNfcIF_Notification_CB_t, void *, void *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_FSM_Complete, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phHciNfc_Target_Select_Notify, phHciNfc_sContext_t *, void *, uint8_t, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phHciNfc_Release_Notify, phHciNfc_sContext_t *, void *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Update_Sequence, phHciNfc_sContext_t *, phHciNfc_eSeqType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Info_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Release, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_IDMgmt_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_ISO15693_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ISO15693_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ISO15693_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ISO15693_Update_Info, phHciNfc_sContext_t *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ISO15693_Info_Sequence, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_ISO15693_Command, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ISO15693_Set_AFI, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Jewel_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_Jewel_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Jewel_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Jewel_Update_Info, phHciNfc_sContext_t *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_Jewel_Command, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Jewel_Info_Sequence, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Jewel_GetRID, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_LinkMgmt_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_LinkMgmt_Release, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_Initiator_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Initiator_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Initiator_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_Presence_Check, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_Target_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Target_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Target_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_Info_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetNAD, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetDID, phHciNfc_sContext_t *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetOptions, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetPSL1, phHciNfc_sContext_t *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_SetPSL2, phHciNfc_sContext_t *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_GetStatus, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_GetParam, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Initiator_Cont_Activate, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_NfcIP_GetATRInfo, phHciNfc_sContext_t *, void *, phHciNfc_eNfcIPType_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Delete_Pipe, phHciNfc_sContext_t *, void *, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_SWP_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phHciNfc_WI_Init_Resources, phHciNfc_sContext_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_Delete_All_Pipes, phHciNfc_sContext_t *, void *, phHciNfc_PipeMgmt_Seq_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_WI_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_SWP_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Update_PipeInfo, phHciNfc_sContext_t *, uint8_t, phHciNfc_Pipe_Info_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Initialise, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Release, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_PollLoop_Cfg, void *, void *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Info_Sequence, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Auto_Activate, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Set_DataRateMax, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_ReaderA_Command, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Cont_Activate, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Update_Info, phHciNfc_sContext_t *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_App_Data, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderA_Fwi_Sfgt, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Update_Info, phHciNfc_sContext_t *, uint8_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Info_Sequence, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Set_AFI, void *, void *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderB_Set_LayerData, void *, void *, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Info_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Release, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_RFReader_Event, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Disable_Discovery, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_Enable_Discovery, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phHciNfc_Send_RFReader_Command, phHciNfc_sContext_t *, void *, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_ReaderMgmt_UICC_Dispatch, phHciNfc_sContext_t *, void *, phHal_eRemDevType_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phHciNfc_FSM_Validate, phHciNfc_sContext_t *, phHciNfc_eState_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Initialise_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_Connect_Sequence, phHciNfc_sContext_t *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_SWP_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_WI_Get_PipeID, phHciNfc_sContext_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phHciNfc_WI_Get_Default, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLlcNfc_Release, void *, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phLlcNfc_TimerUnInit, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phLlcNfc_H_Frame_DeInit, phLlcNfc_Frame_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLlcNfc_Interface_Read, phLlcNfc_Context_t *, uint8_t, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLlcNfc_H_CreateIFramePayload, phLlcNfc_Frame_t *, phLlcNfc_LlcPacket_t *, uint8_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLlcNfc_H_StoreIFrame, phLlcNfc_StoreIFrame_t *, phLlcNfc_LlcPacket_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLlcNfc_Interface_Write, phLlcNfc_Context_t *, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLlcNfc_StartTimers, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phLlcNfc_H_Frame_Init, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLlcNfc_Interface_Init, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLlcNfc_TimerInit, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLlcNfc_CreateTimers);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLlcNfc_H_CreateUFramePayload, phLlcNfc_Context_t *, phLlcNfc_LlcPacket_t *, uint8_t *, phLlcNfc_LlcCmd_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLlcNfc_Interface_Register, phLlcNfc_Context_t *, phNfcLayer_sCfg_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phLlcNfc_H_ComputeCrc, uint8_t *, uint8_t, uint8_t *, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLlcNfc_H_SendTimedOutIFrame, phLlcNfc_Context_t *, phLlcNfc_StoreIFrame_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLlcNfc_H_SendUserIFrame, phLlcNfc_Context_t *, phLlcNfc_StoreIFrame_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLlcNfc_H_SendRejectedIFrame, phLlcNfc_Context_t *, phLlcNfc_StoreIFrame_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLlcNfc_H_CreateSFramePayload, phLlcNfc_Frame_t *, phLlcNfc_LlcPacket_t *, phLlcNfc_LlcCmd_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLlcNfc_H_SendRSETFrame, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLlcNfc_H_WriteWaitCall, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phLlcNfc_H_ProcessIFrame, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phLlcNfc_StopTimers, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phLlcNfc_H_SendInfo, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phLlcNfc_H_ProRecvFrame, phLlcNfc_Context_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLlcNfc_StopAllTimers);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phLlcNfc_H_ChangeState, phLlcNfc_Context_t *, phLlcNfc_State_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phLlcNfc_DeleteTimer);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, uint32_t, phFriNfc_Llcp_Header2Buffer, phFriNfc_Llcp_sPacketHeader_t *, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, uint32_t, phFriNfc_Llcp_Sequence2Buffer, phFriNfc_Llcp_sPacketSequence_t *, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_Send, phFriNfc_LlcpMac_t *, phNfc_sData_t *, phFriNfc_LlcpMac_Send_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_Deactivate, phFriNfc_LlcpMac_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, uint32_t, phFriNfc_Llcp_Buffer2Header, uint8_t *, uint32_t, phFriNfc_Llcp_sPacketHeader_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_Llcp_DecodeTLV, phNfc_sData_t *, uint32_t *, uint8_t *, phNfc_sData_t *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_Llcp_EncodeTLV, phNfc_sData_t *, uint32_t *, uint8_t, uint8_t, uint8_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_Receive, phFriNfc_LlcpMac_t *, phNfc_sData_t *, phFriNfc_LlcpMac_Reveive_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_Reset, phFriNfc_LlcpMac_t *, void *, phFriNfc_LlcpMac_LinkStatus_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_ChkLlcp, phFriNfc_LlcpMac_t *, phHal_sRemoteDevInformation_t *, phFriNfc_LlcpMac_Chk_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_Activate, phFriNfc_LlcpMac_t *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_Llcp_Send, phFriNfc_Llcp_t *, phFriNfc_Llcp_sPacketHeader_t *, phFriNfc_Llcp_sPacketSequence_t *, phNfc_sData_t *, phFriNfc_Llcp_Send_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_Llcp_Recv, phFriNfc_Llcp_t *, phFriNfc_Llcp_Recv_CB_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_Llcp_AppendTLV, phNfc_sData_t *, uint32_t, uint32_t *, uint8_t, uint8_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_Llcp_EncodeMIUX, uint16_t, uint8_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_Llcp_EncodeRW, uint8_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(libnfc_so, phFriNfc_Llcp_CyclicFifoInit, P_UTIL_FIFO_BUFFER, const uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_Llcp_CyclicFifoClear, P_UTIL_FIFO_BUFFER);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, uint32_t, phFriNfc_Llcp_CyclicFifoWrite, P_UTIL_FIFO_BUFFER, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, uint32_t, phFriNfc_Llcp_CyclicFifoFifoRead, P_UTIL_FIFO_BUFFER, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, uint32_t, phFriNfc_Llcp_CyclicFifoUsage, P_UTIL_FIFO_BUFFER);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, uint32_t, phFriNfc_Llcp_CyclicFifoAvailable, P_UTIL_FIFO_BUFFER);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, uint32_t, phFriNfc_Llcp_Buffer2Sequence, uint8_t *, uint32_t, phFriNfc_Llcp_sPacketSequence_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION5(libnfc_so, Handle_ConnectionOriented_IncommingFrame, phFriNfc_LlcpTransport_t *, phNfc_sData_t *, uint8_t, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, Handle_Connectionless_IncommingFrame, phFriNfc_LlcpTransport_t *, phNfc_sData_t *, uint8_t, uint8_t);
#if ANDROID_VERSION_MAJOR>=4 && ANDROID_VERSION_MINOR>=3 || ANDROID_VERSION_MAJOR >= 5
/* see libnfc-nxp commit 7c4b4fad -- since Android 4.3 */
HYBRIS_IMPLEMENT_FUNCTION7(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_LinkSend, phFriNfc_LlcpTransport_t *, phFriNfc_Llcp_sPacketHeader_t *, phFriNfc_Llcp_sPacketSequence_t *, phNfc_sData_t *, phFriNfc_Llcp_LinkSend_CB_t, uint8_t, void *);
#else
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_LinkSend, phFriNfc_LlcpTransport_t *, phFriNfc_Llcp_sPacketHeader_t *, phFriNfc_Llcp_sPacketSequence_t *, phNfc_sData_t *, phFriNfc_Llcp_Send_CB_t, void *);
#endif
HYBRIS_IMPLEMENT_FUNCTION13(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_SendFrameReject, phFriNfc_LlcpTransport_t *, uint8_t, uint8_t, uint8_t, phFriNfc_Llcp_sPacketSequence_t *, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_Close, phFriNfc_LlcpTransport_Socket_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_SendDisconnectMode, phFriNfc_LlcpTransport_t *, uint8_t, uint8_t, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_HandlePendingOperations, phFriNfc_LlcpTransport_Socket_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Connectionless_HandlePendingOperations, phFriNfc_LlcpTransport_Socket_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_SocketGetLocalOptions, phFriNfc_LlcpTransport_Socket_t *, phLibNfc_Llcp_sSocketOptions_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_SocketGetRemoteOptions, phFriNfc_LlcpTransport_Socket_t *, phLibNfc_Llcp_sSocketOptions_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Connectionless_Close, phFriNfc_LlcpTransport_Socket_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_Listen, phFriNfc_LlcpTransport_Socket_t *, pphFriNfc_LlcpTransportSocketListenCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_Accept, phFriNfc_LlcpTransport_Socket_t *, phFriNfc_LlcpTransport_sSocketOptions_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketErrCb_t, pphFriNfc_LlcpTransportSocketAcceptCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_LlcpTransport_ConnectionOriented_Reject, phFriNfc_LlcpTransport_Socket_t *, pphFriNfc_LlcpTransportSocketRejectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_Connect, phFriNfc_LlcpTransport_Socket_t *, uint8_t, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketConnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phLibNfc_LlcpTransport_ConnectionOriented_Disconnect, phFriNfc_LlcpTransport_Socket_t *, pphLibNfc_LlcpSocketDisconnectCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_Send, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketSendCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_ConnectionOriented_Recv, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketRecvCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_LlcpTransport_Connectionless_SendTo, phFriNfc_LlcpTransport_Socket_t *, uint8_t, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketSendCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phLibNfc_LlcpTransport_Connectionless_RecvFrom, phFriNfc_LlcpTransport_Socket_t *, phNfc_sData_t *, pphFriNfc_LlcpTransportSocketRecvFromCb_t, void *);
HYBRIS_IMPLEMENT_FUNCTION6(libnfc_so, NFCSTATUS, phFriNfc_LlcpConnTransport_Send, phFriNfc_Llcp_t *, phFriNfc_Llcp_sPacketHeader_t *, phFriNfc_Llcp_sPacketSequence_t *, phNfc_sData_t *, phFriNfc_Llcp_Send_CB_t, phFriNfc_LlcpTransport_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_LlcpMac_Nfcip_Register, phFriNfc_LlcpMac_t *);
HYBRIS_IMPLEMENT_FUNCTION9(libnfc_so, NFCSTATUS, phFriNfc_OvrHal_Transceive, phFriNfc_OvrHal_t *, phFriNfc_CplRt_t *, phHal_sRemoteDevInformation_t *, phHal_uCmdList_t, phHal_sDepAdditionalInfo_t *, uint8_t *, uint16_t, uint8_t *, uint16_t *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_OvrHal_Receive, phFriNfc_OvrHal_t *, phFriNfc_CplRt_t *, phHal_sRemoteDevInformation_t *, uint8_t *, uint16_t *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, NFCSTATUS, phFriNfc_OvrHal_Send, phFriNfc_OvrHal_t *, phFriNfc_CplRt_t *, phHal_sRemoteDevInformation_t *, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_NdefMap_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_Felica_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_MapTool_ChkSpcVer, const phFriNfc_NdefMap_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_MapTool_SetCardState, phFriNfc_NdefMap_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_Felica_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_Felica_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Felica_EraseNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Felica_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_MifareStdMap_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_MifareStdMap_H_Reset, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_MifareStdMap_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, NFCSTATUS, phFriNfc_OvrHal_Reconnect, phFriNfc_OvrHal_t *, phFriNfc_CplRt_t *, phHal_sRemoteDevInformation_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_OvrHal_Connect, phFriNfc_OvrHal_t *, phFriNfc_CplRt_t *, phHal_sRemoteDevInformation_t *, phHal_sDevInputParam_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_MifareStdMap_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_MifareStdMap_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_MifareUL_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_MifareUL_H_Reset, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_MifareUL_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_MifareUL_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_MifareUL_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_TopazMap_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_TopazMap_H_Reset, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_TopazMap_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_TopazMap_ConvertToReadOnly, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_TopazMap_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_TopazMap_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_Tpz_H_ChkSpcVer, phFriNfc_NdefMap_t *, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_TopazDynamicMap_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_TopazDynamicMap_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_TopazDynamicMap_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_TopazDynamicMap_ConvertToReadOnly, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_TopazDynamicMap_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_Desfire_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_Desfire_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_Desfire_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Desfire_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_ISO15693_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_ISO15693_ChkNdef, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_ISO15693_RdNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phFriNfc_ISO15693_WrNdef, phFriNfc_NdefMap_t *, uint8_t *, uint32_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_ISO15693_ConvertToReadOnly, phFriNfc_NdefMap_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_SetCardState, phFriNfc_NdefMap_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_NdefMap_CheckSpecVersion, phFriNfc_NdefMap_t *, uint8_t);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, int16_t, phFriNfc_NdefReg_Strnicmp, const int8_t *, const int8_t *, uint32_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_NdefReg_DispatchRecord, phFriNfc_NdefReg_t *, phFriNfc_NdefRecord_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_NdefSmtCrd_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_Desfire_Reset, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Desfire_Format, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_Desfire_ConvertToReadOnly, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_Desf_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_SmtCrdFmt_HCrHandler, phFriNfc_sNdefSmtCrdFmt_t *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_MfUL_Reset, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_MfUL_Format, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_MfUL_ConvertToReadOnly, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_MfUL_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_MfStd_Reset, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phFriNfc_MfStd_Format, phFriNfc_sNdefSmtCrdFmt_t *, const uint8_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_MfStd_Process, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phFriNfc_ISO15693_FmtReset, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, NFCSTATUS, phFriNfc_ISO15693_Format, phFriNfc_sNdefSmtCrdFmt_t *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phFriNfc_ISO15693_FmtProcess, void *, NFCSTATUS);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phOsalNfc_Timer_DeferredCall, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, int, phDal4Nfc_msgsnd, int, void *, size_t, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phOsalNfc_DbgString, const char *);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phOsalNfc_DbgTrace, uint8_t *, uint32_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION4(libnfc_so, phOsalNfc_PrintData, const char *, uint32_t, uint8_t *, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phDal4Nfc_uart_initialize);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phDal4Nfc_uart_set_open_from_handle, phHal_sHwReference_t *);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phDal4Nfc_uart_is_opened);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phDal4Nfc_uart_flush);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phDal4Nfc_uart_close);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_uart_open_and_configure, pphDal4Nfc_sConfig_t, void **);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, int, phDal4Nfc_uart_read, uint8_t *, int);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, int, phDal4Nfc_uart_write, uint8_t *, int);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, int, phDal4Nfc_uart_reset, long);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_Shutdown, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phDal4Nfc_ReadWait, void *, void *, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_ReadWaitCancel, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phDal4Nfc_Read, void *, void *, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_Init, void *, void *);
HYBRIS_IMPLEMENT_FUNCTION4(libnfc_so, NFCSTATUS, phDal4Nfc_Write, void *, void *, uint8_t *, uint16_t);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, int, phDal4Nfc_ReaderThread, void *);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phDal4Nfc_i2c_initialize);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(libnfc_so, phDal4Nfc_i2c_set_open_from_handle, phHal_sHwReference_t *);
HYBRIS_IMPLEMENT_FUNCTION0(libnfc_so, int, phDal4Nfc_i2c_is_opened);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phDal4Nfc_i2c_flush);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(libnfc_so, phDal4Nfc_i2c_close);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, NFCSTATUS, phDal4Nfc_i2c_open_and_configure, pphDal4Nfc_sConfig_t, void **);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, int, phDal4Nfc_i2c_read, uint8_t *, int);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, int, phDal4Nfc_i2c_write, uint8_t *, int);
HYBRIS_IMPLEMENT_FUNCTION1(libnfc_so, int, phDal4Nfc_i2c_reset, long);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(libnfc_so, phDal4Nfc_DeferredCall, pphDal4Nfc_DeferFuncPointer_t, void *);
HYBRIS_IMPLEMENT_FUNCTION2(libnfc_so, int, phDal4Nfc_msgget, key_t, int);
HYBRIS_IMPLEMENT_FUNCTION3(libnfc_so, int, phDal4Nfc_msgctl, int, int, void *);
HYBRIS_IMPLEMENT_FUNCTION5(libnfc_so, int, phDal4Nfc_msgrcv, int, void *, size_t, long, int);
/* XXX No prototype for exported symbol: __on_dlclose */
#endif

