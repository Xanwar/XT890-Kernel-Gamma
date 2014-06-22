#ifndef COMPAT_AUTOCONF_INCLUDED
#define COMPAT_AUTOCONF_INCLUDED
/*
 * Automatically generated C config: don't edit
 * Sat Jun 21 22:29:06 CDT 2014 
 * compat-wireless-2.6: compat-wireless-2011-05-19-2-gfd8090a
 * linux-2.6: v2.6.39-1290-g7d5a6a7
 */
#define COMPAT_RELEASE "compat-wireless-2011-05-19-2-gfd8090a"
#define COMPAT_KERNEL_RELEASE "v2.6.39-1290-g7d5a6a7"
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
#error Compat-wireless requirement: Linux >= 2,6,24
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24) */ 
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
#endif /* CONFIG_COMPAT_KERNEL_2_6_24 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27))
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,33))
#if (!defined(RHEL_MAJOR) || RHEL_MAJOR != 6 || RHEL_MINOR < 1)
#if defined(CONFIG_FW_LOADER) || defined(CONFIG_FW_LOADER_MODULE)
#endif /* CONFIG_FW_LOADER */
#endif /* CONFIG_COMPAT_RHEL_6_1 */
#endif /* CONFIG_COMPAT_KERNEL_2_6_33 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36))
#if (!defined(RHEL_MAJOR) || RHEL_MAJOR != 6 || RHEL_MINOR < 1)
#endif /* CONFIG_COMPAT_RHEL_6_1 */
#endif /* CONFIG_COMPAT_KERNEL_2_6_36 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33))
#ifndef CONFIG_COMPAT_BT_SOCK_CREATE_NEEDS_KERN
#define CONFIG_COMPAT_BT_SOCK_CREATE_NEEDS_KERN 1
#endif /* CONFIG_COMPAT_BT_SOCK_CREATE_NEEDS_KERN */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_33 */
#if (defined(RHEL_MAJOR) && RHEL_MAJOR == 6 && RHEL_MINOR >= 0)
#ifndef CONFIG_COMPAT_BT_SOCK_CREATE_NEEDS_KERN
#define CONFIG_COMPAT_BT_SOCK_CREATE_NEEDS_KERN 1
#endif /* CONFIG_COMPAT_BT_SOCK_CREATE_NEEDS_KERN */ 
#endif /* CONFIG_COMPAT_RHEL_6_0 */
#if (defined(RHEL_MAJOR) && RHEL_MAJOR == 6 && RHEL_MINOR >= 0)
#ifndef CONFIG_COMPAT_FIRMWARE_DATA_RW_NEEDS_FILP
#define CONFIG_COMPAT_FIRMWARE_DATA_RW_NEEDS_FILP 1
#endif /* CONFIG_COMPAT_FIRMWARE_DATA_RW_NEEDS_FILP */ 
#endif /* CONFIG_COMPAT_RHEL_6_0 */
#ifndef CONFIG_MAC80211
#define CONFIG_MAC80211 1
#endif /* CONFIG_MAC80211 */ 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,33))
#ifndef CONFIG_MAC80211_DRIVER_API_TRACER
#define CONFIG_MAC80211_DRIVER_API_TRACER 1
#endif /* CONFIG_MAC80211_DRIVER_API_TRACER */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_33 */
#ifndef CONFIG_MAC80211_RC_DEFAULT
#define CONFIG_MAC80211_RC_DEFAULT "minstrel_ht"
#endif /* CONFIG_MAC80211_RC_DEFAULT */ 
#ifndef CONFIG_MAC80211_RC_DEFAULT_MINSTREL
#define CONFIG_MAC80211_RC_DEFAULT_MINSTREL 1
#endif /* CONFIG_MAC80211_RC_DEFAULT_MINSTREL */ 
#ifndef CONFIG_COMPAT_MAC80211_RC_DEFAULT
#define CONFIG_COMPAT_MAC80211_RC_DEFAULT "minstrel_ht"
#endif /* CONFIG_COMPAT_MAC80211_RC_DEFAULT */ 
#ifndef CONFIG_MAC80211_RC_PID
#define CONFIG_MAC80211_RC_PID 1
#endif /* CONFIG_MAC80211_RC_PID */ 
#ifndef CONFIG_MAC80211_RC_MINSTREL
#define CONFIG_MAC80211_RC_MINSTREL 1
#endif /* CONFIG_MAC80211_RC_MINSTREL */ 
#ifndef CONFIG_MAC80211_RC_MINSTREL_HT
#define CONFIG_MAC80211_RC_MINSTREL_HT 1
#endif /* CONFIG_MAC80211_RC_MINSTREL_HT */ 
#if defined(CONFIG_LEDS_TRIGGERS) || defined(CONFIG_LEDS_TRIGGERS_MODULE)
#ifndef CONFIG_MAC80211_LEDS
#define CONFIG_MAC80211_LEDS 1
#endif /* CONFIG_MAC80211_LEDS */ 
#endif /* CONFIG_LEDS_TRIGGERS */
#ifndef CONFIG_MAC80211_MESH
#define CONFIG_MAC80211_MESH 1
#endif /* CONFIG_MAC80211_MESH */ 
#ifndef CONFIG_CFG80211
#define CONFIG_CFG80211 1
#endif /* CONFIG_CFG80211 */ 
#ifndef CONFIG_CFG80211_DEFAULT_PS
#define CONFIG_CFG80211_DEFAULT_PS 1
#endif /* CONFIG_CFG80211_DEFAULT_PS */ 
#ifndef CONFIG_NL80211_TESTMODE
#define CONFIG_NL80211_TESTMODE 1
#endif /* CONFIG_NL80211_TESTMODE */ 
#ifndef CONFIG_LIB80211
#define CONFIG_LIB80211 1
#endif /* CONFIG_LIB80211 */ 
#ifndef CONFIG_LIB80211_CRYPT_WEP
#define CONFIG_LIB80211_CRYPT_WEP 1
#endif /* CONFIG_LIB80211_CRYPT_WEP */ 
#ifndef CONFIG_LIB80211_CRYPT_CCMP
#define CONFIG_LIB80211_CRYPT_CCMP 1
#endif /* CONFIG_LIB80211_CRYPT_CCMP */ 
#ifndef CONFIG_LIB80211_CRYPT_TKIP
#define CONFIG_LIB80211_CRYPT_TKIP 1
#endif /* CONFIG_LIB80211_CRYPT_TKIP */ 
#ifndef CONFIG_BT
#define CONFIG_BT 1
#endif /* CONFIG_BT */ 
#ifndef CONFIG_COMPAT_BT_L2CAP
#define CONFIG_COMPAT_BT_L2CAP 1
#endif /* CONFIG_COMPAT_BT_L2CAP */ 
#ifndef CONFIG_COMPAT_BT_SCO
#define CONFIG_COMPAT_BT_SCO 1
#endif /* CONFIG_COMPAT_BT_SCO */ 
#ifndef CONFIG_BT_RFCOMM
#define CONFIG_BT_RFCOMM 1
#endif /* CONFIG_BT_RFCOMM */ 
#ifndef CONFIG_BT_RFCOMM_TTY
#define CONFIG_BT_RFCOMM_TTY 1
#endif /* CONFIG_BT_RFCOMM_TTY */ 
#ifndef CONFIG_BT_BNEP
#define CONFIG_BT_BNEP 1
#endif /* CONFIG_BT_BNEP */ 
#ifndef CONFIG_BT_BNEP_MC_FILTER
#define CONFIG_BT_BNEP_MC_FILTER 1
#endif /* CONFIG_BT_BNEP_MC_FILTER */ 
#ifndef CONFIG_BT_BNEP_PROTO_FILTER
#define CONFIG_BT_BNEP_PROTO_FILTER 1
#endif /* CONFIG_BT_BNEP_PROTO_FILTER */ 
#if defined(CONFIG_ISDN_CAPI) || defined(CONFIG_ISDN_CAPI_MODULE)
#ifndef CONFIG_BT_CMTP
#define CONFIG_BT_CMTP 1
#endif /* CONFIG_BT_CMTP */ 
#endif /* CONFIG_ISDN_CAPI */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28))
#ifndef CONFIG_COMPAT_BT_HIDP
#define CONFIG_COMPAT_BT_HIDP 1
#endif /* CONFIG_COMPAT_BT_HIDP */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_28 */
#ifndef CONFIG_BT_HCIUART
#define CONFIG_BT_HCIUART "M"
#endif /* CONFIG_BT_HCIUART */ 
#ifndef CONFIG_BT_HCIUART_H4
#define CONFIG_BT_HCIUART_H4 1
#endif /* CONFIG_BT_HCIUART_H4 */ 
#ifndef CONFIG_BT_HCIUART_BCSP
#define CONFIG_BT_HCIUART_BCSP 1
#endif /* CONFIG_BT_HCIUART_BCSP */ 
#ifndef CONFIG_BT_HCIUART_ATH3K
#define CONFIG_BT_HCIUART_ATH3K 1
#endif /* CONFIG_BT_HCIUART_ATH3K */ 
#ifndef CONFIG_BT_HCIUART_LL
#define CONFIG_BT_HCIUART_LL 1
#endif /* CONFIG_BT_HCIUART_LL */ 
#ifndef CONFIG_BT_HCIVHCI
#define CONFIG_BT_HCIVHCI 1
#endif /* CONFIG_BT_HCIVHCI */ 
#ifndef CONFIG_BT_MRVL
#define CONFIG_BT_MRVL 1
#endif /* CONFIG_BT_MRVL */ 
#if defined(CONFIG_PCMCIA) || defined(CONFIG_PCMCIA_MODULE)
#ifndef CONFIG_BT_HCIDTL1
#define CONFIG_BT_HCIDTL1 1
#endif /* CONFIG_BT_HCIDTL1 */ 
#ifndef CONFIG_BT_HCIBT3C
#define CONFIG_BT_HCIBT3C 1
#endif /* CONFIG_BT_HCIBT3C */ 
#ifndef CONFIG_BT_HCIBLUECARD
#define CONFIG_BT_HCIBLUECARD 1
#endif /* CONFIG_BT_HCIBLUECARD */ 
#ifndef CONFIG_BT_HCIBTUART
#define CONFIG_BT_HCIBTUART 1
#endif /* CONFIG_BT_HCIBTUART */ 
#endif /* CONFIG_PCMCIA */
#if defined(CONFIG_WIRELESS_EXT) || defined(CONFIG_WIRELESS_EXT_MODULE)
#ifndef CONFIG_CFG80211_WEXT
#define CONFIG_CFG80211_WEXT 1
#endif /* CONFIG_CFG80211_WEXT */ 
#else /* CONFIG_CFG80211_WEXT */
#endif /* CONFIG_WIRELESS_EXT */
#if defined(CONFIG_STAGING) || defined(CONFIG_STAGING_MODULE)
#ifndef CONFIG_COMPAT_STAGING
#define CONFIG_COMPAT_STAGING 1
#endif /* CONFIG_COMPAT_STAGING */ 
#endif /* CONFIG_STAGING */
#ifndef CONFIG_MAC80211_HWSIM
#define CONFIG_MAC80211_HWSIM 1
#endif /* CONFIG_MAC80211_HWSIM */ 
#ifndef CONFIG_ATH5K
#define CONFIG_ATH5K 1
#endif /* CONFIG_ATH5K */ 
#ifndef CONFIG_ATH9K
#define CONFIG_ATH9K 1
#endif /* CONFIG_ATH9K */ 
#ifndef CONFIG_ATH9K_HW
#define CONFIG_ATH9K_HW 1
#endif /* CONFIG_ATH9K_HW */ 
#ifndef CONFIG_ATH9K_COMMON
#define CONFIG_ATH9K_COMMON 1
#endif /* CONFIG_ATH9K_COMMON */ 
#ifndef CONFIG_ATH9K_RATE_CONTROL
#define CONFIG_ATH9K_RATE_CONTROL 1
#endif /* CONFIG_ATH9K_RATE_CONTROL */ 
#if defined(CONFIG_PCI) || defined(CONFIG_PCI_MODULE)
#ifndef CONFIG_ATH5K_PCI
#define CONFIG_ATH5K_PCI 1
#endif /* CONFIG_ATH5K_PCI */ 
#ifndef CONFIG_ATH9K_PCI
#define CONFIG_ATH9K_PCI 1
#endif /* CONFIG_ATH9K_PCI */ 
#ifndef CONFIG_IWLWIFI
#define CONFIG_IWLWIFI 1
#endif /* CONFIG_IWLWIFI */ 
#ifndef CONFIG_IWLEGACY
#define CONFIG_IWLEGACY 1
#endif /* CONFIG_IWLEGACY */ 
#ifndef CONFIG_COMPAT_IWL4965
#define CONFIG_COMPAT_IWL4965 1
#endif /* CONFIG_COMPAT_IWL4965 */ 
#ifndef CONFIG_IWL3945
#define CONFIG_IWL3945 1
#endif /* CONFIG_IWL3945 */ 
#ifndef CONFIG_B43
#define CONFIG_B43 1
#endif /* CONFIG_B43 */ 
#ifndef CONFIG_B43_HWRNG
#define CONFIG_B43_HWRNG 1
#endif /* CONFIG_B43_HWRNG */ 
#ifndef CONFIG_B43_PCI_AUTOSELECT
#define CONFIG_B43_PCI_AUTOSELECT 1
#endif /* CONFIG_B43_PCI_AUTOSELECT */ 
#if defined(CONFIG_PCMCIA) || defined(CONFIG_PCMCIA_MODULE)
#ifndef CONFIG_B43_PCMCIA
#define CONFIG_B43_PCMCIA 1
#endif /* CONFIG_B43_PCMCIA */ 
#endif /* CONFIG_PCMCIA */
#if defined(CONFIG_MAC80211_LEDS) || defined(CONFIG_MAC80211_LEDS_MODULE)
#ifndef CONFIG_B43_LEDS
#define CONFIG_B43_LEDS 1
#endif /* CONFIG_B43_LEDS */ 
#endif /* CONFIG_MAC80211_LEDS */
#ifndef CONFIG_B43_PHY_LP
#define CONFIG_B43_PHY_LP 1
#endif /* CONFIG_B43_PHY_LP */ 
#ifndef CONFIG_B43_PHY_N
#define CONFIG_B43_PHY_N 1
#endif /* CONFIG_B43_PHY_N */ 
#ifndef CONFIG_B43_PHY_HT
#define CONFIG_B43_PHY_HT 1
#endif /* CONFIG_B43_PHY_HT */ 
#ifndef CONFIG_B43LEGACY
#define CONFIG_B43LEGACY 1
#endif /* CONFIG_B43LEGACY */ 
#ifndef CONFIG_B43LEGACY_HWRNG
#define CONFIG_B43LEGACY_HWRNG 1
#endif /* CONFIG_B43LEGACY_HWRNG */ 
#ifndef CONFIG_B43LEGACY_PCI_AUTOSELECT
#define CONFIG_B43LEGACY_PCI_AUTOSELECT 1
#endif /* CONFIG_B43LEGACY_PCI_AUTOSELECT */ 
#if defined(CONFIG_MAC80211_LEDS) || defined(CONFIG_MAC80211_LEDS_MODULE)
#ifndef CONFIG_B43LEGACY_LEDS
#define CONFIG_B43LEGACY_LEDS 1
#endif /* CONFIG_B43LEGACY_LEDS */ 
#endif /* CONFIG_MAC80211_LEDS */
#ifndef CONFIG_B43LEGACY_DMA
#define CONFIG_B43LEGACY_DMA 1
#endif /* CONFIG_B43LEGACY_DMA */ 
#ifndef CONFIG_B43LEGACY_PIO
#define CONFIG_B43LEGACY_PIO 1
#endif /* CONFIG_B43LEGACY_PIO */ 
#if defined(CONFIG_WIRELESS_EXT) || defined(CONFIG_WIRELESS_EXT_MODULE)
#ifndef CONFIG_LIBIPW
#define CONFIG_LIBIPW 1
#endif /* CONFIG_LIBIPW */ 
#ifndef CONFIG_IPW2100
#define CONFIG_IPW2100 1
#endif /* CONFIG_IPW2100 */ 
#ifndef CONFIG_IPW2100_MONITOR
#define CONFIG_IPW2100_MONITOR 1
#endif /* CONFIG_IPW2100_MONITOR */ 
#ifndef CONFIG_IPW2200
#define CONFIG_IPW2200 1
#endif /* CONFIG_IPW2200 */ 
#ifndef CONFIG_IPW2200_MONITOR
#define CONFIG_IPW2200_MONITOR 1
#endif /* CONFIG_IPW2200_MONITOR */ 
#ifndef CONFIG_IPW2200_RADIOTAP
#define CONFIG_IPW2200_RADIOTAP 1
#endif /* CONFIG_IPW2200_RADIOTAP */ 
#ifndef CONFIG_IPW2200_PROMISCUOUS
#define CONFIG_IPW2200_PROMISCUOUS 1
#endif /* CONFIG_IPW2200_PROMISCUOUS */ 
#ifndef CONFIG_IPW2200_QOS
#define CONFIG_IPW2200_QOS 1
#endif /* CONFIG_IPW2200_QOS */ 
#endif /* CONFIG_WIRELESS_EXT */
#if defined(CONFIG_SSB) || defined(CONFIG_SSB_MODULE)
#ifndef CONFIG_SSB_SPROM
#define CONFIG_SSB_SPROM 1
#endif /* CONFIG_SSB_SPROM */ 
#ifndef CONFIG_SSB_BLOCKIO
#define CONFIG_SSB_BLOCKIO 1
#endif /* CONFIG_SSB_BLOCKIO */ 
#ifndef CONFIG_SSB_PCIHOST
#define CONFIG_SSB_PCIHOST 1
#endif /* CONFIG_SSB_PCIHOST */ 
#ifndef CONFIG_SSB_B43_PCI_BRIDGE
#define CONFIG_SSB_B43_PCI_BRIDGE 1
#endif /* CONFIG_SSB_B43_PCI_BRIDGE */ 
#if defined(CONFIG_PCMCIA) || defined(CONFIG_PCMCIA_MODULE)
#ifndef CONFIG_SSB_PCMCIAHOST
#define CONFIG_SSB_PCMCIAHOST 1
#endif /* CONFIG_SSB_PCMCIAHOST */ 
#endif /* CONFIG_PCMCIA */
#ifndef CONFIG_SSB_DRIVER_PCICORE
#define CONFIG_SSB_DRIVER_PCICORE 1
#endif /* CONFIG_SSB_DRIVER_PCICORE */ 
#ifndef CONFIG_B43_SSB
#define CONFIG_B43_SSB 1
#endif /* CONFIG_B43_SSB */ 
#endif /* CONFIG_SSB */
#ifndef CONFIG_BCMA
#define CONFIG_BCMA 1
#endif /* CONFIG_BCMA */ 
#ifndef CONFIG_BCMA_BLOCKIO
#define CONFIG_BCMA_BLOCKIO 1
#endif /* CONFIG_BCMA_BLOCKIO */ 
#ifndef CONFIG_BCMA_HOST_PCI
#define CONFIG_BCMA_HOST_PCI 1
#endif /* CONFIG_BCMA_HOST_PCI */ 
#ifndef CONFIG_B43_BCMA
#define CONFIG_B43_BCMA 1
#endif /* CONFIG_B43_BCMA */ 
#ifndef CONFIG_B43_BCMA_PIO
#define CONFIG_B43_BCMA_PIO 1
#endif /* CONFIG_B43_BCMA_PIO */ 
#ifndef CONFIG_P54_PCI
#define CONFIG_P54_PCI 1
#endif /* CONFIG_P54_PCI */ 
#ifndef CONFIG_B44
#define CONFIG_B44 1
#endif /* CONFIG_B44 */ 
#ifndef CONFIG_B44_PCI
#define CONFIG_B44_PCI 1
#endif /* CONFIG_B44_PCI */ 
#ifndef CONFIG_RTL8180
#define CONFIG_RTL8180 1
#endif /* CONFIG_RTL8180 */ 
#ifndef CONFIG_ADM8211
#define CONFIG_ADM8211 1
#endif /* CONFIG_ADM8211 */ 
#ifndef CONFIG_RT2X00_LIB_PCI
#define CONFIG_RT2X00_LIB_PCI 1
#endif /* CONFIG_RT2X00_LIB_PCI */ 
#ifndef CONFIG_RT2400PCI
#define CONFIG_RT2400PCI 1
#endif /* CONFIG_RT2400PCI */ 
#ifndef CONFIG_RT2500PCI
#define CONFIG_RT2500PCI 1
#endif /* CONFIG_RT2500PCI */ 
#if defined(CONFIG_CRC_CCITT) || defined(CONFIG_CRC_CCITT_MODULE)
#ifndef CONFIG_RT2800PCI
#define CONFIG_RT2800PCI 1
#endif /* CONFIG_RT2800PCI */ 
#ifndef CONFIG_RT2800PCI_RT33XX
#define CONFIG_RT2800PCI_RT33XX 1
#endif /* CONFIG_RT2800PCI_RT33XX */ 
#endif /* CONFIG_CRC_CCITT */
#if defined(CONFIG_CRC_ITU_T) || defined(CONFIG_CRC_ITU_T_MODULE)
#ifndef CONFIG_RT61PCI
#define CONFIG_RT61PCI 1
#endif /* CONFIG_RT61PCI */ 
#endif /* CONFIG_CRC_ITU_T */
#ifndef CONFIG_MWL8K
#define CONFIG_MWL8K 1
#endif /* CONFIG_MWL8K */ 
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
#undef CONFIG_ATL1C
#else /* CONFIG_COMPAT_KERNEL_2_6_27 */
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#if defined(CONFIG_WIRELESS_EXT) || defined(CONFIG_WIRELESS_EXT_MODULE)
#ifndef CONFIG_HERMES
#define CONFIG_HERMES 1
#endif /* CONFIG_HERMES */ 
#ifndef CONFIG_HERMES_CACHE_FW_ON_INIT
#define CONFIG_HERMES_CACHE_FW_ON_INIT 1
#endif /* CONFIG_HERMES_CACHE_FW_ON_INIT */ 
#if defined(CONFIG_PPC_PMAC) || defined(CONFIG_PPC_PMAC_MODULE)
#ifndef CONFIG_APPLE_AIRPORT
#define CONFIG_APPLE_AIRPORT 1
#endif /* CONFIG_APPLE_AIRPORT */ 
#endif /* CONFIG_PPC_PMAC */
#ifndef CONFIG_PLX_HERMES
#define CONFIG_PLX_HERMES 1
#endif /* CONFIG_PLX_HERMES */ 
#ifndef CONFIG_TMD_HERMES
#define CONFIG_TMD_HERMES 1
#endif /* CONFIG_TMD_HERMES */ 
#ifndef CONFIG_NORTEL_HERMES
#define CONFIG_NORTEL_HERMES 1
#endif /* CONFIG_NORTEL_HERMES */ 
#ifndef CONFIG_PCI_HERMES
#define CONFIG_PCI_HERMES 1
#endif /* CONFIG_PCI_HERMES */ 
#if defined(CONFIG_PCMCIA) || defined(CONFIG_PCMCIA_MODULE)
#ifndef CONFIG_PCMCIA_HERMES
#define CONFIG_PCMCIA_HERMES 1
#endif /* CONFIG_PCMCIA_HERMES */ 
#ifndef CONFIG_PCMCIA_SPECTRUM
#define CONFIG_PCMCIA_SPECTRUM 1
#endif /* CONFIG_PCMCIA_SPECTRUM */ 
#endif /* CONFIG_PCMCIA */
#endif /* CONFIG_WIRELESS_EXT */
#ifndef CONFIG_RTL8192CE
#define CONFIG_RTL8192CE 1
#endif /* CONFIG_RTL8192CE */ 
#ifndef CONFIG_RTL8192SE
#define CONFIG_RTL8192SE 1
#endif /* CONFIG_RTL8192SE */ 
#ifndef CONFIG_RTL8192DE
#define CONFIG_RTL8192DE 1
#endif /* CONFIG_RTL8192DE */ 
#ifndef CONFIG_BRCMSMAC
#define CONFIG_BRCMSMAC 1
#endif /* CONFIG_BRCMSMAC */ 
#ifndef CONFIG_MWIFIEX_PCIE
#define CONFIG_MWIFIEX_PCIE 1
#endif /* CONFIG_MWIFIEX_PCIE */ 
#endif /* CONFIG_PCI */
#if defined(CONFIG_PCMCIA) || defined(CONFIG_PCMCIA_MODULE)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
#undef CONFIG_LIBERTAS
#undef CONFIG_LIBERTAS_CS
#else /* CONFIG_COMPAT_KERNEL_2_6_27 */
#ifndef CONFIG_LIBERTAS_CS
#define CONFIG_LIBERTAS_CS 1
#endif /* CONFIG_LIBERTAS_CS */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#endif /* CONFIG_PCMCIA */
#if defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29))
#ifndef CONFIG_COMPAT_ZD1211RW
#define CONFIG_COMPAT_ZD1211RW 1
#endif /* CONFIG_COMPAT_ZD1211RW */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_29 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,29))
#undef CONFIG_USB_COMPAT_USBNET
#undef CONFIG_USB_NET_COMPAT_RNDIS_HOST
#undef CONFIG_USB_NET_COMPAT_RNDIS_WLAN
#undef CONFIG_USB_NET_COMPAT_CDCETHER
#else /* CONFIG_COMPAT_KERNEL_2_6_29 */
#ifndef CONFIG_USB_COMPAT_USBNET
#define CONFIG_USB_COMPAT_USBNET 1
#endif /* CONFIG_USB_COMPAT_USBNET */ 
#if defined(CONFIG_USB_NET_CDCETHER) || defined(CONFIG_USB_NET_CDCETHER_MODULE)
#ifndef CONFIG_USB_NET_COMPAT_RNDIS_HOST
#define CONFIG_USB_NET_COMPAT_RNDIS_HOST 1
#endif /* CONFIG_USB_NET_COMPAT_RNDIS_HOST */ 
#ifndef CONFIG_USB_NET_COMPAT_RNDIS_WLAN
#define CONFIG_USB_NET_COMPAT_RNDIS_WLAN 1
#endif /* CONFIG_USB_NET_COMPAT_RNDIS_WLAN */ 
#endif /* CONFIG_USB_NET_CDCETHER */
#if defined(CONFIG_USB_NET_CDCETHER_MODULE) || defined(CONFIG_USB_NET_CDCETHER_MODULE_MODULE)
#ifndef CONFIG_USB_NET_COMPAT_RNDIS_HOST
#define CONFIG_USB_NET_COMPAT_RNDIS_HOST 1
#endif /* CONFIG_USB_NET_COMPAT_RNDIS_HOST */ 
#ifndef CONFIG_USB_NET_COMPAT_RNDIS_WLAN
#define CONFIG_USB_NET_COMPAT_RNDIS_WLAN 1
#endif /* CONFIG_USB_NET_COMPAT_RNDIS_WLAN */ 
#endif /* CONFIG_USB_NET_CDCETHER */
#ifndef CONFIG_USB_NET_COMPAT_CDCETHER
#define CONFIG_USB_NET_COMPAT_CDCETHER 1
#endif /* CONFIG_USB_NET_COMPAT_CDCETHER */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_29 */
#ifndef CONFIG_P54_USB
#define CONFIG_P54_USB 1
#endif /* CONFIG_P54_USB */ 
#ifndef CONFIG_RTL8187
#define CONFIG_RTL8187 1
#endif /* CONFIG_RTL8187 */ 
#if defined(CONFIG_MAC80211_LEDS) || defined(CONFIG_MAC80211_LEDS_MODULE)
#ifndef CONFIG_RTL8187_LEDS
#define CONFIG_RTL8187_LEDS 1
#endif /* CONFIG_RTL8187_LEDS */ 
#endif /* CONFIG_MAC80211_LEDS */
#ifndef CONFIG_AT76C50X_USB
#define CONFIG_AT76C50X_USB 1
#endif /* CONFIG_AT76C50X_USB */ 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29))
#ifndef CONFIG_CARL9170
#define CONFIG_CARL9170 1
#endif /* CONFIG_CARL9170 */ 
#if defined(CONFIG_MAC80211_LEDS) || defined(CONFIG_MAC80211_LEDS_MODULE)
#ifndef CONFIG_CARL9170_LEDS
#define CONFIG_CARL9170_LEDS 1
#endif /* CONFIG_CARL9170_LEDS */ 
#endif /* CONFIG_MAC80211_LEDS */
#endif /* CONFIG_COMPAT_KERNEL_2_6_29 */
#ifndef CONFIG_COMPAT_USB_URB_THREAD_FIX
#define CONFIG_COMPAT_USB_URB_THREAD_FIX 1
#endif /* CONFIG_COMPAT_USB_URB_THREAD_FIX */ 
#ifndef CONFIG_ATH9K_HTC
#define CONFIG_ATH9K_HTC 1
#endif /* CONFIG_ATH9K_HTC */ 
#ifndef CONFIG_RT2500USB
#define CONFIG_RT2500USB 1
#endif /* CONFIG_RT2500USB */ 
#if defined(CONFIG_CRC_CCITT) || defined(CONFIG_CRC_CCITT_MODULE)
#ifndef CONFIG_RT2800USB
#define CONFIG_RT2800USB 1
#endif /* CONFIG_RT2800USB */ 
#ifndef CONFIG_RT2800USB_RT33XX
#define CONFIG_RT2800USB_RT33XX 1
#endif /* CONFIG_RT2800USB_RT33XX */ 
#ifndef CONFIG_RT2800USB_UNKNOWN
#define CONFIG_RT2800USB_UNKNOWN 1
#endif /* CONFIG_RT2800USB_UNKNOWN */ 
#endif /* CONFIG_CRC_CCITT */
#ifndef CONFIG_RT2X00_LIB_USB
#define CONFIG_RT2X00_LIB_USB 1
#endif /* CONFIG_RT2X00_LIB_USB */ 
#if defined(CONFIG_CRC_ITU_T) || defined(CONFIG_CRC_ITU_T_MODULE)
#ifndef CONFIG_RT73USB
#define CONFIG_RT73USB 1
#endif /* CONFIG_RT73USB */ 
#endif /* CONFIG_CRC_ITU_T */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
#undef CONFIG_LIBERTAS_THINFIRM_USB
#undef CONFIG_LIBERTAS_USB
#else /* CONFIG_COMPAT_KERNEL_2_6_27 */
#ifndef CONFIG_LIBERTAS_THINFIRM_USB
#define CONFIG_LIBERTAS_THINFIRM_USB 1
#endif /* CONFIG_LIBERTAS_THINFIRM_USB */ 
#ifndef CONFIG_LIBERTAS_USB
#define CONFIG_LIBERTAS_USB 1
#endif /* CONFIG_LIBERTAS_USB */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#ifndef CONFIG_ORINOCO_USB
#define CONFIG_ORINOCO_USB 1
#endif /* CONFIG_ORINOCO_USB */ 
#ifndef CONFIG_BT_HCIBTUSB
#define CONFIG_BT_HCIBTUSB 1
#endif /* CONFIG_BT_HCIBTUSB */ 
#ifndef CONFIG_BT_HCIBCM203X
#define CONFIG_BT_HCIBCM203X 1
#endif /* CONFIG_BT_HCIBCM203X */ 
#ifndef CONFIG_BT_HCIBPA10X
#define CONFIG_BT_HCIBPA10X 1
#endif /* CONFIG_BT_HCIBPA10X */ 
#ifndef CONFIG_BT_HCIBFUSB
#define CONFIG_BT_HCIBFUSB 1
#endif /* CONFIG_BT_HCIBFUSB */ 
#ifndef CONFIG_BT_ATH3K
#define CONFIG_BT_ATH3K 1
#endif /* CONFIG_BT_ATH3K */ 
#ifndef CONFIG_RTL8192CU
#define CONFIG_RTL8192CU 1
#endif /* CONFIG_RTL8192CU */ 
#endif /* CONFIG_USB */
#if defined(CONFIG_SPI_MASTER) || defined(CONFIG_SPI_MASTER_MODULE)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,25))
#if defined(CONFIG_CRC7) || defined(CONFIG_CRC7_MODULE)
#endif /* CONFIG_CRC7 */
#ifndef CONFIG_P54_SPI
#define CONFIG_P54_SPI 1
#endif /* CONFIG_P54_SPI */ 
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
#undef CONFIG_LIBERTAS_SPI
#else /* CONFIG_COMPAT_KERNEL_2_6_27 */
#ifndef CONFIG_LIBERTAS_SPI
#define CONFIG_LIBERTAS_SPI 1
#endif /* CONFIG_LIBERTAS_SPI */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#endif /* CONFIG_COMPAT_KERNEL_2_6_25 */
#endif /* CONFIG_SPI_MASTER */
#if defined(CONFIG_MMC) || defined(CONFIG_MMC_MODULE)
#ifndef CONFIG_SSB_SDIOHOST
#define CONFIG_SSB_SDIOHOST 1
#endif /* CONFIG_SSB_SDIOHOST */ 
#ifndef CONFIG_B43_SDIO
#define CONFIG_B43_SDIO 1
#endif /* CONFIG_B43_SDIO */ 
#if defined(CONFIG_CRC7) || defined(CONFIG_CRC7_MODULE)
#if defined(CONFIG_WL12XX_PLATFORM_DATA) || defined(CONFIG_WL12XX_PLATFORM_DATA_MODULE)
#endif /* CONFIG_WL12XX_PLATFORM_DATA */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#if defined(CONFIG_WL12XX_PLATFORM_DATA) || defined(CONFIG_WL12XX_PLATFORM_DATA_MODULE)
#ifndef CONFIG_COMPAT_WL12XX_SDIO
#define CONFIG_COMPAT_WL12XX_SDIO 1
#endif /* CONFIG_COMPAT_WL12XX_SDIO */ 
#endif /* CONFIG_WL12XX_PLATFORM_DATA */
#endif /* CONFIG_COMPAT_KERNEL_2_6_32 */
#endif /* CONFIG_CRC7 */
#ifndef CONFIG_MWIFIEX_SDIO
#define CONFIG_MWIFIEX_SDIO 1
#endif /* CONFIG_MWIFIEX_SDIO */ 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#ifndef CONFIG_COMPAT_LIBERTAS_SDIO
#define CONFIG_COMPAT_LIBERTAS_SDIO 1
#endif /* CONFIG_COMPAT_LIBERTAS_SDIO */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_32 */
#ifndef CONFIG_IWM
#define CONFIG_IWM 1
#endif /* CONFIG_IWM */ 
#ifndef CONFIG_BT_HCIBTSDIO
#define CONFIG_BT_HCIBTSDIO 1
#endif /* CONFIG_BT_HCIBTSDIO */ 
#ifndef CONFIG_BT_MRVL_SDIO
#define CONFIG_BT_MRVL_SDIO 1
#endif /* CONFIG_BT_MRVL_SDIO */ 
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27))
#ifndef CONFIG_ATH6KL
#define CONFIG_ATH6KL 1
#endif /* CONFIG_ATH6KL */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29))
#ifndef CONFIG_BRCMFMAC
#define CONFIG_BRCMFMAC 1
#endif /* CONFIG_BRCMFMAC */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_29 */
#endif /* CONFIG_MMC */
#ifndef CONFIG_RTLWIFI
#define CONFIG_RTLWIFI 1
#endif /* CONFIG_RTLWIFI */ 
#ifndef CONFIG_RTL8192C_COMMON
#define CONFIG_RTL8192C_COMMON 1
#endif /* CONFIG_RTL8192C_COMMON */ 
#ifndef CONFIG_RT2X00
#define CONFIG_RT2X00 1
#endif /* CONFIG_RT2X00 */ 
#ifndef CONFIG_RT2X00_LIB
#define CONFIG_RT2X00_LIB 1
#endif /* CONFIG_RT2X00_LIB */ 
#ifndef CONFIG_RT2800_LIB
#define CONFIG_RT2800_LIB 1
#endif /* CONFIG_RT2800_LIB */ 
#ifndef CONFIG_RT2X00_LIB_FIRMWARE
#define CONFIG_RT2X00_LIB_FIRMWARE 1
#endif /* CONFIG_RT2X00_LIB_FIRMWARE */ 
#ifndef CONFIG_RT2X00_LIB_CRYPTO
#define CONFIG_RT2X00_LIB_CRYPTO 1
#endif /* CONFIG_RT2X00_LIB_CRYPTO */ 
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25))
#undef CONFIG_RT2X00_LIB_LEDS
#else /* CONFIG_COMPAT_KERNEL_2_6_25 */
#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
#ifndef CONFIG_RT2X00_LIB_LEDS
#define CONFIG_RT2X00_LIB_LEDS 1
#endif /* CONFIG_RT2X00_LIB_LEDS */ 
#endif /* CONFIG_LEDS_CLASS */
#endif /* CONFIG_COMPAT_KERNEL_2_6_25 */
#ifndef CONFIG_P54_COMMON
#define CONFIG_P54_COMMON 1
#endif /* CONFIG_P54_COMMON */ 
#if defined(CONFIG_MAC80211_LEDS) || defined(CONFIG_MAC80211_LEDS_MODULE)
#ifndef CONFIG_P54_LEDS
#define CONFIG_P54_LEDS 1
#endif /* CONFIG_P54_LEDS */ 
#endif /* CONFIG_MAC80211_LEDS */
#ifndef CONFIG_ATH_COMMON
#define CONFIG_ATH_COMMON 1
#endif /* CONFIG_ATH_COMMON */ 
#ifndef CONFIG_BRCMUTIL
#define CONFIG_BRCMUTIL 1
#endif /* CONFIG_BRCMUTIL */ 
#if defined(CONFIG_CRC7) || defined(CONFIG_CRC7_MODULE)
#ifndef CONFIG_WL12XX
#define CONFIG_WL12XX 1
#endif /* CONFIG_WL12XX */ 
#endif /* CONFIG_CRC7 */
#ifndef CONFIG_MWIFIEX
#define CONFIG_MWIFIEX 1
#endif /* CONFIG_MWIFIEX */ 
#if !defined(CONFIG_CORDIC) && !defined(CONFIG_CORDIC_MODULE)
#ifndef CONFIG_COMPAT_CORDIC
#define CONFIG_COMPAT_CORDIC 1
#endif /* CONFIG_COMPAT_CORDIC */ 
#endif /* CONFIG_CORDIC */
#if !defined(CONFIG_CRC8) && !defined(CONFIG_CRC8_MODULE)
#ifndef CONFIG_COMPAT_CRC8
#define CONFIG_COMPAT_CRC8 1
#endif /* CONFIG_COMPAT_CRC8 */ 
#endif /* CONFIG_CRC8 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
#undef CONFIG_LIBERTAS
#else /* CONFIG_COMPAT_KERNEL_2_6_27 */
#ifndef CONFIG_LIBERTAS_THINFIRM
#define CONFIG_LIBERTAS_THINFIRM 1
#endif /* CONFIG_LIBERTAS_THINFIRM */ 
#ifndef CONFIG_LIBERTAS
#define CONFIG_LIBERTAS 1
#endif /* CONFIG_LIBERTAS */ 
#ifndef CONFIG_LIBERTAS_MESH
#define CONFIG_LIBERTAS_MESH 1
#endif /* CONFIG_LIBERTAS_MESH */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_27 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31))
#ifndef CONFIG_RFKILL_BACKPORT
#define CONFIG_RFKILL_BACKPORT 1
#endif /* CONFIG_RFKILL_BACKPORT */ 
#if defined(CONFIG_LEDS_TRIGGERS) || defined(CONFIG_LEDS_TRIGGERS_MODULE)
#ifndef CONFIG_RFKILL_BACKPORT_LEDS
#define CONFIG_RFKILL_BACKPORT_LEDS 1
#endif /* CONFIG_RFKILL_BACKPORT_LEDS */ 
#endif /* CONFIG_LEDS_TRIGGERS */
#ifndef CONFIG_RFKILL_BACKPORT_INPUT
#define CONFIG_RFKILL_BACKPORT_INPUT 1
#endif /* CONFIG_RFKILL_BACKPORT_INPUT */ 
#endif /* CONFIG_COMPAT_KERNEL_2_6_31 */
#ifdef CONFIG_NET_SCHED
#ifdef CONFIG_NETDEVICES_MULTIQUEUE
#ifndef CONFIG_MAC80211_QOS
#define CONFIG_MAC80211_QOS 1
#endif /* CONFIG_MAC80211_QOS */ 
#endif
#endif
#endif /* COMPAT_AUTOCONF_INCLUDED */
