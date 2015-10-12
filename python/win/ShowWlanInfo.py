#  encoding:utf-8
#   Base on python 2.7.9
from ctypes import *
from ctypes.wintypes import *
from sys import exit
def customresize(array, new_size):
    return (array._type_ * new_size).from_address(addressof(array))
def is_chinese(uchar):
    if uchar >= u'\u4e00' and uchar <= u'\u9fa5':
        return True
    else:
        return False
 
def fill_text_to_print_width(text, width):
    stext = str(text)
    utext = stext.decode("utf-8")
    cn_count = 0
    for u in utext:
        if is_chinese(u):
            cn_count += 1
    return utext + " " * (width - cn_count - len(utext)) 

wlanapi = windll.LoadLibrary('wlanapi.dll')
ERROR_SUCCESS = 0
class GUID(Structure):
    _fields_ = [
        ('Data1', c_ulong),
        ('Data2', c_ushort),
        ('Data3', c_ushort),
        ('Data4', c_ubyte * 8),
        ]
WLAN_INTERFACE_STATE = c_uint
(wlan_interface_state_not_ready,
    wlan_interface_state_connected,
    wlan_interface_state_ad_hoc_network_formed,
    wlan_interface_state_disconnecting,
    wlan_interface_state_disconnected,
    wlan_interface_state_associating,
    wlan_interface_state_discovering,
    wlan_interface_state_authenticating) = map(WLAN_INTERFACE_STATE, range(0, 8))
class WLAN_INTERFACE_INFO(Structure):
    _fields_ = [
        ("InterfaceGuid", GUID),
        ("strInterfaceDescription", c_wchar * 256),
        ("isState", WLAN_INTERFACE_STATE)
        ]
class WLAN_INTERFACE_INFO_LIST(Structure):
    _fields_ = [
        ("NumberOfItems", DWORD),
        ("Index", DWORD),
        ("InterfaceInfo", WLAN_INTERFACE_INFO * 1)
        ]
WLAN_MAX_PHY_TYPE_NUMBER = 0x8
DOT11_SSID_MAX_LENGTH = 32
WLAN_REASON_CODE = DWORD
DOT11_BSS_TYPE = c_uint
(dot11_BSS_type_infrastructure,
    dot11_BSS_type_independent,
    dot11_BSS_type_any) = map(DOT11_BSS_TYPE, range(1, 4))
DOT11_PHY_TYPE = c_uint
dot11_phy_type_unknown = 0
dot11_phy_type_any = 0
dot11_phy_type_fhss = 1
dot11_phy_type_dsss = 2
dot11_phy_type_irbaseband = 3
dot11_phy_type_ofdm = 4
dot11_phy_type_hrdsss = 5
dot11_phy_type_erp = 6
dot11_phy_type_ht = 7
dot11_phy_type_IHV_start = 0x80000000
dot11_phy_type_IHV_end = 0xffffffff    
DOT11_AUTH_ALGORITHM = c_uint
DOT11_AUTH_ALGO_80211_OPEN = 1
DOT11_AUTH_ALGO_80211_SHARED_KEY = 2
DOT11_AUTH_ALGO_WPA = 3
DOT11_AUTH_ALGO_WPA_PSK = 4
DOT11_AUTH_ALGO_WPA_NONE = 5
DOT11_AUTH_ALGO_RSNA = 6
DOT11_AUTH_ALGO_RSNA_PSK = 7
DOT11_AUTH_ALGO_IHV_START = 0x80000000
DOT11_AUTH_ALGO_IHV_END = 0xffffffff
DOT11_CIPHER_ALGORITHM = c_uint
DOT11_CIPHER_ALGO_NONE = 0x00
DOT11_CIPHER_ALGO_WEP40 = 0x01
DOT11_CIPHER_ALGO_TKIP = 0x02
DOT11_CIPHER_ALGO_CCMP = 0x04
DOT11_CIPHER_ALGO_WEP104 = 0x05
DOT11_CIPHER_ALGO_WPA_USE_GROUP = 0x100
DOT11_CIPHER_ALGO_RSN_USE_GROUP = 0x100
DOT11_CIPHER_ALGO_WEP = 0x101
DOT11_CIPHER_ALGO_IHV_START = 0x80000000
DOT11_CIPHER_ALGO_IHV_END = 0xffffffff    
WLAN_AVAILABLE_NETWORK_CONNECTED = 1
WLAN_AVAILABLE_NETWORK_HAS_PROFILE = 2
WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_ADHOC_PROFILES = 0x00000001
WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES = 0x00000002


def print_auth(auth):
    auth_msg = {
         DOT11_AUTH_ALGO_80211_OPEN : "OPEN",
         DOT11_AUTH_ALGO_80211_SHARED_KEY :"SHARED_KEY",
         DOT11_AUTH_ALGO_WPA : "WPA",
         DOT11_AUTH_ALGO_WPA_PSK  : "WPA_PSK",
         DOT11_AUTH_ALGO_WPA_NONE  : "WPA_NONE",
         DOT11_AUTH_ALGO_RSNA  : "RSNA",
         DOT11_AUTH_ALGO_RSNA_PSK  : "RSNA_PSK",
    }
    return auth_msg.get(auth)
        
def freq_2_chn(freq):
    if (freq >= 5000000):
        chn = (freq % 5000000) / 1000
        chn = chn / 5
    else:
        chn = (freq % 2412000) / 1000
        chn = chn / 5

    return chn + 1

class DOT11_SSID(Structure):
    _fields_ = [
        ("SSIDLength", c_ulong),
        ("SSID", c_char * DOT11_SSID_MAX_LENGTH)
        ]
class WLAN_AVAILABLE_NETWORK(Structure):
    _fields_ = [
        ("ProfileName", c_wchar * 256),
        ("dot11Ssid", DOT11_SSID),
        ("dot11BssType", DOT11_BSS_TYPE),
        ("NumberOfBssids", c_ulong),
        ("NetworkConnectable", c_bool),
        ("wlanNotConnectableReason", WLAN_REASON_CODE),
        ("NumberOfPhyTypes", c_ulong),
        ("dot11PhyTypes", DOT11_PHY_TYPE * WLAN_MAX_PHY_TYPE_NUMBER),
        ("MorePhyTypes", c_bool),
        ("wlanSignalQuality", c_ulong),
        ("SecurityEnabled", c_bool),
        ("dot11DefaultAuthAlgorithm", DOT11_AUTH_ALGORITHM),
        ("dot11DefaultCipherAlgorithm", DOT11_CIPHER_ALGORITHM),
        ("Flags", DWORD),
        ("Reserved", DWORD)
        ]
class WLAN_AVAILABLE_NETWORK_LIST(Structure):
    _fields_ = [
        ("NumberOfItems", DWORD),
        ("Index", DWORD),
        ("Network", WLAN_AVAILABLE_NETWORK * 1)
        ]

DOT11_MAC_ADDRESS = c_ubyte * 6
DOT11_CIPHER_ALGORITHM = c_uint
DOT11_CIPHER_ALGO_NONE   = 0x00
DOT11_CIPHER_ALGO_WEP40   = 0x01
DOT11_CIPHER_ALGO_TKIP   = 0x02
DOT11_PHY_TYPE = c_uint
DOT11_PHY_TYPE_UNKNOWN = 0
DOT11_PHY_TYPE_ANY   = 0
DOT11_PHY_TYPE_FHSS  = 1
DOT11_PHY_TYPE_DSSS  = 2
DOT11_PHY_TYPE_IRBASEBAND = 3
DOT11_PHY_TYPE_OFDM  = 4
DOT11_PHY_TYPE_HRDSSS  = 5
DOT11_PHY_TYPE_ERP   = 6
DOT11_PHY_TYPE_HT   = 7
DOT11_PHY_TYPE_IHV_START = 0X80000000
DOT11_PHY_TYPE_IHV_END  = 0XFFFFFFFF
class WLAN_RATE_SET(Structure):
    _fields_ = [
        ("uRateSetLength", c_ulong),
        ("usRateSet", c_ushort * 126)
]
class WLAN_BSS_ENTRY(Structure):
    _fields_ = [
        ("dot11Ssid",DOT11_SSID),
        ("uPhyId",c_ulong),
        ("dot11Bssid", DOT11_MAC_ADDRESS),
        ("dot11BssType", DOT11_BSS_TYPE),
        ("dot11BssPhyType", DOT11_PHY_TYPE),
        ("lRssi", c_long),
        ("uLinkQuality", c_ulong),
        ("bInRegDomain", c_bool),
        ("usBeaconPeriod",c_ushort),
        ("ullTimestamp", c_ulonglong),
        ("ullHostTimestamp",c_ulonglong),
        ("usCapabilityInformation",c_ushort),
        ("ulChCenterFrequency", c_ulong),
        ("wlanRateSet",WLAN_RATE_SET),
        ("ulIeOffset", c_ulong),
        ("ulIeSize", c_ulong)]
class WLAN_BSS_LIST(Structure):
    _fields_ = [
        ("TotalSize", DWORD),
        ("NumberOfItems", DWORD),
        ("NetworkBSS", WLAN_BSS_ENTRY * 1)
 ]
class WLAN_AVAILABLE_NETWORK_LIST_BSS(Structure):
    _fields_ = [
        ("TotalSize", DWORD),
        ("NumberOfItems", DWORD),
        ("Network", WLAN_BSS_ENTRY * 1)
 ]
WlanOpenHandle = wlanapi.WlanOpenHandle
WlanOpenHandle.argtypes = (DWORD, c_void_p, POINTER(DWORD), POINTER(HANDLE))
WlanOpenHandle.restype = DWORD
WlanEnumInterfaces = wlanapi.WlanEnumInterfaces
WlanEnumInterfaces.argtypes = (HANDLE, c_void_p,
                                POINTER(POINTER(WLAN_INTERFACE_INFO_LIST)))
WlanEnumInterfaces.restype = DWORD
WlanGetAvailableNetworkList = wlanapi.WlanGetAvailableNetworkList
WlanGetAvailableNetworkList.argtypes = (HANDLE, POINTER(GUID), DWORD, c_void_p,
                                        POINTER(POINTER(WLAN_AVAILABLE_NETWORK_LIST)))
WlanGetAvailableNetworkList.restype = DWORD
WlanGetNetworkBssList = wlanapi.WlanGetNetworkBssList
WlanGetNetworkBssList.argtypes = (HANDLE, POINTER(GUID), POINTER(DOT11_SSID), DOT11_BSS_TYPE, c_bool, c_void_p,
                                  POINTER(POINTER(WLAN_BSS_LIST)))
WlanGetNetworkBssList.restype = DWORD
WlanFreeMemory = wlanapi.WlanFreeMemory
WlanFreeMemory.argtypes = [c_void_p]
if __name__ == '__main__':
    NegotiatedVersion = DWORD()
    ClientHandle = HANDLE()
    ret = WlanOpenHandle(1, None, byref(NegotiatedVersion), byref(ClientHandle))
    if ret != ERROR_SUCCESS:
        exit(FormatError(ret))
    #find all wireless network interfaces
    pInterfaceList = pointer(WLAN_INTERFACE_INFO_LIST())
    ret = WlanEnumInterfaces(ClientHandle, None, byref(pInterfaceList))
    if ret != ERROR_SUCCESS:
        exit(FormatError(ret))
    try:
        ifaces = customresize(pInterfaceList.contents.InterfaceInfo,
                                pInterfaceList.contents.NumberOfItems)
        #find each available network for each interface
        for iface in ifaces:
            print( "Interface: %s" % (iface.strInterfaceDescription))
            pAvailableNetworkList = pointer(WLAN_AVAILABLE_NETWORK_LIST())
            ret = WlanGetAvailableNetworkList(ClientHandle,
                                        byref(iface.InterfaceGuid),
                                        0,
                                        None,
                                        byref(pAvailableNetworkList))
            if ret != ERROR_SUCCESS:
                exit(FormatError(ret))
            try:
                avail_net_list = pAvailableNetworkList.contents
                networks = customresize(avail_net_list.Network,
                                        avail_net_list.NumberOfItems)
                print ("%-33s%-10s%-7s%-9s%-11s%-12s%-18s" % ("[SSID]","[Quality]","[RSSI]","[Freq]","[Channel]","[Auth]","[BSSID]"))
                for network in networks:
                    pAvailableNetworkList2 = pointer(WLAN_BSS_LIST())
                    ret2 = WlanGetNetworkBssList(ClientHandle,
                                                 byref(iface.InterfaceGuid), 
                                                 network.dot11Ssid,
                                                 network.dot11BssType,
                                                 True,
                                                 None,
                                                 byref(pAvailableNetworkList2))
                    if ret2 != ERROR_SUCCESS:
                            exit(FormatError(ret2))
                    avail_net_list2 = pAvailableNetworkList2.contents
                    networks2 = customresize(avail_net_list2.NetworkBSS,
                                             avail_net_list2.NumberOfItems)
                    for network1 in networks2:
                        Bssid   = ':'.join('%02x' % b for b in network1.dot11Bssid).upper()
                        Rssi    = str(network1.lRssi)
                        Freq    = network1.ulChCenterFrequency
                    print ("%s%-10d%-7s%-9d%-11s%-12s%-18s" % (
                       fill_text_to_print_width(network.dot11Ssid.SSID[:network.dot11Ssid.SSIDLength],33),
                       network.wlanSignalQuality,
                       Rssi,
                       Freq,
                       freq_2_chn(Freq),
                       print_auth(network.dot11DefaultAuthAlgorithm),
                       Bssid))
                    
            finally:
                WlanFreeMemory(pAvailableNetworkList)
    finally:
        WlanFreeMemory(pInterfaceList)
