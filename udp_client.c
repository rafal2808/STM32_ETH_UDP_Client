#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "stdio.h"
#include "string.h"

#include "udp_client.h"

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
static void udpClient_send(const char *message);

struct udp_pcb *upcb;
char buffer[100];


void udpClient_connect(void)
{
    err_t err;

    /* 1. Create a new UDP control block  */
    upcb = udp_new();

    /* Bind the block to module's IP and port */
    ip_addr_t myIPaddr;
    IP_ADDR4(&myIPaddr, 192, 168, 8, 200);
    udp_bind(upcb, &myIPaddr, 1100);

    /* Configure destination IP address and port */
    ip_addr_t DestIPaddr;
    IP_ADDR4(&DestIPaddr, 192, 168, 8, 108);
    err = udp_connect(upcb, &DestIPaddr, 12);

    if (err == ERR_OK)
    {
        /* 2. Send "Hello World" message to server */
        udpClient_send("Hello World");

        /* 3. Set a receive callback for the upcb */
        udp_recv(upcb, udp_receive_callback, NULL);
    }
}

static void udpClient_send(const char *message)
{
    struct pbuf *txBuf;
    int len = strlen(message);

    /* Allocate pbuf from pool */
    txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

    if (txBuf != NULL)
    {
        /* Copy data to pbuf */
        pbuf_take(txBuf, message, len);

        /* Send UDP data */
        udp_send(upcb, txBuf);

        /* Free pbuf */
        pbuf_free(txBuf);
    }
}

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    /* Copy the data from the pbuf */
    strncpy(buffer, (char *)p->payload, p->len);
    buffer[p->len] = '\0';  // Ensure null termination

    /* Free receive pbuf */
    pbuf_free(p);
}
