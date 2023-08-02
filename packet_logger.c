#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h> // Include the TCP header file

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RUTY");
MODULE_DESCRIPTION("Kernel module to log HTTP packet information using Netfilter");
MODULE_VERSION("0.01");

static struct nf_hook_ops nfho;

unsigned int packet_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *iph;
    struct tcphdr *tcph; // TCP header structure

    iph = ip_hdr(skb);

    // Check if the packet is using TCP protocol (6) and destination port is 80 (HTTP)
    if (iph->protocol == IPPROTO_TCP) {
        tcph = (struct tcphdr *)(skb_transport_header(skb) + tcp_hdrlen(skb));
        if (ntohs(tcph->dest) == 80) {
            // Log the source and destination IP addresses of HTTP packets
            printk(KERN_INFO "HTTP Packet: Source IP %pI4, Destination IP %pI4\n", &iph->saddr, &iph->daddr);
            return NF_DROP; // do not allow HTTP packets to proceed for further processing - security reason.
        }
    }

    return NF_ACCEPT; // allow all other packets (not TCP or not HTTP)
}

static int __init module_init_func(void) {
    nfho.hook = packet_hook;
    nfho.pf = NFPROTO_IPV4;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.priority = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, &nfho);
    printk(KERN_INFO "HTTP Packet logger Netfilter hook registered.\n");
    return 0;
}

static void __exit module_exit_func(void) {
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "HTTP Packet logger Netfilter hook unregistered.\n");
}

module_init(module_init_func);
module_exit(module_exit_func);
