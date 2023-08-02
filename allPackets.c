#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("RUTY");
MODULE_DESCRIPTION("Kernel module to log packet information using Netfilter");
MODULE_VERSION("0.01");

static struct nf_hook_ops nfho;

// Netfilter hook function
unsigned int packet_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *iph;
    // Extract the IP header from the packet
    iph = ip_hdr(skb);
    
    // Log source and destination IP addresses
    printk(KERN_INFO "Packet: Source IP %pI4, Destination IP %pI4\n", &iph->saddr, &iph->daddr);

    // Return NF_ACCEPT to allow the packet to continue processing, or NF_DROP to drop it.
    return NF_ACCEPT;
}

static int __init module_init_func(void) {
    nfho.hook = packet_hook;
    nfho.pf = NFPROTO_IPV4; // IPv4 packets
    nfho.hooknum = NF_INET_PRE_ROUTING; // Hook point - Incoming packets
    nfho.priority = NF_IP_PRI_FIRST; // Highest priority

    // Register the hook
    nf_register_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Packet logger Netfilter hook registered.\n");
    return 0;
}

static void __exit module_exit_func(void) {
    // Unregister the hook
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Packet logger Netfilter hook unregistered.\n");
}

module_init(module_init_func);
module_exit(module_exit_func);
