/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

/* @file
 *
 * Bluetooth RFCOMM Application Programming Interface
 *
 */

#ifndef _AOS_BT_RFCOMM_H_
#define _AOS_BT_RFCOMM_H_

#include "aos.h"

/*****************************************************
 *        Constants and Type definitions
 ******************************************************/

/* RFCOMM Port Event Masks */
typedef enum aos_bt_rfcomm_port_event_e {
    AOS_BT_RFCOMM_EV_RXCHAR          = 0x00000001,         /* Any Character received */
    AOS_BT_RFCOMM_EV_RXFLAG          = 0x00000002,         /* Received certain character */
    AOS_BT_RFCOMM_EV_TXEMPTY         = 0x00000004,         /* Transmitt Queue Empty */
    AOS_BT_RFCOMM_EV_CTS             = 0x00000008,         /* CTS changed state */
    AOS_BT_RFCOMM_EV_DSR             = 0x00000010,         /* DSR changed state */
    AOS_BT_RFCOMM_EV_RLSD            = 0x00000020,         /* RLSD changed state */
    AOS_BT_RFCOMM_EV_BREAK           = 0x00000040,         /* BREAK received */
    AOS_BT_RFCOMM_EV_ERR             = 0x00000080,         /* Line status error occurred */
    AOS_BT_RFCOMM_EV_RING            = 0x00000100,         /* Ring signal detected */
    AOS_BT_RFCOMM_EV_CTSS            = 0x00000400,         /* CTS state */
    AOS_BT_RFCOMM_EV_DSRS            = 0x00000800,         /* DSR state */
    AOS_BT_RFCOMM_EV_RLSDS           = 0x00001000,         /* RLSD state */
    AOS_BT_RFCOMM_EV_OVERRUN         = 0x00002000,         /* receiver buffer overrun */
    AOS_BT_RFCOMM_EV_TXCHAR          = 0x00004000,         /* Any character transmitted */
    AOS_BT_RFCOMM_EV_CONNECTED       = 0x00000200,         /* RFCOMM connection established */
    AOS_BT_RFCOMM_EV_CONNECT_ERR     = 0x00008000,         /* Was not able to establish connection or disconnected */
    AOS_BT_RFCOMM_EV_FC              = 0x00010000,         /* data flow enabled flag changed by remote */
    AOS_BT_RFCOMM_EV_FCS             = 0x00020000,         /* data flow enable status true = enabled */
} aos_bt_rfcomm_port_event_t;

#define AOS_BT_RFCOMM_MASK_ALL             (AOS_BT_RFCOMM_EV_RXCHAR | AOS_BT_RFCOMM_EV_TXEMPTY | AOS_BT_RFCOMM_EV_CTS | \
                                             AOS_BT_RFCOMM_EV_DSR | AOS_BT_RFCOMM_EV_RLSD | AOS_BT_RFCOMM_EV_BREAK | \
                                             AOS_BT_RFCOMM_EV_ERR | AOS_BT_RFCOMM_EV_RING | AOS_BT_RFCOMM_EV_CONNECT_ERR | \
                                             AOS_BT_RFCOMM_EV_DSRS | AOS_BT_RFCOMM_EV_CTSS | AOS_BT_RFCOMM_EV_RLSDS | \
                                             AOS_BT_RFCOMM_EV_RXFLAG | AOS_BT_RFCOMM_EV_TXCHAR | AOS_BT_RFCOMM_EV_OVERRUN | \
                                             AOS_BT_RFCOMM_EV_FC | AOS_BT_RFCOMM_EV_FCS | AOS_BT_RFCOMM_EV_CONNECTED)



/* RFCOMM Result Codes */
enum aos_bt_rfcomm_result_e {
    AOS_BT_RFCOMM_SUCCESS,                                        /* Success */
    AOS_BT_RFCOMM_ERROR,                                          /* Error */
    AOS_BT_RFCOMM_ALREADY_OPENED,                                 /* Already Opened */
    AOS_BT_RFCOMM_CMD_PENDING,                                    /* Command Pending */
    AOS_BT_RFCOMM_APP_NOT_REGISTERED,                             /* App Not Registered */
    AOS_BT_RFCOMM_NO_MEM,                                         /* No Memory */
    AOS_BT_RFCOMM_NO_RESOURCES,                                   /* No Resources */
    AOS_BT_RFCOMM_BAD_BD_ADDR,                                    /* Bad BD Address */
    AOS_BT_RFCOMM_RESULT_RESERVED0,
    AOS_BT_RFCOMM_BAD_HANDLE,                                     /* Bad Handle */
    AOS_BT_RFCOMM_NOT_OPENED,                                     /* Not Opened */
    AOS_BT_RFCOMM_LINE_ERR,                                       /* Line Error */
    AOS_BT_RFCOMM_START_FAILED,                                   /* Start Failed */
    AOS_BT_RFCOMM_PAR_NEG_FAILED,
    AOS_BT_RFCOMM_RFCOMM_NEG_FAILED,
    AOS_BT_RFCOMM_SEC_FAILED,
    AOS_BT_RFCOMM_PEER_CONNECTION_FAILED,                         /* Peer Connection Failed */
    AOS_BT_RFCOMM_PEER_FAILED,                                    /* Peer Failed */
    AOS_BT_RFCOMM_PEER_TIMEOUT,                                   /* Peer Timeout */
    AOS_BT_RFCOMM_CLOSED,                                         /* Closed */
    AOS_BT_RFCOMM_TX_FULL,
    AOS_BT_RFCOMM_LOCAL_CLOSED,                                   /* Local Closed */
    AOS_BT_RFCOMM_LOCAL_TIMEOUT,                                  /* Local Timeout */
    AOS_BT_RFCOMM_TX_QUEUE_DISABLED,
    AOS_BT_RFCOMM_PAGE_TIMEOUT,                                   /* Page Timeout */
    AOS_BT_RFCOMM_INVALID_SCN                                     /* Invalid SCN */
};
typedef int aos_bt_rfcomm_result_t;                           /* RFCOMM result code (see #aos_bt_rfcomm_result_e) */

/* RFCOMM Signals */
enum aos_bt_rfcomm_signal_e {
    AOS_BT_RFCOMM_SET_DTRDSR = 0x01, /* DTRDSR set */
    AOS_BT_RFCOMM_CLR_DTRDSR,        /* DTRDSR clear */
    AOS_BT_RFCOMM_SET_CTSRTS,        /* CTSRTS set */
    AOS_BT_RFCOMM_CLR_CTSRTS,        /* CTSRTS clear */
    AOS_BT_RFCOMM_SET_RI,            /* RI set (DCE only) */
    AOS_BT_RFCOMM_CLR_RI,            /* RI clear (DCE only) */
    AOS_BT_RFCOMM_SET_DCD,           /* DCD set (DCE only) */
    AOS_BT_RFCOMM_CLR_DCD,           /* DCD clear (DCE only) */
    AOS_BT_RFCOMM_BREAK,             /* BRK */
};
typedef uint8_t aos_bt_rfcomm_signal_t;   /* RFCOMM Signals (see #aos_bt_rfcomm_signal_e) */

/*
 *  Define the callback function prototypes for aos_bt_rfcomm_data_cback_t
 *
 *  @param  port_handle         : A 16-bit unsigned integer returned by @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink.
 *  @param  *p_data             : A pointer to the array of bytes received from the peer device.
 *  @param  len                 : The length of the data received.
 */
typedef int  (aos_bt_rfcomm_data_cback_t) (uint16_t port_handle, void *p_data, uint16_t len);

/*
 *  Port management callback
 *
 *  @param  code                : Result code
 *  @param  port_handle         : Port handle from @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink.
 */
typedef void (aos_bt_port_mgmt_cback_t) (aos_bt_rfcomm_result_t code, uint16_t port_handle);

/*
 *  Port event callback
 *
 *  @param  event               : A 32-bit event code that contains a bit-mask of one  or more events the caller would like to register.
 *  @param  port_handle         : Port handle from @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink.
 */
typedef void (aos_bt_port_event_cback_t) (aos_bt_rfcomm_port_event_t event, uint16_t port_handle);

/*
 *  @addtogroup  rfcomm_api_functions       RFCOMM
 *  @ingroup     aosbt
 *
 *  RFCOMM Functions
 *
 *  @{
 */
/*****************************************************
 *               Function Declarations
 ******************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  Establish serial port connection to the peer device, or allow
 *  RFCOMM to accept a connection from peer devices.
 *
 *  @note
 *  Server can call this function with the same scn parameter multiple times if
 *  it is ready to accept multiple simulteneous connections.
 *
 *  DLCI for the connection is (scn * 2 + 1) if client originates connection on
 *  existing none initiator multiplexer channel.  Otherwise it is (scn * 2).
 *  For the server DLCI can be changed later if client will be calling it using
 *  (scn * 2 + 1) dlci.
 *
 *  @param[in]  uuid            : The Universal Unique Identifier (UUID) of the
 *                                Class ID of the service being opened
 *  @param[in]  scn             : The Service Channel Number(SCN) as registered
 *                                with the SDP (server) or obtained using SDP from
 *                                the peer device (client)
 *  @param[in]  is_server       : TRUE if requesting application is a server
 *  @param[in]  mtu             : The maximum number of bytes transferred per frame
 *                                If 0, a default size of L2CAP_MTU_SIZE minus
 *                                5 bytes is used
 *  @param[in]  bd_addr         : BD_ADDR of the peer (if client), NULL if server
 *  @param[in]  p_mgmt_cb       : Pointer to callback function to receive connection
 *                                up/down events
 *  @param[out]  p_handle       : A pointer to the handle set by RFCOMM to be used in
 *                                consecutive calls for this connection
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_ALREADY_OPENED </b> : If the client tries to establish a connection to the same BD_ADDR
 *              <b> AOS_BT_RFCOMM_NO_RESOURCES </b>   : If there is not enough memory to allocate a control block structure
 *              <b> AOS_BT_RFCOMM_INVALID_SCN </b>    : If Server Channel Number(SCN) is out of in range 1...30
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_create_connection (uint16_t uuid, uint8_t scn,
                                                        aos_bool_t is_server, uint16_t mtu,
                                                        aos_bt_device_address_t bd_addr,
                                                        uint16_t *p_handle,
                                                        aos_bt_port_mgmt_cback_t *p_mgmt_cb);

/*
 *  Close the specified connection.
 *
 *  @param[in]  handle              : The connection handle returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink.
 *  @param[in]  remove_server       : (for server only) If TRUE, then also remove server; otherwise server remains enabled
 *                                    after connection is closed.
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_remove_connection (uint16_t handle, aos_bool_t remove_server);


/*
 *  Set event callback the specified connection.
 *
 *  @param[in]  port_handle         : A 16-bit unsigned integer returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[in]  p_port_cb           : Address of the callback function which should
 *                                    be called from the RFCOMM when an event
 *                                    specified in the mask occurs
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_set_event_callback (uint16_t port_handle, aos_bt_port_event_cback_t *p_port_cb);

/*
 *  Set event data callback the specified connection.
 *
 *  @param[in]  port_handle         : A 16-bit unsigned integer returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[in]  p_cb                : Address of the callback function which should
 *                                    be called from the RFCOMM when a data
 *                                    packet is received
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_set_data_callback (uint16_t port_handle, aos_bt_rfcomm_data_cback_t *p_cb);


/*
 *  Set events for which to be notified
 *
 *  @param[in]  port_handle         : A 16-bit unsigned integer returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[in]  mask                : Event mask
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_set_event_mask (uint16_t port_handle, aos_bt_rfcomm_port_event_t mask);

/*
 *  Send control signal to the peer device.
 *
 *  @param[in]  handle              : The connection handle returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[in]  signal              : Signal to send (see #aos_bt_rfcomm_signal_e)
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_control (uint16_t handle, aos_bt_rfcomm_signal_t signal);

/*
 *  This function directs a specified connection to pass flow control message to the peer device.
 *  Enable flag passed shows if port can accept more data.
 *
 *  @param[in]  handle              : The connection handle returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[in]  enable              : Flow control setting
 *                                    TRUE  Enable data flow
 *                                    FALSE Disable data flow
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_flow_control (uint16_t handle, aos_bool_t enable);

/*
 *  This function directs a specified connection to pass flow control message to the peer device.
 *  Enable flag passed shows if port can accept more data.
 *
 *  @param[in]  handle              : The connection handle returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[in]  p_data              : Data to write
 *  @param[in]  max_len             : Byte count to write
 *  @param[out] p_len               : Bytes written
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_BAD_HANDLE </b>     : If the handle is out of range
 *              <b> AOS_BT_RFCOMM_NOT_OPENED </b>     : If the connection is not opened
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_write_data (uint16_t handle, char *p_data, uint16_t max_len, uint16_t *p_len);

/*
 *  This function checks connection referenced by handle is up and running.
 *
 *  @param[in]  handle              : The connection handle returned by
 *                                    @link aos_bt_rfcomm_create_connection aos_bt_rfcomm_create_connection @endlink
 *  @param[out]  bd_addr            : Peer BD Address
 *  @param[out]  p_lcid             : L2CAP's LCID
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_LINE_ERR </b>       : If connection is not up and running
 */
//aos_bt_rfcomm_result_t aos_bt_rfcomm_check_connection (UINT16 handle, BD_ADDR bd_addr, UINT16 *p_lcid);

#ifdef MPAF_CUSTOM_STACK
/*
 *  This function allocates private pool to be used by RFCOMM.
 *
 *  @param[in]  buffer_size              : size of buffer
 *  @param[out]  buffer_cnt              : buffers
 *
 *  @return     <b> AOS_BT_RFCOMM_SUCCESS </b>        : If successful
 *              <b> AOS_BT_RFCOMM_ERROR </b>          : If pool allocation fails
 */
aos_bt_rfcomm_result_t aos_bt_rfcomm_init(uint32_t buffer_size, uint32_t buffer_cnt);

/*
 *  This function enables flow control based on ACL buffer availability
 *
 *  @param[in]  peer_addr              : Peer BD Address
 *
 *  @return     <b> AOS_TRUE </b>    : If successful
 *              <b> AOS_FALSE </b>   : If fails
 */
aos_bool_t aos_bt_rfcomm_control_data_flow(BD_ADDR peer_bda);
#endif

#ifdef __cplusplus
}
#endif

#endif

/*@} */
