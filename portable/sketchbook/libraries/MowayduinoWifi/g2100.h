/*****************************************************************************

  Filename:		g2100.h
  Description:	Driver for the ZeroG Wireless G2100 series devices

 *****************************************************************************

  Driver for the WiShield 1.0 wireless devices

  Copyright(c) 2009 Async Labs Inc. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Contact Information:
  <asynclabs@asynclabs.com>

   Author               Date        Comment
  ----------------------------------------------------------------------------
   AsyncLabs			02/25/2009	Initial port
   AsyncLabs			05/29/2009	Adding support for new library

 *****************************************************************************/

#ifndef G2100_H_
#define G2100_H_

#define DEBUG_ACTIVE

#define DRV_STATE_INIT                0
#define DRV_STATE_GET_MAC             2
#define DRV_STATE_START_CONN          3
#define DRV_STATE_IDLE_CONNECT        14
#define DRV_STATE_PROCESS_RX          4
#define DRV_STATE_IDLE                5
#define DRV_STATE_SETUP_SECURITY      6
#define DRV_STATE_IDLE_SECURITY       7 
//#define DRV_STATE_INSTALL_PSK	      7
//#define DRV_STATE_ENABLE_CONN_MANAGE  8
#define DRV_STATE_CREATE_PROFILE      9
#define DRV_STATE_SET_SSID            10
#define DRV_STATE_IDLE_SSID           11
#define DRV_STATE_SET_NETWORKTYPE     12
#define DRV_STATE_IDLE_NETWORKTYPE    13
#define DRV_STATE_CHECK_NETWORK       15
#define DRV_STATE_IDLE_CHECK	      16
#define DRV_STATE_ERROR               99

//Host to Zero G long
#define HTOZGL(a) (	 ((a & 0x000000ff)<<24) \
					|((a & 0x0000ff00)<<8)  \
					|((a & 0x00ff0000)>>8)  \
					|((a & 0xff000000)>>24)	)
#define ZGTOHL(a) HTOZGL(a)

// Host to Zero G short
#define HSTOZGS(a) (u16)(((a)<<8) | ((a)>>8))
#define ZGSTOHS(a) HSTOZGS(a)
//#define HTONS(a) HSTOZGS(a)

#define ZG_INTERRUPT_PIN	2	// Pin on Arduino

// Command values which appear in ZG_PREAMBLE_CMD_IDX for each SPI message
#define ZG_CMD_FIFO_ACCESS			(0x80)
#define ZG_CMD_WT_FIFO_DATA			(ZG_CMD_FIFO_ACCESS | 0x20)
#define ZG_CMD_WT_FIFO_MGMT			(ZG_CMD_FIFO_ACCESS | 0x30)
#define ZG_CMD_RD_FIFO				(ZG_CMD_FIFO_ACCESS | 0x00)
#define ZG_CMD_WT_FIFO_DONE			(ZG_CMD_FIFO_ACCESS | 0x40)
#define ZG_CMD_RD_FIFO_DONE			(ZG_CMD_FIFO_ACCESS | 0x50)
#define ZG_CMD_WT_REG				(0x00)
#define ZG_CMD_RD_REG				(0x40)

// Type values which appear in ZG_PREAMBLE_TYPE_IDX for each SPI message
#define ZG_MAC_TYPE_TXDATA_REQ		((u8)1)  
#define ZG_MAC_TYPE_MGMT_REQ		((u8)2)  // WF_MGMT_REQUEST_TYPE

#define ZG_MAC_TYPE_TXDATA_CONFIRM	((u8)1)
#define ZG_MAC_TYPE_MGMT_CONFIRM	((u8)2)
#define ZG_MAC_TYPE_RXDATA_INDICATE	((u8)3)
#define ZG_MAC_TYPE_MGMT_INDICATE	((u8)4)

// Subtype values which appear in ZG_PREAMBLE_SUBTYPE_IDX for each SPI message
// Subtype for ZG_MAC_TYPE_TXDATA_REQ and ZG_MAC_TYPE_TXDATA_CONFIRM
#define ZG_MAC_SUBTYPE_TXDATA_REQ_STD			((u8)1)

// Subtype for ZG_MAC_TYPE_MGMT_REQ and ZG_MAC_TYPE_MGMT_CONFIRM
#define ZG_MAC_SUBTYPE_MGMT_REQ_PMK_KEY			((u8)8)
#define ZG_MAC_SUBTYPE_MGMT_REQ_WEP_KEY			((u8)10)
#define ZG_MAC_SUBTYPE_MGMT_REQ_CALC_PSK		((u8)12)
#define ZG_MAC_SUBTYPE_MGMT_REQ_SET_PARAM		((u8)15)
#define ZG_MAC_SUBTYPE_MGMT_REQ_GET_PARAM		((u8)16)
#define ZG_MAC_SUBTYPE_MGMT_REQ_ADHOC_START		((u8)18)
#define ZG_MAC_SUBTYPE_MGMT_REQ_CONNECT			((u8)19)
#define ZG_MAC_SUBTYPE_MGMT_REQ_CONNECT_MANAGE	        ((u8)20)
#define ZG_MAC_SUBTYPE_CP_CREATE_PROFILE                ((u8)21)
#define ZG_MAC_SUBTYPE_CP_SET_ELEMENT_SUBTYPE           ((u8)24)
#define ZG_MAC_SUBTYPE_CA_SET_ELEMENT_SUBTYPE           ((u8)26)
#define ZG_MAC_SUBTYPE_CM_CONNECT_SUBYTPE               ((u8)28)
#define ZG_MAC_SUBTYPE_CM_GET_CONNECT_SUBYTPE           ((u8)30)


// Subtype for ZG_MAC_TYPE_RXDATA_INDICATE
#define ZG_MAC_SUBTYPE_RXDATA_IND_STD			((u8)1)

// Subtype for ZG_MAC_TYPE_MGMT_INDICATE
#define ZG_MAC_SUBTYPE_MGMT_IND_DISASSOC		((u8)1)
#define ZG_MAC_SUBTYPE_MGMT_IND_DEAUTH			((u8)2)
#define ZG_MAC_SUBTYPE_MGMT_IND_CONN_STATUS		((u8)4)

// Parameter IDs for ZG_MAC_SUBTYPE_MGMT_REQ_SET_PARAM
#define ZG_PARAM_MAC_ADDRESS			(1)
#define ZG_PARAM_ENABLE_MRF24WB0                (30)

//ELEMENT ID
#define WF_CP_ELEMENT_ALL               0  /* sends all elements in CP struct */
#define WF_CP_ELEMENT_SSID              1
#define WF_CP_ELEMENT_BSSID             2
#define WF_CP_ELEMENT_SECURITY          3
#define WF_CP_ELEMENT_NETWORK_TYPE      4
#define WF_CP_ELEMENT_ADHOC_BEHAVIOR    5
#define WF_CP_ELEMENT_WEP_KEY_INDEX     6
#define WF_CP_ELEMENT_SSID_TYPE         7
#define WF_CP_ELEMENT_WEPKEY_TYPE       8


#define WF_CA_ELEMENT_ALL                          0
#define WF_CA_ELEMENT_SCANTYPE                     1
#define WF_CA_ELEMENT_RSSI                         2
#define WF_CA_ELEMENT_CP_LIST                      3
#define WF_CA_ELEMENT_LIST_RETRY_COUNT             4
#define WF_CA_ELEMENT_EVENT_NOTIFICATION_ACTION    5
#define WF_CA_ELEMENT_BEACON_TIMEOUT_ACTION        6
#define WF_CA_ELEMENT_DEAUTH_ACTION                7
#define WF_CA_ELEMENT_CHANNEL_LIST                 8
#define WF_CA_ELEMENT_LISTEN_INTERVAL              9
#define WF_CA_ELEMENT_BEACON_TIMEOUT               10
#define WF_CA_ELEMENT_SCAN_COUNT                   11
#define WF_CA_ELEMENT_MIN_CHANNEL_TIME             12
#define WF_CA_ELEMENT_MAX_CHANNEL_TIME             13
#define WF_CA_ELEMENT_PROBE_DELAY                  14


#define WF_SECURITY_OPEN                         (0)
#define WF_SECURITY_WEP_40                       (1)
#define WF_SECURITY_WEP_104                      (2)
#define WF_SECURITY_WPA_WITH_KEY                 (3)
#define WF_SECURITY_WPA_WITH_PASS_PHRASE         (4)
#define WF_SECURITY_WPA2_WITH_KEY                (5)
#define WF_SECURITY_WPA2_WITH_PASS_PHRASE        (6)
#define WF_SECURITY_WPA_AUTO_WITH_KEY            (7)
#define WF_SECURITY_WPA_AUTO_WITH_PASS_PHRASE    (8)
#define WF_SECURITY_WPS_PUSH_BUTTON              (9)
#define WF_SECURITY_WPS_PIN                      (10)
#define WF_SECURITY_WPA2_ENTERPRISE              (12)

#define WF_CONNECTED_ADHOC		3
#define WF_CONNECTED_INFRA		4

// MAC result code
enum {
    ZG_RESULT_SUCCESS = 1,
    ZG_RESULT_INVALID_SUBTYPE,
    ZG_RESULT_CANCELLED,
    ZG_RESULT_FRAME_EOL,
    ZG_RESULT_FRAME_RETRY_LIMIT,
    ZG_RESULT_FRAME_NO_BSS,
    ZG_RESULT_FRAME_TOO_BIG,
    ZG_RESULT_FRAME_ENCRYPT_FAILURE,
    ZG_RESULT_INVALID_PARAMS,
    ZG_RESULT_ALREADY_AUTH,
    ZG_RESULT_ALREADY_ASSOC,
    ZG_RESULT_INSUFFICIENT_RSRCS,
    ZG_RESULT_TIMEOUT,
    ZG_RESULT_BAD_EXCHANGE,	// frame exchange problem with peer (AP or STA)
    ZG_RESULT_AUTH_REFUSED,		// authenticating node refused our request
    ZG_RESULT_ASSOC_REFUSED,	// associating node refused our request
    ZG_RESULT_REQ_IN_PROGRESS,	// only one mlme request at a time allowed
    ZG_RESULT_NOT_JOINED,			// operation requires that device be joined
    								// with target
    ZG_RESULT_NOT_ASSOC,			// operation requires that device be
    								// associated with target
    ZG_RESULT_NOT_AUTH,				// operation requires that device be
    								// authenticated with target
    ZG_RESULT_SUPPLICANT_FAILED,
    ZG_RESULT_UNSUPPORTED_FEATURE,
    ZG_RESULT_REQUEST_OUT_OF_SYNC	// Returned when a request is recognized
    								// but invalid given the current state
    								// of the MAC
};

/*
 * G2100 command registers
 */
#define ZG_INTR_REG					(0x01)	// 8-bit register containing interrupt bits
#define ZG_INTR_MASK_REG			(0x02)	// 8-bit register containing interrupt mask
#define ZG_SYS_INFO_DATA_REG		(0x21)	// 8-bit register to read system info data window
#define ZG_SYS_INFO_IDX_REG			(0x2b)
#define ZG_INTR2_REG				(0x2d)	// 16-bit register containing interrupt bits
#define ZG_INTR2_MASK_REG			(0x2e)	// 16-bit register containing interrupt mask
#define ZG_BYTE_COUNT_REG			(0x2f)	// 16-bit register containing available write size for fifo0
#define ZG_BYTE_COUNT_FIFO0_REG		(0x33)	// 16-bit register containing bytes ready to read on fifo0
#define ZG_BYTE_COUNT_FIFO1_REG		(0x35)	// 16-bit register containing bytes ready to read on fifo1
#define ZG_PWR_CTRL_REG				(0x3d)	// 16-bit register used to control low power mode
#define ZG_INDEX_ADDR_REG			(0x3e)	// 16-bit register to move the data window
#define ZG_INDEX_DATA_REG			(0x3f)	// 16-bit register to read the address in the ZG_INDEX_ADDR_REG

#define ZG_INTR_REG_LEN				(1)
#define ZG_INTR_MASK_REG_LEN		(1)
#define ZG_SYS_INFO_DATA_REG_LEN	(1)
#define ZG_SYS_INFO_IDX_REG_LEN		(2)
#define ZG_INTR2_REG_LEN			(2)
#define ZG_INTR2_MASK_REG_LEN		(2)
#define ZG_BYTE_COUNT_REG_LEN		(2)
#define ZG_BYTE_COUNT_FIFO0_REG_LEN	(2)
#define ZG_BYTE_COUNT_FIFO1_REG_LEN	(2)
#define ZG_PWR_CTRL_REG_LEN			(2)
#define ZG_INDEX_ADDR_REG_LEN		(2)
#define ZG_INDEX_DATA_REG_LEN		(2)

// Registers accessed through ZG_INDEX_ADDR_REG
#define ZG_RESET_STATUS_REG			(0x2a)	// 16-bit read only register providing HW status bits
#define ZG_RESET_REG				(0x2e)	// 16-bit register used to initiate hard reset
#define ZG_PWR_STATUS_REG			(0x3e)	// 16-bit register read to determine when device
											// out of sleep state

#define ZG_RESET_MASK				(0x10)	// the first byte of the ZG_RESET_STATUS_REG
											// used to determine when the G2100 is in reset

#define ZG_ENABLE_LOW_PWR_MASK		(0x01)	// used by the Host to enable/disable sleep state
											// indicates to G2100 that the Host has completed
											// transactions and the device can go into sleep
											// state if possible

// states for interrupt state machine
#define ZG_INTR_ST_RD_INTR_REG	(1)
#define ZG_INTR_ST_WT_INTR_REG	(2)
#define ZG_INTR_ST_RD_CTRL_REG	(3)

// interrupt state
#define ZG_INTR_DISABLE		((u8)0)
#define ZG_INTR_ENABLE		((u8)1)

// mask values for ZG_INTR_REG and ZG_INTR2_REG
#define	ZG_INTR_MASK_FIFO1		(0x80)
#define ZG_INTR_MASK_FIFO0		(0x40)
#define ZG_INTR_MASK_ALL		(0xff)
#define ZG_INTR2_MASK_ALL		(0xffff)

// Buffer size
#define ZG_BUFFER_SIZE		450

// Types of networks
#define ZG_BSS_INFRA		(1)    // infrastructure only
#define ZG_BSS_ADHOC		(2)    // Ad-hoc only (ibss)

// Max characters in network SSID
#define ZG_MAX_SSID_LENGTH		32

// Security keys
#define ZG_MAX_ENCRYPTION_KEYS 		4
#define ZG_MAX_ENCRYPTION_KEY_SIZE	13
#define ZG_MAX_WPA_PASSPHRASE_LEN	64
#define ZG_MAX_PMK_LEN				32

#define ZG_SECURITY_TYPE_NONE	0
#define ZG_SECURITY_TYPE_WEP	1
#define ZG_SECURITY_TYPE_WPA	2
#define ZG_SECURITY_TYPE_WPA2	3

typedef struct
{
    u8 slot;	/* slot index */
    u8 keyLen;
    u8 defID;	/* the default wep key id */
    u8 ssidLen;	/* num valid bytes in ssid */
    u8 ssid[ZG_MAX_SSID_LENGTH];	/* ssid of network */
    u8 key[ZG_MAX_ENCRYPTION_KEYS][ZG_MAX_ENCRYPTION_KEY_SIZE];	/* wep key data for 4 default keys */
} zg_wep_key_req_t;

#define ZG_WEP_KEY_REQ_SIZE		(4 + ZG_MAX_SSID_LENGTH + ZG_MAX_ENCRYPTION_KEYS*ZG_MAX_ENCRYPTION_KEY_SIZE)

typedef struct
{
    u8 configBits;
    u8 phraseLen;	/* number of valid bytes in passphrase */
    u8 ssidLen;		/* number of valid bytes in ssid */
    u8 reserved;	/* alignment byte */
    u8 ssid[ZG_MAX_SSID_LENGTH];	/* the string of characters representing the ssid */
    u8 passPhrase[ZG_MAX_WPA_PASSPHRASE_LEN]; /* the string of characters representing the passphrase */
} zg_psk_calc_req_t;

#define ZG_PSK_CALC_REQ_SIZE	(4 + ZG_MAX_SSID_LENGTH + ZG_MAX_WPA_PASSPHRASE_LEN) /* 100 bytes */

typedef struct
{
    u8 result;		/* indicating success or other */
    u8 macState;	/* current State of the on-chip MAC */
    u8 keyReturned;	/* 1 if psk contains key data, 0 otherwise */
    u8 reserved;	/* pad byte */
    u8 psk[ZG_MAX_PMK_LEN];	/* the psk bytes */
} zg_psk_calc_cnf_t;

typedef struct
{
    u8 slot;
    u8 ssidLen;
    u8 ssid[ZG_MAX_SSID_LENGTH];
    u8 keyData[ZG_MAX_PMK_LEN];
} zg_pmk_key_req_t;

#define ZG_PMK_KEY_REQ_SIZE		(2 + ZG_MAX_SSID_LENGTH + ZG_MAX_PMK_LEN)

typedef struct
{
    u16        rssi;                      /* the value of the G1000 RSSI when the data frame was received */
	u16        arrivalTime_th;               /* the value of the 32-bit G1000 system clock when the frame arrived */
    u16        arrivalTime_bh;
	u16        dataLen;                   /* the length in bytes of the payload which immediately follows this data structure */
	u8         header[6];		/*Header  */
    u8         dstAddr[6];    /* MAC Address to which the data frame was directed. */
    u8         srcAddr[6];    /* MAC Address of the Station that sent the Data frame. */

} zg_rx_data_ind_t;

typedef struct
{
	u8 secType;		/* security type : 0 - none; 1 - wep; 2 - wpa; 3 - wpa2; 0xff - best available */
    u8 ssidLen;		/* num valid bytes in ssid */
    u8 ssid[ZG_MAX_SSID_LENGTH];	/* the ssid of the target */
    u16 sleepDuration;	/* power save sleep duration in units of 100 milliseconds */
    u8 modeBss;			/* 1 - infra; 2 - adhoc */
    u8 reserved;
} zg_connect_req_t;

#define ZG_CONNECT_REQ_SIZE			(38)

/////////////////*******************************************////////////////////////////////
////////////   ***********    CAMBIOS DANI **********///////////////////////////////////////
/////////////////*******************************************////////////////////////////////
/////////////////*******************************************////////////////////////////////

typedef enum
{
    /* Misc subtypes */
    WF_SCAN_SUBTYPE                             = 1,
    WF_JOIN_SUBTYPE                             = 2, 
    WF_AUTH_SUBTYPE                             = 3, 
    WF_ASSOC_SUBTYPE                            = 4, 
    WF_DISCONNECT_SUBTYPE                       = 5, 
    WF_DISASOCC_SUBTYPE                         = 6,
    WF_SET_POWER_MODE_SUBTYPE                   = 7,
    WF_SET_PM_KEY_SUBTYPE                       = 8,
    WF_SET_WEP_MAP_SUBTYPE                      = 9,
    WF_SET_WEP_KEY_SUBTYPE                      = 10,
    WF_SET_TEMP_KEY_SUBTYPE                     = 11,
    WF_CALC_PSK_KEY_SUBTYPE                     = 12,
    WF_SET_WEP_KEY_ID_SUBTYPE                   = 13, 
    WF_CONFIG_KEY_SPACE_SUBTYPE                 = 14,
    WF_SET_PARAM_SUBTYPE                        = 15,
    WF_GET_PARAM_SUBTYPE                        = 16,
    WF_ADHOC_CONNECT_SUBTYPE                    = 17,
    WF_ADHOC_START_SUBTYPE                      = 18,

    /* Connection Profile Message Subtypes */
    WF_CP_CREATE_PROFILE_SUBTYPE                = 21,
    WF_CP_DELETE_PROFILE_SUBTYPE                = 22,
    WF_CP_GET_ID_LIST_SUBTYPE                   = 23,
    WF_CP_SET_ELEMENT_SUBTYPE                   = 24,
    WF_CP_GET_ELEMENT_SUBTYPE                   = 25,

    /* Connection Algorithm Message Subtypes */
    WF_CA_SET_ELEMENT_SUBTYPE                   = 26,
    WF_CA_GET_ELEMENT_SUBTYPE                   = 27,

    /* Connnection Manager Message Subtypes */
    WF_CM_CONNECT_SUBYTPE                       = 28,
    WF_CM_DISCONNECT_SUBYTPE                    = 29,           
    WF_CM_GET_CONNECTION_STATUS_SUBYTPE         = 30,

    WF_SCAN_START_SUBTYPE                       = 31,
    WF_SCAN_GET_RESULTS_SUBTYPE                 = 32,
    
    WF_CM_INFO_SUBTYPE                          = 33,

    WF_SCAN_FOR_IE_SUBTYPE                      = 34,  /* not yet supported */
    WF_SCAN_IE_GET_RESULTS_SUBTYPE              = 35,  /* not yet supported */
    
    WF_CM_GET_CONNECTION_STATISTICS_SUBYTPE     = 36,  /* not yet supported so moved here for now */
    WF_NUM_REQUEST_SUBTYPES
    
} tMgmtMsgSubtypes;



/////////////////*******************************************////////////////////////////////
/////////////////********* FIN CAMBIOS DANI *********///////////////////////////////////////
/////////////////*******************************************////////////////////////////////
/////////////////*******************************************////////////////////////////////

void zg_init();
void ZG2100_CSInit();
void ZG2100_CSon();

void zg_reset();
void spi_transfer(volatile U8* buf, U16 len, U8 toggle_cs);
void zg_chip_reset();
void zg_interrupt2_reg();
void zg_interrupt_reg(U8 mask, U8 state);
void zg_isr();
void zg_process_isr();
void zg_send(U8* buf, U16 len);
void zg_recv(U8* buf, U16* len);
U16 zg_get_rx_status();
void zg_clear_rx_status();
void zg_set_tx_status(U8 status);
U8 zg_get_conn_state();

void zg_set_buf(U8* buf, U16 buf_len);
U8* zg_get_mac();

#ifdef DEBUG_ACTIVE
//DEBUG
U8 zg_get_drv_state();
void copydatadebug();
void copymessage(char * message);
#endif

void zg_set_ssid(U8* ssid, U8 ssid_len);
void zg_set_sec(U8 sec_type, U8* sec_key, U8 sec_key_len);
void zg_drv_process();

#endif /* G2100_H_ */