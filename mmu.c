/**
 * A simple demo program to show the address translation process for a
 * 6-bit address in a 64-byte address space with page sizes of 16 bytes.
 *
 * The virtual address is divided as follows:
 *
 *  |-------|-------|-------|-------|-------|-------|
 *  |  va5  |  va4  |  va3  |  va2  |  va1  |  va0  |
 *  |-------|-------|-------|-------|-------|-------|
 *  |<<--- VPN --->>|<<---------- OFFSET --------->>|
 *
 * The 2 leading bits are the virtual page number (VPN), which we use
 * to look up the physical frame number (PFN) in the page table.
 *
 * The remaining 4 bits are used to address inside each page. Since
 * one page is 16 bytes, we need 4 bits to address the page. (2^4 = 16).
 *
 * Author: Jerry Yu <jiy066@ucsd.edu>
 */

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

    /* calculate the VPN and offset using bit masking and shifting */
    vpn = (vaddr & VPN_MASK) >> OFFSET_BITS;
    offset = vaddr & OFFSET_MASK;

    /* look up the PFN in the page table */
    pfn = pagetable[vpn];

    printb("vpn", vpn, VPN_BITS);
    printb("offset", offset, OFFSET_BITS);
    printb("pfn", pfn, 2);

    /* form the translated address */
    phys_addr |= pfn << OFFSET_BITS;
    phys_addr |= offset;

    printb("physical address", phys_addr, NBITS);

    return EXIT_SUCCESS;
}