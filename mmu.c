#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mmu.h"

#define NBITS 6                 /* number of bits in each address */
#define MEMSZ 64                /* address space size = 2 ^ 6 */
#define PAGESZ 16               /* size of each page */
#define NPAGES (MEMSZ / PAGESZ) /* 8 pages in address space */

#define VPN_BITS 2    /* 2 bits to address all pages */
#define OFFSET_BITS 4 /* 3 bits to address within each page */
#define VPN_MASK 0b110000
#define OFFSET_MASK 0b001111

#define PFN_BITS VPN_BITS /* physical address space same size as virtual */

/**
 * A completely random page table for a 32-byte address space
 * with 4-byte pages.
 */
const int pagetable[NPAGES] = {
    /* VPN-->PFN */
    [0b00] = 0b11,
    [0b01] = 0b10,
    [0b10] = 0b00,
    [0b11] = 0b01};

int main()
{
    char hexbuf[4];
    int vaddr, vpn, offset, pfn;
    int phys_addr = 0;

    printf("enter address: 0x");
    fflush(stdout);

    fgets(hexbuf, 4, stdin);

    vaddr = parse_addr(hexbuf, NBITS);
    if (vaddr < 0) {
        fprintf(stderr, "Error: invalid %d-bit address.\n", NBITS);
        return EXIT_FAILURE;
    }
    printf("decimal: %d\n", vaddr);
    printb("binary", vaddr, NBITS);

    vpn = (vaddr & VPN_MASK) >> OFFSET_BITS;
    offset = vaddr & OFFSET_MASK;
    pfn = pagetable[vpn];

    printb("vpn", vpn, VPN_BITS);
    printb("offset", offset, OFFSET_BITS);
    printb("pfn", pfn, 2);

    // create new address
    phys_addr |= pfn << OFFSET_BITS;
    phys_addr |= offset;

    printb("physical address", phys_addr, NBITS);

    return EXIT_SUCCESS;
}